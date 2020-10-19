
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
/**
 * @file poutreImageProcessingContainer.hpp
 * @author thomas.retornaz@mines-paris.org
 * @brief Define image container
 * @version 0.1
 * @date 2020-05-08
 *
 * @copyright Copyright (c) 2020
 *
 */
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

#ifndef POUTRE_GEOMETRY_HPP__
#include <poutreBase/poutreGeometry.hpp>
#endif

#ifndef POUTRE_SIMD_HPP__
#include <poutreBase/include/poutreSimd.hpp>
#endif

template <typename valuetype>
using aligned_allocator =
    xs::aligned_allocator<typename poutre::TypeTraits<valuetype>::storage_type, SIMD_IDEAL_MAX_ALIGN_BYTES>;

namespace poutre
{
    /**
     * @addtogroup image_processing_container_group Image Processing Template Container
     * @ingroup image_processing_group
     *@{
     */

    // Maybe inherit/compose with nt2::table or blaze::DenseMatrix or
    // Eigen::Tensor would be more effective depending on algorithms
    //! TODO dispatch on coord and allow unset dims
    template <class valuetype, std::ptrdiff_t NumDims = 2, class allocator_type_t = aligned_allocator<valuetype>>
    class DenseTensor
    {
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

        using coordinate_type = bounds<NumDims>;
        using index_type = index<NumDims>;

        // using index = offset;
        using size_type = std::size_t;

        using allocator_type = allocator_type_t;
        static const std::ptrdiff_t m_numdims = NumDims;

      protected:
        pointer m_data;
        coordinate_type m_coordinnates;
        index_type m_strides;
        allocator_type m_allocator;
        size_type m_numelement;

      public:
        using self_type = DenseTensor<valuetype, NumDims, allocator_type_t>;

      protected:
        DenseTensor() : m_data(nullptr), m_coordinnates(), m_strides(), m_allocator(), m_numelement(0)
        {
        }
        DenseTensor(const std::vector<size_t> &dims)
            : m_data(nullptr), m_coordinnates(), m_strides(), m_allocator(), m_numelement(0)
        {
            if (dims.size() != m_numdims)
                POUTRE_RUNTIME_ERROR("Invalid input initializer regarding NumDims of "
                                     "DenseTensor container");
            for (size_t i = 0; i < m_numdims; ++i)
            {
                m_coordinnates[i] = dims[i];
            }
            // compute full array size
            if (!m_coordinnates.empty())
            {
                m_numelement = m_coordinnates[0];
                for (size_t i = 1; i < (size_t)m_numdims; i++)
                {
                    m_numelement *= (m_coordinnates[i]);
                }
                m_data = m_allocator.allocate(m_numelement);

                // fill stride
                m_strides[m_numdims - 1] = 1;
                for (ptrdiff_t dim = m_numdims - 2; dim >= 0; --dim)
                {
                    m_strides[dim] = m_strides[dim + 1] * (bound()[dim + 1]);
                }
            }
        }

        DenseTensor(const std::initializer_list<size_t> &dims)
            : m_data(nullptr), m_coordinnates(), m_strides(), m_allocator(), m_numelement(0)
        {
            if (dims.size() != m_numdims)
                POUTRE_RUNTIME_ERROR("Invalid input initializer regarding NumDims of "
                                     "DenseTensor container");
            // std::copy(dims.begin( ), dims.end( ), m_coordinnates.begin( ));
            auto it = dims.begin();
            for (size_t i = 0; i < m_numdims; ++i, ++it)
            {
                m_coordinnates[i] = *it;
            }
            // compute full array size
            if (!m_coordinnates.empty())
                m_numelement = m_coordinnates[0];
            for (size_t i = 1; i < (size_t)m_numdims; i++)
            {
                m_numelement *= m_coordinnates[i];
            }
            m_data = m_allocator.allocate(m_numelement);

            // fill stride
            m_strides[m_numdims - 1] = 1;
            for (ptrdiff_t dim = m_numdims - 2; dim >= 0; --dim)
            {
                m_strides[dim] = m_strides[dim + 1] * (bound()[dim + 1]);
            }
        }

      public:
        ~DenseTensor() POUTRE_NOEXCEPT
        {
            if (m_data)
                m_allocator.deallocate(m_data, m_numelement);
        }

