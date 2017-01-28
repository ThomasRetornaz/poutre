
//==============================================================================
//                  Copyright (c) 2015 - Thomas Retornaz                      //
//                     thomas.retornaz@mines-paris.org                        //
//          Distributed under the Boost Software License, Version 1.0.        //
//                 See accompanying file LICENSE.txt or copy at               //
//                     http://www.boost.org/LICENSE_1_0.txt                   //
//==============================================================================

#ifndef POUTRE_IMAGEPROCESSING_CONTAINER_HPP__
#define POUTRE_IMAGEPROCESSING_CONTAINER_HPP__

#include <boost/simd/memory/allocator.hpp>
#include <boost/simd/memory/is_aligned.hpp>
#include <array>
#include <memory>
#include <utility>
#include <vector>
#include <sstream>
#include <iostream>

#ifndef POUTRE_CONFIG__HPP__
#include <poutreBase/poutreConfig.hpp>
#endif

#ifndef POUTRE_DENSE_ITERATOR_HPP__
#include <poutreBase/poutreDenseIterator.hpp>
#endif

#ifndef POUTRE_IMAGEPROCESSING_INTERFACE_HPP__
#include <poutreImageProcessingCore/poutreImageProcessingInterface.hpp>
#endif

#ifndef POUTRE_IMAGEPROCESSING_TYPE_HPP__
#include <poutreImageProcessingCore/poutreImageProcessingType.hpp>
#endif

#ifndef POUTRE_COORDINATE_HPP__
#include <poutreBase/poutreCoordinate.hpp>
#endif

#ifndef POUTRE_CONTAINER_VIEW_HPP__
#include <poutreBase/poutreContainerView.hpp>
#endif

namespace poutre {
// Maybe inherit/compose with nt2::table or blaze::DenseMatrix or
// Eigen::Tensor would be more effective depending on algorithms

//! FIXME dispatch on coord and allow unset dims

template <class valuetype, std::ptrdiff_t NumDims = 2,
          class allocator_type_t = boost::simd::allocator<
              typename TypeTraits<valuetype>::storage_type,
              TypeTraits<valuetype>::alignement>>
class DenseImage : public IInterface {
  static_assert(NumDims > 0, "NumDims must be >0");

public:
  using parent_type = IInterface;

  using value_type = valuetype; // typename TypeTraits<ptype>::storage_type;
  using pointer = typename allocator_type_t::pointer;
  using const_pointer = typename allocator_type_t::const_pointer;
  using reference = value_type &;
  using const_reference = value_type const &;
  using difference_type = std::ptrdiff_t;

  using iterator = pdense_iterator<value_type>;
  using const_iterator = pdense_iterator<const value_type>;

  using reverse_iterator = pdense_reverse_iterator<value_type>;
  using const_reverse_iterator = pdense_reverse_iterator<const value_type>;

  using index = offset;
  using size_type = std::size_t;

  using coordinate_type = bounds<NumDims>;
  using allocator_type = allocator_type_t;

  static const PType m_ptype = TypeTraits<value_type>::pixel_type;
  static const CompoundType m_ctype = TypeTraits<value_type>::compound_type;

  static const std::size_t m_numdims = NumDims;
  static const std::size_t alignement = TypeTraits<valuetype>::alignement;
  static const ImgType m_imgtype = ImgType::ImgType_Dense;

private:
  pointer m_data;
  coordinate_type m_coordinnates;
  allocator_type m_allocator;
  size_type m_numelemwithpaddingifany;

public:
  using self_type = DenseImage<valuetype, NumDims, allocator_type_t>;

public:
  DenseImage(const std::vector<size_t> &dims)
      : m_data(nullptr), m_coordinnates(), m_allocator(),
        m_numelemwithpaddingifany(0) {
    if (dims.size() != m_numdims)
      POUTRE_RUNTIME_ERROR("Invalid input initializer regarding NumDims of "
                           "DenseImage container");
    // std::copy(dims.begin( ), dims.end( ), m_coordinnates.begin( ));
    for (size_t i = 0; i < m_numdims; ++i) {
      m_coordinnates[i] = dims[i];
    }

    // compute full array size with include possible padding for each first
    // stride
    if (!m_coordinnates.empty()) {
      // if (m_numdims == 2)
      //  {
      //  m_numelemwithpadding = ((m_size_list[0] + default_padding_size - 1) &
      //  ~(default_padding_size - 1));
      //  }
      // else
      //  {
      m_numelemwithpaddingifany = m_coordinnates[0];
      //  }
      for (size_t i = 1; i < m_numdims; i++) {
        m_numelemwithpaddingifany *= m_coordinnates[i];
      }
      m_data = m_allocator.allocate(m_numelemwithpaddingifany);
    }
  }

