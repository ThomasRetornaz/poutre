#ifndef POUTREIMAGEPROCESSINGCONTAINER_HPP__
#define POUTREIMAGEPROCESSINGCONTAINER_HPP__


#ifndef POUTREIMAGEPROCESSINGINTERFACE_HPP__
#include <poutreImageProcessing/core/poutreImageProcessingInterface.hpp>
#endif

#ifndef POUTREIMAGEPROCESSINGTYPE_HPP__
#include <poutreImageProcessing/core/poutreImageProcessingType.hpp>
#endif

#ifndef POUTREIMAGEPROCESSINGITERATOR_HPP__
#include <poutreImageProcessing/core/include/poutreImageProcessingIterator.hpp>
#endif

#ifndef POUTRE_CONFIG__HPP__
#include <poutreBase/poutreConfig.hpp>
#endif

#include <boost/simd/memory/allocator.hpp>
#include <boost/simd/memory/is_aligned.hpp>
#include <array>
#include <memory>
#include <utility>
#include <vector>

namespace poutre
  {
  //!FIXME dispatch on coord and allow unset dims
  template <
    class valuetype,
    std::size_t NumDims = 2,
    class allocator_type_t = boost::simd::allocator < TypeTraits<valuetype>::storage_type, TypeTraits<valuetype>::alignement >
  >
  class DenseImage : public IInterface
    {
    public:

      using  self_type = DenseImage < valuetype, NumDims, allocator_type_t >;
      using  parent_type = IInterface;

      using  value_type = valuetype;//typename TypeTraits<ptype>::storage_type;
      using  pointer = typename allocator_type_t::pointer;
      using  const_pointer = typename  allocator_type_t::const_pointer;
      using  reference = value_type&;
      using  const_reference = value_type const &;
      using  difference_type = std::ptrdiff_t;

      using  iterator = pdense_iterator < value_type > ;
      using  const_iterator = pdense_iterator < const value_type > ;

      using  reverse_iterator = pdense_reverse_iterator < value_type > ;
      using  const_reverse_iterator = pdense_reverse_iterator < const value_type > ;

      using  index = offset;
      using  size_type = std::size_t;
         

      using size_list_t = std::array < size_type, NumDims > ;
      using allocator_type = allocator_type_t;

    
      static const PType m_ptype = TypeTraits<value_type>::pixel_type;
      static const CompoundType m_ctype = TypeTraits<value_type>::compound_type;


      static const size_t m_numdims = NumDims;
      static const size_t alignement = TypeTraits<valuetype>::alignement;
      static const ImgType m_imgtype = ImgType::ImgType_Dense;

    private:
      pointer m_data;
      size_list_t m_size_list;
      allocator_type m_allocator;
      size_t m_numelemwithpaddingifany;

    public:
      DenseImage(const std::vector<size_t>& dims) :m_data(nullptr), m_size_list( ), m_allocator( ), m_numelemwithpaddingifany(0)
        {
        if (dims.size( ) != m_numdims) POUTRE_RUNTIME_ERROR("Invalid input initializer regarding NumDims of DenseImage container");
        std::copy(dims.begin( ), dims.end( ), m_size_list.begin( ));


        //compute full array size with include possible padding for each first stride
        if (!m_size_list.empty( ))
          {
          //if (m_numdims == 2)
          //  {
          //  m_numelemwithpadding = ((m_size_list[0] + default_padding_size - 1) & ~(default_padding_size - 1));
          //  }
          //else
          //  {
          m_numelemwithpaddingifany = m_size_list[0];
          //  }
          for (size_t i = 1; i < m_size_list.size( ); i++)
            {
            m_numelemwithpaddingifany *= m_size_list[i];
            }
          m_data = m_allocator.allocate(m_numelemwithpaddingifany);
          }
        }
      DenseImage(const std::initializer_list<size_t>& dims) :m_data(nullptr), m_size_list( ), m_allocator( ), m_numelemwithpaddingifany(0)
        {
        if (dims.size( ) != m_numdims) POUTRE_RUNTIME_ERROR("Invalid input initializer regarding NumDims of DenseImage container");
        std::copy(dims.begin( ), dims.end( ), m_size_list.begin( ));


        //compute full array size with include possible padding for each first stride
        if (!m_size_list.empty( ))
          {
          //if (m_numdims == 2)
          //  {
          //  m_numelemwithpadding = ((m_size_list[0] + default_padding_size - 1) & ~(default_padding_size - 1));
          //  }
          //else
          //  {
          m_numelemwithpaddingifany = m_size_list[0];
          //  }
          for (size_t i = 1; i < m_size_list.size( ); i++)
            {
            m_numelemwithpaddingifany *= m_size_list[i];
            }
          m_data = m_allocator.allocate(m_numelemwithpaddingifany);
          }
        }
      ~DenseImage( ) NOEXCEPT
        {
        if (m_data) m_allocator.deallocate(m_data, m_numelemwithpaddingifany);
        }

          //std::array like interface

          //Capacity
          const size_type size( ) NOEXCEPT
          {
          return m_numelemwithpaddingifany;
          }
            const size_type max_size( ) NOEXCEPT
            {
            return this->size( );
            }

              const bool empty( ) NOEXCEPT
              {
              return this->size( ) == 0;
              }

                //Element access
            reference operator[] (size_type n) NOEXCEPT{ return m_data[n]; }
            const_reference operator[] (size_type n) const NOEXCEPT{ return m_data[n]; }

              reference at(size_type n)
              {
              if (n >= m_numelemwithpaddingifany || n < 0) POUTRE_RUNTIME_ERROR("Access out of bound");
              return m_data[n];
              }
            const_reference at(size_type n) const
              {
              if (n >= m_numelemwithpaddingifany || n < 0) POUTRE_RUNTIME_ERROR("Access out of bound");
              return m_data[n];
              }

            reference front( )  NOEXCEPT{ return m_data[0]; }
            const_reference front( ) const NOEXCEPT{ return m_data[0]; }
            reference back( )  NOEXCEPT{ return m_data[m_numelemwithpaddingifany - 1]; }
            const_reference back( ) const  NOEXCEPT{ return m_data[m_numelemwithpaddingifany - 1]; }
            pointer datas( ) NOEXCEPT{ return m_data; }
            const_pointer datas( ) const NOEXCEPT{ return m_data; }

              //Modifiers
              void assign(const value_type& val)
              {
              this->fill(val);
              }

            void fill(const value_type& val)
              {
              // assign value to all elements
              std::fill(this->begin( ), this->end( ), val);
              }

            void swap(self_type& rhs) //NOEXCEPT(NOEXCEPT (swap(declval<value_type&>( ), declval<value_type&>( )))) //wait MSVC2013 noexcept impl ...
              {
              if (this != &rhs)
                {
                using std::swap;
                swap(this->m_data, rhs.m_data); //nothrow
                swap(this->m_numelemwithpaddingifany, rhs.m_numelemwithpaddingifany);//notthrow
                swap(this->m_size_list, rhs.m_size_list);//?throw
                swap(this->m_allocator, rhs.m_allocator);//?throw
                }
              }



            iterator begin( ) NOEXCEPT{ return iterator(m_data, m_data); }

            const_iterator cbegin( ) const NOEXCEPT{ return const_iterator(m_data, m_data); }

            iterator end( ) NOEXCEPT{ return iterator(m_data + m_numelemwithpaddingifany, m_data); }

            const_iterator cend( ) const NOEXCEPT{ return const_iterator(m_data + m_numelemwithpaddingifany, m_data); }

            reverse_iterator rbegin( ) NOEXCEPT{ return (reverse_iterator(m_data + m_numelemwithpaddingifany - 1, m_data)); }

            const_reverse_iterator crbegin( ) const NOEXCEPT{ return (const_reverse_iterator(m_data + m_numelemwithpaddingifany - 1, m_data)); }

            reverse_iterator rend( ) NOEXCEPT{ return (reverse_iterator(m_data - 1, m_data)); }

            const_reverse_iterator crend( ) const NOEXCEPT{ return (const_reverse_iterator(m_data - 1, m_data)); }

              //end std::array like interface

              //TODO use extern class ?
              std::vector<std::size_t> GetCoords( ) const override
              {
              std::vector<std::size_t> out(m_numdims);
              std::copy(m_size_list.begin( ), m_size_list.end( ), out.begin( ));
              return out;
              }
            size_t GetNumDims( ) const NOEXCEPT{ return m_numdims; }

            CompoundType GetCType( ) const NOEXCEPT  override{ return m_ctype; }

            PType GetPType( ) const NOEXCEPT  override{ return m_ptype; }

            ImgType GetImgType( ) const NOEXCEPT  override{ return m_imgtype; }

              std::unique_ptr<IInterface> Clone( ) const override
              {//FIXME
              //return std::make_unique<DenseImage>(*this);
              std::unique_ptr<IInterface> uni(new DenseImage(*this));
              return uni;
              }




            std::string str( ) const
              {
              std::ostringstream out;
              out << "Image" << std::endl;
              out << "\tType: " << this->GetImgType( ) << std::endl;
              out << "\tCtype: " << this->GetCType( ) << std::endl;
              out << "\tPtype: " << this->GetPType( ) << std::endl;
              const auto &numDims = this->GetNumDims( );
              out << "\tNumdim: " << numDims << std::endl;
              const auto &coords = this->GetCoords( );
              out << "\tcoord: (";
              for (size_t i = 0; i < numDims - 1; i++)
                {
                out << coords[i] << ", ";
                }
              if (numDims - 1 >= 0)
                {
                out << coords[numDims - 1];
                }
              out << ")" << std::endl;
              return out.str( );
              }
    protected:
      //protected copyctor used through clone
      self_type(const self_type& rhs) :m_data(nullptr), m_numelemwithpaddingifany(rhs.m_numelemwithpaddingifany), m_size_list(rhs.m_size_list), m_allocator(rhs.m_allocator)
        {
        m_data = m_allocator.allocate(m_numelemwithpaddingifany);
        std::copy(rhs.m_data, rhs.m_data + m_numelemwithpaddingifany, m_data);
        }
    public:
      //disable copyassignement 
      self_type& operator=(const self_type& other) = delete;

      //move constructor 
      self_type(self_type&& rhs) :m_data(nullptr), m_numelemwithpaddingifany(0), m_size_list(),m_allocator()
          {
          *this = std::move(rhs);
          }

      //move assignment operator 
      self_type& operator= (self_type&& rhs)
        {
        if (this != &rhs) // ?? http://scottmeyers.blogspot.fr/2014/06/the-drawbacks-of-implementing-move.html
          {
          //release resource
          m_allocator.deallocate(m_data, m_numelemwithpaddingifany);
          // Copy the data pointer and its length from the source object.
          m_data = rhs.m_data;
          m_numelemwithpaddingifany = rhs.m_numelemwithpaddingifany;
          m_size_list = std::move(rhs.m_size_list); 
          m_allocator = std::move(rhs.m_allocator); //?

          //release 
          rhs.m_numelemwithpaddingifany = 0;
          rhs.m_data = nullptr;
          }
      return *this;
        }
    };

    template <class coordinate_container>
    POUTRE_STRONG_INLINE coordinate_container GetCoordsFomOffset(const coordinate_container &coord, offset offset) NOEXCEPTRELONLYNDEBUG
      {
      POUTRE_ASSERTCHECK(offset >= 0, "offset must be >=0");
      coordinate_container ret;
      auto numcoord = coord.size( );
      auto i = 0;
      for (; i < numcoord && offset != 0; i++)
        {
        auto current= coord[i];
        POUTRE_ASSERTCHECK(current > 0, "Sub coord is <0 at " + std::to_string(i) + ",see " + std::to_string(current));
        auto dv = std::div(offset,curent);
        ret[i] = dv.rem;//offset % current;
        offset = dv.quot;//offset=offset/current
        }
      for (; i < numcoord; i++)
        {
        ret[i] = 0;
        }
      return ret;
      }


  //todo define macros
  extern template class DenseImage <pUINT8, 1 >;
  extern template class DenseImage <pINT32, 1 >;
  extern template class DenseImage <pFLOAT, 1 >;
  extern template class DenseImage <pINT64, 1 >;
  extern template class DenseImage <pDOUBLE, 1 >;

  extern template class DenseImage < pUINT8, 2 >;
  extern template class DenseImage < pINT32, 2 >;
  extern template class DenseImage < pFLOAT, 2 >;
  extern template class DenseImage < pINT64, 2 >;
  extern template class DenseImage < pDOUBLE, 2 >;
  
  extern template class DenseImage <compound_pixel < pUINT8, 3 >>;
  extern template class DenseImage <compound_pixel < pINT32, 3 >>;
  extern template class DenseImage <compound_pixel < pFLOAT, 3 >>;
  extern template class DenseImage <compound_pixel < pINT64, 3 >>;
  extern template class DenseImage <compound_pixel < pDOUBLE, 3 >>;

  extern template class DenseImage <compound_pixel < pUINT8, 4 >>;
  extern template class DenseImage <compound_pixel < pINT32, 4 >>;
  extern template class DenseImage <compound_pixel < pFLOAT, 4 >>;
  extern template class DenseImage <compound_pixel < pINT64, 4 >>;
  extern template class DenseImage <compound_pixel < pDOUBLE, 4 >>;

  extern template class DenseImage < pUINT8, 3 >;
  extern template class DenseImage < pINT32, 3 >;
  extern template class DenseImage < pFLOAT, 3 >;
  extern template class DenseImage < pINT64, 3 >;
  extern template class DenseImage < pDOUBLE, 3 >;

  extern template class DenseImage < pUINT8, 4 >;
  extern template class DenseImage < pINT32, 4 >;
  extern template class DenseImage < pFLOAT, 4 >;
  extern template class DenseImage < pINT64, 4 >;
  extern template class DenseImage < pDOUBLE, 4 >;

  }
#endif POUTREIMAGEPROCESSINGCONTAINER_HPP__
