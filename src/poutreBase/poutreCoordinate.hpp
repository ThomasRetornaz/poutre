
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
 * @brief Implement bounds,index, View and StridedView as defined in
 * https://isocpp.org/files/papers/N4494.html
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

#include <array>
#include <iostream>
#include <stdexcept>

#ifdef _MSC_VER
#pragma warning(push)
#pragma warning(disable : 4425) // 4425 improper support constexpr
#endif

namespace poutre
{
#ifndef DOXYGEN_SHOULD_SKIP_THIS
    // forward declare
    template <ptrdiff_t Rank> class index;
    template <ptrdiff_t Rank> class bounds;
    template <ptrdiff_t Rank> class bounds_iterator;
#endif

    /*!
     * @defgroup coordinate_group Define coordinates  with bounds, index
     * View and Strided view other multi-dimentionnal array Implement bounds,index, View
     * and StridedView as defined in
     * http://www.open-std.org/jtc1/sc22/wg21/docs/papers/2014/n3851.pdf
     * @ingroup poutre_base_group
     * @{
     */

    /**
     * @brief bounds has N-dimensional rectangle semantic
     * bounds is a type that represents rectangular bounds on an N-dimensional
     * discrete space \image misc\bounds_index.svg
     * @return
     */

    template <ptrdiff_t Rank>
    class bounds : public static_array_base<ptrdiff_t, Rank> // TODO restrain alignement
                                                             // capabilities if any
    {
      public:
#ifndef DOXYGEN_SHOULD_SKIP_THIS
      public:
        POUTRE_STATIC_CONSTEXPR ptrdiff_t rank = Rank;

        // inherit from parent
        using parent = static_array_base<ptrdiff_t, Rank>;
        using value_type = typename parent::value_type;
        using const_value_type = typename parent::const_value_type;
        using pointer = typename parent::pointer;
        using reference = typename parent::reference;

        using const_pointer = typename parent::const_pointer;
        using const_reference = typename parent::const_reference;
        using difference_type = typename parent::difference_type;
        using size_type = typename parent::size_type;

        using self_type = bounds<Rank>;
        using const_iterator = bounds_iterator<Rank>;
        using iterator = bounds_iterator<Rank>;
#if (defined _MSC_VER && _MSC_VER <= 1800)
        static_array_base<ptrdiff_t, Rank>::static_array_base<ptrdiff_t, Rank>;
#else
        using parent::parent;
#endif

        using parent::assign;
        using parent::at;
        using parent::back;
        using parent::data;
        using parent::fill;
        using parent::front;
        using parent::swap;
        // using parent::size; size overhided with a different behavior
        // using parent::max_size;
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

#endif //#ifndef DOXYGEN_SHOULD_SKIP_THIS

      protected:
        using parent::m_array;

      public:
        /** @name Construction and Assignment
         */

        /**@{*/

        POUTRE_CXX14_CONSTEXPR bounds() POUTRE_NOEXCEPT_IF(POUTRE_NOEXCEPT_EXPR(parent())) : parent()
        {
            details::helper_assign_container_valueop<self_type, AssignOpType::AssignOp, Rank>::op(0, *this);
        }

        POUTRE_CONSTEXPR explicit bounds(value_type a) POUTRE_NOEXCEPT_IF(POUTRE_NOEXCEPT_EXPR(parent(a))) : parent(a)
        {
        }

        POUTRE_CXX14_CONSTEXPR explicit bounds(value_type a0, value_type a1)
            POUTRE_NOEXCEPT_IF(POUTRE_NOEXCEPT_EXPR(parent(a0, a1)))
            : parent(a0, a1)
        {
        }

        POUTRE_CXX14_CONSTEXPR explicit bounds(value_type a0, value_type a1, value_type a2)
            POUTRE_NOEXCEPT_IF(POUTRE_NOEXCEPT_EXPR(parent(a0, a1, a2)))
            : parent(a0, a1, a2)
        {
        }

        POUTRE_CXX14_CONSTEXPR explicit bounds(value_type a0, value_type a1, value_type a2, value_type a3)
            POUTRE_NOEXCEPT_IF(POUTRE_NOEXCEPT_EXPR(parent(a0, a1, a2, a3)))
            : parent(a0, a1, a2, a3)
        {
        }

        POUTRE_CXX14_CONSTEXPR
        bounds(const std::initializer_list<value_type> &rhs) : parent(rhs)
        {
        }

        POUTRE_CXX14_CONSTEXPR explicit bounds(const value_type (&rhs)[Rank])
            POUTRE_NOEXCEPT_IF(POUTRE_NOEXCEPT_EXPR(parent(rhs)))
            : parent(rhs)
        {
        }

        POUTRE_CONSTEXPR bounds(const parent &rhs) POUTRE_NOEXCEPT_IF(POUTRE_NOEXCEPT_EXPR(parent(rhs))) : parent(rhs)
        {
        }

        /**@}*/

        /** @name Arithmetic Operators
         * Apply vector like operation on bounds with index aka translate
         */

        /**@{*/

        //    POUTRE_CXX14_CONSTEXPR bounds operator+ (const index<rank>& rhs) const
        //    POUTRE_NOEXCEPT
        //    {
        //      self_type tmp (*this); //return a fresh new object
        //      details::helper_assign_container_op<self_type,
        //      AssignOpType::AssignOpAdd, Rank>::op (rhs, tmp); return tmp;
        //    }
        //
        //    POUTRE_CXX14_CONSTEXPR bounds operator- (const index<rank>& rhs) const
        //    POUTRE_NOEXCEPT
        //    {
        //      self_type tmp (*this); //return a fresh new object
        //      details::helper_assign_container_op<self_type,
        //      AssignOpType::AssignOpMinus, Rank>::op (rhs, tmp); return tmp;
        //    }

        POUTRE_CXX14_CONSTEXPR bounds &operator+=(const index<rank> &rhs) POUTRE_NOEXCEPT
        {
            details::helper_assign_container_op<self_type, AssignOpType::AssignOpAdd, Rank>::op(rhs, *this);
            return *this;
        }

        POUTRE_CXX14_CONSTEXPR bounds &operator-=(const index<rank> &rhs) POUTRE_NOEXCEPT
        {
            details::helper_assign_container_op<self_type, AssignOpType::AssignOpMinus, Rank>::op(
                rhs,
                *this); //*this is the result
                        // operation
                        // *this-=rhs
            return *this;
        }

        POUTRE_CXX14_CONSTEXPR bounds &operator+=(const value_type &rhs) POUTRE_NOEXCEPT
        {
            details::helper_assign_container_valueop<self_type, AssignOpType::AssignOpAdd, Rank>::op(rhs, *this);
            return *this;
        }

        POUTRE_CXX14_CONSTEXPR bounds &operator-=(const value_type &rhs) POUTRE_NOEXCEPT
        {
            details::helper_assign_container_valueop<self_type, AssignOpType::AssignOpMinus, Rank>::op(
                rhs, *this); //*this is the result operation *this-=rhs
            return *this;
        }

        /**@}*/

        //! returns  a  hyper  volume  of  the  rectangular  space  enclosed  by
        //! *this,  i.e.  the  product  of  all elements.

        POUTRE_CXX14_CONSTEXPR size_type size() const POUTRE_NOEXCEPT
        {
            size_t volume = m_array[0]; // at least rank==1
            for (size_t i = 1; i < rank; i++)
            {
                volume *= m_array[i];
            }
            return volume;
        }

        //
        //@brief Checks whether the passed index  is contained within  bounds
        // returns true if every element of idx is equal or greater than zero and is
        // less than the corresponding element of *this.

        POUTRE_CXX14_CONSTEXPR bool contains(const index<rank> &idx) const POUTRE_NOEXCEPT
        {
            return details::helper_contains_container_op<bounds<rank>, index<rank>, Rank>::op(*this, idx);
        }

        const_iterator begin() const POUTRE_NOEXCEPT
        {
            return const_iterator(*this);
        }

        const_iterator end() const POUTRE_NOEXCEPT
        {
            // define index end
            index<rank> m_idxend(*this);
            m_idxend -= 1;
            const_iterator tmp(*this, m_idxend);
            return ++tmp;
        }

        const_iterator cbegin() const POUTRE_NOEXCEPT
        {
            return (*this).begin();
        }

        const_iterator cend() const POUTRE_NOEXCEPT
        {
            return (*this).end();
        }
    };

