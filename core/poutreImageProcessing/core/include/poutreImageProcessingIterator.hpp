#ifndef POUTREIMAGEPROCESSINGITERATOR_HPP__
#define POUTREIMAGEPROCESSINGITERATOR_HPP__

#ifndef POUTREIMAGEPROCESSING_HPP__
#include <poutreImageProcessing/poutreImageProcessing.hpp>
#endif

#ifndef POUTRE_TRACE__HPP__
#include <poutreBase/poutreTrace.hpp>
#endif

#include <memory>

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
      using self = pdense_iterator < DataType > ;

      pdense_iterator(DataType* ptr = nullptr, DataType* ptrorig = nullptr) :m_ptr(ptr), m_ptrorig(ptrorig){}
      pdense_iterator(const pdense_iterator<DataType>& rawIterator) = default;
      pdense_iterator<DataType>&                  operator=(const pdense_iterator<DataType>& rawIterator) = default;
      //pdense_iterator<DataType>&                  operator=(DataType* ptr, DataType* ptrorig){ m_ptr = ptr; m_ptrorig = ptrorig; return (*this); }

      ~pdense_iterator(){}

      operator bool() const
        {
        if (m_ptr) && (m_ptrorig) return true;
        return false;
        }

      bool                                        operator==(const pdense_iterator<DataType>& rawIterator)const{ return (m_ptr == rawIterator.getConstPtr() && m_ptrorig == rawIterator.getConstOrigPtr()); }
      bool                                        operator!=(const pdense_iterator<DataType>& rawIterator)const{ return (m_ptr != rawIterator.getConstPtr() || m_ptrorig != rawIterator.getConstOrigPtr()); }

      pdense_iterator<DataType>&                  operator+=(const std::ptrdiff_t& movement){ m_ptr += movement; return (*this); }
      pdense_iterator<DataType>&                  operator-=(const std::ptrdiff_t& movement){ m_ptr -= movement; return (*this); }
      pdense_iterator<DataType>&                  operator++(){ ++m_ptr; return (*this); }
      pdense_iterator<DataType>&                  operator--(){ --m_ptr; return (*this); }

      //pdense_iterator<DataType>                   operator++(){ auto temp(*this); ++m_ptr; return temp; }
      //pdense_iterator<DataType>                   operator--(){ auto temp(*this); --m_ptr; return temp; }
      //pdense_iterator<DataType>                   operator+(const std::ptrdiff_t& movement){ auto oldPtr = m_ptr; m_ptr += movement; auto temp(*this); m_ptr = oldPtr; return temp; }
      //pdense_iterator<DataType>                   operator-(const std::ptrdiff_t& movement){ auto oldPtr = m_ptr; m_ptr -= movement; auto temp(*this); m_ptr = oldPtr; return temp; }

      ptrdiff_t                                   operator-(const pdense_iterator<DataType>& rawIterator){ return std::distance(rawIterator.getPtr(), this->getPtr()); }

      DataType&                                 operator*(){ return *m_ptr; }
      const DataType&                           operator*()const{ return *m_ptr; }
      DataType*                                 operator->(){ return m_ptr; }

      DataType*                                 getPtr()const{ return m_ptr; }
      DataType*                                 getOrigPtr()const{ return m_ptrorig; }
      void                                      setPtr(DataType* ptr) { m_ptr = ptr; }
      void                                      setOrigPtr(DataType* ptr) { m_ptrorig = ptr; }
      const DataType*                           getConstPtr()const{ return m_ptr; }
      const DataType*                           getConstOrigPtr()const{ return m_ptrorig; }

      const offset                              getOffset()const{ return m_ptr - m_ptrorig; }
    protected:

      DataType*                                 m_ptr;
      DataType*                                 m_ptrorig;
    };


  template<typename DataType>
  class pdense_reverse_iterator : public pdense_iterator < DataType >
    {
    public:
      using parent = pdense_iterator < DataType > ;
      using self = pdense_reverse_iterator < DataType > ;

      pdense_reverse_iterator(DataType* ptr = nullptr, DataType* ptrorig = nullptr) :parent(ptr, ptrorig){}
      pdense_reverse_iterator(const pdense_iterator<DataType>& rawIterator){ this->m_ptr = rawIterator.getPtr(); this->m_ptrorig = rawIterator.getOrigPtr(); }
      pdense_reverse_iterator(const pdense_reverse_iterator<DataType>& rawReverseIterator) = default;
      ~pdense_reverse_iterator(){}

      pdense_reverse_iterator<DataType>&           operator=(const pdense_reverse_iterator<DataType>& rawReverseIterator) = default;
      pdense_reverse_iterator<DataType>&           operator=(const pdense_iterator<DataType>& rawIterator){ this->m_ptr = rawIterator.getPtr(); this->m_ptrorig = rawIterator.getOrigPtr(); return (*this); }
      //pdense_reverse_iterator<DataType>&           operator=(DataType* ptr, DataType* ptrorig){ this->setPtr(ptr); this->setOrigPtr(ptrorig); return (*this); }

      pdense_reverse_iterator<DataType>&           operator+=(const std::ptrdiff_t& movement){ this->m_ptr -= movement; return (*this); }
      pdense_reverse_iterator<DataType>&           operator-=(const std::ptrdiff_t& movement){ this->m_ptr += movement; return (*this); }
      pdense_reverse_iterator<DataType>&           operator++(){ --this->m_ptr; return (*this); }
      pdense_reverse_iterator<DataType>&           operator--(){ ++this->m_ptr; return (*this); }
      //pdense_reverse_iterator<DataType>            operator++(){ auto temp(*this); --this->m_ptr; return temp; }
      //pdense_reverse_iterator<DataType>            operator--(){ auto temp(*this); ++this->m_ptr; return temp; }
      //pdense_reverse_iterator<DataType>            operator+(const int& movement){ auto oldPtr = this->m_ptr; this->m_ptr -= movement; auto temp(*this); this->m_ptr = oldPtr; return temp; }
      //pdense_reverse_iterator<DataType>            operator-(const int& movement){ auto oldPtr = this->m_ptr; this->m_ptr += movement; auto temp(*this); this->m_ptr = oldPtr; return temp; }

      ptrdiff_t                                    operator-(const pdense_reverse_iterator<DataType>& rawReverseIterator){/*todo assert same orig?*/ return std::distance(this->getPtr(), rawReverseIterator.getPtr()); }

      pdense_iterator<DataType>                    base(){ pdense_iterator<DataType> forwardIterator(this->m_ptr, this->m_ptrorig); ++forwardIterator; return forwardIterator; }
    };




  }
#endif POUTREIMAGEPROCESSINGITERATOR_HPP__