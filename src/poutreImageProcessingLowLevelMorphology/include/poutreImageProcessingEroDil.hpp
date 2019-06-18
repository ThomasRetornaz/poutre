//==============================================================================
//                  Copyright (c) 2015 - Thomas Retornaz                      //
//                     thomas.retornaz@mines-paris.org                        //
//          Distributed under the Boost Software License, Version 1.0.        //
//                 See accompanying file LICENSE.txt or copy at               //
//                     http://www.boost.org/LICENSE_1_0.txt                   //
//==============================================================================

#ifndef POUTRE_IMAGEPROCESSING_ERO_DIL_HPP__
#define POUTRE_IMAGEPROCESSING_ERO_DIL_HPP__

/**
 * @file   poutreImageProcessingEroDil.hpp
 * @author Thomas Retornaz
 * @brief  Define erosion/dilatation function
 *
 *
 */
#ifndef POUTRE_IMAGEPROCESSINGCORE_HPP__
#include <poutreImageProcessingCore/poutreImageProcessingCore.hpp>
#endif

#ifndef POUTRE_IMAGEPROCESSINGTYPE_HPP__
#include <poutreImageProcessingCore/poutreImageProcessingType.hpp>
#endif

#ifndef POUTRE_TRACE__HPP__
#include <poutreBase/poutreTrace.hpp>
#endif

#ifndef POUTRE_CONTAINER_VIEW_HPP__
#include <poutreBase/poutreContainerView.hpp>
#endif

#ifndef POUTRE_IPSENL_HPP__
#include <poutreImageProcessingSE/poutreImageProcessingSENeighborList.hpp>
#endif

#ifndef POUTRE_IMAGEPROCESSING_ARITH_HPP__ 
#include <poutreImageProcessingPixelOperation/include/poutreImageProcessingArith.hpp>
#endif
namespace poutre
{
	/**
	 * @addtogroup image_processing_group Image Processing Group
	 *
	 *@{
	 */

	 /**
	  * @addtogroup image_processing_erodil_group Image Processing Erosion Dilatation functions
	  * @ingroup image_processing_group
	  *@{
	  */

	  //template<se::NeighborListStaticSE nl, typename T1, typename T2, ptrdiff_t Rank, template <typename, ptrdiff_t> class View1, template <typename, ptrdiff_t> class View2,class BinOp>
	  //struct t_ErodeDilateOpispatcher
	  //{
	  //	static_assert(Rank == se::NeighborListStaticSETraits<nl>::Rank, "SE and view have not the same Rank");
	  //	void operator()(const View1<T1, Rank>& i_vin, const View2<T2, Rank>& o_vout, const BinOp& op)
	  //	{
	  //		POUTRE_CHECK(i_vin.size() == o_vout.size(), "Incompatible views size");

