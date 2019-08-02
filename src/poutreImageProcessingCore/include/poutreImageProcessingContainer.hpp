
//==============================================================================
//                  Copyright (c) 2015 - Thomas Retornaz                      //
//                     thomas.retornaz@mines-paris.org                        //
//          Distributed under the Boost Software License, Version 1.0.        //
//                 See accompanying file LICENSE.txt or copy at               //
//                     http://www.boost.org/LICENSE_1_0.txt                   //
//==============================================================================

#ifndef POUTRE_IMAGEPROCESSING_CONTAINER_HPP__
#define POUTRE_IMAGEPROCESSING_CONTAINER_HPP__

#include <array>
#include <cstring>
#include <iostream>
#include <memory>
#include <sstream>
#include <utility>
#include <vector>

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

template <typename valuetype>
using aligned_allocator =
    nsimd::allocator<typename poutre::TypeTraits<valuetype>::storage_type>;

namespace poutre {
// Maybe inherit/compose with nt2::table or blaze::DenseMatrix or
// Eigen::Tensor would be more effective depending on algorithms
//! TODO dispatch on coord and allow unset dims
template <class valuetype, std::ptrdiff_t NumDims = 2,
          class allocator_type_t = aligned_allocator<valuetype>>
class DenseTensor {
  static_assert(NumDims > 0, "NumDims must be >0");

public:
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
  static const std::ptrdiff_t m_numdims = NumDims;

protected:
  pointer m_data;
  coordinate_type m_coordinnates;
  coordinate_type m_strides;
  allocator_type m_allocator;
  size_type m_numelemwithpaddingifany;

public:
  using self_type = DenseTensor<valuetype, NumDims, allocator_type_t>;

public:
  DenseTensor(const std::vector<size_t> &dims)
      : m_data(nullptr), m_coordinnates(), m_strides(), m_allocator(),
        m_numelemwithpaddingifany(0) {
    if (dims.size() != m_numdims)
      POUTRE_RUNTIME_ERROR("Invalid input initializer regarding NumDims of "
                           "DenseTensor container");
    for (size_t i = 0; i < m_numdims; ++i) {
      m_coordinnates[i] = dims[i];
    }

    // compute full array size with include possible padding for each first
    // stride
    if (!m_coordinnates.empty()) {
      m_numelemwithpaddingifany = m_coordinnates[0];
      for (size_t i = 1; i < (size_t)m_numdims; i++) {
        m_numelemwithpaddingifany *= m_coordinnates[i];
      }
      m_data = m_allocator.allocate(m_numelemwithpaddingifany);

      // fill stride
      m_strides[m_numdims - 1] = 1;
      for (ptrdiff_t dim = m_numdims - 2; dim >= 0; --dim) {
        m_strides[dim] = m_strides[dim + 1] * bound()[dim + 1];
      }
    }
  }

  DenseTensor(const std::initializer_list<size_t> &dims)
      : m_data(nullptr), m_coordinnates(),m_strides(), m_allocator(),
        m_numelemwithpaddingifany(0) {
    if (dims.size() != m_numdims)
      POUTRE_RUNTIME_ERROR("Invalid input initializer regarding NumDims of "
                           "DenseTensor container");
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
      for (size_t i = 1; i < (size_t)m_numdims; i++) {
        m_numelemwithpaddingifany *= m_coordinnates[i];
      }
      m_data = m_allocator.allocate(m_numelemwithpaddingifany);

      // fill stride
      m_strides[m_numdims - 1] = 1;
      for (ptrdiff_t dim = m_numdims - 2; dim >= 0; --dim) {
        m_strides[dim] = m_strides[dim + 1] * bound()[dim + 1];
      }
    }
  }

  ~DenseTensor() POUTRE_NOEXCEPT {
    if (m_data)
      m_allocator.deallocate(m_data, m_numelemwithpaddingifany);
  }

  const coordinate_type bound() const POUTRE_NOEXCEPT { return m_coordinnates; }
  const coordinate_type shape() const POUTRE_NOEXCEPT { return m_coordinnates; }
  const coordinate_type stride() const POUTRE_NOEXCEPT { return m_strides; }

