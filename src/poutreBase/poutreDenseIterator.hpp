
//==============================================================================
//                  Copyright (c) 2015 - Thomas Retornaz                      //
//                     thomas.retornaz@mines-paris.org                        //
//          Distributed under the Boost Software License, Version 1.0.        //
//                 See accompanying file LICENSE.txt or copy at               //
//                     http://www.boost.org/LICENSE_1_0.txt                   //
//==============================================================================

#ifndef POUTRE_DENSE_ITERATOR_HPP__
#define POUTRE_DENSE_ITERATOR_HPP__

/**
 * @file   poutreDenseIterator.hpp
 * @author Thomas Retornaz
 * @brief  Define iterator capabilities over contiguous allocated contener
 *
 *
 */

#include <iterator>
#include <poutreBase/poutreConfig.hpp>
#include <poutreBase/poutreTypes.hpp>

namespace poutre
{
  /*!
   * @defgroup iterator_group Define iterators
   * @ingroup poutre_base_group
   * @{
   */
  struct strided_iterator_tag : std::random_access_iterator_tag
  {
  };

  struct contiguous_iterator_tag : std::random_access_iterator_tag
  {
  };

  //! Raw dense iterator with random access
  template<typename DataType> class pdense_iterator
  {
    public:
    using value_type        = DataType;
    using const_value_type  = typename std::add_const<value_type>::type;
    using pointer           = typename std::add_pointer<value_type>::type;
    using reference         = typename std::add_lvalue_reference<value_type>::type;
    using const_pointer     = typename std::add_pointer<const_value_type>::type;
    using const_reference   = typename std::add_lvalue_reference<const_value_type>::type;
    using difference_type   = std::ptrdiff_t;
    using iterator_category = contiguous_iterator_tag;
    using self_type         = pdense_iterator<DataType>;

    pdense_iterator(DataType *ptr = nullptr, DataType *ptrorig = nullptr) : m_ptr(ptr), m_ptrorig(ptrorig) {}
    pdense_iterator(const self_type &rhs) = default;
    self_type &operator=(const self_type &rhs) = default;

    ~pdense_iterator() {}

    POUTRE_CXX14_CONSTEXPR operator bool() const
    {
      if( (m_ptr) && (m_ptrorig) )
        return true;
      return false;
    }

    POUTRE_CXX14_CONSTEXPR bool operator==(const self_type &rhs) const
    {
      return (m_ptr == rhs.getConstPtr() && m_ptrorig == rhs.getConstOrigPtr());
    }
    POUTRE_CXX14_CONSTEXPR bool operator!=(const self_type &rhs) const
    {
      return (m_ptr != rhs.getConstPtr() || m_ptrorig != rhs.getConstOrigPtr());
    }

    POUTRE_CXX14_CONSTEXPR pdense_iterator<DataType> &operator+=(const std::ptrdiff_t &movement)
    {
      m_ptr += movement;
      return (*this);
    }
    POUTRE_CXX14_CONSTEXPR pdense_iterator<DataType> &operator-=(const std::ptrdiff_t &movement)
    {
      m_ptr -= movement;
      return (*this);
    }
    POUTRE_CXX14_CONSTEXPR pdense_iterator<DataType> &operator++()
    {
      ++m_ptr;
      return (*this);
    }
    POUTRE_CXX14_CONSTEXPR pdense_iterator<DataType> &operator--()
    {
      --m_ptr;
      return (*this);
    }

    POUTRE_CXX14_CONSTEXPR pdense_iterator<DataType> operator++(int)
    {
      auto temp(*this);
      ++m_ptr;
      return temp;
    }
    POUTRE_CXX14_CONSTEXPR pdense_iterator<DataType> operator--(int)
    {
      auto temp(*this);
      --m_ptr;
      return temp;
    }

    POUTRE_CXX14_CONSTEXPR ptrdiff_t operator-(const self_type &rawIterator)
    {
      return std::distance(rawIterator.getPtr(), this->getPtr());
    }

    POUTRE_CXX14_CONSTEXPR reference operator*() { return *m_ptr; }

    POUTRE_CXX14_CONSTEXPR const_reference operator*() const { return *m_ptr; }

    POUTRE_CXX14_CONSTEXPR pointer operator->() { return m_ptr; }

    POUTRE_CXX14_CONSTEXPR pointer getPtr() const { return m_ptr; }

    POUTRE_CXX14_CONSTEXPR pointer       getOrigPtr() const { return m_ptrorig; }
    POUTRE_CXX14_CONSTEXPR void          setPtr(DataType *ptr) { m_ptr = ptr; }
    POUTRE_CXX14_CONSTEXPR void          setOrigPtr(DataType *ptr) { m_ptrorig = ptr; }
    POUTRE_CXX14_CONSTEXPR const_pointer getConstPtr() const { return m_ptr; }
    POUTRE_CXX14_CONSTEXPR const_pointer getConstOrigPtr() const { return m_ptrorig; }

    POUTRE_CXX14_CONSTEXPR const offset getOffset() const { return m_ptr - m_ptrorig; }

    protected:
    pointer m_ptr;
    pointer m_ptrorig;
  };

