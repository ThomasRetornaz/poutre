
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

#ifndef POUTRE_TYPE_HPP__
#include <poutreBase/poutreTypes.hpp>
#endif

#include <boost/geometry.hpp>
#include <boost/geometry/geometries/box.hpp>
#include <boost/geometry/geometries/point.hpp>
#include <boost/geometry/index/rtree.hpp>

//#ifdef _MSC_VER
//#pragma warning( push )
//#pragma warning( disable : 4244 )
//#endif

namespace poutre
{
    // see http://www.boost.org/doc/libs/1_58_0/libs/geometry/doc/html/geometry/matrix.html
    namespace bg = boost::geometry;
    namespace bgi = boost::geometry::index;

    // alias template over boost::geometry
    template <typename T, int NumDims> using point_t = bg::model::point<T, NumDims, bg::cs::cartesian>;
    template <int NumDims> using point_rcoord = point_t<rcoord, NumDims>;
    template <int NumDims> using point_scoord = point_t<scoord, NumDims>;

    template <int NumDims> using bounding_box = bg::model::box<point_t<scoord, NumDims>>;

    // extern template class point_scoord<1>;
    // extern template class point_scoord<2>;
    // extern template class point_scoord<3>;
    // extern template class point_scoord<4>;

    // extern template class point_rcoord<1>;
    // extern template class point_rcoord<2>;
    // extern template class point_rcoord<3>;
    // extern template class point_rcoord<4>;

    typedef point_scoord<1> pt1D_scoord; //! alias scalar point 1D
    typedef point_scoord<2> pt2D_scoord; //! alias scalar point 2D
    typedef point_scoord<3> pt3D_scoord; //! alias scalar point 3D
    typedef point_scoord<4> pt4D_scoord; //! alias scalar point 4D

    typedef bounding_box<1> bb1D_scoord; //! alias boundingbox 1D
    typedef bounding_box<2> bb2D_scoord; //! alias boundingbox 2D
    typedef bounding_box<3> bb3D_scoord; //! alias boundingbox 3D
    typedef bounding_box<4> bb4D_scoord; //! alias boundingbox 4D

    typedef point_rcoord<1> pt1D_rcoord; //! alias real point 1D
    typedef point_rcoord<2> pt2D_rcoord; //! alias real point 2D
    typedef point_rcoord<3> pt3D_rcoord; //! alias real point 3D
    typedef point_rcoord<4> pt4D_rcoord; //! alias real point 4D

} // namespace poutre

//#ifdef _MSC_VER
//#pragma warning( pop )
//#endif

#endif // POUTRE_GEOMETRY_HPP__