    //! bounds equality
    template <ptrdiff_t Rank>
    POUTRE_CXX14_CONSTEXPR bool operator==(const bounds<Rank> &lhs, const bounds<Rank> &rhs) POUTRE_NOEXCEPT
    {
        return lhs.operator==(rhs);
    }
    template <ptrdiff_t Rank>
    POUTRE_CXX14_CONSTEXPR bool operator!=(const bounds<Rank> &lhs, const bounds<Rank> &rhs) POUTRE_NOEXCEPT
    {
        return lhs.operator!=(rhs);
    }

    template <ptrdiff_t Rank>
    POUTRE_CXX14_CONSTEXPR bounds<Rank> operator*(const bounds<Rank> &rhs, ptrdiff_t v)POUTRE_NOEXCEPT
    {
        return rhs * v;
    }

    template <ptrdiff_t Rank>
    POUTRE_CXX14_CONSTEXPR bounds<Rank> operator*(ptrdiff_t v, const bounds<Rank> &rhs)POUTRE_NOEXCEPT
    {
        return rhs * v;
    }

    template <ptrdiff_t Rank> POUTRE_CXX14_CONSTEXPR bounds<Rank> operator/(const bounds<Rank> &rhs, ptrdiff_t v)
    {
        if (v == 0)
            POUTRE_RUNTIME_ERROR("0 division error in bounds<Rank> operator/ (const "
                                 "index<Rank>& rhs,ptrdiff_t v");
        return rhs / v;
    }