  //! Raw reversed dense iterator with random access
  template<typename DataType> class pdense_reverse_iterator : public pdense_iterator<DataType>
  {
    public:
    using parent = pdense_iterator<DataType>;

    using value_type = typename parent::value_type;
    ;
    using const_value_type = typename std::add_const<value_type>::type;
    using pointer          = typename std::add_pointer<value_type>::type;
    using reference        = typename std::add_lvalue_reference<value_type>::type;
    using const_pointer    = typename std::add_pointer<const_value_type>::type;
    using const_reference  = typename std::add_lvalue_reference<const_value_type>::type;
    using difference_type  = std::ptrdiff_t;

    using parent::m_ptr;
    using parent::m_ptrorig;
    using self_type = pdense_reverse_iterator<DataType>;

    pdense_reverse_iterator(DataType *ptr = nullptr, DataType *ptrorig = nullptr) : parent(ptr, ptrorig) {}
    // pdense_reverse_iterator(const self_type& rhs)
    //  {
    //  this->m_ptr = rhs.getPtr( ); this->m_ptrorig = rhs.getOrigPtr( );
    //  }

    pdense_reverse_iterator(const self_type &rhs) = default;
    self_type &operator=(const self_type &rawReverseIterator) = default;
    ~pdense_reverse_iterator() {}

    // self_type&           operator=(const self_type& rawIterator)
    //  {
    //  this->m_ptr = rawIterator.getPtr( ); this->m_ptrorig =
    //  rawIterator.getOrigPtr( ); return (*this);
    //  }

    self_type &operator+=(const std::ptrdiff_t &movement)
    {
      this->m_ptr -= movement;
      return (*this);
    }
    self_type &operator-=(const std::ptrdiff_t &movement)
    {
      this->m_ptr += movement;
      return (*this);
    }
    self_type &operator++()
    {
      --this->m_ptr;
      return (*this);
    }
    self_type &operator--()
    {
      ++this->m_ptr;
      return (*this);
    }
    self_type operator++(int)
    {
      auto temp(*this);
      --this->m_ptr;
      return temp;
    }
    self_type operator--(int)
    {
      auto temp(*this);
      ++this->m_ptr;
      return temp;
    }

    ptrdiff_t operator-(const self_type &rawReverseIterator)
    { /*todo assert same orig?*/
      return std::distance(this->getPtr(), rawReverseIterator.getPtr());
    }

    const offset getOffset() const { return m_ptr - m_ptrorig; }

    pdense_iterator<DataType> base()
    {
      pdense_iterator<DataType> forwardIterator(this->m_ptr, this->m_ptrorig);
      ++forwardIterator;
      return forwardIterator;
    }
  };
  /**
  //! @} doxygroup: iterator_group
  */
} // namespace poutre
#endif // POUTRE_DENSE_ITERATOR_HPP__
