
//==============================================================================
//                  Copyright (c) 2015 - Thomas Retornaz                      //
//                     thomas.retornaz@mines-paris.org                        //
//          Distributed under the Boost Software License, Version 1.0.        //
//                 See accompanying file LICENSE.txt or copy at               //
//                     http://www.boost.org/LICENSE_1_0.txt                   //
//==============================================================================

#ifndef POUTRE_COORDINATE_HPP__
#define POUTRE_COORDINATE_HPP__

/**
 * @file   poutreCoordinate.hpp
 * @author Thomas Retornaz
 * @brief
 *
 *
 */

#ifndef POUTRE_CONFIG__HPP__
#include <poutreBase/poutreConfig.hpp>
#endif

#ifndef POUTRE_TYPE_HPP__
#include <poutreBase/poutreTypes.hpp>
#endif

#ifndef POUTRE_STATIC_CONTAINER_HPP__
#include <poutreBase/poutreStaticContainer.hpp>
#endif

#include <stdexcept>
#include <array>

#ifdef _MSC_VER
#pragma warning( push )
#pragma warning( disable : 4425 )//4425 improper support constexpr
#endif

namespace poutre
{

   //forward declare
   template <int Rank> class index;
   template <int Rank> class bounds;
   template <int Rank> class bounds_iterator;


   /*!
    * @defgroup coordinate_group_group Define coordinates  with bounds, index, View and Strided view other multi-dimentionnal array
    * Implement bounds,index, View and StridedView as defined in http://www.open-std.org/jtc1/sc22/wg21/docs/papers/2014/n3851.pdf
    * @ingroup poutre_base
    * @{
    */

   /**
    * @brief bounds has N-dimensional rectangle semantic
    * bounds is a type that represents rectangular bounds on an N-dimensional discrete space
    * @return
    */