  DenseImage(const std::initializer_list<size_t> &dims)
      : m_data(nullptr), m_coordinnates(), m_allocator(),
        m_numelemwithpaddingifany(0) {
    if (dims.size() != m_numdims)
      POUTRE_RUNTIME_ERROR("Invalid input initializer regarding NumDims of "
                           "DenseImage container");
    // std::copy(dims.begin( ), dims.end( ), m_coordinnates.begin( ));
    auto it = dims.begin();
    for (size_t i = 0; i < m_numdims; ++i, ++it) {
      m_coordinnates[i] = *it;
    }

    // compute full array size with include possible padding for each first
    // stride
    if (!m_coordinnates.empty()) {
      // if (m_numdims == 2)
      //  {
      //  m_numelemwithpadding = ((m_size_list[0] + default_padding_size - 1) &
      //  ~(default_padding_size - 1));
      //  }
      // else
      //  {
      m_numelemwithpaddingifany = m_coordinnates[0];
      //  }
      for (size_t i = 1; i < m_numdims; i++) {
        m_numelemwithpaddingifany *= m_coordinnates[i];
      }
      m_data = m_allocator.allocate(m_numelemwithpaddingifany);
    }
  }

  ~DenseImage() POUTRE_NOEXCEPT {
    if (m_data)
      m_allocator.deallocate(m_data, m_numelemwithpaddingifany);
  }

  const coordinate_type bound() const POUTRE_NOEXCEPT { return m_coordinnates; }

  // std::array like interface

  // Capacity

  const size_type size() const POUTRE_NOEXCEPT {
    return m_numelemwithpaddingifany; // FIXME COULD BE != if padding present
  }

  const size_type max_size() const POUTRE_NOEXCEPT { return this->size(); }

  const bool empty() const POUTRE_NOEXCEPT { return this->size() == 0; }

  // Element access

  reference operator[](size_type n)POUTRE_NOEXCEPT 
    {
      POUTRE_ASSERTCHECK(n < m_numelemwithpaddingifany, "Access out of bound");
      POUTRE_ASSERTCHECK(n >= 0, "Access out of bound");
      return m_data[n]; 
    }

  const_reference operator[](size_type n) const POUTRE_NOEXCEPT 
  {
      POUTRE_ASSERTCHECK(n < m_numelemwithpaddingifany, "Access out of bound");
      POUTRE_ASSERTCHECK(n >= 0, "Access out of bound");
    return m_data[n];
  }

  reference at(size_type n) {
    if (n >= m_numelemwithpaddingifany || n < 0)
      POUTRE_RUNTIME_ERROR("Access out of bound");
    return m_data[n];
  }

  const_reference at(size_type n) const {
    if (n >= m_numelemwithpaddingifany || n < 0)
      POUTRE_RUNTIME_ERROR("Access out of bound");
    return m_data[n];
  }

  reference front() POUTRE_NOEXCEPT { return m_data[0]; }

  const_reference front() const POUTRE_NOEXCEPT { return m_data[0]; }

  reference back() POUTRE_NOEXCEPT {
    return m_data[m_numelemwithpaddingifany - 1];
  }

  const_reference back() const POUTRE_NOEXCEPT {
    return m_data[m_numelemwithpaddingifany - 1];
  }

  pointer datas() POUTRE_NOEXCEPT { return m_data; }

  const_pointer datas() const POUTRE_NOEXCEPT { return m_data; }

  // Modifiers

  void assign(const value_type &val) { this->fill(val); }