    /**
     * @brief  index is a type that represents an offset or a point in such space
     * \image bounds_index.svg
     */
    template <ptrdiff_t Rank>
    class index : public static_array_base<ptrdiff_t, Rank> // TODO restrain alignement
                                                            // capabilities if any
    {
      public:
        // inherit from parent
        using parent = static_array_base<ptrdiff_t, Rank>;
        using value_type = typename parent::value_type;
        using const_value_type = typename parent::const_value_type;
        using pointer = typename parent::pointer;
        using reference = typename parent::reference;

        using const_pointer = typename parent::const_pointer;
        using const_reference = typename parent::const_reference;
        using difference_type = typename parent::difference_type;
        using size_type = typename parent::size_type;

        using self_type = index<Rank>;
#if (defined _MSC_VER && _MSC_VER <= 1800)
        using static_array_base<ptrdiff_t, Rank>::static_array_base<ptrdiff_t, Rank>;
#else
        using parent::parent;
#endif

        using parent::assign;
        using parent::at;
        using parent::back;
        using parent::data;
        using parent::fill;
        using parent::front;
        using parent::rank;
        using parent::swap;
        // using parent::size;
        // using parent::max_size;
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
        // using parent::operator%;

      protected:
        using parent::m_array;

      public:
        /** @name Construction and Assignment
         */

        /**@{*/

        POUTRE_CXX14_CONSTEXPR index() POUTRE_NOEXCEPT_IF(POUTRE_NOEXCEPT_EXPR(parent())) : parent()
        {
            details::helper_assign_container_valueop<self_type, AssignOpType::AssignOp, Rank>::op(0, *this);
        }

        POUTRE_CONSTEXPR explicit index(value_type a) POUTRE_NOEXCEPT_IF(POUTRE_NOEXCEPT_EXPR(parent(a))) : parent(a)
        {
        }

        POUTRE_CXX14_CONSTEXPR explicit index(value_type a0, value_type a1)
            POUTRE_NOEXCEPT_IF(POUTRE_NOEXCEPT_EXPR(parent(a0, a1)))
            : parent(a0, a1)
        {
        }

        POUTRE_CXX14_CONSTEXPR explicit index(value_type a0, value_type a1, value_type a2)
            POUTRE_NOEXCEPT_IF(POUTRE_NOEXCEPT_EXPR(parent(a0, a1, a2)))
            : parent(a0, a1, a2)
        {
        }

        POUTRE_CXX14_CONSTEXPR explicit index(value_type a0, value_type a1, value_type a2, value_type a3)
            POUTRE_NOEXCEPT_IF(POUTRE_NOEXCEPT_EXPR(parent(a0, a1, a2, a3)))
            : parent(a0, a1, a2, a3)
        {
        }

        POUTRE_CXX14_CONSTEXPR
        index(const std::initializer_list<value_type> &rhs) : parent(rhs)
        {
        }

        POUTRE_CXX14_CONSTEXPR explicit index(const value_type (&rhs)[Rank])
            POUTRE_NOEXCEPT_IF(POUTRE_NOEXCEPT_EXPR(parent(rhs)))
            : parent(rhs)
        {
        }

        POUTRE_CONSTEXPR index(const parent &rhs) POUTRE_NOEXCEPT_IF(POUTRE_NOEXCEPT_EXPR(parent(rhs))) : parent(rhs)
        {
        }

        /**@}*/

        /** @name Arithmetic Operators
         * The binary operator+  and operator-  perform the corresponding operation
         * component-wise on a  copy of *this  and  the  function  argument, returning
         * the  copy.  operator+=  and  operator-=  work  analogously, operating on
         * and returning *this.
         *
         * Pre-  and  post-  increment  and  decrement  operators  have  the
         * traditional  semantics,  and  as  such  are allowed only on index  with
         * rank = 1.  This decision follows the logic that e.g. ++idx  shall be
         * equivalent to idx += 1, which with implicit constructor is naturally
         * supported for rank = 1 as idx += index<1>{ 1 }, while ill-formed for any
         * other rank.
         *
         * The  unary  operator+  returns  a copy of *this,  while  the  unary
         * operator-  returns  a  copy  of  the  object  with  all components negated.
         */
        POUTRE_CXX14_CONSTEXPR self_type operator+(const self_type &rhs) const POUTRE_NOEXCEPT
        {
            self_type tmp(*this); // return a fresh new object
            details::helper_assign_container_op<self_type, AssignOpType::AssignOpAdd, Rank>::op(rhs, tmp);
            return tmp;
        }

        POUTRE_CXX14_CONSTEXPR self_type operator-(const self_type &rhs) const POUTRE_NOEXCEPT
        {
            self_type tmp(*this); // return a fresh new object
            details::helper_assign_container_op<self_type, AssignOpType::AssignOpMinus, Rank>::op(rhs, tmp);
            return tmp;
        }

        POUTRE_CXX14_CONSTEXPR self_type &operator+=(const self_type &rhs) POUTRE_NOEXCEPT
        {
            details::helper_assign_container_op<self_type, AssignOpType::AssignOpAdd, Rank>::op(rhs, *this);
            return *this;
        }

        POUTRE_CXX14_CONSTEXPR self_type &operator-=(const self_type &rhs) POUTRE_NOEXCEPT
        {
            details::helper_assign_container_op<self_type, AssignOpType::AssignOpMinus, Rank>::op(rhs, *this);
            return *this;
        }

        template <ptrdiff_t R = Rank, typename std::enable_if<R == 1, void>::type * = nullptr>
        POUTRE_CXX14_CONSTEXPR self_type &operator++() POUTRE_NOEXCEPT
        {
            (++(*this)[0]);
            return *this;
        }

        template <ptrdiff_t R = Rank, typename std::enable_if<R == 1, void>::type * = nullptr>
        POUTRE_CXX14_CONSTEXPR self_type operator++(int) POUTRE_NOEXCEPT
        {
            return (index<Rank>{(*this)[0]++});
        }

        template <ptrdiff_t R = Rank, typename std::enable_if<R == 1, void>::type * = nullptr>
        POUTRE_CXX14_CONSTEXPR self_type &operator--() POUTRE_NOEXCEPT
        {
            (--(*this)[0]);
            return *this;
        }

        template <ptrdiff_t R = Rank, typename std::enable_if<R == 1, void>::type * = nullptr>
        POUTRE_CXX14_CONSTEXPR self_type operator--(int) POUTRE_NOEXCEPT
        {
            return (index<Rank>{(*this)[0]--});
        }

        POUTRE_CXX14_CONSTEXPR self_type operator+() const POUTRE_NOEXCEPT
        {
            return *this;
        }

        POUTRE_CXX14_CONSTEXPR self_type operator-() const POUTRE_NOEXCEPT
        {
            auto tmp(*this); // return a fresh new object
            details::helper_assign_container_op<self_type, AssignOpType::AssignOpNegate, Rank>::op(tmp, tmp);
            return tmp;
        }

        POUTRE_CXX14_CONSTEXPR self_type &operator+=(const value_type &rhs) POUTRE_NOEXCEPT
        {
            details::helper_assign_container_valueop<self_type, AssignOpType::AssignOpAdd, Rank>::op(rhs, *this);
            return *this;
        }

        POUTRE_CXX14_CONSTEXPR self_type &operator-=(const value_type &rhs) POUTRE_NOEXCEPT
        {
            details::helper_assign_container_valueop<self_type, AssignOpType::AssignOpMinus, Rank>::op(rhs, *this);
            return *this;
        }

        POUTRE_CXX14_CONSTEXPR bool operator<(const self_type &rhs) const POUTRE_NOEXCEPT
        {
            return details::helper_comp_lexicographic_less_container_op<self_type>::op(*this, rhs);
        }

        POUTRE_CXX14_CONSTEXPR bool operator<=(const self_type &rhs) const POUTRE_NOEXCEPT
        {
            return !details::helper_comp_lexicographic_sup_container_op<self_type>::op(*this, rhs);
        }

        POUTRE_CXX14_CONSTEXPR bool operator>(const self_type &rhs) const POUTRE_NOEXCEPT
        {
            return details::helper_comp_lexicographic_sup_container_op<self_type>::op(*this, rhs);
        }

        POUTRE_CXX14_CONSTEXPR bool operator>=(const self_type &rhs) const POUTRE_NOEXCEPT
        {
            return !details::helper_comp_lexicographic_less_container_op<self_type>::op(*this, rhs);
        }

        /**@}*/
    };

