
//==============================================================================
//                  Copyright (c) 2015 - Thomas Retornaz                      //
//                     thomas.retornaz@mines-paris.org                        //
//          Distributed under the Boost Software License, Version 1.0.        //
//                 See accompanying file LICENSE.txt or copy at               //
//                     http://www.boost.org/LICENSE_1_0.txt                   //
//==============================================================================

#ifndef POUTRE_IMAGEPROCESSING_BINARYOP_HPP__
#define POUTRE_IMAGEPROCESSING_BINARYOP_HPP__

#ifndef POUTRE_IMAGEPROCESSINGTYPE_HPP__
#include <poutreImageProcessingCore/poutreImageProcessingType.hpp>
#endif

#ifndef POUTRE_TRACE__HPP__
#include <poutreBase/poutreTrace.hpp>
#endif

#ifndef POUTRE_CONTAINER_VIEW_HPP__
#include <poutreBase/poutreContainerView.hpp>
#endif

#include <algorithm>//transform
#include <simdpp/algorithm/transform.h>


namespace poutre
{
    namespace simd= simdpp::SIMDPP_ARCH_NAMESPACE;
    /****************************************************************************************/
    /*                               PixelWiseBinaryOp                                      */
    /****************************************************************************************/
    // primary use strided view
    template<typename T1, typename T2, typename Tout, ptrdiff_t Rank, template <typename, ptrdiff_t> class View1, template <typename, ptrdiff_t> class View2, template <typename, ptrdiff_t> class ViewOut,class BinOp>
    struct PixelWiseBinaryOpDispatcher
    {
		static_assert((
			std::is_same< View1<T1, Rank>, strided_array_view<T1, Rank> >::value
			|| std::is_same< View1<T1, Rank>, strided_array_view<const T1, Rank> >::value
			|| std::is_same< View2<T2, Rank>, strided_array_view<T2, Rank> >::value
			|| std::is_same< View2<T2, Rank>, strided_array_view<const T2, Rank> >::value
			|| std::is_same< ViewOut<Tout, Rank>, strided_array_view<Tout, Rank> >::value
			), "strided view only specialization fail for arrayview");

        void operator()(const View1<T1, Rank>& i_vin1,const BinOp& op,const View2<T2, Rank>& i_vin2, ViewOut<Tout, Rank>& o_vout) const
        {
			//std::cout << "\n" << "call PixelWiseBinaryOpDispatcher strided view";

            //More runtime dispatch
            auto vInbound1 = i_vin1.bound();
            auto vInbound2 = i_vin2.bound();
            auto vOutbound = o_vout.bound();
            auto stridevIN1 = i_vin1.stride();
            auto stridevIN2 = i_vin2.stride();
            auto stridevOut = o_vout.stride();

            if (vInbound1 == vOutbound && vInbound2 == vOutbound && stridevIN1 == stridevOut && stridevIN2 == stridevOut) //same bound + same stride -> one idx
            {
                auto beg1 = begin(vInbound1);
                auto end1 = end(vInbound1);
                for (; beg1 != end1; ++beg1)
                {
                    o_vout[*beg1] = static_cast<Tout>(op(i_vin1[*beg1], i_vin2[*beg1]));
                }

                //TODO stride==1 in least significant dimension
            }
            else //default three idx
            {
                auto beg1 = begin(vInbound1);
                auto end1 = end(vInbound1);
                auto beg2 = begin(vInbound2);
                auto beg3 = begin(vOutbound);

                for (; beg1 != end1; ++beg1, ++beg2, ++beg3)
                {
                    o_vout[*beg3] = static_cast<Tout>(op(i_vin1[*beg1], i_vin2[*beg2]));
                }
            }
        }
    };

    //template specialization both array_view
    template<typename T1, typename T2, typename Tout, ptrdiff_t Rank,class BinOp>
    struct PixelWiseBinaryOpDispatcher<T1, T2, Tout, Rank, array_view, array_view, array_view, BinOp>
    {