	  //		//More runtime dispatch
	  //		auto vInbound = i_vin.bound();
	  //		auto vOutbound = o_vout.bound();
	  //		auto stridevIN = i_vin.stride();
	  //		auto stridevOut = o_vout.stride();
	  //		auto NlList = se::NeighborListStaticSETraits<nl>::NlList;
	  //		if (vInbound == vOutbound && stridevIN == stridevOut) //same bound + same stride -> one idx
	  //		{
	  //			auto beg1 = begin(vInbound);
	  //			auto end1 = end(vInbound);
	  //			for (; beg1 != end1; ++beg1)
	  //			{
	  //				auto val = i_vin[*beg1];
	  //				for (const auto& idxnl : NlList)
	  //				{
	  //					auto currentidx = *beg1 + idxnl;
	  //					if (!vInbound.contains(currentidx)) continue;
	  //					val = op(val, i_vin[currentidx]);
	  //				}
	  //				o_vout[*beg1] = static_cast<T2>(max);
	  //			}
	  //			//TODO stride==1 in least significant dimension
	  //		}
	  //		else //default two idx
	  //		{
	  //			auto beg1 = begin(vInbound);
	  //			auto end1 = end(vInbound);
	  //			auto beg2 = begin(vOutbound);
	  //			for (; beg1 != end1; ++beg1, ++beg2)
	  //			{
	  //				auto max = i_vin[*beg1];
	  //				for (const auto& idxnl : NlList)
	  //				{
	  //					auto currentidx = *beg1 + idxnl;
	  //					if (!vInbound.contains(currentidx)) continue;
	  //					max = op(max, i_vin[currentidx]);
	  //				}
	  //				o_vout[*beg2] = static_cast<T2>(max);
	  //			}
	  //		}
	  //	}
	  //};
	template<se::NeighborListStaticSE nl, typename T1, typename T2, ptrdiff_t Rank, template <typename, ptrdiff_t> class View1, template <typename, ptrdiff_t> class View2>
	struct t_DilateOpispatcher
	{
		static_assert(Rank == se::NeighborListStaticSETraits<nl>::Rank, "SE and view have not the same Rank");
		void operator()(const View1<T1, Rank>& i_vin, const View2<T2, Rank>& o_vout)
		{
			POUTRE_CHECK(i_vin.size() == o_vout.size(), "Incompatible views size");

			//More runtime dispatch
			auto vInbound = i_vin.bound();
			auto vOutbound = o_vout.bound();
			auto stridevIN = i_vin.stride();
			auto stridevOut = o_vout.stride();
			auto NlList = se::NeighborListStaticSETraits<nl>::NlList;
			if (vInbound == vOutbound && stridevIN == stridevOut) //same bound + same stride -> one idx
			{
				auto beg1 = begin(vInbound);
				auto end1 = end(vInbound);
				for (; beg1 != end1; ++beg1)
				{
					auto max = i_vin[*beg1];
					for (const auto& idxnl : NlList)
					{
						auto currentidx = *beg1 + idxnl;
						if (!vInbound.contains(currentidx)) continue;
						max = std::max<T1>(max, i_vin[currentidx]);
					}
					o_vout[*beg1] = static_cast<T2>(max);
				}
				//TODO stride==1 in least significant dimension
			}
			else //default two idx
			{
				auto beg1 = begin(vInbound);
				auto end1 = end(vInbound);
				auto beg2 = begin(vOutbound);
				for (; beg1 != end1; ++beg1, ++beg2)
				{
					auto max = i_vin[*beg1];
					for (const auto& idxnl : NlList)
					{
						auto currentidx = *beg1 + idxnl;
						if (!vInbound.contains(currentidx)) continue;
						max = std::max<T1>(max, i_vin[currentidx]);
					}
					o_vout[*beg2] = static_cast<T2>(max);
				}
			}
		}
	};

	template<typename TIn, typename TOut, ptrdiff_t Rank, template <typename, ptrdiff_t> class ViewIn, template <typename, ptrdiff_t> class ViewOut>
	void t_Dilate(const ViewIn<TIn, Rank>& i_vin, se::NeighborListStaticSE nl, ViewOut<TOut, Rank>& o_vout)
	{
		POUTRE_CHECK(i_vin.size() == o_vout.size(), "Incompatible views size");
		switch (nl)
		{
		case se::NeighborListStaticSE::NeighborListStaticSE2DCT8:
		{
			//op_Sup<T1, T1, TOut, tag_SIMD_disabled> op;
			//t_ErodeDilateOpispatcher<se::NeighborListStaticSE::NeighborListStaticSE2DCT8, TIn, TOut, Rank, ViewIn, ViewOut, op_Sup<T1, T1, TOut, tag_SIMD_disabled>> dispatcher;
			//dispatcher(i_vin, o_vout, op);
			t_DilateOpispatcher<se::NeighborListStaticSE::NeighborListStaticSE2DCT8, TIn, TOut, Rank, ViewIn, ViewOut> dispatcher;
			dispatcher(i_vin, o_vout);
		}break;
		case se::NeighborListStaticSE::NeighborListStaticSE2DCT4:
		{
			t_DilateOpispatcher<se::NeighborListStaticSE::NeighborListStaticSE2DCT4, TIn, TOut, Rank, ViewIn, ViewOut> dispatcher;
			dispatcher(i_vin, o_vout);
		}break;

		default:
		{
			POUTRE_RUNTIME_ERROR("t_Dilate se::NeighborListStaticSE not implemented");
		}
		}
	}

