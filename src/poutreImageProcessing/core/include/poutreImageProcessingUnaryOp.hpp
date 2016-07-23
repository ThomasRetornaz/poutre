
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

#include <algorithm>//transform
#include <boost/simd/sdk/simd/algorithm.hpp>//"simd" transform



//move this in a dedicated here in PoutreCore
namespace poutre
{
    namespace bs = boost::simd;

    /****************************************************************************************/
   /*                               UnaryOp                                                */
   /****************************************************************************************/

    template<typename T1, typename T2, ptrdiff_t Rank, template <typename, ptrdiff_t> class View1, template <typename, ptrdiff_t> class View2, class UnOp>
    void UnaryOp(const View1<T1, Rank>& i_vin, View2<T2, Rank>& o_vout, UnOp& op)
    {
        POUTRE_CHECK(i_vin.size() == o_vout.size(), "Incompatible views size");
        UnaryOpDispatcher<T1, T2, Rank, View1, View2, UnOp> dispatcher;
        dispatcher(i_vin, o_vout, op);
    }

    // primary use strided view 
    template<typename T1, typename T2, ptrdiff_t Rank, template <typename, ptrdiff_t> class View1, template <typename, ptrdiff_t> class View2, class UnOp>
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
                //std::cout << "\n" << "call UnaryOpDispatcher one idx";
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
                //std::cout << "\n" << "call UnaryOpDispatcher primary template default two idx";
                auto beg1 = begin(vInbound);
                auto end1 = end(vInbound);
                auto beg2 = begin(vOutbound);
                for (; beg1 != end1; ++beg1, ++beg2)
                {
                    o_vout[*beg2] = op(i_vin[*beg1]);
                }
            }
        }
    };

    //template specialization both array_view but different type
    template<typename T1, typename T2, ptrdiff_t Rank, class UnOp>
    struct UnaryOpDispatcher<T1, T2, Rank, array_view, array_view, UnOp>
    {

        void operator()(const array_view<T1, Rank>& i_vin, array_view<T2, Rank>& o_vout, UnOp& op) const
        {
            //std::cout << "\n" << "call UnaryOpDispatcher array view template specialization,fall back ptr";
            auto i_vinbeg = i_vin.data();
            auto i_vinend = i_vin.data() + i_vin.size();
            auto i_voutbeg = o_vout.data();
           /* for (; i_vinbeg != i_vinend; i_vinbeg++, i_voutbeg++)
            {
                *i_voutbeg = op(*i_vinbeg);
            }*/
        }

    };

    //template specialization both array_view and same type
    template<typename T, ptrdiff_t Rank, class UnOp>
    struct UnaryOpDispatcher<T, T, Rank, array_view, array_view, UnOp>
    {

        void operator()(const array_view<T, Rank>& i_vin, array_view<T, Rank>& o_vout, UnOp& op) const
        {
            //std::cout << "\n" << "call UnaryOpDispatcher array view template specialization same type,fall back ptr + SIMD";

            auto i_vinbeg = i_vin.data();
            auto i_vinend = i_vin.data() + i_vin.size();
            auto i_voutbeg = o_vout.data();
            bs::transform(i_vinbeg, i_vinend, i_voutbeg, op);
        }

    };

}//namespace poutre

#endif//POUTRE_IMAGEPROCESSING_UNARYOP_HPP__