        void operator()(const array_view<T1, Rank>& i_vin1,const BinOp& op,const array_view<T2, Rank>& i_vin2, array_view<Tout, Rank>& o_vout) const
        {
			//std::cout << "\n" << "call PixelWiseBinaryOpDispatcher array view template specialization array view ptr";

            auto i_vinbeg1 = i_vin1.data();
            auto i_vinend1 = i_vin1.data() + i_vin1.size();
            auto i_vinbeg2 = i_vin2.data();
            auto i_voutbeg = o_vout.data();
            for (; i_vinbeg1 != i_vinend1; ++i_vinbeg1, ++i_vinbeg2, ++i_voutbeg)
            {
                *i_voutbeg = static_cast<Tout>(op(*i_vinbeg1, *i_vinbeg2));
            }
        }

    };

    template<typename T1, typename T2, typename Tout, ptrdiff_t Rank, template <typename, ptrdiff_t> class View1, template <typename, ptrdiff_t> class View2, template <typename, ptrdiff_t> class View3,class BinOp>
    void PixelWiseBinaryOp(const View1<T1, Rank>& i_vin1,const BinOp& op,const View2<T2, Rank>& i_vin2, View3<Tout, Rank>& o_vout)
    {
        POUTRE_CHECK(i_vin1.size() == i_vin2.size(), "Incompatible views size");
        POUTRE_CHECK(o_vout.size() == i_vin2.size(), "Incompatible views size");
        PixelWiseBinaryOpDispatcher<T1, T2, Tout, Rank, View1, View2, View3, BinOp> dispatcher;
        dispatcher(i_vin1,op,i_vin2, o_vout);
    }

    /****************************************************************************************/
    /*                               PixelWiseBinaryOpWithTag                               */
    /****************************************************************************************/

    // primary use strided view
    template<typename T1, typename T2, typename Tout, ptrdiff_t Rank, template <typename, ptrdiff_t> class View1, template <typename, ptrdiff_t> class View2, template <typename, ptrdiff_t> class ViewOut, template <typename, typename, typename, class TAG> class BinOp,typename=void>
    struct PixelWiseBinaryOpDispatcherWithTag
    {
		static_assert((
			std::is_same< View1<T1, Rank>, strided_array_view<T1, Rank> >::value
			|| std::is_same< View1<T1, Rank>, strided_array_view<const T1, Rank> >::value
			|| std::is_same< View2<T2, Rank>, strided_array_view<T2, Rank> >::value
			|| std::is_same< View2<T2, Rank>, strided_array_view<const T2, Rank> >::value
			|| std::is_same< ViewOut<Tout, Rank>, strided_array_view<Tout, Rank> >::value
			), "strided view only specilization fail for arrayview");


        void operator()(const View1<T1, Rank>& i_vin1, const View2<T2, Rank>& i_vin2, ViewOut<Tout, Rank>& o_vout) const
        {
            //get the specialized operator
            using real_op = /*typename*/ BinOp<T1, T2, Tout, tag_SIMD_disabled>;
            real_op op;
			//std::cout << "\n" << "call PixelWiseBinaryOpDispatcherWithTag strided view";
            //More runtime dispatch
            auto vInbound1 = i_vin1.bound();
            auto vInbound2 = i_vin2.bound();
            auto vOutbound = o_vout.bound();
            auto stridevIN1 = i_vin1.stride();
            auto stridevIN2 = i_vin2.stride();
            auto stridevOut = o_vout.stride();

            if (vInbound1 == vOutbound && vInbound2 == vOutbound && stridevIN1 == stridevOut && stridevIN2 == stridevOut) //same bound + same stride -> one idx
            {
                auto beg1 = begin(vInbound1);
                auto end1 = end(vInbound1);
                for (; beg1 != end1; ++beg1)
                {
                    o_vout[*beg1] = static_cast<Tout>(op(i_vin1[*beg1], i_vin2[*beg1]));
                }

                //TODO stride==1 in least significant dimension
            }
            else //default three idx
            {
                auto beg1 = begin(vInbound1);
                auto end1 = end(vInbound1);
                auto beg2 = begin(vInbound2);
                auto beg3 = begin(vOutbound);

                for (; beg1 != end1; ++beg1, ++beg2, ++beg3)
                {
                    o_vout[*beg3] = static_cast<Tout>(op(i_vin1[*beg1], i_vin2[*beg2]));
                }
            }
        }
    };