    //! index equality
    template <ptrdiff_t Rank>
    POUTRE_CXX14_CONSTEXPR bool operator==(const index<Rank> &rhs, const index<Rank> &lhs) POUTRE_NOEXCEPT
    {
        return lhs.operator==(rhs);
    }
    template <ptrdiff_t Rank>
    POUTRE_CXX14_CONSTEXPR bool operator!=(const index<Rank> &rhs, const index<Rank> &lhs) POUTRE_NOEXCEPT
    {
        return lhs.operator!=(rhs);
    }

    //! index arithmetic
    template <ptrdiff_t Rank>
    POUTRE_CXX14_CONSTEXPR index<Rank> operator+(const index<Rank> &rhs, const index<Rank> &lhs) POUTRE_NOEXCEPT
    {
        return (rhs + lhs);
    }

    template <ptrdiff_t Rank>
    POUTRE_CXX14_CONSTEXPR index<Rank> operator-(const index<Rank> &rhs, const index<Rank> &lhs) POUTRE_NOEXCEPT
    {
        return (rhs - lhs);
    }

    template <ptrdiff_t Rank>
    POUTRE_CXX14_CONSTEXPR index<Rank> operator*(const index<Rank> &rhs, ptrdiff_t v)POUTRE_NOEXCEPT
    {
        return (rhs * v);
    }

