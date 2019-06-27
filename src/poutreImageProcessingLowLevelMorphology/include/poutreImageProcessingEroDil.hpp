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

    template<typename T1,typename T2>
    struct BinOpInf
    {
        public:
            static constexpr T1 neutral=std::numeric_limits<T1>::max();
            static T2 process(const T1& A0, const T1& A1)
            {
                return (T2)std::min<T1>(A0,A1);
            }
    };
    
    template<typename T1,typename T2>
    struct BinOpSup
    {
    public:
        static constexpr T1 neutral = std::numeric_limits<T1>::lowest();
        static T2 process(const T1& A0, const T1& A1)
        {
            return (T2)std::max<T1>(A0, A1);
        }
    };

    template<se::NeighborListStaticSE nl, typename T1, typename T2, ptrdiff_t Rank, template <typename, ptrdiff_t> class View1, template <typename, ptrdiff_t> class View2,class BinOp>
    struct t_ErodeDilateOpispatcher
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
                    auto val= BinOp::neutral;
                    for (const auto& idxnl : NlList)
                    {
                        auto currentidx = *beg1 + idxnl;
                        if (!vInbound.contains(currentidx)) continue;
                        val = BinOp::process(val, i_vin[currentidx]);
                    }
                    o_vout[*beg1] = static_cast<T2>(val);
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
                    auto val = op::neutral;
                    for (const auto& idxnl : NlList)
                    {
                        auto currentidx = *beg1 + idxnl;
                        if (!vInbound.contains(currentidx)) continue;
                        val = op::process(val, i_vin[currentidx]);
                    }
                    o_vout[*beg2] = static_cast<T2>(max);
                }
            }
        }
    };

    template<se::NeighborListStaticSE nl, typename T1, typename T2,class BinOp>
    void t_ErodeDilateIterateArrayView2DHelper(const array_view<T1, 2> & i_vin, const array_view<T2, 2> & o_vout, ptrdiff_t i_XCenter, ptrdiff_t i_YCenter)
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
        ptrdiff_t NbNeighbor = se::NeighborListStaticSETraits<nl>::NbNeighbor;
        auto i_vinbeg = i_vin.data();
        auto o_voutbeg = o_vout.data();
        auto val = BinOp::neutral;
        idx2d center(i_YCenter, i_XCenter);
        for (ptrdiff_t idxNB = 0; idxNB < NbNeighbor; ++idxNB)
        {
            idx2d idxnl(NlList[idxNB]);
            scoord currentx = center[1] + idxnl[1];
            scoord currenty = center[0] + idxnl[0];
            if (currentx < 0) continue;
            if (currenty < 0) continue;
            if (currentx >= xsize) continue;
            if (currenty >= ysize) continue;
            val = BinOp::process(val, i_vinbeg[xsize * currenty + currentx]);
        }
        o_voutbeg[xsize * i_YCenter + i_XCenter] = static_cast<T2>(val);
    }
    template<se::NeighborListStaticSE nl, typename T1, typename T2,class BinOp>
    struct t_ErodeDilateOpispatcher<nl, T1, T2, 2, array_view, array_view,BinOp>
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
            ptrdiff_t NbNeighbor = se::NeighborListStaticSETraits<nl>::NbNeighbor;
            ptrdiff_t suroundingHalfSizeX = se::NeighborListStaticSETraits<nl>::surroundingSquareHalfSize[1];
            ptrdiff_t suroundingHalfSizeY = se::NeighborListStaticSETraits<nl>::surroundingSquareHalfSize[0];
            auto i_vinbeg = i_vin.data();
            auto o_voutbeg = o_vout.data();
            //handling the upper lines
            for (ptrdiff_t y = 0; y < suroundingHalfSizeY; ++y)
            {
                for (ptrdiff_t  x = 0; x < xsize; ++x)
                {
                    t_ErodeDilateIterateArrayView2DHelper<nl, T1, T2, BinOp>(i_vin, o_vout, x, y);
                }
            }
            //handling the lower lines
            for (ptrdiff_t  y = ysize - suroundingHalfSizeY; y < ysize; ++y)
            {
                for (ptrdiff_t  x = 0; x < xsize; ++x)
                {
                    t_ErodeDilateIterateArrayView2DHelper<nl, T1, T2, BinOp>(i_vin, o_vout, x, y);
                }
            }
            //Main lines area
            for (ptrdiff_t  y = suroundingHalfSizeY; y < ysize - suroundingHalfSizeY; ++y)
            {
                //handling the first columns
                for (ptrdiff_t  x = 0; x < suroundingHalfSizeX; ++x)
                {
                    t_ErodeDilateIterateArrayView2DHelper<nl, T1, T2, BinOp>(i_vin, o_vout, x, y);
                }
                for (ptrdiff_t  x = suroundingHalfSizeX; x < xsize - suroundingHalfSizeX; ++x)
                {
                    auto val = BinOp::neutral;
                    idx2d center(y, x);
                    //ver slow
                    //for (const auto& idxnl : NlList)
                    //{
                    //    auto currentidx = center+ idxnl;
                    //    max = std::max<T1>(max, i_vinbeg[xsize*currentidx[0]+ currentidx[1]]);
                    //}
                    for (ptrdiff_t idxNB = 0; idxNB < NbNeighbor; ++idxNB)
                    {
                        idx2d idxnl(NlList[idxNB]);
                        scoord currentx = center[1] + idxnl[1];
                        scoord currenty = center[0] + idxnl[0];
                        val = BinOp::process(val, i_vinbeg[xsize * currenty + currentx]);
                    }
                    o_voutbeg[xsize * y + x] = static_cast<T2>(val);
                }
                //handling the last columns
                for (ptrdiff_t  x = xsize - suroundingHalfSizeX; x < xsize; ++x)
                {
                    t_ErodeDilateIterateArrayView2DHelper<nl, T1, T2, BinOp>(i_vin, o_vout, x, y);
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
            t_ErodeDilateOpispatcher<se::NeighborListStaticSE::NeighborListStaticSE2DCT8, TIn, TOut, Rank, ViewIn, ViewOut, BinOpSup<TIn,TOut>> dispatcher;
            dispatcher(i_vin, o_vout);
        }break;
        case se::NeighborListStaticSE::NeighborListStaticSE2DCT4:
        {
            t_ErodeDilateOpispatcher<se::NeighborListStaticSE::NeighborListStaticSE2DCT4, TIn, TOut, Rank, ViewIn, ViewOut, BinOpSup<TIn, TOut>> dispatcher;
            dispatcher(i_vin, o_vout);
        }break;

        default:
        {
            POUTRE_RUNTIME_ERROR("t_Dilate se::NeighborListStaticSE not implemented");
        }
        }
    }


    template<typename TIn, typename TOut, ptrdiff_t Rank, template <typename, ptrdiff_t> class ViewIn, template <typename, ptrdiff_t> class ViewOut>
    void t_Erode(const ViewIn<TIn, Rank>& i_vin, se::NeighborListStaticSE nl, ViewOut<TOut, Rank>& o_vout)
    {
        POUTRE_CHECK(i_vin.size() == o_vout.size(), "Incompatible views size");
        switch (nl)
        {
        case se::NeighborListStaticSE::NeighborListStaticSE2DCT8:
        {
            t_ErodeDilateOpispatcher<se::NeighborListStaticSE::NeighborListStaticSE2DCT8, TIn, TOut, Rank, ViewIn, ViewOut, BinOpInf<TIn, TOut>> dispatcher;
            dispatcher(i_vin, o_vout);
        }break;
        case se::NeighborListStaticSE::NeighborListStaticSE2DCT4:
        {
            t_ErodeDilateOpispatcher<se::NeighborListStaticSE::NeighborListStaticSE2DCT4, TIn, TOut, Rank, ViewIn, ViewOut, BinOpInf<TIn, TOut>> dispatcher;
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