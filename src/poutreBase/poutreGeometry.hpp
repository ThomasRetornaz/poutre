
//==============================================================================
//                  Copyright (c) 2015 - Thomas Retornaz                      //
//                     thomas.retornaz@mines-paris.org                        //
//          Distributed under the Boost Software License, Version 1.0.        //
//                 See accompanying file LICENSE.txt or copy at               //
//                     http://www.boost.org/LICENSE_1_0.txt                   //
//==============================================================================

#ifndef POUTRE_GEOMETRY_HPP__
#define POUTRE_GEOMETRY_HPP__

/**
 * @file   poutreGeometry.hpp
 * @author Thomas Retornaz
 * @brief  Define points, bounds suitable for geometricals artithmetic
 *
 *
 */

#include <boost/geometry.hpp>
#include <boost/geometry/geometries/box.hpp>
#include <boost/geometry/geometries/point.hpp>
#include <boost/geometry/index/rtree.hpp>
#include <poutreBase/poutreTypes.hpp>

//#ifdef _MSC_VER
//#pragma warning( push )
//#pragma warning( disable : 4244 )
//#endif

namespace poutre
{
  /*!
   * @defgroup geometry_group Define common geometry object
   * @ingroup poutre_base_group
   * @{
   */
  // see http://www.boost.org/doc/libs/1_58_0/libs/geometry/doc/html/geometry/matrix.html
  namespace bg  = boost::geometry;
  namespace bgi = boost::geometry::index;

  // alias template over boost::geometry
  template<typename T, int Rank> using point_t = bg::model::point<T, Rank, bg::cs::cartesian>;
  template<int Rank> using point_rcoord        = point_t<rcoord, Rank>;
  template<int Rank> using point_scoord        = point_t<scoord, Rank>;

  template<int Rank> using bounding_box = bg::model::box<point_t<scoord, Rank>>;

  using pt1D_scoord = point_scoord<1>; //! alias scalar point 1D
  using pt2D_scoord = point_scoord<2>; //! alias scalar point 2D
  using pt3D_scoord = point_scoord<3>; //! alias scalar point 3D
  using pt4D_scoord = point_scoord<4>; //! alias scalar point 4D

  using bb1D_scoord = bounding_box<1>; //! alias boundingbox 1D
  using bb2D_scoord = bounding_box<2>; //! alias boundingbox 2D
  using bb3D_scoord = bounding_box<3>; //! alias boundingbox 3D
  using bb4D_scoord = bounding_box<4>; //! alias boundingbox 4D

  using pt1D_rcoord = point_rcoord<1>; //! alias real point 1D
  using pt2D_rcoord = point_rcoord<2>; //! alias real point 2D
  using pt3D_rcoord = point_rcoord<3>; //! alias real point 3D
  using pt4D_rcoord = point_rcoord<4>; //! alias real point 4D
                                       /**
                                       //! @} doxygroup: geometry_group
                                       */
} // namespace poutre

//#ifdef _MSC_VER
//#pragma warning( pop )
//#endif

#endif // POUTRE_GEOMETRY_HPP__