	template<se::NeighborListStaticSE nl, typename T1, typename T2, ptrdiff_t Rank, template <typename, ptrdiff_t> class View1, template <typename, ptrdiff_t> class View2>
	struct t_ErodeOpispatcher
	{
		static_assert(Rank == se::NeighborListStaticSETraits<nl>::Rank, "SE and view have not the same Rank");
		void operator()(const View1<T1, Rank>& i_vin, const View2<T2, Rank>& o_vout)
		{
			POUTRE_CHECK(i_vin.size() == o_vout.size(), "Incompatible views size");

			//More runtime dispatch
			auto vInbound = i_vin.bound();
			auto vOutbound = o_vout.bound();
			auto stridevIN = i_vin.stride();
			auto stridevOut = o_vout.stride();
			auto NlList = se::NeighborListStaticSETraits<nl>::NlList;
			if (vInbound == vOutbound && stridevIN == stridevOut) //same bound + same stride -> one idx
			{
				auto beg1 = begin(vInbound);
				auto end1 = end(vInbound);
				for (; beg1 != end1; ++beg1)
				{
					auto min = i_vin[*beg1];
					for (const auto& idxnl : NlList)
					{
						auto currentidx = *beg1 + idxnl;
						if (!vInbound.contains(currentidx)) continue;
						min = std::min<T1>(min, i_vin[currentidx]);
					}
					o_vout[*beg1] = static_cast<T2>(min);
				}
				//TODO stride==1 in least significant dimension
			}
			else //default two idx
			{
				auto beg1 = begin(vInbound);
				auto end1 = end(vInbound);
				auto beg2 = begin(vOutbound);
				for (; beg1 != end1; ++beg1, ++beg2)
				{
					auto min = i_vin[*beg1];
					for (const auto& idxnl : NlList)
					{
						auto currentidx = *beg1 + idxnl;
						if (!vInbound.contains(currentidx)) continue;
						min = std::max<T1>(min, i_vin[currentidx]);
					}
					o_vout[*beg2] = static_cast<T2>(min);
				}
			}
		}
	};

	template<typename TIn, typename TOut, ptrdiff_t Rank, template <typename, ptrdiff_t> class ViewIn, template <typename, ptrdiff_t> class ViewOut>
	void t_Erode(const ViewIn<TIn, Rank>& i_vin, se::NeighborListStaticSE nl, ViewOut<TOut, Rank>& o_vout)
	{
		POUTRE_CHECK(i_vin.size() == o_vout.size(), "Incompatible views size");
		switch (nl)
		{
		case se::NeighborListStaticSE::NeighborListStaticSE2DCT8:
		{
			t_ErodeOpispatcher<se::NeighborListStaticSE::NeighborListStaticSE2DCT8, TIn, TOut, Rank, ViewIn, ViewOut> dispatcher;
			dispatcher(i_vin, o_vout);
		}break;
		case se::NeighborListStaticSE::NeighborListStaticSE2DCT4:
		{
			t_ErodeOpispatcher<se::NeighborListStaticSE::NeighborListStaticSE2DCT4, TIn, TOut, Rank, ViewIn, ViewOut> dispatcher;
			dispatcher(i_vin, o_vout);
		}break;

		default:
		{
			POUTRE_RUNTIME_ERROR("t_Erode se::NeighborListStaticSE not implemented");
		}
		}
	}
	//! @} doxygroup: image_processing_erodil_group
	//! @} doxygroup: image_processing_group
}//poutre
#endif POUTRE_IMAGEPROCESSING_ERO_DIL_HPP__