        POUTRE_CXX14_CONSTEXPR const coordinate_type bound() const POUTRE_NOEXCEPT
        {
            return m_coordinnates;
        }
        POUTRE_CXX14_CONSTEXPR const coordinate_type shape() const POUTRE_NOEXCEPT
        {
            return m_coordinnates;
        }
        POUTRE_CXX14_CONSTEXPR const index_type stride() const POUTRE_NOEXCEPT
        {
            return m_strides;
        }
        POUTRE_CXX14_CONSTEXPR std::size_t GetNumDims() const POUTRE_NOEXCEPT
        {
            return m_numdims;
        }
        // std::array like interface

        // Capacity
        POUTRE_CXX14_CONSTEXPR const size_type size() const POUTRE_NOEXCEPT
        {
            return m_numelement;
        }

        POUTRE_CXX14_CONSTEXPR const size_type max_size() const POUTRE_NOEXCEPT
        {
            return this->size();
        }

        POUTRE_CXX14_CONSTEXPR const bool empty() const POUTRE_NOEXCEPT
        {
            return this->size() == 0;
        }

        // Element access

        POUTRE_CXX14_CONSTEXPR reference operator[](size_type n) POUTRE_NOEXCEPT
        {
            POUTRE_ASSERTCHECK(n < m_numelement, "Access out of bound");
            POUTRE_ASSERTCHECK(n >= 0, "Access out of bound");
            return m_data[n];
        }

        POUTRE_CXX14_CONSTEXPR const_reference operator[](size_type n) const POUTRE_NOEXCEPT
        {
            POUTRE_ASSERTCHECK(n < m_numelement, "Access out of bound");
            // POUTRE_ASSERTCHECK(n >= 0, "Access out of bound");
            return m_data[n];
        }

        POUTRE_CXX14_CONSTEXPR reference at(size_type n)
        {
            if (n >= m_numelement)
                POUTRE_RUNTIME_ERROR("Access out of bound");
            return m_data[n];
        }

        POUTRE_CXX14_CONSTEXPR const_reference at(size_type n) const
        {
            if (n >= m_numelement)
                POUTRE_RUNTIME_ERROR("Access out of bound");
            return m_data[n];
        }

        POUTRE_CXX14_CONSTEXPR reference front() POUTRE_NOEXCEPT
        {
            return m_data[0];
        }

        POUTRE_CXX14_CONSTEXPR const_reference front() const POUTRE_NOEXCEPT
        {
            return m_data[0];
        }

        POUTRE_CXX14_CONSTEXPR reference back() POUTRE_NOEXCEPT
        {
            return m_data[m_numelement - 1];
        }

        POUTRE_CXX14_CONSTEXPR const_reference back() const POUTRE_NOEXCEPT
        {
            return m_data[m_numelement - 1];
        }

        POUTRE_CXX14_CONSTEXPR pointer data() POUTRE_NOEXCEPT
        {
            return m_data;
        }

        POUTRE_CXX14_CONSTEXPR const_pointer data() const POUTRE_NOEXCEPT
        {
            return m_data;
        }

        // Modifiers

        POUTRE_CXX14_CONSTEXPR void assign(const value_type &val)
        {
            this->fill(val);
        }

        POUTRE_CXX14_CONSTEXPR void fill(const value_type &val)
        {
            // assign value to all elements
            std::fill(this->begin(), this->end(), val);
        }

        POUTRE_CXX14_CONSTEXPR void swap(self_type &rhs) // POUTRE_NOEXCEPT(POUTRE_NOEXCEPT
                                                         // (swap(declval<value_type&>( ),
                                                         // declval<value_type&>( )))) //wait MSVC2013
                                                         // noexcept impl ...
        {
            if (this != &rhs)
            {
                using std::swap;
                swap(this->m_data, rhs.m_data); // nothrow
                swap(this->m_numelement,
                     rhs.m_numelement);                         // notthrow
                swap(this->m_coordinnates, rhs.m_coordinnates); //?throw
                swap(this->m_strides, rhs.m_strides);           //?throw
                swap(this->m_allocator, rhs.m_allocator);       //?throw
            }
        }

        iterator begin() POUTRE_NOEXCEPT
        {
            return iterator(m_data, m_data);
        }

        const_iterator cbegin() const POUTRE_NOEXCEPT
        {
            return const_iterator(m_data, m_data);
        }

        iterator end() POUTRE_NOEXCEPT
        {
            return iterator(m_data + m_numelement, m_data);
        }

        const_iterator cend() const POUTRE_NOEXCEPT
        {
            return const_iterator(m_data + m_numelement, m_data);
        }