   template
      <
      int Rank
      > class bounds :public static_array_base < ptrdiff_t, Rank > //TODO restrain alignement capabilities if any
   {
   public:

      //inherit from parent
      using parent = static_array_base < ptrdiff_t, Rank > ;
      using size_type = parent::size_type;
      using value_type = typename parent::value_type;
      using const_value_type = typename  parent::const_value_type;
      using pointer = typename parent::pointer;
      using reference = typename  parent::reference;

      using const_pointer = typename  parent::const_pointer;
      using const_reference = typename  parent::const_reference;
      using difference_type = typename parent::difference_type;
      using size_type = parent::size_type;

      using self_type = bounds < Rank > ;
      using static_array_base<ptrdiff_t, Rank>::static_array_base < ptrdiff_t, Rank > ;

      using parent::rank;
      using parent::data;
      using parent::back;
      using parent::front;
      using parent::at;
      using parent::assign;
      using parent::fill;
      using parent::swap;
      //using parent::size; size overhided with a different behavior 
      //using parent::max_size;
      using parent::empty;

      using parent::operator=;
      using parent::operator==;
      using parent::operator!=;
      using parent::operator/=;
      using parent::operator*=;
      using parent::operator%=;
      using parent::operator+=;
      using parent::operator-=;
      using parent::operator/;
      using parent::operator*;
      using parent::operator%;

   protected:
      using parent::m_array;


   public:
      /** @name Construction and Assignment
       */

      /**@{*/

      POUTRE_CONSTEXPR bounds( ) :parent( ) POUTRE_NOEXCEPT_IF(parent( ))
      {
      }

      //redefine specialisation du to conflict with ctor with initializer_list
      template <typename other>
         POUTRE_CONSTEXPR explicit bounds(typename std::enable_if<std::is_convertible<other, value_type>::value && Rank == 1, value_type>::type a)   POUTRE_NOEXCEPT
      {
         m_array[0] = value_type(a);
      }

      template <typename other>
         POUTRE_CONSTEXPR bounds(typename std::enable_if<std::is_convertible<other, value_type>::value && Rank == 2, value_type>::type a0, other a1) POUTRE_NOEXCEPT
      {
         m_array[0] = value_type(a0);
         m_array[1] = value_type(a1);
      }

      template <typename other>
         POUTRE_CONSTEXPR bounds(typename std::enable_if<std::is_convertible<other, value_type>::value && Rank == 3, value_type>::type a0, other a1, value_type a2) /*: parent(a0, a1, a2) */ POUTRE_NOEXCEPT
      {
         m_array[0] = value_type(a0);
         m_array[1] = value_type(a1);
         m_array[2] = value_type(a2);
      }

      POUTRE_CXX14_CONSTEXPR bounds(const std::initializer_list<value_type>& rhs) : parent(rhs)
      {
      }

      POUTRE_CXX14_CONSTEXPR explicit bounds(const value_type(&rhs)[Rank]) : parent(rhs) POUTRE_NOEXCEPT_IF(parent(rhs))
      {
      }

      POUTRE_CONSTEXPR bounds(const parent& rhs) : parent(rhs) POUTRE_NOEXCEPT_IF(parent(rhs))
      {
      }

      /**@}*/

      /** @name Arithmetic Operators
       * Apply vector like operation on bounds with index aka translate
       */
      /**@{*/
      POUTRE_CXX14_CONSTEXPR bounds  operator+(const index<rank>& rhs) const POUTRE_NOEXCEPT
      {
         self_type tmp(*this); //return a fresh new object
         details::helper_assign_container_op<self_type, AssignOpType::AssignOpAdd, Rank>::op(rhs, tmp);
         return tmp;
      }

      POUTRE_CXX14_CONSTEXPR bounds  operator-(const index<rank>& rhs) const POUTRE_NOEXCEPT
      {
         self_type tmp(*this); //return a fresh new object
         details::helper_assign_container_op<self_type, AssignOpType::AssignOpMinus, Rank>::op(rhs, tmp);
         return tmp;
      }

      POUTRE_CXX14_CONSTEXPR bounds& operator+=(const index<rank>& rhs) POUTRE_NOEXCEPT
      {
         details::helper_assign_container_op<self_type, AssignOpType::AssignOpAdd, Rank>::op(rhs, *this);
         return *this;
      }

      POUTRE_CXX14_CONSTEXPR bounds& operator-=(const index<rank>& rhs) POUTRE_NOEXCEPT
      {
         details::helper_assign_container_op<self_type, AssignOpType::AssignOpMinus, Rank>::op(rhs, *this);
         return *this;
      }

      POUTRE_CXX14_CONSTEXPR bounds& operator+=(const value_type& rhs) POUTRE_NOEXCEPT
      {
         details::helper_assign_container_valueop<self_type, AssignOpType::AssignOpAdd, Rank>::op(rhs, *this);
         return *this;
      }

      POUTRE_CXX14_CONSTEXPR bounds& operator-=(const value_type& rhs) POUTRE_NOEXCEPT
      {
         details::helper_assign_container_valueop<self_type, AssignOpType::AssignOpMinus, Rank>::op(rhs, *this);
         return *this;
      }

      /**@}*/


      //!returns  a  hyper  volume  of  the  rectangular  space  enclosed  by  *this,  i.e.  the  product  of  all components.
      POUTRE_CONSTEXPR size_type size( ) const POUTRE_NOEXCEPT
      {
         size_t volume = m_array[0]; //at least rank==1
         for ( size_t i = 1; i < rank; i++ ) volume *= m_array[i];
         return volume;
      }

      //
      //@brief Checks whether the passed index  is contained within  bounds  
      //returns true if every component of idx is equal or greater than zero and is less than the corresponding component of *this.
      POUTRE_CXX14_CONSTEXPR bool  contains(const index<rank>& idx) const POUTRE_NOEXCEPT
      {
      return details::helper_contains_container_op<bounds<rank>, index<rank>, Rank>::op(*this,idx);
      }


      //const_iterator begin( ) const POUTRE_NOEXCEPT
      //  {
      //  return const_iterator(*this);
      //  }

      //const_iterator end( ) const POUTRE_NOEXCEPT
      //  {
      //  //FIXME
      //  return const_iterator(*this);
      //  }

   };