    template <ptrdiff_t Rank>
    POUTRE_CXX14_CONSTEXPR index<Rank> operator*(ptrdiff_t v, const index<Rank> &rhs)POUTRE_NOEXCEPT
    {
        return (rhs * v);
    }

    template <ptrdiff_t Rank> POUTRE_CXX14_CONSTEXPR index<Rank> operator/(const index<Rank> &rhs, ptrdiff_t v)
    {
        if (v == 0)
            POUTRE_RUNTIME_ERROR("0 division error in index<Rank> operator/ (const "
                                 "index<Rank>& rhs,ptrdiff_t v");
        return (rhs / v);
    }

    //! bounds index arithmetic
    template <ptrdiff_t Rank>
    POUTRE_CXX14_CONSTEXPR bounds<Rank> operator+(const index<Rank> &lhs, const bounds<Rank> &rhs) POUTRE_NOEXCEPT
    {
        return (rhs + lhs);
    }

    template <ptrdiff_t Rank>
    POUTRE_CXX14_CONSTEXPR bounds<Rank> operator+(const bounds<Rank> &rhs, const index<Rank> &lhs)
    {
        bounds<Rank> tmp(rhs); // return a fresh new object
        details::helper_assign_container_op<bounds<Rank>, AssignOpType::AssignOpAdd, Rank>::op(lhs, tmp);
        return tmp;
    }
    template <ptrdiff_t Rank>
    POUTRE_CXX14_CONSTEXPR bounds<Rank> operator-(const bounds<Rank> &rhs, const index<Rank> &lhs)
    {
        bounds<Rank> tmp(rhs); // return a fresh new object
        details::helper_assign_container_op<bounds<Rank>, AssignOpType::AssignOpMinus, Rank>::op(lhs, tmp);
        return tmp;
    }

