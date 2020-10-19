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

#include <poutreBase/poutreContainerView.hpp>
#include <poutreImageProcessingCore/include/poutreImageProcessingContainerCopieConvert.hpp>
#include <poutreImageProcessingCore/poutreImageProcessingCore.hpp>
#include <poutreImageProcessingCore/poutreImageProcessingType.hpp>
#include <poutreImageProcessingPixelOperation/include/poutreImageProcessingArith.hpp>
#include <poutreImageProcessingSE/poutreImageProcessingSENeighborList.hpp>

namespace poutre
{
    template <typename T1, typename T2> struct BinOpInf
    {
      public:
        static constexpr T1 neutral = std::numeric_limits<T1>::max();
        static /*POUTRE_ALWAYS_INLINE*/ T2 process(const T1 &A0, const T1 &A1)
        {
            return (T2)std::min<T1>(A0, A1);
        }
    };

    template <typename T1, typename T2> struct BinOpSup
    {
      public:
        static constexpr T1 neutral = std::numeric_limits<T1>::lowest();
        static /*POUTRE_ALWAYS_INLINE*/ T2 process(const T1 &A0, const T1 &A1)
        {
            return (T2)std::max<T1>(A0, A1);
        }
    };

    //-----STRIDED VIEW OR DenseImage generic with static SE
    template <se::NLS nl, typename T1, typename T2, ptrdiff_t Rank, template <typename, ptrdiff_t> class View1,
              template <typename, ptrdiff_t> class View2, class BinOp>
    struct t_ErodeDilateOpispatcher
    {
        static_assert(Rank == se::NeighborListStaticSETraits<nl>::Rank, "SE and view have not the same Rank");
        void operator()(const View1<T1, Rank> &i_vin, const View2<T2, Rank> &o_vout)
        {
            POUTRE_CHECK(i_vin.size() == o_vout.size(), "Incompatible views size");

            // More runtime dispatch
            auto vInbound = i_vin.bound();
            auto vOutbound = o_vout.bound();
            auto stridevIN = i_vin.stride();
            auto stridevOut = o_vout.stride();
            auto NlList = se::NeighborListStaticSETraits<nl>::nlStaticSE;
            if (vInbound == vOutbound && stridevIN == stridevOut) // same bound + same stride -> one idx
            {
                auto beg1 = begin(vInbound);
                auto end1 = end(vInbound);
                for (; beg1 != end1; ++beg1)
                {
                    auto val = BinOp::neutral;
                    for (const auto &idxnl : NlList)
                    {
                        auto currentidx = *beg1 + idxnl;
                        if (!vInbound.contains(currentidx))
                            continue;
                        val = BinOp::process(val, i_vin[currentidx]);
                    }
                    o_vout[*beg1] = static_cast<T2>(val);
                }
                // TODO stride==1 in least significant dimension
            }
            else // default two idx
            {
                auto beg1 = begin(vInbound);
                auto end1 = end(vInbound);
                auto beg2 = begin(vOutbound);
                for (; beg1 != end1; ++beg1, ++beg2)
                {
                    auto val = BinOp::neutral;
                    for (const auto &idxnl : NlList)
                    {
                        auto currentidx = *beg1 + idxnl;
                        if (!vInbound.contains(currentidx))
                            continue;
                        val = BinOp::process(val, i_vin[currentidx]);
                    }
                    o_vout[*beg2] = static_cast<T2>(val);
                }
            }
        }
    };
} // namespace poutre

// specialization for some 2D case
#include <poutreImageProcessingLowLevelMorphology/include/poutreImageProcessingEroDilNLS.hpp>
// specialization for line case
#include <poutreImageProcessingLowLevelMorphology/include/poutreImageProcessingEroDilLine.hpp>

#endif // POUTRE_IMAGEPROCESSING_ERO_DIL_HPP__