   /**
    * @brief  index is a type that represents an offset or a point in such space
    */
   template
      <
      int Rank
      > class index :public static_array_base < ptrdiff_t, Rank > //TODO restrain alignement capabilities if any
   {
   public:

      //inherit from parent
      using parent = static_array_base < ptrdiff_t, Rank > ;
      using size_type = parent::size_type;
      using value_type = typename parent::value_type;
      using const_value_type = typename  parent::const_value_type;
      using pointer = typename  parent::pointer;
      using reference = typename parent::reference;

      using const_pointer = typename parent::const_pointer;
      using const_reference = typename parent::const_reference;
      using difference_type = typename parent::difference_type;
      using size_type = parent::size_type;

      using self_type = index < Rank > ;
      using static_array_base<ptrdiff_t, Rank>::static_array_base < ptrdiff_t, Rank > ;

      using parent::rank;
      using parent::data;
      using parent::back;
      using parent::front;
      using parent::at;
      using parent::assign;
      using parent::fill;
      using parent::swap;
      //using parent::size;
      //using parent::max_size;
      using parent::empty;

      using parent::operator=;
      using parent::operator==;
      using parent::operator!=;
      using parent::operator/=;
      using parent::operator*=;
      using parent::operator%=;
      using parent::operator+=;
      using parent::operator-=;
      using parent::operator/;
      using parent::operator*;
      using parent::operator%;

   protected:
      using parent::m_array;


   public:
      /** @name Construction and Assignment
       */

      /**@{*/

      POUTRE_CONSTEXPR index( ) :parent( ) POUTRE_NOEXCEPT_IF(parent( ))
      {
      }

      //redefine specialisation du to conflict with ctor with initializer_list
      template <typename other>
         POUTRE_CONSTEXPR explicit index(typename std::enable_if<std::is_convertible<other, value_type>::value && Rank == 1, value_type>::type a)   POUTRE_NOEXCEPT
      {
         m_array[0] = value_type(a);
      }

      template <typename other>
         POUTRE_CONSTEXPR index(typename std::enable_if<std::is_convertible<other, value_type>::value && Rank == 2, value_type>::type a0, other a1) POUTRE_NOEXCEPT
      {
         m_array[0] = value_type(a0);
         m_array[1] = value_type(a1);
      }

      template <typename other>
         POUTRE_CONSTEXPR index(typename std::enable_if<std::is_convertible<other, value_type>::value && Rank == 3, value_type>::type a0, other a1, value_type a2) /*: parent(a0, a1, a2) */ POUTRE_NOEXCEPT
      {
         m_array[0] = value_type(a0);
         m_array[1] = value_type(a1);
         m_array[2] = value_type(a2);
      }

      POUTRE_CXX14_CONSTEXPR index(const std::initializer_list<value_type>& rhs) : parent(rhs)
      {
      }

      POUTRE_CXX14_CONSTEXPR explicit index(const value_type(&rhs)[Rank]) : parent(rhs) POUTRE_NOEXCEPT_IF(parent(rhs))
      {
      }


      POUTRE_CONSTEXPR index(const parent& rhs) : parent(rhs) POUTRE_NOEXCEPT_IF(parent(rhs))
        {
        }

      /**@}*/


      /** @name Arithmetic Operators
       * The binary operator+  and operator-  perform the corresponding operation component-wise on a  copy of
       * *this  and  the  function  argument,  returning  the  copy.  operator+=  and  operator-=  work  analogously,
       * operating on and returning *this.
       *
       * Pre-  and  post-  increment  and  decrement  operators  have  the  traditional  semantics,  and  as  such  are
       * allowed only on index  with  rank = 1.  This decision follows the logic that e.g. ++idx  shall be equivalent to
       * idx += 1, which with implicit constructor is naturally supported for rank = 1 as idx += index<1>{ 1 }, while
       * ill-formed for any other rank.
       *
       * The  unary  operator+  returns  a copy of *this,  while  the  unary  operator-  returns  a  copy  of  the  object  with  all
       * components negated.
       */
      POUTRE_CXX14_CONSTEXPR self_type  operator+(const self_type& rhs) const POUTRE_NOEXCEPT
      {
         self_type tmp(*this); //return a fresh new object
         details::helper_assign_container_op<self_type, AssignOpType::AssignOpAdd, Rank>::op(rhs, tmp);
         return tmp;
      }

      POUTRE_CXX14_CONSTEXPR self_type  operator-(const self_type& rhs) const POUTRE_NOEXCEPT
      {
         self_type tmp(*this); //return a fresh new object
         details::helper_assign_container_op<self_type, AssignOpType::AssignOpMinus, Rank>::op(rhs, tmp);
         return tmp;
      }

      POUTRE_CXX14_CONSTEXPR self_type& operator+=(const self_type& rhs) POUTRE_NOEXCEPT
      {
         details::helper_assign_container_op<self_type, AssignOpType::AssignOpAdd, Rank>::op(rhs, *this);
         return *this;
      }

      POUTRE_CXX14_CONSTEXPR self_type& operator-=(const self_type& rhs) POUTRE_NOEXCEPT
      {
         details::helper_assign_container_op<self_type, AssignOpType::AssignOpMinus, Rank>::op(rhs, *this);
         return *this;
      }


      template <int Rank, typename std::enable_if<Rank == 1, void>::type* = nullptr>
         POUTRE_CXX14_CONSTEXPR self_type& operator++() POUTRE_NOEXCEPT
      {
         (++(*this)[0]);
         return *this;
      }

      template <int Rank, typename std::enable_if<Rank == 1, void>::type* = nullptr>
         POUTRE_CXX14_CONSTEXPR self_type  operator++(int) POUTRE_NOEXCEPT
      {
         return (index < Rank > {(*this)[0]++});
      }

      template <int Rank, typename std::enable_if<Rank == 1, void>::type* = nullptr>
         POUTRE_CXX14_CONSTEXPR self_type& operator--() POUTRE_NOEXCEPT
      {
         (--(*this)[0]);
         return *this;
      }

      template <int Rank, typename std::enable_if<Rank == 1, void>::type* = nullptr>
         POUTRE_CXX14_CONSTEXPR self_type  operator--(int) POUTRE_NOEXCEPT
      {
         return (index < Rank > {(*this)[0]--});
      }


      POUTRE_CXX14_CONSTEXPR self_type operator+() const POUTRE_NOEXCEPT
      {
         return *this;
      }

      POUTRE_CXX14_CONSTEXPR self_type operator-() const POUTRE_NOEXCEPT
      {
         auto tmp(*this); //return a fresh new object
         details::helper_assign_container_op<self_type, AssignOpType::AssignOpNegate, Rank>::op(tmp, tmp);
         return tmp;
      }


      POUTRE_CXX14_CONSTEXPR self_type& operator+=(const value_type& rhs) POUTRE_NOEXCEPT
      {
         details::helper_assign_container_valueop<self_type, AssignOpType::AssignOpAdd, Rank>::op(rhs, *this);
         return *this;
      }

      POUTRE_CXX14_CONSTEXPR self_type& operator-=(const value_type& rhs) POUTRE_NOEXCEPT
      {
         details::helper_assign_container_valueop<self_type, AssignOpType::AssignOpMinus, Rank>::op(rhs, *this);
         return *this;
      }

      /**@}*/


   };