        reverse_iterator rbegin() POUTRE_NOEXCEPT
        {
            return (reverse_iterator(m_data + m_numelement - 1, m_data));
        }

        const_reverse_iterator crbegin() const POUTRE_NOEXCEPT
        {
            return (const_reverse_iterator(m_data + m_numelement - 1, m_data));
        }

        reverse_iterator rend() POUTRE_NOEXCEPT
        {
            return (reverse_iterator(m_data - 1, m_data));
        }

        const_reverse_iterator crend() const POUTRE_NOEXCEPT
        {
            return (const_reverse_iterator(m_data - 1, m_data));
        }

        // end std::array like interface
      protected:
        // protected copyctor used through clone

        DenseTensor(const self_type &rhs)
            : m_data(nullptr), m_numelement(rhs.m_numelement), m_coordinnates(rhs.m_coordinnates),
              m_strides(rhs.m_strides), m_allocator(rhs.m_allocator)
        {
            m_data = m_allocator.allocate(m_numelement);
            std::copy(rhs.m_data, rhs.m_data + m_numelement, m_data);
        }

      public:
        // disable copyassignement
        self_type &operator=(const self_type &other) = delete;

        // move constructor

        DenseTensor(self_type &&rhs) noexcept : m_data(nullptr), m_numelement(0), m_coordinnates(), m_allocator()
        {
            m_data = rhs.m_data;
            m_numelement = rhs.m_numelement;
            m_coordinnates = rhs.m_coordinnates;
            m_strides = rhs.m_strides;
            m_allocator = rhs.m_allocator;

            // release
            rhs.m_numelement = 0;
            rhs.m_data = nullptr;
            /*m_coordinnates = {};*/
        }

        // move assignment operator

        POUTRE_CXX14_CONSTEXPR self_type &operator=(self_type &&rhs) noexcept
        {
            if (this != &rhs) // ??
                              // http://scottmeyers.blogspot.fr/2014/06/the-drawbacks-of-implementing-move.html
            {
                // release resource
                m_allocator.deallocate(m_data, m_numelement);
                // Copy the data pointer and its length from the source object.
                m_data = rhs.m_data;
                m_numelement = rhs.m_numelement;
                m_coordinnates = rhs.m_coordinnates;
                m_strides = rhs.m_strides;
                m_allocator = rhs.m_allocator;

                // release
                rhs.m_numelement = 0;
                rhs.m_data = nullptr;
                /* m_coordinnates = {};*/
            }
            return *this;
        }
    }; // namespace poutre

    template <class valuetype, std::ptrdiff_t NumDims = 2, class allocator_type_t = aligned_allocator<valuetype>>
    class DenseImage : public IInterface, public DenseTensor<valuetype, NumDims, allocator_type_t>
    {

        static_assert(NumDims > 0, "NumDims must be >0");

      public:
        using self_type = DenseImage<valuetype, NumDims, allocator_type_t>;
        using parent_template = DenseTensor<valuetype, NumDims, allocator_type_t>;
        using parent_interface = IInterface;
        using allocator_type = typename parent_template::allocator_type;
        using value_type = typename parent_template::value_type; // typename
                                                                 // TypeTraits<ptype>::storage_type;
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

        using coordinate_type = typename parent_template::coordinate_type;
        using index_type = typename parent_template::index_type;
        // static const std::ptrdiff_t m_numdims = NumDims;
        static const ImgType m_imgtype = ImgType::ImgType_Dense;
        static const PType m_ptype = TypeTraits<value_type>::pixel_type;
        static const CompoundType m_ctype = TypeTraits<value_type>::compound_type;

        DenseImage(const std::vector<size_t> &dims) : parent_template()
        {
            if (dims.size() != parent_template::m_numdims)
                POUTRE_RUNTIME_ERROR("Invalid input initializer regarding NumDims of "
                                     "DenseImage container");
            for (size_t i = 0; i < parent_template::m_numdims; ++i)
            {
                parent_template::m_coordinnates[i] = dims[i];
            }
            // compute full array size
            if (!parent_template::m_coordinnates.empty())
            {
                parent_template::m_numelement = parent_template::m_coordinnates[0];
                for (size_t i = 1; i < (size_t)parent_template::m_numdims; i++)
                {
                    parent_template::m_numelement *= (parent_template::m_coordinnates[i]);
                }
                parent_template::m_data = parent_template::m_allocator.allocate(parent_template::m_numelement);

                // fill stride
                parent_template::m_strides[parent_template::m_numdims - 1] = 1;
                for (ptrdiff_t dim = parent_template::m_numdims - 2; dim >= 0; --dim)
                {
                    parent_template::m_strides[dim] = parent_template::m_strides[dim + 1] * (bound()[dim + 1]);
                }
            }
        }

