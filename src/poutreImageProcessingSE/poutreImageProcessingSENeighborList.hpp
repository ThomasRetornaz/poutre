
//==============================================================================
//                  Copyright (c) 2018 - Thomas Retornaz                      //
//                     thomas.retornaz@mines-paris.org                        //
//          Distributed under the Boost Software License, Version 1.0.        //
//                 See accompanying file LICENSE.txt or copy at               //
//                     http://www.boost.org/LICENSE_1_0.txt                   //
//==============================================================================

#ifndef POUTRE_IPSENL_HPP__
#define POUTRE_IPSENL_HPP__

#include <poutreBase/poutreContainerView.hpp>
#include <poutreImageProcessingSE/include/poutreImageProcessingSENeighborhood.hpp>
#include <poutreImageProcessingSE/poutreImageProcessingSEInterface.hpp>

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
    enum class NLS
    {
      NLS_Undef,         //!< Undefined type
      NLS_c2_1d,         //!< Connection 1D segement SE (symmetric no center)
      NLS_c8_2d,         //!< Connection 2D Square SE (symmetric no center)
      NLS_c4_2d,         //!< Connection 2D Cross SE (symmetric no center)
      NLS_c2_2dH,        //!< Connection 2D Horizontal Segement (symmetric no center)
      NLS_c2_2dV,        //!< Connection 2D Vertical Segement (symmetric no center)
      NLS_c2_2d_Diag45,  //!< Connection 2D 45 degree Segement (symmetric no center)
      NLS_c2_2d_Diag135, //!< Connection 2D 135 degree Segement (symmetric no center)
      NLS_c6_3d,         //!< Connection 3D C6 (symmetric no center)
      NLS_c26_3d,        //!< Connection 3D C6 (symmetric no center)
    };
    //! @} // se_nl_grp

    /************************************************/
    //---1D
    /************************************************/

    struct c2_1D_t : neighborhood_t_interface<c2_1D_t>
    {
      public:
      using idx    = idx1d;
      using view_t = array_view<const idx, 1>;

      private:
      static POUTRE_CONSTEXPR const std::array<idx, 2> m_neighboursList =
          // clang-format off
        {{
            idx1d{-1},          idx1d{+1},
        }};
      // clang-format on

      public:
      NLS nlstype        = NLS::NLS_c2_1d;
      using category     = static_neighbourhood_tag;
      using incremental  = std::false_type;
      using decomposable = std::false_type;
      using separable    = std::false_type;

      POUTRE_STATIC_CONSTEXPR ptrdiff_t Rank         = 1;
      POUTRE_STATIC_CONSTEXPR ptrdiff_t NbNeighbours = 2;

      POUTRE_STATIC_CONSTEXPR view_t offsets() { return {m_neighboursList}; }
      POUTRE_STATIC_CONSTEXPR view_t before_offsets() { return {m_neighboursList.data(), bd1d {1}}; }
      POUTRE_STATIC_CONSTEXPR view_t after_offsets() { return {m_neighboursList.data() + 1, bd1d {1}}; }

      POUTRE_STATIC_CONSTEXPR ptrdiff_t radial_extent() { return 1; }
    };

    const POUTRE_STATIC_CONSTEXPR c2_1D_t c2_1D = {};

    /************************************************/
    //---2D
    /************************************************/

    struct c2_2DH_t : neighborhood_t_interface<c2_2DH_t>
    {
      public:
      using view_t = array_view<const idx2d, 1>;

      private:
      static POUTRE_CONSTEXPR const std::array<idx2d, 2> m_neighboursList =
          // clang-format off
        {{
            idx2d{-1,+0},          idx2d{+1,+0},
        }};
      // clang-format on

      public:
      NLS nlstype        = NLS::NLS_c2_2dH;
      using category     = static_neighbourhood_tag;
      using incremental  = std::false_type;
      using decomposable = std::false_type;
      using separable    = std::false_type;

      POUTRE_STATIC_CONSTEXPR ptrdiff_t Rank         = 2;
      POUTRE_STATIC_CONSTEXPR ptrdiff_t NbNeighbours = 2;

      POUTRE_STATIC_CONSTEXPR view_t offsets() { return {m_neighboursList}; }
      POUTRE_STATIC_CONSTEXPR view_t before_offsets() { return {m_neighboursList.data(), bd1d {1}}; }
      POUTRE_STATIC_CONSTEXPR view_t after_offsets() { return {m_neighboursList.data() + 1, bd1d {1}}; }

      POUTRE_STATIC_CONSTEXPR ptrdiff_t radial_extent() { return 1; }
    };

    const POUTRE_STATIC_CONSTEXPR c2_2DH_t c2_2DH = {};

    struct c2_2DV_t : neighborhood_t_interface<c2_2DV_t>
    {
      public:
      using view_t = array_view<const idx2d, 1>;

      private:
      static POUTRE_CONSTEXPR const std::array<idx2d, 2> m_neighboursList =
          // clang-format off
        {{
            idx2d{+0,-1},
            idx2d{+0,+1}
        }};
      // clang-format on

      public:
      NLS nlstype        = NLS::NLS_c2_2dV;
      using category     = static_neighbourhood_tag;
      using incremental  = std::false_type;
      using decomposable = std::false_type;
      using separable    = std::false_type;

      POUTRE_STATIC_CONSTEXPR ptrdiff_t Rank         = 2;
      POUTRE_STATIC_CONSTEXPR ptrdiff_t NbNeighbours = 2;

      POUTRE_STATIC_CONSTEXPR view_t offsets() { return {m_neighboursList}; }
      POUTRE_STATIC_CONSTEXPR view_t before_offsets() { return {m_neighboursList.data(), bd1d {1}}; }
      POUTRE_STATIC_CONSTEXPR view_t after_offsets() { return {m_neighboursList.data() + 1, bd1d {1}}; }

      POUTRE_STATIC_CONSTEXPR ptrdiff_t radial_extent() { return 1; }
    };

    const POUTRE_STATIC_CONSTEXPR c2_2DV_t c2_2DV = {};

    struct c2_2D_Diag45_t : neighborhood_t_interface<c2_2D_Diag45_t>
    {
      public:
      using view_t = array_view<const idx2d, 1>;

      private:
      static POUTRE_CONSTEXPR const std::array<idx2d, 2> m_neighboursList =
          // clang-format off
        {{
                        idx2d{+1,-1},

            idx2d{-1,+1}
        }};

      public:
      NLS nlstype        = NLS::NLS_c2_2d_Diag45;
      using category     = static_neighbourhood_tag;
      using incremental  = std::false_type;
      using decomposable = std::false_type;
      using separable    = std::false_type;

      POUTRE_STATIC_CONSTEXPR ptrdiff_t Rank         = 2;
      POUTRE_STATIC_CONSTEXPR ptrdiff_t NbNeighbours = 2;

      POUTRE_STATIC_CONSTEXPR view_t offsets() { return {m_neighboursList}; }
      POUTRE_STATIC_CONSTEXPR view_t before_offsets() { return {m_neighboursList.data(), bd1d {1}}; }
      POUTRE_STATIC_CONSTEXPR view_t after_offsets() { return {m_neighboursList.data() + 1, bd1d {1}}; }

      POUTRE_STATIC_CONSTEXPR ptrdiff_t radial_extent() { return 1; }
    };

    // clang-format on
    const POUTRE_STATIC_CONSTEXPR c2_2D_Diag45_t c2_2D_Diag45 = {};

    struct c2_2D_Diag135_t : neighborhood_t_interface<c2_2D_Diag135_t>
    {
      public:
      using view_t = array_view<const idx2d, 1>;

      private:
      static POUTRE_CONSTEXPR const std::array<idx2d, 2> m_neighboursList =
          // clang-format off
      {{
            idx2d{-1,-1},

                          idx2d{+1,+1}
        }};
      // clang-format on

      public:
      NLS nlstype        = NLS::NLS_c2_2d_Diag135;
      using category     = static_neighbourhood_tag;
      using incremental  = std::false_type;
      using decomposable = std::false_type;
      using separable    = std::false_type;

      POUTRE_STATIC_CONSTEXPR ptrdiff_t Rank         = 2;
      POUTRE_STATIC_CONSTEXPR ptrdiff_t NbNeighbours = 2;

      POUTRE_STATIC_CONSTEXPR view_t offsets() { return {m_neighboursList}; }
      POUTRE_STATIC_CONSTEXPR view_t before_offsets() { return {m_neighboursList.data(), bd1d {1}}; }
      POUTRE_STATIC_CONSTEXPR view_t after_offsets() { return {m_neighboursList.data() + 1, bd1d {1}}; }

      POUTRE_STATIC_CONSTEXPR ptrdiff_t radial_extent() { return 1; }
    };

    const POUTRE_STATIC_CONSTEXPR c2_2D_Diag135_t c2_2D_Diag135 = {};

    struct c4_2D_t : neighborhood_t_interface<c4_2D_t>
    {
      public:
      using view_t = array_view<const idx2d, 1>;

      private:
      static POUTRE_CONSTEXPR const std::array<idx2d, 4> m_neighboursList =
          // clang-format off
      {{
                 {+0, -1},
        {-1, +0},          {+1, +0},
                 {+0, +1}
        }};
      // clang-format on

      public:
      NLS nlstype                                    = NLS::NLS_c4_2d;
      using category                                 = static_neighbourhood_tag;
      using incremental                              = std::false_type;
      using decomposable                             = std::false_type;
      using separable                                = std::false_type;
      POUTRE_STATIC_CONSTEXPR ptrdiff_t Rank         = 2;
      POUTRE_STATIC_CONSTEXPR ptrdiff_t NbNeighbours = 4;

      POUTRE_STATIC_CONSTEXPR view_t offsets() { return {m_neighboursList}; }
      POUTRE_STATIC_CONSTEXPR view_t before_offsets() { return {m_neighboursList.data(), bd1d {2}}; }
      POUTRE_STATIC_CONSTEXPR view_t after_offsets() { return {m_neighboursList.data() + 2, bd1d {2}}; }

      POUTRE_STATIC_CONSTEXPR ptrdiff_t radial_extent() { return 1; }
    };

    const POUTRE_STATIC_CONSTEXPR c4_2D_t c4_2D = {};

    struct c8_2D_t : neighborhood_t_interface<c8_2D_t>
    {
      public:
      using view_t = array_view<const idx2d, 1>;

      private:
      static POUTRE_CONSTEXPR const std::array<idx2d, 8> m_neighboursList =
          // clang-format off
      {{
        {-1, -1}, {+0, -1}, {+1, -1},
        {-1, +0},           {+1, +0},
        {-1, +1}, {+0, +1}, {+1, +1}
        }};
      // clang-format on

      public:
      NLS nlstype                                    = NLS::NLS_c8_2d;
      using category                                 = static_neighbourhood_tag;
      using incremental                              = std::false_type;
      using decomposable                             = std::false_type;
      using separable                                = std::false_type;
      POUTRE_STATIC_CONSTEXPR ptrdiff_t Rank         = 2;
      POUTRE_STATIC_CONSTEXPR ptrdiff_t NbNeighbours = 8;

      POUTRE_STATIC_CONSTEXPR view_t offsets() { return {m_neighboursList}; }
      POUTRE_STATIC_CONSTEXPR view_t before_offsets()
      {
        return {m_neighboursList.data(), bd1d {4}}; //-V112
      }
      POUTRE_STATIC_CONSTEXPR view_t after_offsets()
      {
        return {m_neighboursList.data() + 4, bd1d {4}}; //-V112
      }

      POUTRE_STATIC_CONSTEXPR ptrdiff_t radial_extent() { return 1; }
    };
    const POUTRE_STATIC_CONSTEXPR c8_2D_t c8_2D = {};

    /************************************************/
    //---3D
    /************************************************/
    struct c6_3D_t : neighborhood_t_interface<c6_3D_t>
    {
      public:
      using view_t = array_view<const idx3d, 1>;

      private:
      static POUTRE_CONSTEXPR const std::array<idx3d, 6> m_neighboursList =
          // clang-format off
      {{
          {+0, +0, -1}, {+0, -1, +0}, {-1, +0, +0},          {+1, +0, +0}, {+0, +1, +0}, {+0, +0, +1}
          }};
      // clang-format on

      public:
      NLS nlstype                                    = NLS::NLS_c6_3d;
      using category                                 = static_neighbourhood_tag;
      using incremental                              = std::false_type;
      using decomposable                             = std::false_type;
      using separable                                = std::false_type;
      POUTRE_STATIC_CONSTEXPR ptrdiff_t Rank         = 3;
      POUTRE_STATIC_CONSTEXPR ptrdiff_t NbNeighbours = 6;

      POUTRE_STATIC_CONSTEXPR view_t offsets() { return {m_neighboursList}; }
      POUTRE_STATIC_CONSTEXPR view_t before_offsets() { return {m_neighboursList.data(), bd1d {3}}; }
      POUTRE_STATIC_CONSTEXPR view_t after_offsets() { return {m_neighboursList.data() + 3, bd1d {3}}; }

      POUTRE_STATIC_CONSTEXPR ptrdiff_t radial_extent() { return 1; }
    };

    const POUTRE_STATIC_CONSTEXPR c6_3D_t c6_3D = {};

    struct c26_3D_t : neighborhood_t_interface<c26_3D_t>
    {
      public:
      using view_t = array_view<const idx3d, 1>;

      private:
      static POUTRE_CONSTEXPR const std::array<idx3d, 26> m_neighboursList =
          // clang-format off
      {{
        {-1, -1, -1}, {+0, -1, -1}, {+1, -1, -1},
        {-1, +0, -1}, {+0, +0, -1}, {+1, +0, -1},
        {-1, +1, -1}, {+0, +1, -1}, {+1, +1, -1},
        // ----
        {-1, -1, +0}, {+0, -1, +0}, {+1, -1, +0},
        {-1, +0, +0},               {+1, +0, +0},
        {-1, +1, +0}, {+0, +1, +0}, {+1, +1, +0},
        // ----
        {-1, -1, +1}, {+0, -1, +1}, {+1, -1, +1},
        {-1, +0, +1}, {+0, +0, +1}, {+1, +0, +1},
        {-1, +1, +1}, {+0, +1, +1}, {+1, +1, +1},
     }};
      // clang-format on

      public:
      NLS nlstype                                    = NLS::NLS_c26_3d;
      using category                                 = static_neighbourhood_tag;
      using incremental                              = std::false_type;
      using decomposable                             = std::false_type;
      using separable                                = std::false_type;
      POUTRE_STATIC_CONSTEXPR ptrdiff_t Rank         = 3;
      POUTRE_STATIC_CONSTEXPR ptrdiff_t NbNeighbours = 6;

      POUTRE_STATIC_CONSTEXPR view_t offsets() { return {m_neighboursList}; }
      POUTRE_STATIC_CONSTEXPR view_t before_offsets()
      {
        return {m_neighboursList.data(), bd1d {4}}; //-V112
      }
      POUTRE_STATIC_CONSTEXPR view_t after_offsets()
      {
        return {m_neighboursList.data() + 4, bd1d {4}}; //-V112
      }

      POUTRE_STATIC_CONSTEXPR ptrdiff_t radial_extent() { return 1; }
    };

    const POUTRE_STATIC_CONSTEXPR c26_3D_t c26_3D = {};

    template<NLS> struct NeighborListStaticSETraits
    {
    };

    template<> struct NeighborListStaticSETraits<NLS::NLS_c2_1d>
    {
      using nlStaticSE = c2_1D_t;
    };

    template<> struct NeighborListStaticSETraits<NLS::NLS_c8_2d>
    {
      using nlStaticSE = c8_2D_t;
    };

    template<> struct NeighborListStaticSETraits<NLS::NLS_c4_2d>
    {
      using nlStaticSE = c4_2D_t;
    };
    template<> struct NeighborListStaticSETraits<NLS::NLS_c2_2dH>
    {
      using nlStaticSE = c2_2DH_t;
    };
    template<> struct NeighborListStaticSETraits<NLS::NLS_c2_2dV>
    {
      using nlStaticSE = c2_2DH_t;
    };
    template<> struct NeighborListStaticSETraits<NLS::NLS_c2_2d_Diag45>
    {
      using nlStaticSE = c2_2D_Diag45_t;
    };
    template<> struct NeighborListStaticSETraits<NLS::NLS_c2_2d_Diag135>
    {
      using nlStaticSE = c2_2D_Diag135_t;
    };
    template<> struct NeighborListStaticSETraits<NLS::NLS_c6_3d>
    {
      using nlStaticSE = c6_3D_t;
    };
    template<> struct NeighborListStaticSETraits<NLS::NLS_c26_3d>
    {
      using nlStaticSE = c26_3D_t;
    };
  } // namespace se

} // namespace poutre
#endif // POUTRE_IPSENL_HPP__