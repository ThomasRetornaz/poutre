
//==============================================================================
//                  Copyright (c) 2015 - Thomas Retornaz                      //
//                     thomas.retornaz@mines-paris.org                        //
//          Distributed under the Boost Software License, Version 1.0.        //
//                 See accompanying file LICENSE.txt or copy at               //
//                     http://www.boost.org/LICENSE_1_0.txt                   //
//==============================================================================

#ifndef POUTRE_IMAGEPROCESSING_TERNARYOP_HPP__
#define POUTRE_IMAGEPROCESSING_TERNARYOP_HPP__

#ifndef POUTRE_IMAGEPROCESSINGCORE_HPP__
#include <poutreImageProcessingCore/poutreImageProcessingCore.hpp>
#endif

#ifndef POUTRE_IMAGEPROCESSINGTYPE_HPP__
#include <poutreImageProcessingCore/poutreImageProcessingType.hpp>
#endif

namespace poutre
{

    /****************************************************************************************/
    /*                               PixelWiseTernaryOp                                      */
    /****************************************************************************************/

    // primary use strided view
    template<typename T1, typename T2, typename T3, typename Tout, ptrdiff_t Rank,
        template <typename, ptrdiff_t> class View1,
        template <typename, ptrdiff_t> class View2,
        template <typename, ptrdiff_t> class View3,
        template <typename, ptrdiff_t> class ViewOut,
        class TerOp>
    struct PixelWiseTernaryOpDispatcher
    {
		static_assert((
			std::is_same< View1<T1, Rank>, strided_array_view<T1, Rank> >::value
			|| std::is_same< View1<T1, Rank>, strided_array_view<const T1, Rank> >::value
			|| std::is_same< View2<T2, Rank>, strided_array_view<T2, Rank> >::value
			|| std::is_same< View2<T2, Rank>, strided_array_view<const T2, Rank> >::value
			|| std::is_same< View2<T3, Rank>, strided_array_view<T3, Rank> >::value
			|| std::is_same< View2<T3, Rank>, strided_array_view<const T3, Rank> >::value
			|| std::is_same< ViewOut<Tout, Rank>, strided_array_view<Tout, Rank> >::value
			), "strided view only specilization fail for arrayview");

        void operator()(const View1<T1, Rank>& i_vin1, const TerOp& op, const View2<T2, Rank>& i_vin2, const View3<T3, Rank>& i_vin3, ViewOut<Tout, Rank>& o_vout) const
        {
            //More runtime dispatch
            auto vInbound1 = i_vin1.bound();
            auto vInbound2 = i_vin2.bound();
            auto vInbound3 = i_vin3.bound();
            auto vOutbound = o_vout.bound();
            auto stridevIN1 = i_vin1.stride();
            auto stridevIN2 = i_vin2.stride();
            auto stridevIN3 = i_vin3.stride();
            auto stridevOut = o_vout.stride();

            if (vInbound1 == vOutbound && vInbound2 == vOutbound && vInbound1 == vInbound3 && stridevIN1 == stridevOut && stridevIN2 == stridevOut && stridevIN1 == stridevIN3) //same bound + same stride -> one idx
            {
                auto beg1 = begin(vInbound1);
                auto end1 = end(vInbound1);
                for (; beg1 != end1; ++beg1)
                {
                    o_vout[*beg1] = static_cast<Tout>(op(i_vin1[*beg1], i_vin2[*beg1], i_vin3[*beg1]));
                }

                //TODO stride==1 in least significant dimension
            }
            else //default four idx
            {
                //std::cout << "\n" << "call UnaryOpDispatcher primary template default two idx";
                auto beg1 = begin(vInbound1);
                auto end1 = end(vInbound1);
                auto beg2 = begin(vInbound2);
                auto beg3 = begin(vInbound3);
                auto begout = begin(vOutbound);

                for (; beg1 != end1; ++beg1, ++beg2, ++beg3, ++begout)
                {
                    o_vout[*begout] = static_cast<Tout>(op(i_vin1[*beg1], i_vin2[*beg2], i_vin3[*beg3]));
                }
            }
        }
    };

    //template specialization both array_view but different type
    template<typename T1, typename T2, typename T3, typename Tout, ptrdiff_t Rank,class TerOp>
    struct PixelWiseTernaryOpDispatcher<T1, T2, T3, Tout, Rank, array_view, array_view, array_view, array_view, TerOp>
    {

        void operator()(const array_view<T1, Rank>& i_vin1, const TerOp& op, const array_view<T2, Rank>& i_vin2, const array_view<T3, Rank>& i_vin3, array_view<Tout, Rank>& o_vout) const
        {
            auto i_vinbeg1 = i_vin1.data();
            auto i_vinend1 = i_vin1.data() + i_vin1.size();
            auto i_vinbeg2 = i_vin2.data();
            auto i_vinbeg3 = i_vin3.data();
            auto i_voutbeg = o_vout.data();
            for (; i_vinbeg1 != i_vinend1; ++i_vinbeg1, ++i_vinbeg2, ++i_vinbeg3, ++i_voutbeg)
            {
                *i_voutbeg = static_cast<Tout>(op(*i_vinbeg1, *i_vinbeg2, *i_vinbeg3));
            }
        }

    };

    template<typename T1, typename T2, typename T3, typename Tout, ptrdiff_t Rank,
        template <typename, ptrdiff_t> class View1,
        template <typename, ptrdiff_t> class View2,
        template <typename, ptrdiff_t> class View3,
        template <typename, ptrdiff_t> class ViewOut,
        class TerOp>
    void PixelWiseTernaryOp(const View1<T1, Rank>& i_vin1,const TerOp& op,const View2<T2, Rank>& i_vin2, const View3<T3, Rank>& i_vin3, ViewOut<Tout, Rank>& o_vout) POUTRE_NOEXCEPTONLYNDEBUG
    {
        POUTRE_ASSERTCHECK(i_vin1.size() == i_vin2.size(), "Incompatible views size");
        POUTRE_ASSERTCHECK(i_vin2.size() == i_vin3.size(), "Incompatible views size");
        POUTRE_ASSERTCHECK(o_vout.size() == i_vin3.size(), "Incompatible views size");
        PixelWiseTernaryOpDispatcher<T1, T2, T3, Tout, Rank, View1, View2, View3, ViewOut, TerOp> dispatcher;
        dispatcher(i_vin1,op,i_vin2, i_vin3, o_vout);
    }

}//namespace

#endif //POUTRE_IMAGEPROCESSING_TERNARYOP_HPP__