   template <int Rank>
      POUTRE_CXX14_CONSTEXPR bounds<Rank>  operator+(const index<Rank>& lhs, const bounds<Rank>& rhs) POUTRE_NOEXCEPT
   {
      return (bounds<Rank>(rhs) +lhs);
   }

   template <int Rank>
      POUTRE_CXX14_CONSTEXPR bounds<Rank> operator*(ptrdiff_t v, const bounds<Rank>& rhs) POUTRE_NOEXCEPT
   {      
      return (bounds<Rank>(rhs)*v);
   }

   template <int Rank>
      POUTRE_CXX14_CONSTEXPR index<Rank>  operator*(ptrdiff_t v, const index<Rank>& rhs) POUTRE_NOEXCEPT
   {
      return (index<Rank>(rhs)*v);
   }


   /**
    * @brief Iterator over bounds NOT an iterator on value_type of a container
    *
    * The bounds_iterator  is  provided as an  interoperability feature, enabling  the  usage of the  multidimensional
    * indices  with the  existing non-mutable iterator-based algorithms. The bounds_iterator  is  dissimilar to other
    * C++ Library iterators, as it does not perform iterations over containers or streams, but rather over an
    * imaginary space imposed by  the  bounds. Dereferencing the iterator returns  an  index  object designating
    * the current element in the space.
    */
   template <int Rank>
      class bounds_iterator : public std::iterator <
      std::random_access_iterator_tag,
      index<Rank>,
      ptrdiff_t,
      const index<Rank>,
      const index < Rank >> //NOTE HERE BY VALUE
   {
      static_assert(Rank > 0,
                    "bounds_iterator requires a Rank>0");
   public:
      using parent = std::iterator < std::random_access_iterator_tag, index<Rank>, ptrdiff_t, const index<Rank>, const index<Rank> > ;
      using self_type = bounds_iterator < Rank > ;

   protected:
      bounds<Rank> m_bnd;
      index<Rank>  m_idx;

   public:
      bounds_iterator(const bounds<Rank>& bnd, const index<Rank>& curr = index < Rank > {}) POUTRE_NOEXCEPT : m_bnd(bnd), m_idx(curr)
      {
         //The precondition is that bnd.contains(curr) unless bnd.size() = 0.
         if ( bnd.size( ) != 0 && bnd.contains(m_idx) )
         {
            bnd += m_idx;
         }
         else
         {//invalidate bounds 
            bnd = bounds < Rank >( );
         }
      }

      bounds_iterator(const self_type& rhs) = default;
      self_type& operator=(const self_type& rhs) = default;

      ~bounds_iterator( )
      {
      }

      operator bool( ) const
      {
         return (0 != m_bounds.size( ));
      }

      bool         operator==(const self_type& rhs) const POUTRE_NOEXCEPT
      {
         return (m_idx == rhs.m_idx);
      }
      bool         operator!=(const self_type& rhs) const POUTRE_NOEXCEPT
      {
         return (m_idx != rhs.m_idx);
      }


      /*bool operator<(const bounds_iterator& rhs) const POUTRE_NOEXCEPT
        bool operator<=(const bounds_iterator& rhs) const POUTRE_NOEXCEPT
        bool operator>(const bounds_iterator& rhs) const POUTRE_NOEXCEPT
        bool operator>=(const bounds_iterator& rhs) const POUTRE_NOEXCEPT*/

      bounds_iterator& operator++()
      {
         for ( auto i = rank - 1; i >= 0; --i )
         {
            if ( ++m_idx[i] < m_bounds[i] )
               return;
            m_idx[i] = 0;
         }
         //FIXME
         //m_idx = unspecified past - the - end value;
         return *this;
      }

      bounds_iterator  operator++(int)
      {
         return ++tmp(*this);
      }

      /* bounds_iterator& operator--();
         bounds_iterator  operator--(int);*/

      /* bounds_iterator  operator+(difference_type n) const;
         bounds_iterator& operator+=(difference_type n);
         bounds_iterator  operator-(difference_type n) const;
         bounds_iterator& operator-=(difference_type n);

         difference_type  operator-(const bounds_iterator& rhs) const;

         reference operator*() const;
         pointer   operator->() const;
         reference operator[](difference_type n) const;*/

      //!Note here a return by value
      value_type                  operator*() POUTRE_NOEXCEPT{ return m_idx; }

      //!Note here a return by value
      value_type                  operator*() const  POUTRE_NOEXCEPT{ return m_idx; }


      //const offset                              getOffset( ) const POUTRE_NOEXCEPT { return m_ptr - m_ptrorig; }

      };

      template <int Rank> bounds_iterator<Rank> begin(const bounds<Rank>& bnd) POUTRE_NOEXCEPT
      {
         return  bounds_iterator(bnd);
      }

      template <int Rank> bounds_iterator<Rank> end(const bounds<Rank>& bnd) POUTRE_NOEXCEPT
      {
         //FIXME
         return  bounds_iterator(bnd);
      }




      //! @} doxygroup: coordinate_group   

      //extern template class bounds < 1 > ;
      //extern template class bounds < 2 > ;
      //extern template class bounds < 3 > ;
      //extern template class bounds < 4 > ;

      //extern template class index <1>;
      //extern template class index <2>;
      //extern template class index <3>;
      //extern template class index <4>;

      typedef bounds <1> bounds_1;
      typedef bounds <2> bounds_2;
      typedef bounds <3> bounds_3;
      typedef bounds <4> bounds_4;

      typedef index <1> index_1;
      typedef index <2> index_2;
      typedef index <3> index_3;
      typedef index <4> index_4;


   } //namespace

#ifdef _MSC_VER
#pragma warning( pop ) 
#endif

#endif //POUTRE_COORDINATE_HPP__
