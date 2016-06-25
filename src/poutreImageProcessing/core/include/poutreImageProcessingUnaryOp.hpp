
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

#ifndef POUTRE_CONTAINER_VIEW_HPP__
#include <poutreBase/poutreContainerView.hpp>
#endif

#include <boost/simd/sdk/simd/native.hpp>
#include <boost/simd/include/functions/aligned_load.hpp>
#include <boost/simd/include/functions/aligned_store.hpp>
#include <boost/simd/include/functions/load.hpp>
#include <boost/simd/include/functions/splat.hpp>
#include <boost/simd/memory/align_on.hpp>
#include <boost/simd/memory/is_aligned.hpp>
#include <boost/simd/sdk/simd/algorithm.hpp>
#include <boost/mpl/assert.hpp>
#include <boost/assert.hpp>
#include <algorithm>
#include <boost/config.hpp>

//move this in a dedicated here in PoutreCore
namespace poutre
{
  /*  template<class view_type>
    using is_strided = IsStrided< view_type>::value;
*/
	namespace bs = boost::simd;

   /* template<class view_type>
    using target_type = std::conditional< is_strided::value, view_type::value_type, bs::simd::pack<view_type::value_type>>;*/

     /****************************************************************************************/
    /*                               UnaryOp                                                */
    /****************************************************************************************/

    template<typename T1, typename T2, ptrdiff_t Rank, template <typename, ptrdiff_t> class View1, template <typename, ptrdiff_t> class View2, class UnOp>
    void UnaryOp(const View1<T1, Rank>& i_vin, View2<T2, Rank>& o_vout, UnOp& op)
    {
        POUTRE_CHECK(i_vin.size() == o_vout.size(), "Incompatible views size");
        UnaryOpDispatcher<T1,T2, Rank, View1, View2, UnOp> dispatcher;
        dispatcher(i_vin, o_vout, op);
    }

    // primary use strided view 
    template<typename T1, typename T2, ptrdiff_t Rank, template <typename, ptrdiff_t> class View1, template <typename, ptrdiff_t> class View2,class UnOp>
    struct UnaryOpDispatcher
    {
        void operator()(const View1<T1, Rank>& i_vin, View2<T2, Rank>& o_vout, UnOp& op) const
        {
            //More runtime dispatch
            auto vInbound = i_vin.bound();
            auto vOutbound = o_vout.bound();
            auto stridevIN = i_vin.stride();
            auto stridevOut = o_vout.stride();

            if (vInbound == vOutbound && stridevIN == stridevOut) //same bound + same stride -> one idx 
            {
                //if (stridevIN[Rank - 1] == 1)  //same bound + same stride + stride equal 1 in less significant 
                //{
                //    std::cout << "\n" << "call UnaryOpDispatcher one idx stride equal 1 in less significant dimention";
                //}
                //else
                //{
                    std::cout << "\n" << "call UnaryOpDispatcher one idx";
                    auto beg1 = begin(vInbound);
                    auto end1 = end(vInbound);                  
                    for (; beg1 != end1; ++beg1)
                    {
                        o_vout[*beg1] = op(i_vin[*beg1]);
                    }
                //}
            }
            else //default two idx
            {
                std::cout << "\n" << "call UnaryOpDispatcher primary template default two idx";
                auto beg1 = begin(vInbound);
                auto end1 = end(vInbound);
                auto beg2 = begin(vOutbound);
                for (; beg1 != end1; ++beg1, ++beg2)
                {
                    o_vout[*beg2]=op(i_vin[*beg1]);
                }
            }
        }
    };

    //template specialization both array_view but different type
    template<typename T1, typename T2, ptrdiff_t Rank, class UnOp>
    struct UnaryOpDispatcher<T1,T2, Rank, array_view, array_view, UnOp>
    {

        void operator()(const array_view<T1, Rank>& i_vin, array_view<T2, Rank>& o_vout, UnOp& op) const
        {
            std::cout << "\n" << "call UnaryOpDispatcher array view template specialization,fall back ptr";
            auto i_vinbeg = i_vin.data();
            auto i_vinend = i_vin.data()+i_vin.size();
            auto i_voutbeg = o_vout.data();
            for (; i_vinbeg != i_vinend; i_vinbeg++, i_voutbeg++)
            {
                *i_voutbeg = op(*i_vinbeg);
            }
        }

    };

    //template specialization both array_view and same type
    template<typename T,ptrdiff_t Rank, class UnOp>
    struct UnaryOpDispatcher<T,T, Rank, array_view,array_view, UnOp>
    {

        void operator()(const array_view<T, Rank>& i_vin, array_view<T, Rank>& o_vout, UnOp& op) const
        {
            std::cout << "\n" << "call UnaryOpDispatcher array view template specialization same type,fall back ptr + SIMD";
            
            auto i_vinbeg = i_vin.data();
            auto i_vinend = i_vin.data() + i_vin.size();
            auto i_voutbeg = o_vout.data();
            //boost::simd::transform(i_vinbeg, i_vinend, i_voutbeg, op());
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
}//namespace poutre

#endif//POUTRE_IMAGEPROCESSING_UNARYOP_HPP__