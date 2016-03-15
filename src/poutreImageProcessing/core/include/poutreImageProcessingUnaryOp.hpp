
//==============================================================================
//                  Copyright (c) 2015 - Thomas Retornaz                      //
//                     thomas.retornaz@mines-paris.org                        //
//          Distributed under the Boost Software License, Version 1.0.        //
//                 See accompanying file LICENSE.txt or copy at               //
//                     http://www.boost.org/LICENSE_1_0.txt                   //
//==============================================================================

#ifndef POUTRE_IMAGEPROCESSING_UNARYOP_HPP__
#define POUTRE_IMAGEPROCESSING_UNARYOP_HPP__

#ifndef POUTRE_IMAGEPROCESSING_HPP__
#include <poutreImageProcessing/poutreImageProcessing.hpp>
#endif

#ifndef POUTRE_IMAGEPROCESSINGTYPE_HPP__
#include <poutreImageProcessing/core/poutreImageProcessingType.hpp>
#endif

#ifndef POUTRE_TRACE__HPP__
#include <poutreBase/poutreTrace.hpp>
#endif


#include <boost/simd/sdk/simd/native.hpp>
#include <boost/simd/include/functions/aligned_load.hpp>
#include <boost/simd/include/functions/aligned_store.hpp>
#include <boost/simd/include/functions/load.hpp>
#include <boost/simd/include/functions/splat.hpp>
#include <boost/simd/memory/align_on.hpp>
#include <boost/simd/memory/is_aligned.hpp>
#include <boost/mpl/assert.hpp>
#include <boost/assert.hpp>
#include <algorithm>
#include <boost/config.hpp>




//move this in a dedicated here in PoutreCore
namespace poutre
{

	namespace bs = boost::simd;

	enum class DispatchView {
		DispatchViewUndef = 0, //!< Undefined dispatch
		DispatchViewCompatibleOffsetSamePtrType =	1 << 0, //!<Ptr arthimetic compatible
		DispatchViewCompatibleOffset =		1 << 1, //!< Offset Compatible. Iterate with one iterator
		// DispatchViewCompatibleArrayView =
		// 1 << 2, //!< Ptr Compatible. Iterate with two ptr
		DispatchViewAtLeastOneIsAStridedView =	1 << 3, //!< One is strided fallback to index iteration
	};

	std::ostream& operator<<(std::ostream& os, DispatchView dispatchview)
	{
		switch (dispatchview)
		{
		case DispatchView::DispatchViewCompatibleOffsetSamePtrType:
			os << "DispatchViewCompatibleOffsetSamePtrType";
			break;
		case DispatchView::DispatchViewCompatibleOffset:
			os << "DispatchViewCompatibleOffset";
			break;
			//case DispatchView::DispatchViewCompatibleArrayView:
			//  os << "DispatchViewCompatibleArrayView";
			//  break;
		case DispatchView::DispatchViewAtLeastOneIsAStridedView:
			os << "DispatchViewAtLeastOneIsAStridedView";
			break;
		default://  enumDispatchView::DispatchViewUndef; 
			os << "Unknown dispatchView";
			break;
		}
		return os;
	}

	std::istream& operator>>(std::istream& is, DispatchView& dispatchview)
	{
		dispatchview = DispatchView::DispatchViewUndef;

		if (!is.good())
			return is;

		std::string strType;
		is >> strType;
		if (is.bad())
			return is;
		if (strType == "DispatchViewCompatibleOffsetSamePtrType")
			dispatchview = DispatchView::DispatchViewCompatibleOffsetSamePtrType;
		else if (strType == "DispatchViewCompatibleOffset")
			dispatchview = DispatchView::DispatchViewCompatibleOffset;
		//else if (strType == "DispatchViewCompatibleArrayView")
		//  dispatchview = DispatchView::DispatchViewCompatibleArrayView;
		else if (strType == "DispatchViewAtLeastOneIsAStridedView")
			dispatchview = DispatchView::DispatchViewAtLeastOneIsAStridedView;
		else
		{
			POUTRE_RUNTIME_ERROR("Unable to read dispatchview from stream");
		}
		return is;
	}

	template <poutre::DispatchView dispatchview>
	struct pApplyImageUnaryViewOp_Specialize
	{
	};

	template <>
	struct pApplyImageUnaryViewOp_Specialize<DispatchView::DispatchViewCompatibleOffsetSamePtrType>
	{

		//template <class UnOp,class ViewInOut>
		template <class UnOp, class ViewIn,class ViewOut>
		void operator()(UnOp& op,const ViewIn& vIn, ViewOut& vOut) const
		{

			std::cout << "\n" << "call pApplyImageUnaryViewOp<DispatchView::DispatchViewCompatibleOffsetSamePtrType>";
		}
	};


	template <>
	struct pApplyImageUnaryViewOp_Specialize<DispatchView::DispatchViewCompatibleOffset>
	{