    //template specialization both array_view but different type
    template<typename T1, typename T2, typename Tout, ptrdiff_t Rank, template <typename, typename, typename, class TAG> class BinOp>
    struct PixelWiseBinaryOpDispatcherWithTag<T1, T2, Tout, Rank, array_view, array_view, array_view, BinOp,
		std::enable_if_t<
		!std::is_same<std::remove_const_t<T1>, std::remove_const_t<T2>>::value || !std::is_same<std::remove_const_t<T2>, std::remove_const_t<Tout>>::value
		>
	>
    {
        void operator()(const array_view<T1, Rank>& i_vin1, const array_view<T2, Rank>& i_vin2, array_view<Tout, Rank>& o_vout) const
        {
            //get the specialized operator
            using real_op = /*typename*/ BinOp<T1, T2, Tout, tag_SIMD_disabled>;
            real_op op;
			//std::cout << "\n" << "call PixelWiseBinaryOpDispatcherWithTag array view template specialization same type,fall back ptr";
            auto i_vinbeg1 = i_vin1.data();
            auto i_vinend1 = i_vin1.data() + i_vin1.size();
            auto i_vinbeg2 = i_vin2.data();
            auto i_voutbeg = o_vout.data();
            for (; i_vinbeg1 != i_vinend1; ++i_vinbeg1, ++i_vinbeg2, ++i_voutbeg)
            {
                *i_voutbeg = static_cast<Tout>(op(*i_vinbeg1, *i_vinbeg2));
            }
        }

    };

    //template specialization both array_view and same type, use simd counterpart
    template<typename T1, typename T2, typename Tout, ptrdiff_t Rank, template <typename, typename, typename, class TAG> class BinOp>
    struct PixelWiseBinaryOpDispatcherWithTag<T1, T2, Tout, Rank, array_view, array_view, array_view, BinOp,std::enable_if_t<
		std::is_same<std::remove_const_t<T1>, std::remove_const_t<T2>>::value && std::is_same<std::remove_const_t<T2>, std::remove_const_t<Tout>>::value && std::is_arithmetic<T1>::value
	>
	>
    {

        void operator()(const array_view<T1, Rank>& i_vin1, const array_view<T2, Rank>& i_vin2, array_view<Tout, Rank>& o_vout) const
        {
            //get the specialized operator
            using real_op = /*typename*/ BinOp<T1, T1, T1, tag_SIMD_enabled>;
            real_op op;
			//std::cout << "\n" << "call PixelWiseBinaryOpDispatcherWithTag array view template specialization same type,fall back ptr + SIMD";
            auto i_vinbeg1 = i_vin1.data();
            auto i_vinend1 = i_vin1.data() + i_vin1.size();
            auto i_vinbeg2 = i_vin2.data();
            auto i_voutbeg = o_vout.data();
            simd::transform(i_vinbeg1, i_vinend1, i_vinbeg2, i_voutbeg, op);
        }

    };

    template<typename T1, typename T2, typename Tout, ptrdiff_t Rank, template <typename, ptrdiff_t> class View1, template <typename, ptrdiff_t> class View2, template <typename, ptrdiff_t> class View3, template <typename, typename, typename, class TAG> class BinOp>
    void PixelWiseBinaryOp(const View1<T1, Rank>& i_vin1, const View2<T2, Rank>& i_vin2, View3<Tout, Rank>& o_vout)
    {
        POUTRE_CHECK(i_vin1.size() == i_vin2.size(), "Incompatible views size");
        POUTRE_CHECK(o_vout.size() == i_vin2.size(), "Incompatible views size");
        PixelWiseBinaryOpDispatcherWithTag<T1, T2, Tout, Rank, View1, View2, View3, BinOp> dispatcher;
        dispatcher(i_vin1, i_vin2, o_vout);
    }



} //namespace poutre
#endif //POUTRE_IMAGEPROCESSING_BINARYOP_HPP__
