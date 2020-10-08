//==============================================================================
//                  Copyright (c) 2015 - Thomas Retornaz                      //
//                     thomas.retornaz@mines-paris.org                        //
//          Distributed under the Boost Software License, Version 1.0.        //
//                 See accompanying file LICENSE.txt or copy at               //
//                     http://www.boost.org/LICENSE_1_0.txt                   //
//==============================================================================

#ifndef POUTRE_IMAGEPROCESSING_ERO_DIL_LINE_HPP__
#define POUTRE_IMAGEPROCESSING_ERO_DIL_LINE_HPP__

/**
 * @file   poutreImageProcessingEroDil.hpp
 * @author Thomas Retornaz
 * @brief  Define erosion/dilatation using line se
 *
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

#ifndef POUTRE_IPSENL_HPP__
#include <poutreImageProcessingSE/poutreImageProcessingSENeighborList.hpp>
#endif

#ifndef POUTRE_IMAGEPROCESSING_ARITH_HPP__
#include <poutreImageProcessingPixelOperation/include/poutreImageProcessingArith.hpp>
#endif

#ifndef POUTRE_IMAGEPROCESSING_CONTAINER_COPIECONVERT_HPP__
#include <poutreImageProcessingCore/include/poutreImageProcessingContainerCopieConvert.hpp>
#endif

#ifndef POUTRE_IMAGEPROCESSING_INTERFACE_COPIECONVERT_HPP__
#include <poutreImageProcessingCore/poutreImageProcessingInterfaceCopieConvert.hpp>
#endif

#ifndef POUTRE_IMAGEPROCESSING_INTERFACE_HPP__
#include <poutreImageProcessingCore/include/poutreImageProcessingInterface.hpp>
#endif
#include <algorithm>
#include <numeric>

namespace poutre
{
    /**
     * @addtogroup image_processing_mm_group Image Processing Morphology
     * @ingroup image_processing_group
     *@{
     */

    /**
     * @addtogroup image_processing_erodil_group Image Processing Erosion Dilatation
     *functions
     * @ingroup image_processing_mm_group
     *@{
     */

    // stolen from pylene todo propagate __restrict everywhere
    template <class T, class BinaryFunction>
    void running_max_min_1d(T *__restrict f, T *__restrict g, T *__restrict h, ptrdiff_t n, ptrdiff_t k,
                            BinaryFunction func, bool use_extension = true)
    {
        if (n == 0)
            return;

        const ptrdiff_t alpha = 2 * k + 1;
        const ptrdiff_t size = n + 2 * k;

        // Forward pass
        // Compute g[x] = Max f(y), y ∈ [α * ⌊x / α⌋ : x]
        {
            ptrdiff_t chunk_start = use_extension ? -k : 0;
            ptrdiff_t rem = use_extension ? size : n;

            for (; rem > 0; chunk_start += alpha, rem -= alpha)
            {
                ptrdiff_t chunk_size = std::min(rem, alpha);
                std::partial_sum(f + chunk_start, f + chunk_start + chunk_size, g + chunk_start, func);
            }
        }

        // Backward pass
        // Compute h[x] = Max f(y) y ∈ [x : α * (⌊x/α⌋+1))
        {
            ptrdiff_t chunk_start = use_extension ? -k : 0;
            ptrdiff_t rem = use_extension ? size : n;

            for (; rem > 0; chunk_start += alpha, rem -= alpha)
            {
                ptrdiff_t chunk_size = std::min(alpha, rem);
                std::partial_sum(std::make_reverse_iterator(f + chunk_start + chunk_size),
                                 std::make_reverse_iterator(f + chunk_start),
                                 std::make_reverse_iterator(h + chunk_start + chunk_size), func);
            }
        }

        // Compute local maximum out[x] = Max f(y) y ∈ [x-k:x+k]
        // out[x] = Max   (Max f[x-k:b),  Max f[b:x+k]) with b = α.⌈(x-k)/α⌉ = α.⌊(x+k)/α⌋
        //        = Max( h[x-k], g[x+k] )
        {
            for (ptrdiff_t i = 0; i < n; ++i)
                f[i] = func(h[i - k], g[i + k]);
        }
    }
    template <typename T1, typename T2, ptrdiff_t Rank, template <typename, ptrdiff_t> class View1,
              template <typename, ptrdiff_t> class View2>
    struct t_DilateXOpLineDispatcher
    {
        // generic case not supported yet
        // static_assert(false, "To be implemented for generic views");
    };

    template <typename T> struct t_DilateXOpLineDispatcher<T, T, 1, array_view, array_view>
    {
        void operator()(const array_view<T, 1> &i_vin, ptrdiff_t k, array_view<T, 1> &o_vout)
        {
            POUTRE_ASSERTCHECK(i_vin.size() == o_vout.size(), "Incompatible views size");
            auto ibd = i_vin.bound();
            auto obd = o_vout.bound();
            scoord n = ibd[0];
            if (n == 0)
                return;
            if (k <= 0)
            {
                std::memcpy((void *)o_vout.data(), (void *)i_vin.data(), sizeof(T) * i_vin.size());
                return;
            }
            using lineView = T *;
            lineView rawLineIn = i_vin.data();
            lineView rawLineOut = o_vout.data();

            // need auxilary buffer
            using tmpBuffer =
                std::vector<T, xs::aligned_allocator<T, SIMD_IDEAL_MAX_ALIGN_BYTES>>; // alignement not required
            tmpBuffer f(n + 2 * k), g(n + 2 * k), h(n + 2 * k);
            std::fill_n(f.begin(), f.size(), std::numeric_limits<T>::lowest());
            std::memcpy(f.data() + k, rawLineIn, n * sizeof(T));
            auto sup = [](auto x, auto y) { return std::max(x, y); };
            running_max_min_1d(f.data() + k, g.data() + k, h.data() + k, n, k, sup);
        }
    };

    template <typename T> struct t_DilateXOpLineDispatcher<T, T, 2, array_view, array_view>
    {
        void operator()(const array_view<T, 2> &i_vin, ptrdiff_t k, array_view<T, 2> &o_vout)
        {
            POUTRE_ASSERTCHECK(i_vin.size() == o_vout.size(), "Incompatible views size");
            auto ibd = i_vin.bound();
            auto obd = o_vout.bound();
            auto istride = i_vin.stride();
            auto ostride = o_vout.stride();
            scoord ysize = ibd[0];
            scoord xsize = ibd[1];
            POUTRE_ASSERTCHECK(ibd == obd, "bound not compatible");
            POUTRE_ASSERTCHECK(istride == ostride, "stride not compatible");
            if (xsize == 0 || ysize == 0)
                return;
            if (k <= 0)
            {
                t_Copy(i_vin, o_vout);
            }
            using lineView = T *;
            ptrdiff_t n = xsize;
            // need auxilary buffer
            using tmpBuffer =
                std::vector<T, xs::aligned_allocator<T, SIMD_IDEAL_MAX_ALIGN_BYTES>>; // alignement not required
            tmpBuffer f(n + 2 * k), g(n + 2 * k), h(n + 2 * k);

            for (scoord y = 0; y < ysize; y++)
            {
                lineView bufInputCurrentLine = i_vin.data() + y * xsize;
                lineView bufOuputCurrentLine = o_vout.data() + y * xsize;
                std::fill_n(f.begin(), f.size(), std::numeric_limits<T>::lowest());
                std::memcpy(f.data() + k, bufInputCurrentLine, n * sizeof(T));
                auto sup = [](auto x, auto y) { return std::max(x, y); };
                running_max_min_1d(f.data() + k, g.data() + k, h.data() + k, n, k, sup);
                std::memcpy(bufOuputCurrentLine, f.data() + k, n * sizeof(T));
            }
        }
    };

    template <typename T1, typename T2, ptrdiff_t Rank, template <typename, ptrdiff_t> class View1,
              template <typename, ptrdiff_t> class View2>
    struct t_ErodeXOpLineDispatcher
    {
        // generic case not supported yet
        // static_assert(false, "To be implemented for generic views");
    };

    template <typename T> struct t_ErodeXOpLineDispatcher<T, T, 1, array_view, array_view>
    {
        void operator()(const array_view<T, 1> &i_vin, ptrdiff_t k, array_view<T, 1> &o_vout)
        {
            POUTRE_ASSERTCHECK(i_vin.size() == o_vout.size(), "Incompatible views size");
            auto ibd = i_vin.bound();
            auto obd = o_vout.bound();
            scoord n = ibd[0];
            if (n == 0)
                return;
            if (k <= 0)
            {
                std::memcpy((void *)o_vout.data(), (void *)i_vin.data(), sizeof(T) * i_vin.size());
                return;
            }
            using lineView = T *;
            lineView rawLineIn = i_vin.data();
            lineView rawLineOut = o_vout.data();

            // need auxilary buffer
            using tmpBuffer =
                std::vector<T, xs::aligned_allocator<T, SIMD_IDEAL_MAX_ALIGN_BYTES>>; // alignement not required
            tmpBuffer f(n + 2 * k), g(n + 2 * k), h(n + 2 * k);
            std::fill_n(f.begin(), f.size(), std::numeric_limits<T>::max());
            std::memcpy(f.data() + k, rawLineIn, n * sizeof(T));
            auto inf = [](auto x, auto y) { return std::min(x, y); };
            running_max_min_1d(f.data() + k, g.data() + k, h.data() + k, n, k, inf);
        }
    };
    template <typename T> struct t_ErodeXOpLineDispatcher<T, T, 2, array_view, array_view>
    {
        void operator()(const array_view<T, 2> &i_vin, ptrdiff_t k, array_view<T, 2> &o_vout)
        {
            POUTRE_ASSERTCHECK(i_vin.size() == o_vout.size(), "Incompatible views size");
            auto ibd = i_vin.bound();
            auto obd = o_vout.bound();
            auto istride = i_vin.stride();
            auto ostride = o_vout.stride();
            scoord ysize = ibd[0];
            scoord xsize = ibd[1];
            POUTRE_ASSERTCHECK(ibd == obd, "bound not compatible");
            POUTRE_ASSERTCHECK(istride == ostride, "stride not compatible");
            if (xsize == 0 || ysize == 0)
                return;
            if (k <= 0)
            {
                t_Copy(i_vin, o_vout);
            }
            using lineView = T *;
            ptrdiff_t n = xsize;
            // need auxilary buffer
            using tmpBuffer =
                std::vector<T, xs::aligned_allocator<T, SIMD_IDEAL_MAX_ALIGN_BYTES>>; // alignement not required
            tmpBuffer f(n + 2 * k), g(n + 2 * k), h(n + 2 * k);

            for (scoord y = 0; y < ysize; y++)
            {
                lineView bufInputCurrentLine = i_vin.data() + y * xsize;
                lineView bufOuputCurrentLine = o_vout.data() + y * xsize;
                std::fill_n(f.begin(), f.size(), std::numeric_limits<T>::max());
                std::memcpy(f.data() + k, bufInputCurrentLine, n * sizeof(T));
                auto inf = [](auto x, auto y) { return std::min(x, y); };
                running_max_min_1d(f.data() + k, g.data() + k, h.data() + k, n, k, inf);
                std::memcpy(bufOuputCurrentLine, f.data() + k, n * sizeof(T));
            }
        }
    };

    template <typename TIn, typename TOut, ptrdiff_t Rank, template <typename, ptrdiff_t> class ViewIn,
              template <typename, ptrdiff_t> class ViewOut>
    void t_DilateX(const ViewIn<TIn, Rank> &i_vin, ptrdiff_t k, ViewOut<TOut, Rank> &o_vout)
    {
        POUTRE_CHECK(i_vin.size() == o_vout.size(), "Incompatible views size");
        t_DilateXOpLineDispatcher<TIn, TOut, Rank, ViewIn, ViewOut> dispatcher;
        dispatcher(i_vin, k, o_vout);
    }
    template <typename TIn, typename TOut, ptrdiff_t Rank>
    void t_DilateX(const DenseImage<TIn, Rank> &i_vin, ptrdiff_t k, DenseImage<TOut, Rank> &o_vout)
    {
        AssertSizesCompatible(i_vin, o_vout, "t_DilateX incompatible size");
        AssertAsTypesCompatible(i_vin, o_vout, "t_DilateX incompatible types");
        auto viewIn = view(const_cast<DenseImage<TIn, Rank> &>(i_vin));
        auto viewOut = view(o_vout);
        t_DilateX(viewIn, k, viewOut);
    }

    template <typename TIn, typename TOut, ptrdiff_t Rank, template <typename, ptrdiff_t> class ViewIn,
              template <typename, ptrdiff_t> class ViewOut>
    void t_DilateY(const ViewIn<TIn, Rank> &i_vin, ptrdiff_t k, ViewOut<TOut, Rank> &o_vout)
    {
        POUTRE_CHECK(i_vin.size() == o_vout.size(), "Incompatible views size");
    }
    template <typename TIn, typename TOut, ptrdiff_t Rank>
    void t_DilateY(const DenseImage<TIn, Rank> &i_vin, ptrdiff_t k, DenseImage<TOut, Rank> &o_vout)
    {
        AssertSizesCompatible(i_vin, o_vout, "t_DilateY incompatible size");
        AssertAsTypesCompatible(i_vin, o_vout, "t_DilateY incompatible types");
        auto viewIn = view(const_cast<DenseImage<TIn, Rank> &>(i_vin));
        auto viewOut = view(o_vout);
        t_DilateY(viewIn, k, viewOut);
    }

    template <typename TIn, typename TOut, ptrdiff_t Rank, template <typename, ptrdiff_t> class ViewIn,
              template <typename, ptrdiff_t> class ViewOut>
    void t_ErodeX(const ViewIn<TIn, Rank> &i_vin, ptrdiff_t k, ViewOut<TOut, Rank> &o_vout)
    {
        POUTRE_CHECK(i_vin.size() == o_vout.size(), "Incompatible views size");
        t_ErodeXOpLineDispatcher<TIn, TOut, Rank, ViewIn, ViewOut> dispatcher;
        dispatcher(i_vin, k, o_vout);
    }
    template <typename TIn, typename TOut, ptrdiff_t Rank>
    void t_ErodeX(const DenseImage<TIn, Rank> &i_vin, ptrdiff_t k, DenseImage<TOut, Rank> &o_vout)
    {
        AssertSizesCompatible(i_vin, o_vout, "t_ErodeX incompatible size");
        AssertAsTypesCompatible(i_vin, o_vout, "t_ErodeX incompatible types");
        auto viewIn = view(const_cast<DenseImage<TIn, Rank> &>(i_vin));
        auto viewOut = view(o_vout);
        t_ErodeX(viewIn, k, viewOut);
    }

    template <typename TIn, typename TOut, ptrdiff_t Rank, template <typename, ptrdiff_t> class ViewIn,
              template <typename, ptrdiff_t> class ViewOut>
    void t_ErodeY(const ViewIn<TIn, Rank> &i_vin, ptrdiff_t k, ViewOut<TOut, Rank> &o_vout)
    {
        POUTRE_CHECK(i_vin.size() == o_vout.size(), "Incompatible views size");
    }
    template <typename TIn, typename TOut, ptrdiff_t Rank>
    void t_ErodeY(const DenseImage<TIn, Rank> &i_vin, ptrdiff_t k, DenseImage<TOut, Rank> &o_vout)
    {
        // AssertSizesCompatible(i_vin, o_vout, "t_ErodeY incompatible size");
        AssertAsTypesCompatible(i_vin, o_vout, "t_ErodeY incompatible types");
        auto viewIn = view(const_cast<DenseImage<TIn, Rank> &>(i_vin));
        auto viewOut = view(o_vout);
        t_ErodeX(viewIn, k, viewOut);
    }
    //! @} doxygroup: image_processing_erodil_group
    //! @} doxygroup: image_processing_group
} // namespace poutre
#endif // POUTRE_IMAGEPROCESSING_ERO_DIL_LINE_HPP__
