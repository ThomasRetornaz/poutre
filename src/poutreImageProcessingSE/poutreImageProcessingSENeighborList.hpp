
//==============================================================================
//                  Copyright (c) 2018 - Thomas Retornaz                      //
//                     thomas.retornaz@mines-paris.org                        //
//          Distributed under the Boost Software License, Version 1.0.        //
//                 See accompanying file LICENSE.txt or copy at               //
//                     http://www.boost.org/LICENSE_1_0.txt                   //
//==============================================================================

#ifndef POUTRE_IPSENL_HPP__
#define POUTRE_IPSENL_HPP__

#ifndef POUTRE_IPSECORE_HPP__
#include <poutreImageProcessingSE/poutreImageProcessingSEInterface.hpp>
#endif
#ifndef POUTRE_COORDINATE_HPP__
#include <poutreBase/poutreCoordinate.hpp>
#endif
/**
 * @file   poutreImageProcessingSENeighborList.hpp
 * @author Thomas Retornaz
 * @brief  Static Neighborlist Structuring element type
 *
 *
 */

namespace poutre
{
    namespace se
    {
        /**
         * @addtogroup se_nl_grp NeighborList Structuring Element Group
         * @ingroup se_grp
         *@{
         */
        enum class NeighborListStaticSE
        {
            NeighborListStaticSE2DCTUndefined, //!< Undefined type
            NeighborListStaticSE2DSquare,      //!< Connection Square SE
            NeighborListStaticSE2DCross,       //!< Connection Cross SE
            NeighborListStaticSE2DSeg0,        //!< Connection Seg0zontal Segement symmetric SE
            NeighborListStaticSE2DSeg90,       //!< Connection Vertical Segement symmetric SE
            NeighborListStaticSE2DSeg45,       //!< Connection 45° Segement symmetric SE
            NeighborListStaticSE2DSeg135,      //!< Connection 135° Segement symmetric SE
        };
        //! @} // se_nl_grp

        template <NeighborListStaticSE> struct NeighborListStaticSETraits
        {
        };

        template <> struct NeighborListStaticSETraits<NeighborListStaticSE::NeighborListStaticSE2DSquare>
        {
            POUTRE_STATIC_CONSTEXPR ptrdiff_t Rank = 2;
            POUTRE_STATIC_CONSTEXPR ptrdiff_t NbNeighbor = 9;
            const static idx2d surroundingSquareHalfSize; // FXIME should be constexpr
            const static std::array<idx2d, 9> NlList;
            const static std::array<idx2d, 9> NlListTransposed;
        };

        const idx2d
            NeighborListStaticSETraits<NeighborListStaticSE::NeighborListStaticSE2DSquare>::surroundingSquareHalfSize =
                idx2d{1, 1};
        const std::array<idx2d, 9>
            NeighborListStaticSETraits<NeighborListStaticSE::NeighborListStaticSE2DSquare>::NlList = {
                idx2d{0, 0},
                //
                idx2d{-1, -1},
                idx2d{-1, 0},
                idx2d{-1, 1},
                //
                idx2d{0, -1},
                idx2d{0, 1},
                //
                idx2d{1, -1},
                idx2d{1, 0},
                idx2d{1, 1},
        };
        const std::array<idx2d, 9>
            NeighborListStaticSETraits<NeighborListStaticSE::NeighborListStaticSE2DSquare>::NlListTransposed = {
                idx2d{0, 0},
                //
                idx2d{-1, -1},
                idx2d{-1, 0},
                idx2d{-1, 1},
                //
                idx2d{0, -1},
                idx2d{0, 1},
                //
                idx2d{1, -1},
                idx2d{1, 0},
                idx2d{1, 1},
        };
        template <> struct NeighborListStaticSETraits<NeighborListStaticSE::NeighborListStaticSE2DCross>
        {
            POUTRE_STATIC_CONSTEXPR ptrdiff_t Rank = 2;
            POUTRE_STATIC_CONSTEXPR ptrdiff_t NbNeighbor = 5;
            const static idx2d surroundingSquareHalfSize; // FXIME should be constexpr
            const static std::array<idx2d, 5> NlList;
            const static std::array<idx2d, 5> NlListTransposed;
        };

        const idx2d
            NeighborListStaticSETraits<NeighborListStaticSE::NeighborListStaticSE2DCross>::surroundingSquareHalfSize =
                idx2d{1, 1};
        const std::array<idx2d, 5>
            NeighborListStaticSETraits<NeighborListStaticSE::NeighborListStaticSE2DCross>::NlList = {idx2d{0, 0},
                                                                                                     //
                                                                                                     idx2d{-1, 0},
                                                                                                     //
                                                                                                     idx2d{0, -1},
                                                                                                     idx2d{0, 1},
                                                                                                     //
                                                                                                     idx2d{1, 0}};

        const std::array<idx2d, 5>
            NeighborListStaticSETraits<NeighborListStaticSE::NeighborListStaticSE2DCross>::NlListTransposed = {
                idx2d{0, 0},
                //
                idx2d{-1, 0},
                //
                idx2d{0, -1}, idx2d{0, 1},
                //
                idx2d{1, 0}};