        DenseImage(const std::initializer_list<size_t> &dims) : parent_template()
        {
            if (dims.size() != parent_template::m_numdims)
                POUTRE_RUNTIME_ERROR("Invalid input initializer regarding NumDims of "
                                     "DenseImage container");
            // std::copy(dims.begin( ), dims.end( ), m_coordinnates.begin( ));
            auto it = dims.begin();
            for (size_t i = 0; i < this->m_numdims; ++i, ++it)
            {
                this->m_coordinnates[i] = *it;
            }
            if (!this->m_coordinnates.empty())
            {
                this->m_numelement = this->m_coordinnates[0];
                for (size_t i = 1; i < (size_t)this->m_numdims; i++)
                {
                    this->m_numelement *= (this->m_coordinnates[i]);
                }
                this->m_data = this->m_allocator.allocate(this->m_numelement);

                // fill stride
                this->m_strides[this->m_numdims - 1] = 1;
                for (ptrdiff_t dim = this->m_numdims - 2; dim >= 0; --dim)
                {
                    this->m_strides[dim] = this->m_strides[dim + 1] * (bound()[dim + 1]);
                }
            }
        }

        std::vector<std::size_t> GetCoords() const override
        {
            std::vector<std::size_t> out(this->m_numdims);
            // use normal traversal
            for (size_t i = 0; i < this->m_numdims; ++i)
            {
                out[i] = this->m_coordinnates[i];
            }
            return out;
        }
        std::size_t GetNumDims() const POUTRE_NOEXCEPT override
        {
            return parent_template::GetNumDims();
        }

        CompoundType GetCType() const POUTRE_NOEXCEPT override
        {
            return m_ctype;
        }

        PType GetPType() const POUTRE_NOEXCEPT override
        {
            return m_ptype;
        }

        ImgType GetImgType() const POUTRE_NOEXCEPT override
        {
            return m_imgtype;
        }

        std::unique_ptr<IInterface> Clone() const override
        {
            return std::make_unique<self_type>(*this);
        }

        std::string str() const POUTRE_NOEXCEPT override
        {
            std::ostringstream out;
            out << "Image" << std::endl;
            out << "\tType: " << this->GetImgType() << std::endl;
            out << "\tCtype: " << this->GetCType() << std::endl;
            out << "\tPtype: " << this->GetPType() << std::endl;
            const auto &numDims = this->GetNumDims();
            out << "\tNumdim: " << numDims << std::endl;
            const auto &coords = this->GetCoords();
            out << "\tcoord: (";
            for (size_t i = 0; i < numDims - 1; i++)
            {
                out << coords[i] << ", ";
            }
            if ((ptrdiff_t)numDims - 1 >= 0)
            {
                out << coords[numDims - 1];
            }
            out << ")" << std::endl;
            return out.str();
        }

        using parent_template::bound;
        using parent_template::GetNumDims;
        using parent_template::shape;
        using parent_template::stride;

        template <ptrdiff_t Rank = NumDims, class = typename std::enable_if<details::IsRankEqual2<Rank>::value>::type>
        POUTRE_CXX14_CONSTEXPR scoord GetYSize() const POUTRE_NOEXCEPT
        {
            return this->m_coordinnates[0];
        }
        template <ptrdiff_t Rank = NumDims, class = typename std::enable_if<details::IsRankEqual2<Rank>::value>::type>
        POUTRE_CXX14_CONSTEXPR scoord GetXSize() const POUTRE_NOEXCEPT
        {
            return this->m_coordinnates[1];
        }
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

        template <ptrdiff_t Rank = NumDims, class = typename std::enable_if<details::IsRankEqual2<Rank>::value>::type>
        POUTRE_CXX14_CONSTEXPR void SetPixel(scoord x, scoord y, value_type value) POUTRE_NOEXCEPTONLYNDEBUG
        {
            POUTRE_ASSERTCHECK(x < this->m_coordinnates[1], "Access out of bound");
            POUTRE_ASSERTCHECK(x >= 0, "Access out of bound");
            POUTRE_ASSERTCHECK(y < this->m_coordinnates[0], "Access out of bound");
            POUTRE_ASSERTCHECK(y >= 0, "Access out of bound");
            this->m_data[y * this->m_strides[0] + x * this->m_strides[1]] = value;
        }