		template <class UnOp,class ViewIn, class ViewOut>
		void operator()(UnOp& op,const ViewIn vIn, ViewOut vOut) const
		{
			std::cout << "\n" << "call pApplyImageUnaryViewOp<DispatchView::DispatchViewCompatibleOffset>";
		}
	};

	template <>
	struct pApplyImageUnaryViewOp_Specialize<DispatchView::DispatchViewAtLeastOneIsAStridedView>
	{

		template <class UnOp, class ViewIn, class ViewOut>
		void operator()(UnOp& op, const ViewIn vIn, ViewOut vOut) const
		{
			std::cout << "\n" << "call pApplyImageUnaryViewOp<DispatchView::DispatchViewAtLeastOneIsAStridedView>";
		}
	};


	struct pApplyImageUnaryViewOp_dispatcher
	{

		template <class UnOp,class ViewIn, class ViewOut>
		void operator()(UnOp& op, const ViewIn vIn, ViewOut vOut) const
		{
			//debug precondition
			POUTRE_ASSERTCHECK(vIn.size() == vOut.size(), "pApplyImageUnaryIterOp size of view are not compatible");

			auto vInbound = vIn.bound();
			auto vOutbound = vOut.bound();
			auto stridevIN = vIn.stride();
			auto stridevOut = vOut.stride();

			//FIXME A REPRENDRE
			if ((vInbound == vOutbound) && (stridevIN == stridevOut))
			{
				if (std::is_same<ViewIn, ViewOut>::value)
				{
					//fallback ptr + simd 
					pApplyImageUnaryViewOp_Specialize<poutre::DispatchView::DispatchViewCompatibleOffsetSamePtrType>()(op,vIn, vOut);
				}
				else
				{
					//fallback different ptr type
					pApplyImageUnaryViewOp_Specialize<poutre::DispatchView::DispatchViewCompatibleOffset>()(op, vIn, vOut);
				}
			}
			else
			{
				pApplyImageUnaryViewOp_Specialize<poutre::DispatchView::DispatchViewAtLeastOneIsAStridedView>()(op,vIn, vOut);
			}
		}

	};

	/*
	//stolen from <boost/simd/sdk/simd/algorithm.hpp>
	/*
	template<PType ptypeIn, PType ptypeOut, class UnOp>
	struct pImageUnaryIterOp
	{
		using value_typeIn = TypeTraits<ptypeIn>::storage_type;
		using value_typeOut = TypeTraits<ptypeOut>::storage_type;
		using pointerin = value_typeIn*;
		using pointerout = value_typeOut*;
		using const_pointerin = const value_typeIn *;
		//using const_pointerout = const value_typeOut *;
		using bsvalue_typeIn = bs::pack < value_typeIn >;
		using bsvalue_typeOut = bs::pack < value_typeOut >;

		void operator()(const_pointerin beginin, const_pointerin endin, pointerout beginout, UnOp f) const
		{
			BOOST_MPL_ASSERT_MSG(bsvalue_typeIn::static_size == bsvalue_typeOut::static_size
				, BOOST_SIMD_TRANSFORM_INPUT_OUTPUT_NOT_SAME_SIZE
				, (value_typeIn, value_typeOut)
				);
			static const std::size_t N = bsvalue_typeIn::static_size;

			const std::size_t shift = simd::align_on(beginout, N * sizeof(value_typeOut)) - beginout;
			const_pointerin end2 = beginin + std::min<size_t>(shift, endin - beginin);
			const_pointerin end3 = end2 + (endin - end2) / N*N;

			// prologue
			for (; beginin != end2; ++beginin, ++beginout)
				*beginout = f(*beginin);

			//main loop
			//TODO UNROLL ?
			if (simd::is_aligned(beginin, N * sizeof(value_typeIn)) && simd::is_aligned(beginout, N * sizeof(value_typeOut)))
			{
				for (; beginin != end3; beginin += N, beginout += N)
					simd::aligned_store(f(simd::aligned_load<value_typeIn>(begin1)), beginout);
			}
			else
			{
				for (; beginin != end3; beginin += N, beginout += N)
					simd::store(f(simd::load<value_typeIn>(begin1)), beginout);
			}

			// epilogue
			for (; beginin != end; ++beginin, ++beginout)
				*beginout = f(*beginin);
		}
	};



	//more generic version
	template <class tag>
	struct pApplyImageUnaryIterOp
	{
		template <class UnOp, class IterIn, class IterOut, class ImageIn, class ImageOut>
		void operator()(op_& op, IterIn it, const IterIn ite, IterOut itout, ImageIn&, ImageOut&) const
		{
			for (; it != ite; ++it, ++itout)
			{
				*itout = op(*it);
			}
		}
	};
	*/
}

#endif//POUTRE_IMAGEPROCESSING_UNARYOP_HPP__