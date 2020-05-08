
//==============================================================================
//                  Copyright (c) 2015 - Thomas Retornaz                      //
//                     thomas.retornaz@mines-paris.org                        //
//          Distributed under the Boost Software License, Version 1.0.        //
//                 See accompanying file LICENSE.txt or copy at               //
//                     http://www.boost.org/LICENSE_1_0.txt                   //
//==============================================================================

#ifndef POUTRE_IMAGEPROCESSING_UNARYOP_HPP__
#define POUTRE_IMAGEPROCESSING_UNARYOP_HPP__
/**
 * @file poutreImageProcessingUnaryOp.hpp
 * @author thomas.retornaz@mines-paris.org
 * @brief Genneric unary operations over images or views
 * @version 0.1
 * @date 2020-05-08
 *
 * @copyright Copyright (c) 2020
 *
 */
#ifndef POUTRE_IMAGEPROCESSINGCORE_HPP__
#include <poutreImageProcessingCore/poutreImageProcessingCore.hpp>
#endif

#ifndef POUTRE_IMAGEPROCESSINGTYPE_HPP__
#include <poutreImageProcessingCore/poutreImageProcessingType.hpp>
#endif

#ifndef POUTRE_CONTAINER_VIEW_HPP__
#include <poutreBase/poutreContainerView.hpp>
#endif

#include <algorithm> //transform

#ifndef POUTRE_SIMD_ALGORITHM_HPP__
#include <poutreBase/include/poutreSimdAlgorithm.hpp> //simd transform
#endif

// move this in a dedicated here in PoutreCore
namespace poutre
{
    /**
     * @addtogroup image_processing_linear_group
     *@{
     */

    /****************************************************************************************/
    /*                               PixelWiseUnaryOp                                        */
    /****************************************************************************************/
    // primary use strided view
    template <typename TIn, typename TOut, ptrdiff_t Rank, template <typename, ptrdiff_t> class ViewIn,
              template <typename, ptrdiff_t> class ViewOut, class UnOp, typename = void>
    struct PixelWiseUnaryOpDispatcher
    {
        static_assert((std::is_same<ViewIn<TIn, Rank>, strided_array_view<TIn, Rank>>::value ||
                       std::is_same<ViewIn<TIn, Rank>, strided_array_view<const TIn, Rank>>::value ||
                       std::is_same<ViewOut<TOut, Rank>, strided_array_view<TOut, Rank>>::value),
                      "strided view only specilization fail for arrayview");

        void operator()(const ViewIn<TIn, Rank> &i_vin, UnOp op, ViewOut<TOut, Rank> &o_vout) const
        {
            // std::cout << "\n" << "call PixelWiseUnaryOpDispatcher strided view";

            // More runtime dispatch
            auto vInbound = i_vin.bound();
            auto vOutbound = o_vout.bound();
            auto stridevIN = i_vin.stride();
            auto stridevOut = o_vout.stride();

            if (vInbound == vOutbound && stridevIN == stridevOut) // same bound + same stride -> one idx
            {
                // std::cout << "\n" << "call UnaryOpDispatcher one idx";
                auto beg1 = begin(vInbound);
                auto end1 = end(vInbound);
                for (; beg1 != end1; ++beg1)
                {
                    o_vout[*beg1] = static_cast<TOut>(op(i_vin[*beg1]));
                }
                // TODO stride==1 in least significant dimension
            }
            else // default two idx
            {
                // std::cout << "\n" << "call UnaryOpDispatcher primary template default two idx";
                auto beg1 = begin(vInbound);
                auto end1 = end(vInbound);
                auto beg2 = begin(vOutbound);
                for (; beg1 != end1; ++beg1, ++beg2)
                {
                    o_vout[*beg2] = static_cast<TOut>(op(i_vin[*beg1]));
                }
            }
        }
    };

    // template specialization both array_view
    template <typename TIn, typename TOut, ptrdiff_t Rank, class UnOp>
    struct PixelWiseUnaryOpDispatcher<TIn, TOut, Rank, array_view, array_view, UnOp>
    {