        template <ptrdiff_t Rank = NumDims, class = typename std::enable_if<details::IsRankEqual2<Rank>::value>::type>
        POUTRE_CXX14_CONSTEXPR void SetPixel(idx2d index, value_type value) POUTRE_NOEXCEPTONLYNDEBUG
        {
            this->SetPixel(index[1], index[0], value);
        }

        template <ptrdiff_t Rank = NumDims, class = typename std::enable_if<details::IsRankEqual2<Rank>::value>::type>
        POUTRE_CXX14_CONSTEXPR void SetPixel(pt2D_scoord pt2D, value_type value) POUTRE_NOEXCEPTONLYNDEBUG
        {
            this->SetPixel(pt2D.get<0>(), pt2D.get<1>(), value);
        }

        template <ptrdiff_t Rank = NumDims, class = typename std::enable_if<details::IsRankEqual2<Rank>::value>::type>
        POUTRE_CXX14_CONSTEXPR value_type GetPixel(scoord x, scoord y) const POUTRE_NOEXCEPTONLYNDEBUG
        {
            POUTRE_ASSERTCHECK(x < this->m_coordinnates[1], "Access out of bound");
            POUTRE_ASSERTCHECK(x >= 0, "Access out of bound");
            POUTRE_ASSERTCHECK(y < this->m_coordinnates[0], "Access out of bound");
            POUTRE_ASSERTCHECK(y >= 0, "Access out of bound");
            return this->m_data[y * this->m_strides[0] + x * this->m_strides[1]];
        }

        template <ptrdiff_t Rank = NumDims, class = typename std::enable_if<details::IsRankEqual2<Rank>::value>::type>
        POUTRE_CXX14_CONSTEXPR value_type GetPixel(idx2d index) const POUTRE_NOEXCEPTONLYNDEBUG
        {
            return this->GetPixel(index[1], index[0]);
        }

        template <ptrdiff_t Rank = NumDims, class = typename std::enable_if<details::IsRankEqual2<Rank>::value>::type>
        POUTRE_CXX14_CONSTEXPR value_type GetPixel(pt2D_scoord pt2D) const POUTRE_NOEXCEPTONLYNDEBUG
        {
            return this->GetPixel(pt2D.get<0>(), pt2D.get<1>());
        }

        template <ptrdiff_t Rank = NumDims, class = typename std::enable_if<details::IsRankEqual2<Rank>::value>::type>
        POUTRE_CXX14_CONSTEXPR pointer GetLineBuffer(scoord y) POUTRE_NOEXCEPTONLYNDEBUG
        {

            POUTRE_ASSERTCHECK(y < this->m_coordinnates[0], "Access out of bound");
            POUTRE_ASSERTCHECK(y >= 0, "Access out of bound");
            return this->m_data + y * this->m_strides[0];
        }

        template <ptrdiff_t Rank = NumDims, class = typename std::enable_if<details::IsRankEqual2<Rank>::value>::type>
        POUTRE_CXX14_CONSTEXPR const_pointer GetLineBuffer(scoord y) const POUTRE_NOEXCEPTONLYNDEBUG
        {
            POUTRE_ASSERTCHECK(y < this->m_coordinnates[0], "Access out of bound");
            POUTRE_ASSERTCHECK(y >= 0, "Access out of bound");
            return this->m_data + y * this->m_strides[0];
        }
        // Modifiers
        using parent_template::assign;
        using parent_template::fill;

        POUTRE_CXX14_CONSTEXPR void swap(self_type &rhs) // POUTRE_NOEXCEPT(POUTRE_NOEXCEPT
                                                         // (swap(declval<value_type&>( ),
                                                         // declval<value_type&>( )))) //wait MSVC2013
                                                         // noexcept impl ...
        {
            if (this != &rhs)
            {
                using std::swap;
                // swap(this->, rhs.); // nothrow)
                parent_template::swap(*this);
            }
        }

        iterator begin() POUTRE_NOEXCEPT
        {
            return iterator(parent_template::m_data, parent_template::m_data);
        }

        const_iterator cbegin() const POUTRE_NOEXCEPT
        {
            return const_iterator(parent_template::m_data, parent_template::m_data);
        }

        iterator end() POUTRE_NOEXCEPT
        {
            return iterator(parent_template::m_data + parent_template::m_numelement, parent_template::m_data);
        }

        const_iterator cend() const POUTRE_NOEXCEPT
        {
            return const_iterator(parent_template::m_data + parent_template::m_numelement, parent_template::m_data);
        }

