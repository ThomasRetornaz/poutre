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
            auto NlList = se::NeighborListStaticSETraits<nl>::NlListTransposed;
            if (vInbound == vOutbound && stridevIN == stridevOut) //same bound + same stride -> one idx
            {
                auto beg1 = begin(vInbound);
                auto end1 = end(vInbound);
                for (; beg1 != end1; ++beg1)
                {
                    auto max = std::numeric_limits<T1>::lowest();
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
                    auto max = std::numeric_limits<T1>::lowest();
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
    template<se::NeighborListStaticSE nl, typename T1, typename T2>/*, template <typename, ptrdiff_t> class View1,template <typename, ptrdiff_t> class View2>*/
    void t_DilateIterateArrayView2DHelper(const array_view<T1, 2>& i_vin, const array_view<T2, 2>& o_vout,ptrdiff_t i_XCenter, ptrdiff_t i_YCenter)
    {
        static_assert(2 == se::NeighborListStaticSETraits<nl>::Rank, "SE and view have not the same Rank");
        POUTRE_CHECK(i_vin.size() == o_vout.size(), "Incompatible views size");
        auto ibd = i_vin.bound();
        auto obd = o_vout.bound();
        auto istride = i_vin.stride();
        auto ostride = o_vout.stride();
        auto ysize = ibd[0];
        auto xsize = ibd[1];

        POUTRE_CHECK(ibd == obd, "bound not compatible");
        POUTRE_CHECK(istride == ostride, "stride not compatible");
        auto NlList = se::NeighborListStaticSETraits<nl>::NlListTransposed;
        auto NbNeighbor = se::NeighborListStaticSETraits<nl>::NbNeighbor;
        auto i_vinbeg = i_vin.data();
        auto o_voutbeg = o_vout.data();
        auto max = std::numeric_limits<T1>::lowest();
        idx2d center(i_YCenter,i_XCenter);
        for (auto idxNB = 0; idxNB < NbNeighbor; ++idxNB)
        {
            idx2d idxnl(NlList[idxNB]);
            scoord currentx = center[1] + idxnl[1];
            scoord currenty = center[0] + idxnl[0];
            if (currentx < 0) continue;
            if (currenty < 0) continue;
            if (currentx >= xsize) continue;
            if (currenty >= ysize) continue;
            max = std::max<T1>(max, i_vinbeg[xsize * currenty+ currentx]);
        }
        o_voutbeg[xsize * i_YCenter + i_XCenter] = static_cast<T2>(max);
    }
    template<se::NeighborListStaticSE nl, typename T1, typename T2>
    struct t_DilateOpispatcher<nl, T1, T2, 2, array_view, array_view>
    {
        static_assert(2 == se::NeighborListStaticSETraits<nl>::Rank, "SE and view have not the same Rank");
        void operator()(const array_view<T1, 2> & i_vin, array_view<T2, 2> & o_vout)
        {
            POUTRE_CHECK(i_vin.size() == o_vout.size(), "Incompatible views size");
            auto ibd = i_vin.bound();
            auto obd = o_vout.bound();
            auto istride = i_vin.stride();
            auto ostride = o_vout.stride();
            scoord ysize = ibd[0];
            scoord xsize = ibd[1];

            POUTRE_CHECK(ibd == obd, "bound not compatible");
            POUTRE_CHECK(istride == ostride, "stride not compatible");
            auto NlList = se::NeighborListStaticSETraits<nl>::NlListTransposed;
            auto NbNeighbor = se::NeighborListStaticSETraits<nl>::NbNeighbor;
            auto suroundingHalfSizeX= se::NeighborListStaticSETraits<nl>::surroundingSquareHalfSize[1];
            auto suroundingHalfSizeY = se::NeighborListStaticSETraits<nl>::surroundingSquareHalfSize[0];
            auto i_vinbeg = i_vin.data();
            auto o_voutbeg = o_vout.data();
            //handling the upper lines
            for (auto y = 0; y < suroundingHalfSizeY; ++y)
            {
                for (auto x = 0; x < xsize; ++x)
                {
                    t_DilateIterateArrayView2DHelper<nl,T1,T2>(i_vin,o_vout, x, y);
                }
            }
            //handling the lower lines
            for (auto y = ysize - suroundingHalfSizeY; y < ysize; ++y)
            {
                for (auto x = 0; x < xsize; ++x)
                {
                    t_DilateIterateArrayView2DHelper<nl,T1,T2>(i_vin, o_vout, x, y);
                }
            }
            //Main lines area
            for (auto y = suroundingHalfSizeY; y < ysize- suroundingHalfSizeY; ++y)
            {
                //handling the first columns
                for (auto x =0 ; x < suroundingHalfSizeX; ++x)
                {
                    t_DilateIterateArrayView2DHelper<nl, T1, T2>(i_vin, o_vout, x, y);
                }
                for (auto x = suroundingHalfSizeX; x < xsize- suroundingHalfSizeX; ++x)
                {
                    auto max = std::numeric_limits<T1>::lowest();
                    idx2d center(y,x);
                    //ver slow
                    //for (const auto& idxnl : NlList)
                    //{
                    //    auto currentidx = center+ idxnl;
                    //    max = std::max<T1>(max, i_vinbeg[xsize*currentidx[0]+ currentidx[1]]);
                    //}
                    /*for (const auto& idxnl : NlList)
                    {*/
                    for(auto idxNB=0;idxNB< NbNeighbor;++idxNB)
                    {
                        idx2d idxnl(NlList[idxNB]);
                        scoord currentx = center[1]+ idxnl[1];
                        scoord currenty = center[0] + idxnl[0];
                        max = std::max<T1>(max, i_vinbeg[xsize*currenty+ currentx]);
                    }
                    o_voutbeg[xsize*y+x] = static_cast<T2>(max);
                }
                //handling the last columns
                for (auto x = xsize- suroundingHalfSizeX; x <xsize ; ++x)
                {
                    t_DilateIterateArrayView2DHelper<nl, T1, T2>(i_vin, o_vout, x, y);
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
            auto NlList = se::NeighborListStaticSETraits<nl>::NlListTransposed;
            if (vInbound == vOutbound && stridevIN == stridevOut) //same bound + same stride -> one idx
            {
                auto beg1 = begin(vInbound);
                auto end1 = end(vInbound);
                for (; beg1 != end1; ++beg1)
                {
                    auto min = std::numeric_limits<T1>::max();
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
                    auto min = std::numeric_limits<T1>::max();
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