        void operator()(const array_view<TIn, Rank> &i_vin, UnOp op, array_view<TOut, Rank> &o_vout) const
        {
            // std::cout << "\n" << "call PixelWiseUnaryOpDispatcher array view template specialization,fall back ptr";
            auto i_vinbeg = i_vin.data();
            auto i_vinend = i_vin.data() + i_vin.size();
            auto i_voutbeg = o_vout.data();
            for (; i_vinbeg != i_vinend; i_vinbeg++, i_voutbeg++)
            {
                *i_voutbeg = static_cast<TOut>(op(*i_vinbeg));
            }
        }
    };

    /****************************************************************************************/
    /*                               PixelWiseUnaryOpWithTag                                */
    /****************************************************************************************/

    // primary use strided view
    template <typename TIn, typename TOut, ptrdiff_t Rank, template <typename, ptrdiff_t> class ViewIn,
              template <typename, ptrdiff_t> class ViewOut, template <typename, typename, class TAG> class UnOp,
              typename = void>
    struct PixelWiseUnaryOpDispatcherWithTag
    {
        static_assert((std::is_same<ViewIn<TIn, Rank>, strided_array_view<TIn, Rank>>::value ||
                       std::is_same<ViewIn<TIn, Rank>, strided_array_view<const TIn, Rank>>::value ||
                       std::is_same<ViewOut<TOut, Rank>, strided_array_view<TOut, Rank>>::value),
                      "strided view only specilization fail for arrayview");

        void operator()(const ViewIn<TIn, Rank> &i_vin, ViewOut<TOut, Rank> &o_vout) const
        {
            // get the specialized operator
            using real_op = /*typename*/ UnOp<TIn, TOut, tag_SIMD_disabled>;
            real_op op;

            // std::cout << "\n" << "call PixelWiseUnaryOpDispatcherWithTag strided view";
            // More runtime dispatch
            auto vInbound = i_vin.bound();
            auto vOutbound = o_vout.bound();
            auto stridevIN = i_vin.stride();
            auto stridevOut = o_vout.stride();

            if (vInbound == vOutbound && stridevIN == stridevOut) // same bound + same stride -> one idx
            {
                // std::cout << "\n" << "call UnaryOpDispatcher one idx";
                auto beg1 = begin(vInbound);
                auto end1 = end(vInbound);
                for (; beg1 != end1; ++beg1)
                {
                    o_vout[*beg1] = static_cast<TOut>(op(i_vin[*beg1]));
                }
                // TODO stride==1 in least significant dimension
            }
            else // default two idx
            {
                // std::cout << "\n" << "call UnaryOpDispatcher primary template default two idx";
                auto beg1 = begin(vInbound);
                auto end1 = end(vInbound);
                auto beg2 = begin(vOutbound);
                for (; beg1 != end1; ++beg1, ++beg2)
                {
                    o_vout[*beg2] = static_cast<TOut>(op(i_vin[*beg1]));
                }
            }
        }
    };

    template <typename TIn, typename TOut, ptrdiff_t Rank, template <typename, ptrdiff_t> class ViewIn,
              template <typename, ptrdiff_t> class ViewOut, class UnOp>
    void PixelWiseUnaryOp(const ViewIn<TIn, Rank> &i_vin, UnOp op,
                          ViewOut<TOut, Rank> &o_vout) POUTRE_NOEXCEPTONLYNDEBUG
    {
        POUTRE_ASSERTCHECK(i_vin.size() == o_vout.size(), "Incompatible views size");
        PixelWiseUnaryOpDispatcher<TIn, TOut, Rank, ViewIn, ViewOut, UnOp> dispatcher;
        dispatcher(i_vin, op, o_vout);
    }

    // template specialization both array_view but different type
    template <typename TIn, typename TOut, ptrdiff_t Rank, template <typename, typename, class TAG> class UnOp>
    struct PixelWiseUnaryOpDispatcherWithTag<
        TIn, TOut, Rank, array_view, array_view, UnOp,
        std::enable_if_t<!std::is_same<std::remove_const_t<TIn>, std::remove_const_t<TOut>>::value>>
    {

        void operator()(const array_view<TIn, Rank> &i_vin, array_view<TOut, Rank> &o_vout) const
        {
            // get the specialized operator
            using real_op = /*typename*/ UnOp<TIn, TOut, tag_SIMD_disabled>;
            real_op op;

            // std::cout << "\n" << "call PixelWiseUnaryOpDispatcherWithTag array view template specialization,fall back
            // ptr";
            auto i_vinbeg = i_vin.data();
            auto i_vinend = i_vin.data() + i_vin.size();
            auto i_voutbeg = o_vout.data();
            for (; i_vinbeg != i_vinend; i_vinbeg++, i_voutbeg++)
            {
                *i_voutbeg = static_cast<TOut>(op(*i_vinbeg));
            }
        }
    };