        reverse_iterator rbegin() POUTRE_NOEXCEPT
        {
            return (
                reverse_iterator(parent_template::m_data + parent_template::m_numelement - 1, parent_template::m_data));
        }

        const_reverse_iterator crbegin() const POUTRE_NOEXCEPT
        {
            return (const_reverse_iterator(parent_template::m_data + parent_template::m_numelement - 1,
                                           parent_template::m_data));
        }

        reverse_iterator rend() POUTRE_NOEXCEPT
        {
            return (reverse_iterator(parent_template::m_data - 1, parent_template::m_data));
        }

        const_reverse_iterator crend() const POUTRE_NOEXCEPT
        {
            return (const_reverse_iterator(parent_template::m_data - 1, parent_template::m_data));
        }
        // end std::array like interface
      public:
        // public copyctor used through clone (for make_unique ask this constructor
        // has to be public)

        DenseImage(const self_type &rhs) : parent_template(rhs)
        {
        }

      public:
        // disable copyassignement
        self_type &operator=(const self_type &other) = delete;

        // move constructor

        DenseImage(self_type &&rhs) noexcept : parent_template(rhs)
        {
        }

        // move assignment operator

        self_type &operator=(self_type &&rhs) noexcept
        {
            if (this != &rhs) // http://scottmeyers.blogspot.fr/2014/06/the-drawbacks-of-implementing-move.html
            {
                // release resource
                this->m_allocator.deallocate(this->m_data, this->m_numelement);
                // Copy the data pointer and its length from the source object.
                this->m_data = rhs.m_data;
                this->m_numelement = rhs.m_numelement;
                this->m_coordinnates = rhs.m_coordinnates;
                this->m_strides = rhs.m_strides;
                this->m_allocator = rhs.m_allocator;

                // release
                rhs.m_numelement = 0;
                rhs.m_data = nullptr;
                /* m_coordinnates = {};*/
            }
            return *this;
        }
    };

    ///////////////////////////////////////////////////////////////////////////////////
    /* Translate to view*/

    // Linear view, linearize container
    template <class valuetype, std::ptrdiff_t Rank>
    POUTRE_CXX14_CONSTEXPR array_view<valuetype, 1> lview(DenseImage<valuetype, Rank> &iImg)
    {
        return poutre::array_view<valuetype, 1>(iImg.data(), bd1d{(ptrdiff_t)iImg.size()});
    }

    // FIXME convertion loose qualifiers
    template <class valuetype, std::ptrdiff_t Rank>
    POUTRE_CXX14_CONSTEXPR poutre::array_view<valuetype, 1> lview(const DenseImage<valuetype, Rank> &iImg)
    { //-V659
        return view(const_cast<DenseImage<valuetype, Rank> &>(iImg));
    }

    // Default view
    template <class valuetype, std::ptrdiff_t Rank>
    POUTRE_CXX14_CONSTEXPR poutre::array_view<valuetype, Rank> view(DenseImage<valuetype, Rank> &iImg)
    {
        return poutre::array_view<valuetype, Rank>(iImg.data(), iImg.shape());
    }

    // FIXME convertion loose qualifiers
    template <class valuetype, std::ptrdiff_t Rank>
    POUTRE_CXX14_CONSTEXPR poutre::array_view<valuetype, Rank> view(const DenseImage<valuetype, Rank> &iImg)
    {
        return view(const_cast<DenseImage<valuetype, Rank> &>(iImg));
    }
    ///////////////////////////////////////////////////////////////////////////////////

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

    extern template class DenseImage<compound<pUINT8, 3>, 2>;
    extern template class DenseImage<compound<pINT32, 3>, 2>;
    extern template class DenseImage<compound<pFLOAT, 3>, 2>;
    extern template class DenseImage<compound<pINT64, 3>, 2>;
    extern template class DenseImage<compound<pDOUBLE, 3>, 2>;

    extern template class DenseImage<compound<pUINT8, 4>, 2>;
    extern template class DenseImage<compound<pINT32, 4>, 2>;
    extern template class DenseImage<compound<pFLOAT, 4>, 2>;
    extern template class DenseImage<compound<pINT64, 4>, 2>;
    extern template class DenseImage<compound<pDOUBLE, 4>, 2>;

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

    //! @} doxygroup: image_processing_container_group
} // namespace poutre
#endif // POUTRE_IMAGEPROCESSING_CONTAINER_HPP__