  std::size_t GetNumDims() const POUTRE_NOEXCEPT { return m_numdims; }
  // std::array like interface

  // Capacity

  const size_type size() const POUTRE_NOEXCEPT {
    return m_numelemwithpaddingifany; // FIXME COULD BE != if padding present
  }

  const size_type max_size() const POUTRE_NOEXCEPT { return this->size(); }

  const bool empty() const POUTRE_NOEXCEPT { return this->size() == 0; }

  // Element access

  reference operator[](size_type n) POUTRE_NOEXCEPT {
    POUTRE_ASSERTCHECK(n < m_numelemwithpaddingifany, "Access out of bound");
    POUTRE_ASSERTCHECK(n >= 0, "Access out of bound");
    return m_data[n];
  }

  const_reference operator[](size_type n) const POUTRE_NOEXCEPT {
    POUTRE_ASSERTCHECK(n < m_numelemwithpaddingifany, "Access out of bound");
    // POUTRE_ASSERTCHECK(n >= 0, "Access out of bound");
    return m_data[n];
  }

  reference at(size_type n) {
    if (n >= m_numelemwithpaddingifany)
      POUTRE_RUNTIME_ERROR("Access out of bound");
    return m_data[n];
  }

  const_reference at(size_type n) const {
    if (n >= m_numelemwithpaddingifany)
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

  pointer data() POUTRE_NOEXCEPT { return m_data; }

  const_pointer data() const POUTRE_NOEXCEPT { return m_data; }

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
           rhs.m_numelemwithpaddingifany);            // notthrow
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
protected:
  // protected copyctor used through clone

  DenseTensor(const self_type &rhs)
      : m_data(nullptr),
        m_numelemwithpaddingifany(rhs.m_numelemwithpaddingifany),
        m_coordinnates(rhs.m_coordinnates), m_allocator(rhs.m_allocator) {
    m_data = m_allocator.allocate(m_numelemwithpaddingifany);
    std::copy(rhs.m_data, rhs.m_data + m_numelemwithpaddingifany, m_data);
  }

public:
  // disable copyassignement
  self_type &operator=(const self_type &other) = delete;

  // move constructor

  DenseTensor(self_type &&rhs) noexcept
      : m_data(nullptr), m_numelemwithpaddingifany(0), m_coordinnates(),
        m_allocator() {
    m_data = rhs.m_data;
    m_numelemwithpaddingifany = rhs.m_numelemwithpaddingifany;
    m_coordinnates = rhs.m_coordinnates;
    m_allocator = rhs.m_allocator;

    // relase
    rhs.m_numelemwithpaddingifany = 0;
    rhs.m_data = nullptr;
    /*m_coordinnates = {};*/
  }

  // move assignment operator

