
//==============================================================================
//                  Copyright (c) 2015 - Thomas Retornaz                      //
//                     thomas.retornaz@mines-paris.org                        //
//          Distributed under the Boost Software License, Version 1.0.        //
//                 See accompanying file LICENSE.txt or copy at               //
//                     http://www.boost.org/LICENSE_1_0.txt                   //
//==============================================================================

#ifndef POUTRE_DOMAIN_ITERATOR_HPP__
#define POUTRE_DOMAIN_ITERATOR_HPP__

/**
 * @file   poutreDomainIterator.hpp
 * @author Thomas Retornaz
 * @brief  Define iterator capabilities over image domain (ie handle the
 * padding)
 *
 *
 */

#ifndef POUTRE_TYPE_HPP__
#include <poutreBase/poutreTypes.hpp>
#endif

#ifndef POUTRE_COORDINATE_HPP__
#include <poutreBase/poutreCoordinate.hpp>
#endif

#ifndef POUTRE_CONTAINER_VIEW_HPP__
#include <poutreBase/poutreContainerView.hpp>
#endif

#include <iterator>

namespace poutre
{
    /*!
     * @defgroup iterator_domain_group Define iterators which handle padding
     * @ingroup poutre_base_group
     * @{
     */
    //! Domain iterator with bidirectional access
    template <typename DataType, ptrdiff_t Rank>
    class pdomain_iterator
        : public std::iterator<std::bidirectional_iterator_tag, DataType, std::ptrdiff_t, DataType *, DataType &>
    {

      public:
        using parent = std::iterator<std::bidirectional_iterator_tag, DataType, std::ptrdiff_t, DataType *, DataType &>;
        using reference = typename parent::reference;
        using pointer = typename parent::pointer;
        using difference_type = typename parent::difference_type;
        using value_type = typename parent::value_type;
        using iterator_category = typename parent::iterator_category;
        using self_type = pdomain_iterator<DataType, Rank>;

      protected:
        pointer m_ptr;
        bounds_iterator<Rank> m_boundit;
        index<Rank> m_strides;
        ptrdiff_t m_currentiter;
        ptrdiff_t m_enditer;

      public:
        pdomain_iterator(DataType *ptr, bounds<Rank> bnd, index<Rank> strides, ptrdiff_t currentiter)
            : m_ptr(ptr), m_boundit(bnd), m_strides(strides), m_currentiter(currentiter), m_enditer(bnd.size())
        {
        }
        pdomain_iterator(const self_type &rhs) = default;
        self_type &operator=(const self_type &rhs) = default;

        ~pdomain_iterator()
        {
        }

        operator bool() const
        {
            if ((m_ptr))
                return true;
            return false;
        }

        bool operator==(const self_type &rhs) const
        {
            return (m_ptr == rhs.getPtr() && m_currentiter == rhs.m_currentiter && m_enditer == rhs.m_enditer);
        }
        bool operator!=(const self_type &rhs) const
        {
            return (m_ptr != rhs.getPtr() || m_currentiter != rhs.m_currentiter || m_enditer != rhs.m_enditer);
        }

        self_type &operator++()
        {
            ++m_boundit;
            ++m_currentiter;
            return (*this);
        }
        self_type &operator--()
        {
            --m_boundit;
            --m_currentiter;
            return (*this);
        }

        self_type operator++(int)
        {
            auto temp(*this);
            ++m_boundit;
            ++m_currentiter;
            return temp;
        }
        self_type operator--(int)
        {
            auto temp(*this);
            --m_boundit;
            --m_currentiter;
            return temp;
        }

        //   ptrdiff_t operator-(const self_type &rawIterator) {
        //     return std::distance(rawIterator.getPtr(), this->getPtr());
        //   }

        reference operator*()
        {
            return m_ptr[getOffset()];
        }
        const reference operator*() const
        {
            return m_ptr[getOffset()];
        }
        pointer operator->()
        {
            return &m_ptr[getOffset()];
        }

        pointer getPtr() const
        {
            return m_ptr;
        }
        void setPtr(DataType *ptr)
        {
            m_ptr = ptr;
        }
        const offset getOffset() const
        {
            // auto o = details::view_offset(*m_boundit, m_strides);
            // std::cout << "\n Offset" << o << "\n";
            // return o;
            return details::view_offset(*m_boundit, m_strides);
        }
    };
    //! @} doxygroup: iterator_domain_group
} // namespace poutre
#endif // POUTRE_DOMAIN_ITERATOR_HPP__