    //  POUTRE_CXX14_CONSTEXPR bounds operator+ (const index<rank>& rhs) const
    //  POUTRE_NOEXCEPT
    //    {
    //      self_type tmp (*this); //return a fresh new object
    //      details::helper_assign_container_op<self_type,
    //      AssignOpType::AssignOpAdd, Rank>::op (rhs, tmp); return tmp;
    //    }
    //
    //    POUTRE_CXX14_CONSTEXPR bounds operator- (const index<rank>& rhs) const
    //    POUTRE_NOEXCEPT
    //    {
    //      self_type tmp (*this); //return a fresh new object
    //      details::helper_assign_container_op<self_type,
    //      AssignOpType::AssignOpMinus, Rank>::op (rhs, tmp); return tmp;
    //    }

    /**
     * @brief Iterator over bounds NOT an iterator on value_type of a container
     *
     * The bounds_iterator  is  provided as an  interoperability feature, enabling
     * the  usage of the  multidimensional indices  with the  existing non-mutable
     * iterator-based algorithms. The bounds_iterator  is  dissimilar to other C++
     * Library iterators, as it does not perform iterations over containers or
     * streams, but rather over an imaginary space imposed by  the  bounds.
     * Dereferencing the iterator returns  an  index  object designating the current
     * element in the space.
     */
    template <ptrdiff_t Rank> class bounds_iterator
    {
        static_assert(Rank > 0, "bounds_iterator requires a Rank>0");
        POUTRE_STATIC_CONSTEXPR ptrdiff_t rank = Rank;

      public:
        using self_type = bounds_iterator<Rank>;
        using difference_type = ptrdiff_t;
        using reference = const index<Rank>; // NOTE HERE BY VALUE
        using pointer = const index<Rank>;   // NOTE HERE BY VALUE
        using value_type = index<Rank>;
        using iterator_category = typename strided_iterator_tag;

      protected:
        bounds<Rank> m_bnd;
        index<Rank> m_idx;
        index<Rank> m_idxend;

      public:
        bounds_iterator(const bounds<Rank> &bnd, const index<Rank> &curr = index<Rank>{}) POUTRE_NOEXCEPT
            : m_bnd(bnd),
              m_idx(curr),
              m_idxend(m_bnd)
        {
            // The precondition is that bnd.contains(curr) unless bnd.size() = 0.
            if (bnd.size() != 0)
            {
                m_idxend -= 1;
                ++m_idxend[Rank - 1];
                if (!bnd.contains(m_idx))
                {
                    m_idx = m_idxend;
                }
            }
            else
            {
                // invalidate whole
                m_bnd.assign(0);
                m_idxend.assign(0);
                ++m_idxend[0];
                m_idx = m_idxend;
            }
        }

        bounds_iterator(const self_type &rhs) = default;
        self_type &operator=(const self_type &rhs) = default;

        ~bounds_iterator()
        {
        }

        /*  operator bool( ) const
          {
             return (0 != m_bounds.size( ));
          }*/

        bool operator==(const self_type &rhs) const POUTRE_NOEXCEPT
        {
            return (m_idx == rhs.m_idx); // TODO ALSO check bound ?
        }

        bool operator!=(const self_type &rhs) const POUTRE_NOEXCEPT
        {
            return (m_idx != rhs.m_idx);
        }

        bool operator<(const self_type &rhs) const POUTRE_NOEXCEPT
        {
            return m_idx < rhs.m_idx;
        }

        bool operator<=(const self_type &rhs) const POUTRE_NOEXCEPT
        {
            return m_idx <= rhs.m_idx;
        }

        bool operator>(const self_type &rhs) const POUTRE_NOEXCEPT
        {
            return m_idx > rhs.m_idx;
        }

        bool operator>=(const self_type &rhs) const POUTRE_NOEXCEPT
        {
            return m_idx >= rhs.m_idx;
        }

        bounds_iterator &operator++() POUTRE_NOEXCEPTONLYNDEBUG
        {
            //@todo profile for low rank, may unroll
            for (auto i = rank - 1; i >= 0; --i)
            {
                if (++m_idx[i] < m_bnd[i])
                    return *this;
                m_idx[i] = 0;
            }
            m_idx = m_idxend; // set to end
            return *this;
        }

        bounds_iterator operator++(int) POUTRE_NOEXCEPTONLYNDEBUG
        {
            auto tmp(*this);
            ++(*this); // Call pre-increment on this.
            return tmp;
        }

        bounds_iterator &operator--() POUTRE_NOEXCEPTONLYNDEBUG
        {
            //@todo profile for low rank, may unroll
            for (auto i = rank - 1; i >= 0; --i)
            {
                if (--m_idx[i] >= 0)
                    return *this;
                m_idx[i] = m_bnd[i] - 1;
            }
            m_idx.assign(0);
            --m_idx[rank - 1]; // just before start
            return *this;
        }

        bounds_iterator operator--(int) POUTRE_NOEXCEPTONLYNDEBUG
        {
            bounds_iterator tmp(*this);
            --(*this);
            return tmp;
        }

        //    bounds_iterator operator+ (difference_type n) const
        //    POUTRE_NOEXCEPTONLYNDEBUG
        //    {
        //      auto tmp (*this);
        //      poutre::details::shift_op<bounds<Rank>, index < Rank>>::op (tmp.m_bnd,
        //      tmp.m_idx, n, tmp.m_idx); if (!tmp.m_bnd.contains (tmp.m_idx))
        //        {
        //          tmp.m_idx = tmp.m_idxend;
        //        }
        //      return (tmp);
        //    }

        bounds_iterator &operator+=(difference_type n) POUTRE_NOEXCEPTONLYNDEBUG
        {
            poutre::details::shift_op<bounds<Rank>, index<Rank>>::op(m_bnd, m_idx, n, m_idx);
            if (!m_bnd.contains(m_idx))
            {
                m_idx = m_idxend;
            }
            return (*this);
        }

        //    bounds_iterator operator- (difference_type n) const
        //    POUTRE_NOEXCEPTONLYNDEBUG
        //    {
        //      auto tmp (*this);
        //      poutre::details::shift_op<bounds<Rank>, index < Rank>>::op (tmp.m_bnd,
        //      tmp.m_idx, -n, tmp.m_idx); if (!tmp.m_bnd.contains (tmp.m_idx))
        //        {
        //          tmp.m_idx.assign (0);
        //          --tmp.m_idx[Rank - 1]; //just before start
        //        }
        //      return (tmp);
        //    }

        bounds_iterator &operator-=(difference_type n) POUTRE_NOEXCEPTONLYNDEBUG
        {
            poutre::details::shift_op<bounds<Rank>, index<Rank>>::op(m_bnd, m_idx, -n, m_idx);
            if (!m_bnd.contains(m_idx))
            {
                m_idx.assign(0);
                --m_idx[Rank - 1]; // just before start
            }
            return (*this);
        }

        difference_type operator-(const bounds_iterator &rhs) const POUTRE_NOEXCEPTONLYNDEBUG
        {
            return details::get_offset_from_coord_nostride<bounds<Rank>, index<Rank>>::op(m_bnd, m_idx) -
                   details::get_offset_from_coord_nostride<bounds<Rank>, index<Rank>>::op(rhs.m_bnd, rhs.m_idx);
        }

        //   pointer   operator->() const;  //implement or throw ?

        //!

        difference_type operator[](difference_type n) const POUTRE_NOEXCEPT
        {
            return m_idx[n];
        }

        //! Note here a return by value

        reference operator*() const POUTRE_NOEXCEPT
        {
            return m_idx;
        }
    };