  void fill(const value_type &val) {
    // assign value to all elements
    std::fill(this->begin(), this->end(), val);
  }

  void swap(self_type &rhs) // POUTRE_NOEXCEPT(POUTRE_NOEXCEPT
                            // (swap(declval<value_type&>( ),
                            // declval<value_type&>( )))) //wait MSVC2013
                            // noexcept impl ...
  {
    if (this != &rhs) {
      using std::swap;
      swap(this->m_data, rhs.m_data); // nothrow
      swap(this->m_numelemwithpaddingifany,
           rhs.m_numelemwithpaddingifany); // notthrow
      swap(this->m_coordinnates, rhs.m_coordinnates); //?throw
      swap(this->m_allocator, rhs.m_allocator);       //?throw
    }
  }

  iterator begin() POUTRE_NOEXCEPT { return iterator(m_data, m_data); }

  const_iterator begin() const POUTRE_NOEXCEPT {
    return const_iterator(m_data, m_data);
  }

  const_iterator cbegin() const POUTRE_NOEXCEPT {
    return const_iterator(m_data, m_data);
  }

  iterator end() POUTRE_NOEXCEPT {
    return iterator(m_data + m_numelemwithpaddingifany, m_data);
  }

  const_iterator end() const POUTRE_NOEXCEPT {
    return const_iterator(m_data + m_numelemwithpaddingifany, m_data);
  }

  const_iterator cend() const POUTRE_NOEXCEPT {
    return const_iterator(m_data + m_numelemwithpaddingifany, m_data);
  }

  reverse_iterator rbegin() POUTRE_NOEXCEPT {
    return (reverse_iterator(m_data + m_numelemwithpaddingifany - 1, m_data));
  }

  const_reverse_iterator crbegin() const POUTRE_NOEXCEPT {
    return (
        const_reverse_iterator(m_data + m_numelemwithpaddingifany - 1, m_data));
  }

  reverse_iterator rend() POUTRE_NOEXCEPT {
    return (reverse_iterator(m_data - 1, m_data));
  }

  const_reverse_iterator crend() const POUTRE_NOEXCEPT {
    return (const_reverse_iterator(m_data - 1, m_data));
  }

  // end std::array like interface

  // TODO use extern class ?

  std::vector<std::size_t> GetCoords() const override 
  {
    std::vector<std::size_t> out(m_numdims);
    // use normal traversal
    for (auto i = 0; i < m_numdims; ++i) {
      out[i] = m_coordinnates[i];
    }
    return out;
  }

  size_t GetNumDims() const POUTRE_NOEXCEPT override { return m_numdims; }

  CompoundType GetCType() const POUTRE_NOEXCEPT override { return m_ctype; }

  PType GetPType() const POUTRE_NOEXCEPT override { return m_ptype; }

  ImgType GetImgType() const POUTRE_NOEXCEPT override { return m_imgtype; }

  std::unique_ptr<IInterface> Clone() const override 
  { // FIXME
    // return std::make_unique<DenseImage>(*this);
    std::unique_ptr<IInterface> uni(new DenseImage(*this));
    return uni;
  }

  std::string str() const POUTRE_NOEXCEPT override {
    std::ostringstream out;
    out << "Image" << std::endl;
    out << "\tType: " << this->GetImgType() << std::endl;
    out << "\tCtype: " << this->GetCType() << std::endl;
    out << "\tPtype: " << this->GetPType() << std::endl;
    const auto &numDims = this->GetNumDims();
    out << "\tNumdim: " << numDims << std::endl;
    const auto &coords = this->GetCoords();
    out << "\tcoord: (";
    for (size_t i = 0; i < numDims - 1; i++) {
      out << coords[i] << ", ";
    }
    if (numDims - 1 >= 0) {
      out << coords[numDims - 1];
    }
    out << ")" << std::endl;
    return out.str();
  }

protected:
  // protected copyctor used through clone

