
//==============================================================================
//                  Copyright (c) 2015 - Thomas Retornaz                      //
//                     thomas.retornaz@mines-paris.org                        //
//          Distributed under the Boost Software License, Version 1.0.        //
//                 See accompanying file LICENSE.txt or copy at               //
//                     http://www.boost.org/LICENSE_1_0.txt                   //
//==============================================================================

#ifndef POUTRE_CONTAINER_VIEW_HPP__
#define POUTRE_CONTAINER_VIEW_HPP__

/**
 * @file   poutreContainerView.hpp
 * @author Thomas Retornaz
 * @brief Implement View and StridedView as defined in http://www.open-std.org/jtc1/sc22/wg21/docs/papers/2014/n3851.pdf
 *
 *
 */

#ifndef POUTRE_COORDINATE_HPP__
#include <poutreBase/poutreCoordinate.hpp>
#endif

#ifdef _MSC_VER
#pragma warning( push )
#pragma warning( disable : 4425 )//4425 improper support constexpr
#endif

namespace poutre
{
   /*!
    * @defgroup view_group Container View and Strided View
    * Implement View and StridedView as defined in http://www.open-std.org/jtc1/sc22/wg21/docs/papers/2014/n3851.pdf
    * @ingroup poutre_base
    * @{
    */

  //forward declare
  template <class T, int Rank = 1> class array_view;
  //forward declare
  template <class T, int Rank = 1> class strided_array_view;



   //An array_view is a potentially multidimensional view on a sequence of uniformly strided objects of a uniform type, contiguous in the least significant dimension.

  template <class valuetype, int Rank = 1>
  class array_view
    {
    public:
      POUTRE_STATIC_CONSTEXPR int rank = Rank;
      using index_type = index<Rank>;
      using bounds_type = bounds<Rank>;


      using size_type = size_t;
      using value_type = valuetype;
      using const_value_type = std::add_const_t < value_type >;
      using pointer = std::add_pointer_t < value_type >;
      using reference = typename std::add_lvalue_reference < value_type >::type;
      using const_pointer = std::add_pointer_t < const_value_type >;
      using const_reference = typename std::add_lvalue_reference < const_value_type >::type;
      using difference_type = std::ptrdiff_t;
      
      using self_type = array_view < value_type, rank >;
      
    private:
      bounds_type m_view_bnd;
      index_type m_view_idx;
      pointer m_data;

    public:
      /** @name Construction and Assignment
      */

      /**@{*/
      POUTRE_CONSTEXPR array_view( ) POUTRE_NOEXCEPT;

      template <class Viewable>
      POUTRE_CONSTEXPR array_view(Viewable&& vw);                              // only if Rank == 1
      
      template <class U, int AnyRank>
      POUTRE_CONSTEXPR array_view(const array_view<U, AnyRank>& rhs) POUTRE_NOEXCEPT; // only if Rank == 1

      template <class ArrayType>
      POUTRE_CONSTEXPR array_view(ArrayType& arr) POUTRE_NOEXCEPT;                    // only if Rank == rank_v<ArrayType>

      template <class U>
      POUTRE_CONSTEXPR array_view(const array_view<U, Rank>& rhs) POUTRE_NOEXCEPT;

      template <class Viewable>
      POUTRE_CONSTEXPR array_view(Viewable&& vw, bounds_type bounds);
      
      POUTRE_CONSTEXPR array_view(pointer ptr, bounds_type bounds);

      template <class U>
      POUTRE_CONSTEXPR array_view& operator=(const array_view<U, Rank>& rhs) POUTRE_NOEXCEPT;
      /**@}*/

      /** @name Observers
      */
      /**@{*/
      POUTRE_CONSTEXPR bounds_type bounds( ) const POUTRE_NOEXCEPT;
      POUTRE_CONSTEXPR size_type   size( ) const POUTRE_NOEXCEPT;
      POUTRE_CONSTEXPR index_type  stride( ) const POUTRE_NOEXCEPT;
      POUTRE_CONSTEXPR pointer     data( ) const POUTRE_NOEXCEPT;
      /**@}*/


      /** @name Access
      */
      /**@{*/

      POUTRE_CONSTEXPR reference operator[](const index_type& idx) const;
      /**@}*/

      /** @name Slicing and sectioning
      */
      /**@{*/
      POUTRE_CONSTEXPR array_view<T, Rank - 1>
        operator[](difference_type slice) const;      // only if Rank > 1

      POUTRE_CONSTEXPR strided_array_view<T, Rank>
        section(const index_type& origin, const bounds_type& section_bnd) const;

      POUTRE_CONSTEXPR strided_array_view<T, Rank>
        section(const index_type& origin) const;
      /**@}*/
    };

   //A strided_array_view is a potentially multidimensional view on a sequence of uniformly strided objects of a uniform type

   //! @} doxygroup: coordinate_group   

} //namespace

#ifdef _MSC_VER
#pragma warning( pop ) 
#endif

#endif //POUTRE_CONTAINER_VIEW_HPP__