  self_type &operator=(self_type &&rhs) noexcept {
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

template <class valuetype, std::ptrdiff_t NumDims = 2,
          class allocator_type_t = aligned_allocator<valuetype>>
class DenseImage : public IInterface,
                   public DenseTensor<valuetype, NumDims, allocator_type_t> {

  static_assert(NumDims > 0, "NumDims must be >0");

public:
  using self_type = DenseImage<valuetype, NumDims, allocator_type_t>;
  using parent_template = DenseTensor<valuetype, NumDims, allocator_type_t>;
  using parent_interface = IInterface;
  using allocator_type = typename parent_template::allocator_type;
  using value_type =
      typename parent_template::value_type; // typename
                                            // TypeTraits<ptype>::storage_type;
  using pointer = typename allocator_type_t::pointer;
  using const_pointer = typename allocator_type_t::const_pointer;
  using reference = value_type &;
  using const_reference = value_type const &;
  using difference_type = std::ptrdiff_t;

  using contigous_iterator = pdense_iterator<value_type>;
  using const_contigous_iterator = pdense_iterator<const value_type>;

  using reverse_contigous_iterator = pdense_reverse_iterator<value_type>;
  using const_reverse_contigous_iterator =
      pdense_reverse_iterator<const value_type>;

  using index = offset;
  using size_type = std::size_t;

  using coordinate_type = bounds<NumDims>;

  static const ImgType m_imgtype = ImgType::ImgType_Dense;
  static const PType m_ptype = TypeTraits<value_type>::pixel_type;
  static const CompoundType m_ctype = TypeTraits<value_type>::compound_type;

  DenseImage(const std::vector<size_t> &dims) : parent_template(dims) {}
  DenseImage(const std::initializer_list<size_t> &dims)
      : parent_template(dims) {}

  std::vector<std::size_t> GetCoords() const override {
    std::vector<std::size_t> out(this->m_numdims);
    // use normal traversal
    for (size_t i = 0; i < this->m_numdims; ++i) {
      out[i] = this->m_coordinnates[i];
    }
    return out;
  }
  std::size_t GetNumDims() const POUTRE_NOEXCEPT override {
    return parent_template::GetNumDims();
  }

  CompoundType GetCType() const POUTRE_NOEXCEPT override { return m_ctype; }

  PType GetPType() const POUTRE_NOEXCEPT override { return m_ptype; }

  ImgType GetImgType() const POUTRE_NOEXCEPT override { return m_imgtype; }

  std::unique_ptr<IInterface> Clone() const override {
    return std::make_unique<self_type>(*this);
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
    if ((ptrdiff_t)numDims - 1 >= 0) {
      out << coords[numDims - 1];
    }
    out << ")" << std::endl;
    return out.str();
  }

  using parent_template::bound;
  using parent_template::GetNumDims;
  using parent_template::shape;
  using parent_template::stride;
  // std::array like interface

  // Capacity
  using parent_template::empty;
  using parent_template::max_size;
  using parent_template::size;

  // Element access
  using parent_template::operator[];
  using parent_template::at;
  using parent_template::back;
  using parent_template::data;
  using parent_template::front;
  // Modifiers
  using parent_template::assign;
  using parent_template::fill;

  void swap(self_type &rhs) // POUTRE_NOEXCEPT(POUTRE_NOEXCEPT
                            // (swap(declval<value_type&>( ),
                            // declval<value_type&>( )))) //wait MSVC2013
                            // noexcept impl ...
  {
    if (this != &rhs) {
      using std::swap;
      parent_template::swap(*this);
    }
  }
  using parent_template::begin;
  using parent_template::cbegin;
  using parent_template::cend;
  using parent_template::crbegin;
  using parent_template::end;
  using parent_template::rbegin;

  using parent_template::crend;
  using parent_template::rend;
  // end std::array like interface
public:
  // public copyctor used through clone (make_unique ask this constructor to be
  // public)

  DenseImage(const self_type &rhs) : parent_template(rhs) {}

public:
  // disable copyassignement
  self_type &operator=(const self_type &other) = delete;

  // move constructor

  DenseImage(self_type &&rhs) noexcept : parent_template(rhs) {}

  // move assignment operator

  self_type &operator=(self_type &&rhs) noexcept {
    if (this !=
        &rhs) // http://scottmeyers.blogspot.fr/2014/06/the-drawbacks-of-implementing-move.html
    {
      // release resource
      this->m_allocator.deallocate(this->m_data,
                                   this->m_numelemwithpaddingifany);
      // Copy the data pointer and its length from the source object.
      this->m_data = rhs.m_data;
      this->m_numelemwithpaddingifany = rhs.m_numelemwithpaddingifany;
      this->m_coordinnates = rhs.m_coordinnates;
      this->m_allocator = rhs.m_allocator;

      // release
      rhs.m_numelemwithpaddingifany = 0;
      rhs.m_data = nullptr;
      /* m_coordinnates = {};*/
    }
    return *this;
  }
};

template <typename valuetype>
class Image2D : public DenseImage<valuetype, 2,
                                  nsimd::allocator<typename poutre::TypeTraits<
                                      valuetype>::storage_type>> {
public:
  using parent_type = DenseImage<
      valuetype, 2,
      nsimd::allocator<typename poutre::TypeTraits<valuetype>::storage_type>>;
  using self_type = Image2D<valuetype>;
  using contigous_iterator = typename parent_type::contigous_iterator;
  using const_contigous_iterator =
      typename parent_type::const_contigous_iterator;

  using reverse_contigous_iterator =
      typename parent_type::reverse_contigous_iterator;
  using const_reverse_contigous_iterator =
      typename parent_type::const_reverse_contigous_iterator;

  Image2D(const std::vector<size_t> &dims) : parent_type(dims) {}
  Image2D(const std::initializer_list<size_t> &dims) : parent_type(dims) {}

  using parent_type::bound;
  using parent_type::GetNumDims;
  using parent_type::shape;
  using parent_type::stride;
  // std::array like interface

  // Capacity
  using parent_type::empty;
  using parent_type::max_size;
  using parent_type::size;

  // Element access
  using parent_type::operator[];
  using parent_type::at;
  using parent_type::back;
  using parent_type::data;
  using parent_type::front;
  // Modifiers
  using parent_type::assign;
  using parent_type::fill;

  void swap(self_type &rhs) // POUTRE_NOEXCEPT(POUTRE_NOEXCEPT
                            // (swap(declval<value_type&>( ),
                            // declval<value_type&>( )))) //wait MSVC2013
                            // noexcept impl ...
  {
    if (this != &rhs) {
      using std::swap;
      parent_type::swap(*this);
    }
  }
  using parent_type::begin;
  using parent_type::cbegin;
  using parent_type::cend;
  using parent_type::crbegin;
  using parent_type::end;
  using parent_type::rbegin;

  using parent_type::crend;
  using parent_type::rend;
  // end std::array like interface
public:
  // public copyctor used through clone (make_unique ask this constructor to be
  // public)

  Image2D(const self_type &rhs) : parent_type(rhs) {}

public:
  // disable copyassignement
  self_type &operator=(const self_type &other) = delete;

  // move constructor

  Image2D(self_type &&rhs) noexcept : parent_type(rhs) {}

  // move assignment operator

  self_type &operator=(self_type &&rhs) noexcept {
    if (this !=
        &rhs) // http://scottmeyers.blogspot.fr/2014/06/the-drawbacks-of-implementing-move.html
    {
      // release resource
      this->m_allocator.deallocate(this->m_data,
                                   this->m_numelemwithpaddingifany);
      // Copy the data pointer and its length from the source object.
      this->m_data = rhs.m_data;
      this->m_numelemwithpaddingifany = rhs.m_numelemwithpaddingifany;
      this->m_coordinnates = rhs.m_coordinnates;
      this->m_allocator = rhs.m_allocator;

      // release
      rhs.m_numelemwithpaddingifany = 0;
      rhs.m_data = nullptr;
      /* m_coordinnates = {};*/
    }
    return *this;
  }
};

template <typename valuetype>
class Image3D : public DenseImage<valuetype, 3,
                                  nsimd::allocator<typename poutre::TypeTraits<
                                      valuetype>::storage_type>> {
public:
  using parent_type = DenseImage<
      valuetype, 3,
      nsimd::allocator<typename poutre::TypeTraits<valuetype>::storage_type>>;
  using self_type = Image3D<valuetype>;
  using contigous_iterator = typename parent_type::contigous_iterator;
  using const_contigous_iterator =
      typename parent_type::const_contigous_iterator;

  using reverse_contigous_iterator =
      typename parent_type::reverse_contigous_iterator;
  using const_reverse_contigous_iterator =
      typename parent_type::const_reverse_contigous_iterator;

  Image3D(const std::vector<size_t> &dims) : parent_type(dims) {}
  Image3D(const std::initializer_list<size_t> &dims) : parent_type(dims) {}
  using parent_type::bound;
  using parent_type::GetNumDims;
  using parent_type::shape;
  // std::array like interface

  // Capacity
  using parent_type::empty;
  using parent_type::max_size;
  using parent_type::size;
  using parent_type::stride;

  // Element access
  using parent_type::operator[];
  using parent_type::at;
  using parent_type::back;
  using parent_type::data;
  using parent_type::front;
  // Modifiers
  using parent_type::assign;
  using parent_type::fill;

  void swap(self_type &rhs) // POUTRE_NOEXCEPT(POUTRE_NOEXCEPT
                            // (swap(declval<value_type&>( ),
                            // declval<value_type&>( )))) //wait MSVC2013
                            // noexcept impl ...
  {
    if (this != &rhs) {
      using std::swap;
      parent_type::swap(*this);
    }
  }
  using parent_type::begin;
  using parent_type::cbegin;
  using parent_type::cend;
  using parent_type::crbegin;
  using parent_type::end;
  using parent_type::rbegin;

  using parent_type::crend;
  using parent_type::rend;
  // end std::array like interface
public:
  // public copyctor used through clone (make_unique ask this constructor to be
  // public)

  Image3D(const self_type &rhs) : parent_type(rhs) {}

public:
  // disable copyassignement
  self_type &operator=(const self_type &other) = delete;

  // move constructor

  Image3D(self_type &&rhs) noexcept : parent_type(rhs) {}

  // move assignment operator

  self_type &operator=(self_type &&rhs) noexcept {
    if (this !=
        &rhs) // http://scottmeyers.blogspot.fr/2014/06/the-drawbacks-of-implementing-move.html
    {
      // release resource
      this->m_allocator.deallocate(this->m_data,
                                   this->m_numelemwithpaddingifany);
      // Copy the data pointer and its length from the source object.
      this->m_data = rhs.m_data;
      this->m_numelemwithpaddingifany = rhs.m_numelemwithpaddingifany;
      this->m_coordinnates = rhs.m_coordinnates;
      this->m_allocator = rhs.m_allocator;

      // release
      rhs.m_numelemwithpaddingifany = 0;
      rhs.m_data = nullptr;
      /* m_coordinnates = {};*/
    }
    return *this;
  }
};

template <typename valuetype>
class Signal : public DenseImage<valuetype, 1,
                                 nsimd::allocator<typename poutre::TypeTraits<
                                     valuetype>::storage_type>> {
public:
  using parent_type = DenseImage<
      valuetype, 1,
      nsimd::allocator<typename poutre::TypeTraits<valuetype>::storage_type>>;
  using self_type = Signal<valuetype>;
  using contigous_iterator = typename parent_type::contigous_iterator;
  using const_contigous_iterator =
      typename parent_type::const_contigous_iterator;

  using reverse_contigous_iterator =
      typename parent_type::reverse_contigous_iterator;
  using const_reverse_contigous_iterator =
      typename parent_type::const_reverse_contigous_iterator;

  Signal(const std::vector<size_t> &dims) : parent_type(dims) {}
  Signal(const std::initializer_list<size_t> &dims) : parent_type(dims) {}

  using parent_type::bound;
  using parent_type::GetNumDims;
  using parent_type::shape;
  using parent_type::stride;
  // std::array like interface

  // Capacity
  using parent_type::empty;
  using parent_type::max_size;
  using parent_type::size;

  // Element access
  using parent_type::operator[];
  using parent_type::at;
  using parent_type::back;
  using parent_type::data;
  using parent_type::front;
  // Modifiers
  using parent_type::assign;
  using parent_type::fill;

  void swap(self_type &rhs) // POUTRE_NOEXCEPT(POUTRE_NOEXCEPT
                            // (swap(declval<value_type&>( ),
                            // declval<value_type&>( )))) //wait MSVC2013
                            // noexcept impl ...
  {
    if (this != &rhs) {
      using std::swap;
      parent_type::swap(*this);
    }
  }
  using parent_type::begin;
  using parent_type::cbegin;
  using parent_type::cend;
  using parent_type::crbegin;
  using parent_type::end;
  using parent_type::rbegin;

  using parent_type::crend;
  using parent_type::rend;
  // end std::array like interface
public:
  // public copyctor used through clone (make_unique ask this constructor to be
  // public)

  Signal(const self_type &rhs) : parent_type(rhs) {}

public:
  // disable copyassignement
  self_type &operator=(const self_type &other) = delete;

  // move constructor

  Signal(self_type &&rhs) noexcept : parent_type(rhs) {}

  // move assignment operator

  self_type &operator=(self_type &&rhs) noexcept {
    if (this !=
        &rhs) // http://scottmeyers.blogspot.fr/2014/06/the-drawbacks-of-implementing-move.html
    {
      // release resource
      this->m_allocator.deallocate(this->m_data,
                                   this->m_numelemwithpaddingifany);
      // Copy the data pointer and its length from the source object.
      this->m_data = rhs.m_data;
      this->m_numelemwithpaddingifany = rhs.m_numelemwithpaddingifany;
      this->m_coordinnates = rhs.m_coordinnates;
      this->m_allocator = rhs.m_allocator;

      // release
      rhs.m_numelemwithpaddingifany = 0;
      rhs.m_data = nullptr;
      /* m_coordinnates = {};*/
    }
    return *this;
  }
};

///////////////////////////////////////////////////////////////////////////////////
/* Translate to view*/

// Linear view, linearize container padding included
template <class valuetype, std::ptrdiff_t Rank>
poutre::array_view<valuetype, 1> lview(DenseImage<valuetype, Rank> &iImg) {
  return poutre::array_view<valuetype, 1>(iImg.data(),
                                          bd1d{(ptrdiff_t)iImg.size()});
}

// FIXME convertion loose qualifiers
template <class valuetype, std::ptrdiff_t Rank>
poutre::array_view<valuetype, 1>
lview(const DenseImage<valuetype, Rank> &iImg) {
  return lview(const_cast<DenseImage<valuetype, Rank> &>(iImg));
}

// Default view is stridded due to padding
template <class valuetype, std::ptrdiff_t Rank>
poutre::strided_array_view<valuetype, Rank>
view(DenseImage<valuetype, Rank> &iImg) {
  return poutre::strided_array_view<valuetype, Rank>(iImg.data(), iImg.shape(),
                                                     iImg.stride());
}

// FIXME convertion loose qualifiers
template <class valuetype, std::ptrdiff_t Rank>
poutre::strided_array_view<valuetype, Rank>
view(const DenseImage<valuetype, Rank> &iImg) {
  return view(const_cast<DenseImage<valuetype, Rank> &>(iImg));
}
///////////////////////////////////////////////////////////////////////////////////

// todo define macros
extern template class Signal<pUINT8>;
extern template class Signal<pINT32>;
extern template class Signal<pFLOAT>;
extern template class Signal<pINT64>;
extern template class Signal<pDOUBLE>;

extern template class Image2D<pUINT8>;
extern template class Image2D<pINT32>;
extern template class Image2D<pFLOAT>;
extern template class Image2D<pINT64>;
extern template class Image2D<pDOUBLE>;

extern template class Image2D<compound_pixel<pUINT8, 3>>;
extern template class Image2D<compound_pixel<pINT32, 3>>;
extern template class Image2D<compound_pixel<pFLOAT, 3>>;
extern template class Image2D<compound_pixel<pINT64, 3>>;
extern template class Image2D<compound_pixel<pDOUBLE, 3>>;

extern template class Image2D<compound_pixel<pUINT8, 4>>;
extern template class Image2D<compound_pixel<pINT32, 4>>;
extern template class Image2D<compound_pixel<pFLOAT, 4>>;
extern template class Image2D<compound_pixel<pINT64, 4>>;
extern template class Image2D<compound_pixel<pDOUBLE, 4>>;

extern template class Image3D<pUINT8>;
extern template class Image3D<pINT32>;
extern template class Image3D<pFLOAT>;
extern template class Image3D<pINT64>;
extern template class Image3D<pDOUBLE>;

extern template class DenseImage<pUINT8, 4>;
extern template class DenseImage<pINT32, 4>;
extern template class DenseImage<pFLOAT, 4>;
extern template class DenseImage<pINT64, 4>;
extern template class DenseImage<pDOUBLE, 4>;

} // namespace poutre
#endif // POUTRE_IMAGEPROCESSING_CONTAINER_HPP__