  DenseImage(const self_type &rhs)
      : m_data(nullptr),
        m_numelemwithpaddingifany(rhs.m_numelemwithpaddingifany),
        m_coordinnates(rhs.m_coordinnates), m_allocator(rhs.m_allocator) 
  {
    m_data = m_allocator.allocate(m_numelemwithpaddingifany);
    std::copy(rhs.m_data, rhs.m_data + m_numelemwithpaddingifany, m_data);
  }

public:
  // disable copyassignement
  self_type &operator=(const self_type &other) = delete;

  // move constructor

  DenseImage(self_type &&rhs) noexcept
      : m_data(nullptr), m_numelemwithpaddingifany(0), m_coordinnates(),
        m_allocator() 
  {
      m_data = rhs.m_data;
      m_numelemwithpaddingifany = rhs.m_numelemwithpaddingifany;
      m_coordinnates = rhs.m_coordinnates;
      m_allocator = rhs.m_allocator;

      //relase
      rhs.m_numelemwithpaddingifany = 0;
      rhs.m_data = nullptr;
      /*m_coordinnates = {};*/

  }

  // move assignment operator

  self_type &operator=(self_type &&rhs)  noexcept
  {
    if (this !=
        &rhs) // ??
              // http://scottmeyers.blogspot.fr/2014/06/the-drawbacks-of-implementing-move.html
    {
      // release resource
      m_allocator.deallocate(m_data, m_numelemwithpaddingifany);
      // Copy the data pointer and its length from the source object.
      m_data = rhs.m_data;
      m_numelemwithpaddingifany = rhs.m_numelemwithpaddingifany;
      m_coordinnates = rhs.m_coordinnates;
      m_allocator = rhs.m_allocator;

      // release
      rhs.m_numelemwithpaddingifany = 0;
      rhs.m_data = nullptr;
     /* m_coordinnates = {};*/
    }
    return *this;
  }
};

template <class valuetype, std::ptrdiff_t Rank>
poutre::array_view<valuetype, Rank> view(DenseImage<valuetype, Rank> &iImg) {
  return poutre::array_view<valuetype, Rank>(iImg.datas(), iImg.bound());
}

template <class valuetype, std::ptrdiff_t Rank>
poutre::carray_view<valuetype, Rank>
view(const DenseImage<valuetype, Rank> &iImg) {
  return poutre::carray_view<valuetype, Rank>(iImg.datas(), iImg.bound());
}

// todo define macros
extern template class DenseImage<pUINT8, 1>;
extern template class DenseImage<pINT32, 1>;
extern template class DenseImage<pFLOAT, 1>;
extern template class DenseImage<pINT64, 1>;
extern template class DenseImage<pDOUBLE, 1>;

extern template class DenseImage<pUINT8, 2>;
extern template class DenseImage<pINT32, 2>;
extern template class DenseImage<pFLOAT, 2>;
extern template class DenseImage<pINT64, 2>;
extern template class DenseImage<pDOUBLE, 2>;

extern template class DenseImage<compound_pixel<pUINT8, 3>>;
extern template class DenseImage<compound_pixel<pINT32, 3>>;
extern template class DenseImage<compound_pixel<pFLOAT, 3>>;
extern template class DenseImage<compound_pixel<pINT64, 3>>;
extern template class DenseImage<compound_pixel<pDOUBLE, 3>>;

extern template class DenseImage<pUINT8, 3>;
extern template class DenseImage<pINT32, 3>;
extern template class DenseImage<pFLOAT, 3>;
extern template class DenseImage<pINT64, 3>;
extern template class DenseImage<pDOUBLE, 3>;

extern template class DenseImage<pUINT8, 4>;
extern template class DenseImage<pINT32, 4>;
extern template class DenseImage<pFLOAT, 4>;
extern template class DenseImage<pINT64, 4>;
extern template class DenseImage<pDOUBLE, 4>;

extern template class DenseImage<compound_pixel<pUINT8, 4>>;
extern template class DenseImage<compound_pixel<pINT32, 4>>;
extern template class DenseImage<compound_pixel<pFLOAT, 4>>;
extern template class DenseImage<compound_pixel<pINT64, 4>>;
extern template class DenseImage<compound_pixel<pDOUBLE, 4>>;
}
#endif // POUTRE_IMAGEPROCESSING_CONTAINER_HPP__