    template <ptrdiff_t Rank> bounds_iterator<Rank> begin(const bounds<Rank> &bnd) POUTRE_NOEXCEPT
    {
        return bnd.begin();
    }

    template <ptrdiff_t Rank> bounds_iterator<Rank> end(const bounds<Rank> &bnd) POUTRE_NOEXCEPT
    {
        return bnd.end();
    }

    template <ptrdiff_t Rank> bounds_iterator<Rank> cbegin(const bounds<Rank> &bnd) POUTRE_NOEXCEPT
    {
        return bnd.cbegin();
    }

    template <ptrdiff_t Rank> bounds_iterator<Rank> cend(const bounds<Rank> &bnd) POUTRE_NOEXCEPT
    {
        return bnd.cend();
    }

    template <ptrdiff_t Rank>
    bounds_iterator<Rank> operator+(typename bounds_iterator<Rank>::difference_type n, const bounds_iterator<Rank> &rhs)
    {
        auto tmp(rhs);
        return (tmp + n);
    }

    template <ptrdiff_t Rank>
    bool operator==(const bounds_iterator<Rank> &lhs, const bounds_iterator<Rank> &rhs) POUTRE_NOEXCEPT
    {
        return (lhs == rhs);
    }

    template <ptrdiff_t Rank>
    bool operator!=(const bounds_iterator<Rank> &lhs, const bounds_iterator<Rank> &rhs) POUTRE_NOEXCEPT
    {
        return (lhs != rhs);
    }