    // template specialization both array_view same type and arithmetic -> SIMD aware
    template <typename TIn, typename TOut, ptrdiff_t Rank, template <typename, typename, class TAG> class UnOp>
    struct PixelWiseUnaryOpDispatcherWithTag<
        TIn, TOut, Rank, array_view, array_view, UnOp,
        std::enable_if_t<std::is_same<std::remove_const_t<TIn>, std::remove_const_t<TOut>>::value &&
                         std::is_arithmetic<TIn>::value>>

    {
        void operator()(const array_view<TIn, Rank> &i_vin, array_view<TOut, Rank> &o_vout) const
        {
            // get the specialized operator
            using real_op = /*typename*/ UnOp<TIn, TIn, tag_SIMD_enabled>;
            real_op op;

            // std::cout << "\n" << "call PixelWiseUnaryOpDispatcherWithTag array view template specialization same
            // type,fall back ptr + SIMD";

            auto i_vinbeg = i_vin.data();
            auto i_vinend = i_vin.data() + i_vin.size();
            auto i_voutbeg = o_vout.data();

            simd::transform(i_vinbeg, i_vinend, i_voutbeg, op);
        }
    };

    /****************************************************************************************/
    /*                               PixelWiseUnaryOpWithTag with value                     */
    /****************************************************************************************/

    template <typename TIn, typename TOut, ptrdiff_t Rank, template <typename, ptrdiff_t> class ViewIn,
              template <typename, ptrdiff_t> class ViewOut, template <typename, typename, class TAG> class UnOp>
    void PixelWiseUnaryOp(const ViewIn<TIn, Rank> &i_vin, ViewOut<TOut, Rank> &o_vout) POUTRE_NOEXCEPTONLYNDEBUG
    {
        POUTRE_ASSERTCHECK(i_vin.size() == o_vout.size(), "Incompatible views size");
        PixelWiseUnaryOpDispatcherWithTag<TIn, TOut, Rank, ViewIn, ViewOut, UnOp> dispatcher;
        dispatcher(i_vin, o_vout);
    }

    /****************************************************************************************/
    /*                               PixelWiseUnaryOpWithTag with value                     */
    /****************************************************************************************/

    // primary use strided view
    template <typename TIn, typename TOut, ptrdiff_t Rank, template <typename, ptrdiff_t> class ViewIn,
              template <typename, ptrdiff_t> class ViewOut, template <typename, typename, class TAG> class UnOp,
              typename = void>
    struct PixelWiseUnaryOpWithValueDispatcherWithTag
    {
        static_assert((std::is_same<ViewIn<TIn, Rank>, strided_array_view<TIn, Rank>>::value ||
                       std::is_same<ViewIn<TIn, Rank>, strided_array_view<const TIn, Rank>>::value ||
                       std::is_same<ViewOut<TOut, Rank>, strided_array_view<TOut, Rank>>::value),
                      "strided view only specilization fail for arrayview");

        void operator()(const ViewIn<TIn, Rank> &i_vin, TOut a0, ViewOut<TOut, Rank> &o_vout) const
        {

            // get the specialized operator
            using real_op = /*typename*/ UnOp<TIn, TOut, tag_SIMD_disabled>;
            real_op op(a0);

            // More runtime dispatch
            auto vInbound = i_vin.bound();
            auto vOutbound = o_vout.bound();
            auto stridevIN = i_vin.stride();
            auto stridevOut = o_vout.stride();
            // std::cout << "\n" << "call PixelWiseUnaryOpWithValueDispatcherWithTag strided view";
            if (vInbound == vOutbound && stridevIN == stridevOut) // same bound + same stride -> one idx
            {
                // std::cout << "\n" << "call UnaryOpDispatcher one idx";
                auto beg1 = begin(vInbound);
                auto end1 = end(vInbound);
                for (; beg1 != end1; ++beg1)
                {
                    o_vout[*beg1] = static_cast<TOut>(op(i_vin[*beg1]));
                }
                // TODO stride==1 in least significant dimension
            }
            else // default two idx
            {
                // std::cout << "\n" << "call UnaryOpDispatcher primary template default two idx";
                auto beg1 = begin(vInbound);
                auto end1 = end(vInbound);
                auto beg2 = begin(vOutbound);
                for (; beg1 != end1; ++beg1, ++beg2)
                {
                    o_vout[*beg2] = static_cast<TOut>(op(i_vin[*beg1]));
                }
            }
        }
    };