        template <> struct NeighborListStaticSETraits<NeighborListStaticSE::NeighborListStaticSE2DSeg0>
        {
            POUTRE_STATIC_CONSTEXPR ptrdiff_t Rank = 2;
            POUTRE_STATIC_CONSTEXPR ptrdiff_t NbNeighbor = 3;
            const static idx2d surroundingSquareHalfSize; // FXIME should be constexpr
            const static std::array<idx2d, NbNeighbor> NlList;
            const static std::array<idx2d, NbNeighbor> NlListTransposed;
        };

        const idx2d
            NeighborListStaticSETraits<NeighborListStaticSE::NeighborListStaticSE2DSeg0>::surroundingSquareHalfSize =
                idx2d{1, 1};
        const std::array<idx2d, 3>
            NeighborListStaticSETraits<NeighborListStaticSE::NeighborListStaticSE2DSeg0>::NlList = {
                idx2d{0, 0},
                //
                idx2d{0, 1},
                idx2d{0, -1},
        };
        const std::array<idx2d, 3>
            NeighborListStaticSETraits<NeighborListStaticSE::NeighborListStaticSE2DSeg0>::NlListTransposed = {
                idx2d{0, 0},
                //
                idx2d{0, 1},
                idx2d{0, -1},
        };

        template <> struct NeighborListStaticSETraits<NeighborListStaticSE::NeighborListStaticSE2DSeg90>
        {
            POUTRE_STATIC_CONSTEXPR ptrdiff_t Rank = 2;
            POUTRE_STATIC_CONSTEXPR ptrdiff_t NbNeighbor = 3;
            const static idx2d surroundingSquareHalfSize; // FXIME should be constexpr
            const static std::array<idx2d, NbNeighbor> NlList;
            const static std::array<idx2d, NbNeighbor> NlListTransposed;
        };

        const idx2d
            NeighborListStaticSETraits<NeighborListStaticSE::NeighborListStaticSE2DSeg90>::surroundingSquareHalfSize =
                idx2d{1, 1};
        const std::array<idx2d, 3>
            NeighborListStaticSETraits<NeighborListStaticSE::NeighborListStaticSE2DSeg90>::NlList = {
                idx2d{0, 0},
                //
                idx2d{1, 0},
                idx2d{-1, 0},
        };
        const std::array<idx2d, 3>
            NeighborListStaticSETraits<NeighborListStaticSE::NeighborListStaticSE2DSeg90>::NlListTransposed = {
                idx2d{0, 0},
                //
                idx2d{1, 0},
                idx2d{-1, 0},
        };

        template <> struct NeighborListStaticSETraits<NeighborListStaticSE::NeighborListStaticSE2DSeg45>
        {
            POUTRE_STATIC_CONSTEXPR ptrdiff_t Rank = 2;
            POUTRE_STATIC_CONSTEXPR ptrdiff_t NbNeighbor = 3;
            const static idx2d surroundingSquareHalfSize; // FXIME should be constexpr
            const static std::array<idx2d, NbNeighbor> NlList;
            const static std::array<idx2d, NbNeighbor> NlListTransposed;
        };

        const idx2d
            NeighborListStaticSETraits<NeighborListStaticSE::NeighborListStaticSE2DSeg45>::surroundingSquareHalfSize =
                idx2d{1, 1};
        const std::array<idx2d, 3>
            NeighborListStaticSETraits<NeighborListStaticSE::NeighborListStaticSE2DSeg45>::NlList = {
                idx2d{0, 0},
                //
                idx2d{-1, -1},
                idx2d{1, 1},
        };
        const std::array<idx2d, 3>
            NeighborListStaticSETraits<NeighborListStaticSE::NeighborListStaticSE2DSeg45>::NlListTransposed = {
                idx2d{0, 0},
                //
                idx2d{-1, -1},
                idx2d{1, 1},
        };

        template <> struct NeighborListStaticSETraits<NeighborListStaticSE::NeighborListStaticSE2DSeg135>
        {
            POUTRE_STATIC_CONSTEXPR ptrdiff_t Rank = 2;
            POUTRE_STATIC_CONSTEXPR ptrdiff_t NbNeighbor = 3;
            const static idx2d surroundingSquareHalfSize; // FXIME should be constexpr
            const static std::array<idx2d, NbNeighbor> NlList;
            const static std::array<idx2d, NbNeighbor> NlListTransposed;
        };

        const idx2d
            NeighborListStaticSETraits<NeighborListStaticSE::NeighborListStaticSE2DSeg135>::surroundingSquareHalfSize =
                idx2d{1, 1};
        const std::array<idx2d, 3>
            NeighborListStaticSETraits<NeighborListStaticSE::NeighborListStaticSE2DSeg135>::NlList = {
                idx2d{0, 0},
                //
                idx2d{-1, 1},
                idx2d{1, -1},
        };
        const std::array<idx2d, 3>
            NeighborListStaticSETraits<NeighborListStaticSE::NeighborListStaticSE2DSeg135>::NlListTransposed = {
                idx2d{0, 0},
                //
                idx2d{-1, 1},
                idx2d{1, -1},
        };
    } // namespace se

} // namespace poutre
#endif // POUTRE_IPSENL_HPP__