    template <ptrdiff_t Rank>
    bool operator<(const bounds_iterator<Rank> &lhs, const bounds_iterator<Rank> &rhs) POUTRE_NOEXCEPT
    {
        return lhs < rhs;
    }

    template <ptrdiff_t Rank>
    bool operator<=(const bounds_iterator<Rank> &lhs, const bounds_iterator<Rank> &rhs) POUTRE_NOEXCEPT
    {
        return lhs <= rhs;
    }

    template <ptrdiff_t Rank>
    bool operator>(const bounds_iterator<Rank> &lhs, const bounds_iterator<Rank> &rhs) POUTRE_NOEXCEPT
    {
        return lhs > rhs;
    }

    template <ptrdiff_t Rank>
    bool operator>=(const bounds_iterator<Rank> &lhs, const bounds_iterator<Rank> &rhs) POUTRE_NOEXCEPT
    {
        return lhs >= rhs;
    }

    //! @} doxygroup: coordinate_group

    // extern template class bounds < 1 > ;
    // extern template class bounds < 2 > ;
    // extern template class bounds < 3 > ;
    // extern template class bounds < 4 > ;

    // extern template class index <1>;
    // extern template class index <2>;
    // extern template class index <3>;
    // extern template class index <4>;

    typedef bounds<1> bd1d; //! alias bounds 1D
    typedef bounds<2> bd2d; //! alias bounds 2D
    typedef bounds<3> bd3d; //! alias bounds 3D
    typedef bounds<4> bd4d; //! alias bounds 4D

    typedef index<1> idx1d; //! alias index 1D
    typedef index<2> idx2d; //! alias index 2D
    typedef index<3> idx3d; //! alias index 3D
    typedef index<4> idx4d; //! alias index 4D

} // namespace poutre

#ifdef _MSC_VER
#pragma warning(pop)
#endif

#endif // POUTRE_COORDINATE_HPP__
