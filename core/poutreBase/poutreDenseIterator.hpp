
//==============================================================================
//                  Copyright (c) 2015 - Thomas Retornaz                      //
//                     thomas.retornaz@mines-paris.org                        //
//          Distributed under the Boost Software License, Version 1.0.        //
//                 See accompanying file LICENSE.txt or copy at               //
//                     http://www.boost.org/LICENSE_1_0.txt                   //
//==============================================================================

#ifndef POUTRE_DENSE_ITERATOR_HPP__
#define POUTRE_DENSE_ITERATOR_HPP__


#include <iterator>


#ifndef POUTRE_TYPE_HPP__
#include <poutreBase/poutreTypes.hpp>
#endif

namespace poutre
{


   //adapted from https://github.com/navyenzo/blIteratorAPI

   // Raw pdense_iterator with random access  
   template<typename DataType>
      class pdense_iterator : public std::iterator < std::random_access_iterator_tag,
      DataType,
      std::ptrdiff_t,
      DataType*,
      DataType& >
   {
   public:
      using parent = std::iterator < std::random_access_iterator_tag, DataType, std::ptrdiff_t, DataType*, DataType& > ;
      using self_type = pdense_iterator < DataType > ;

      pdense_iterator(DataType* ptr = nullptr, DataType* ptrorig = nullptr) :m_ptr(ptr), m_ptrorig(ptrorig)
      {
      }
      pdense_iterator(const self_type& rhs) = default;
      self_type&                  operator=(const self_type& rhs) = default;

      ~pdense_iterator( )
      {
      }

      operator bool( ) const
      {
         if ( (m_ptr) && (m_ptrorig) ) return true;
         return false;
      }

      bool                                        operator==(const self_type& rhs) const
      {
         return (m_ptr == rhs.getConstPtr( ) && m_ptrorig == rhs.getConstOrigPtr( ));
      }
      bool                                        operator!=(const self_type& rhs) const
      {
         return (m_ptr != rhs.getConstPtr( ) || m_ptrorig != rhs.getConstOrigPtr( ));
      }

      pdense_iterator<DataType>&                  operator+=(const std::ptrdiff_t& movement)
      {
         m_ptr += movement; return (*this);
      }
      pdense_iterator<DataType>&                  operator-=(const std::ptrdiff_t& movement)
      {
         m_ptr -= movement; return (*this);
      }
      pdense_iterator<DataType>&                  operator++()
      {
         ++m_ptr; return (*this);
      }
      pdense_iterator<DataType>&                  operator--()
      {
         --m_ptr; return (*this);
      }

      pdense_iterator<DataType>                   operator++(int)
      {
         auto temp(*this); ++m_ptr; return temp;
      }
      pdense_iterator<DataType>                   operator--(int)
      {
         auto temp(*this); --m_ptr; return temp;
      }

      ptrdiff_t                                   operator-(const self_type& rawIterator)
      {
         return std::distance(rawIterator.getPtr( ), this->getPtr( ));
      }

      reference                                 operator*()
      {
         return *m_ptr;
      }
      const reference                           operator*() const
      {
         return *m_ptr;
      }
      pointer                                   operator->()
      {
         return m_ptr;
      }

      pointer                                   getPtr( ) const
      {
         return m_ptr;
      }
      pointer                                   getOrigPtr( ) const
      {
         return m_ptrorig;
      }
      void                                      setPtr(DataType* ptr)
      {
         m_ptr = ptr;
      }
      void                                      setOrigPtr(DataType* ptr)
      {
         m_ptrorig = ptr;
      }
      const pointer                             getConstPtr( ) const
      {
         return m_ptr;
      }
      const pointer                             getConstOrigPtr( ) const
      {
         return m_ptrorig;
      }

      const offset                              getOffset( ) const
      {
         return m_ptr - m_ptrorig;
      }
   protected:

      pointer                                 m_ptr;
      pointer                                 m_ptrorig;
   };


   template<typename DataType>
      class pdense_reverse_iterator : public pdense_iterator < DataType >
   {
   public:
      using parent = pdense_iterator < DataType > ;
      using self_type = pdense_reverse_iterator < DataType > ;

      pdense_reverse_iterator(DataType* ptr = nullptr, DataType* ptrorig = nullptr) :parent(ptr, ptrorig)
      {
      }
      //pdense_reverse_iterator(const self_type& rhs)
      //  {
      //  this->m_ptr = rhs.getPtr( ); this->m_ptrorig = rhs.getOrigPtr( );
      //  }

      pdense_reverse_iterator(const self_type& rhs) = default;
      self_type&           operator=(const self_type& rawReverseIterator) = default;
      ~pdense_reverse_iterator( )
      {
      }

      //self_type&           operator=(const self_type& rawIterator)
      //  {
      //  this->m_ptr = rawIterator.getPtr( ); this->m_ptrorig = rawIterator.getOrigPtr( ); return (*this);
      //  }

      self_type&           operator+=(const std::ptrdiff_t& movement)
      {
         this->m_ptr -= movement; return (*this);
      }
      self_type&           operator-=(const std::ptrdiff_t& movement)
      {
         this->m_ptr += movement; return (*this);
      }
      self_type&           operator++()
      {
         --this->m_ptr; return (*this);
      }
      self_type&           operator--()
      {
         ++this->m_ptr; return (*this);
      }
      self_type            operator++(int)
      {
         auto temp(*this); --this->m_ptr; return temp;
      }
      self_type            operator--(int)
      {
         auto temp(*this); ++this->m_ptr; return temp;
      }

      ptrdiff_t                                    operator-(const self_type& rawReverseIterator)
      {/*todo assert same orig?*/ 
         return std::distance(this->getPtr( ), rawReverseIterator.getPtr( ));
      }

      const offset                                 getOffset( ) const
      {
         return m_ptr - m_ptrorig;
      }

      pdense_iterator<DataType>                    base( )
      {
         pdense_iterator<DataType> forwardIterator(this->m_ptr, this->m_ptrorig); ++forwardIterator; return forwardIterator;
      }
   };




}
#endif //POUTRE_DENSE_ITERATOR_HPP__