    // template specialization both array_view but different type
    template <typename TIn, typename TOut, ptrdiff_t Rank, template <typename, typename, class TAG> class UnOp>
    struct PixelWiseUnaryOpWithValueDispatcherWithTag<
        TIn, TOut, Rank, array_view, array_view, UnOp,
        std::enable_if_t<!std::is_same<std::remove_const_t<TIn>, std::remove_const_t<TOut>>::value>>
    {
        void operator()(array_view<TIn, Rank> const &i_vin, TOut a0, array_view<TOut, Rank> &o_vout) const
        {

            // get the specialized operator
            using real_op = /*typename*/ UnOp<TIn, TOut, tag_SIMD_disabled>;
            real_op op(a0);

            // std::cout << "\n" << "call PixelWiseUnaryOpWithValueDispatcherWithTag array view template
            // specialization,fall back ptr";
            auto i_vinbeg = i_vin.data();
            auto i_vinend = i_vin.data() + i_vin.size();
            auto i_voutbeg = o_vout.data();
            for (; i_vinbeg != i_vinend; i_vinbeg++, i_voutbeg++)
            {
                *i_voutbeg = static_cast<TOut>(op(*i_vinbeg));
            }
        }
    };

    // template specialization both array_view same type and arithmetic -> SIMD aware
    template <typename TIn, typename TOut, ptrdiff_t Rank, template <typename, typename, class TAG> class UnOp>
    struct PixelWiseUnaryOpWithValueDispatcherWithTag<
        TIn, TOut, Rank, array_view, array_view, UnOp,
        std::enable_if_t<std::is_same<std::remove_const_t<TIn>, std::remove_const_t<TOut>>::value &&
                         std::is_arithmetic<TIn>::value>>
    {

        /*POUTRE_ALWAYS_INLINE*/ void operator()(array_view<TIn, Rank> const &i_vin, TIn a0,
                                                 array_view<TOut, Rank> &o_vout) const
        {

            // get the specialized operator
            using real_op = /*typename*/ UnOp<TIn, TIn, tag_SIMD_enabled>;
            real_op op(a0);

            // std::cout << "\n" << "call PixelWiseUnaryOpWithValueDispatcherWithTag array view template specialization
            // same type,fall back ptr + SIMD";

            auto i_vinbeg = i_vin.data();
            auto i_vinend = i_vin.data() + i_vin.size();
            auto i_voutbeg = o_vout.data();
            simd::transform(i_vinbeg, i_vinend, i_voutbeg, op);
        }
    };

    template <typename TIn, typename TOut, ptrdiff_t Rank, template <typename, ptrdiff_t> class ViewIn,
              template <typename, ptrdiff_t> class ViewOut, template <typename, typename, class TAG> class UnOp>
    void PixelWiseUnaryOpWithValue(const ViewIn<TIn, Rank> &i_vin, TOut a0,
                                   ViewOut<TOut, Rank> &o_vout) POUTRE_NOEXCEPTONLYNDEBUG
    {
        POUTRE_ASSERTCHECK(i_vin.size() == o_vout.size(), "Incompatible views size");
        PixelWiseUnaryOpWithValueDispatcherWithTag<TIn, TOut, Rank, ViewIn, ViewOut, UnOp> dispatcher;
        dispatcher(i_vin, a0, o_vout);
    }
    //! @} doxygroup: image_processing_linear_group
} // namespace poutre

#endif // POUTRE_IMAGEPROCESSING_UNARYOP_HPP__
