#ifndef POUTREMULTIDIMBOUNDINDEX_HPP__
#define POUTREMULTIDIMBOUNDINDEX_HPP__

/**
 * @file   poutreMultiDimBoundIndex.hpp
 * @author Thomas Retornaz
 * @brief  
 * 
 * 
 */

#ifndef POUTRE_CONFIG__HPP__
#include <poutreBase/poutreConfig.hpp>
#endif

#ifndef POUTRETYPE_HPP__
#include <poutreBase/poutreTypes.hpp>
#endif

#ifndef POUTREMULTIDIMBOUNDINDEX_DETAILS_HPP__
#include <poutreBase/poutreMultiDimBoundIndexDetails.hpp>
#endif

#include <stdexcept>
#include <array>

#ifdef _MSC_VER
#pragma warning( push )
#pragma warning( disable : 4425 )//4425 improper support constexpr
#endif

namespace poutre
{

   template <int Rank> class bounds;
   template <int Rank> class index;
   template <int Rank> class bounds_iterator;

   /*!
    * @defgroup multidimensionnale_group Multidimensional Capabilities bounds, index, View and Strided view other multi-dimentionnal array
    * Implement bounds and index defined in http://www.open-std.org/jtc1/sc22/wg21/docs/papers/2014/n3851.pdf coming from previous study in c++AMP http://download.microsoft.com/download/4/0/E/40EA02D8-23A7-4BD2-AD3A-0BFFFB640F28/CppAMPLanguageAndProgrammingModel.pdf 
    * @ingroup poutre_base
    * @{
    */

   /**
    * @brief bounds has N-dimensional rectangle semantic 
    * bounds is a type that represents rectangular bounds on an N-dimensional discrete space
    * @return 
    */
   template <int Rank> class bounds
   {
      static_assert(Rank > 0,
                    "bounds requires a Rank>0");
   public:
      static /*constexpr*/ const int rank = Rank;

      using size_type = size_t;
      using value_type = ptrdiff_t;
      using const_value_type = std::add_const_t<value_type>;
      using pointer = std::add_pointer_t < value_type > ;
      using reference = typename std::add_lvalue_reference < value_type >::type ;
      using const_pointer = std::add_pointer_t < const_value_type >;
      using const_reference = typename std::add_lvalue_reference < const_value_type >::type;
      using difference_type = std::ptrdiff_t;
      using size_type = std::size_t;
      using const_iterator = bounds_iterator <rank>;

      using array_storage = std::array < value_type, rank > ;
      using self_type = bounds < rank > ;

      array_storage m_size_list;

   public:
     /** @name Construction and Assignment
     */

     /**@{*/
      POUTRE_CONSTEXPR bounds( ) POUTRE_NOEXCEPT : m_size_list( )
      {
      }

      template <int Rank,typename std::enable_if<Rank == 1, void>::type* = nullptr>
      POUTRE_CONSTEXPR explicit bounds(const value_type& a)  POUTRE_NOEXCEPT
      {
         m_size_list[0]=a;
      }

      template <int Rank, typename std::enable_if<Rank == 2, void>::type* = nullptr>
      POUTRE_CONSTEXPR bounds(const value_type& a0, const value_type& a1)  POUTRE_NOEXCEPT
        {
        m_size_list[0] = a0;
        m_size_list[1] = a1;
        }

      template <int Rank, typename std::enable_if<Rank == 3, void>::type* = nullptr>
        POUTRE_CONSTEXPR bounds(const value_type& a0, const value_type& a1, const value_type& a2)  POUTRE_NOEXCEPT
          {
          m_size_list[0] = a0;
          m_size_list[1] = a1;
          m_size_list[2] = a2;
          }


      POUTRE_CXX14_CONSTEXPR bounds(const std::initializer_list<value_type>& rhs) POUTRE_NOEXCEPT
            {
            //!FIXME add check on rhs size for non c++14 constexpr
#if !defined(BOOST_NO_CXX14_CONSTEXPR)
            static_assert(rhs.size()==Rank,"Ill formed initializer list");
#endif 
            //   details::helper_assign_container_op<self_type, AssignOpType::AssignOp, Rank>::op(rhs, *this);
            //safe but silly behavior if no static assertion
            std::copy_n(rhs.begin( ), std::min<ptrdiff_t>((ptrdiff_t)rank, rhs.size( )), m_size_list.begin( ));
            }

      POUTRE_CXX14_CONSTEXPR explicit bounds(const value_type (&rhs)[Rank]) POUTRE_NOEXCEPT
      {
         details::helper_assign_container_op<self_type, AssignOpType::AssignOp, Rank>::op(rhs, *this);
      }

      POUTRE_CONSTEXPR bounds(const self_type& rhs) POUTRE_NOEXCEPT
      {
      details::helper_assign_container_op<self_type, AssignOpType::AssignOp, Rank>::op(rhs, *this);
      }

      POUTRE_CXX14_CONSTEXPR self_type& operator=(const self_type &rhs) POUTRE_NOEXCEPT
      {
         if (this != &rhs)
         {
         details::helper_assign_container_op<self_type, AssignOpType::AssignOp, Rank>::op(rhs, *this);
         }
         return *this;
      }

      //is this really needed ?
      bounds(self_type&& rhs) POUTRE_NOEXCEPT
      {
         *this = std::move(rhs);
      }

      //is this really needed ?
      self_type& operator= (self_type&& rhs) POUTRE_NOEXCEPT_IF(is_nothrow_move_constructible<self_type::array_storage>::value && is_nothrow_move_assignable<self_type::array_storage>::value)
      {
         if (this != &rhs) // ?? http://scottmeyers.blogspot.fr/2014/06/the-drawbacks-of-implementing-move.html
         {
            m_size_list = std::move(rhs.m_size_list);
         }
         return *this;
      }
      /**@}*/

      /** @name Accessing Components
      */
      /**@{*/
      POUTRE_ALWAYS_INLINE POUTRE_CXX14_CONSTEXPR reference operator[] (size_type n) POUTRE_NOEXCEPT
      {
         //POUTRE_ASSERTCHECK(n >= 0, "bounds operator[n] n must be in >=0");
         //POUTRE_ASSERTCHECK(n < rank, "bounds operator[n] n must be in [0,m_numdims[");
         //!FIXME add check on rhs size for non c++14 constexpr ?
         #if !defined(BOOST_NO_CXX14_CONSTEXPR)
         static_assert(rhs.size( ) == Rank, "Ill formed initializer list");
         #endif 

         return m_size_list[n];
      }

      POUTRE_ALWAYS_INLINE POUTRE_CONSTEXPR const_reference operator[] (size_type n) const POUTRE_NOEXCEPT
      {
        //POUTRE_ASSERTCHECK(n >= 0, "bounds operator[n] n must be in >=0");
        //POUTRE_ASSERTCHECK(n < rank, "bounds operator[n] n must be in [0,m_numdims[");
        //!FIXME add check on rhs size for non c++14 constexpr ?
        #if !defined(BOOST_NO_CXX14_CONSTEXPR)
        static_assert(rhs.size( ) == Rank, "Ill formed initializer list");
        #endif 
        return m_size_list[n];
      }

      /**@}*/
      /** @name Comparison Operators
      */
      /**@{*/
      POUTRE_CXX14_CONSTEXPR bool operator==(const self_type& rhs) const POUTRE_NOEXCEPT
      {
      return  details::helper_comp_equal_container_op<self_type, Rank>::op(rhs, *this);
      }
      POUTRE_CXX14_CONSTEXPR bool operator!=(const self_type& rhs) const POUTRE_NOEXCEPT
      {
      return  !details::helper_comp_equal_container_op<self_type, Rank>::op(rhs, *this);
      }
      /**@}*/

      friend std::ostream& operator<<(std::ostream& out, const self_type& rhs) POUTRE_NOEXCEPT
      {
         for (const auto& val : rhs.m_size_list)
         {
            out << val;
            out << ",";
         }
         return out;
      }

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

        /** @name Scaling Operators 
        */
        /**@{*/
      template <typename ArithmeticType, typename std::enable_if<std::is_arithmetic<ArithmeticType>::value>::type* = nullptr>
      POUTRE_CXX14_CONSTEXPR   bounds  operator*(ArithmeticType v) const POUTRE_NOEXCEPT
      {
         self_type tmp(*this); //return a fresh new object
         details::helper_assign_container_valueop<self_type, AssignOpType::AssignOpMul, Rank>::op(v, tmp);
         return tmp;
      }

     template <typename ArithmeticType, typename std::enable_if<std::is_arithmetic<ArithmeticType>::value>::type* = nullptr>
      POUTRE_CXX14_CONSTEXPR   bounds  operator/(ArithmeticType v) const POUTRE_NOEXCEPT
      {
         self_type tmp(*this); //return a fresh new object
         details::helper_assign_container_valueop<self_type, AssignOpType::AssignOpDiv, Rank>::op(v, tmp);
         return tmp;
      }

      template <typename ArithmeticType, typename std::enable_if<std::is_arithmetic<ArithmeticType>::value>::type* = nullptr>
      POUTRE_CXX14_CONSTEXPR   bounds& operator*=(ArithmeticType v) POUTRE_NOEXCEPT
      {
         details::helper_assign_container_valueop<self_type, AssignOpType::AssignOpMul, Rank>::op(v, *this);
         return *this;
      }

      template <typename ArithmeticType, typename std::enable_if<std::is_arithmetic<ArithmeticType>::value>::type* = nullptr>
      POUTRE_CXX14_CONSTEXPR   bounds& operator/=(ArithmeticType v) POUTRE_NOEXCEPT
      {
         details::helper_assign_container_valueop<self_type, AssignOpType::AssignOpDiv, Rank>::op(v, *this);
         return *this;
      }
        /**@}*/


     //!returns  a  hyper  volume  of  the  rectangular  space  enclosed  by  *this,  i.e.  the  product  of  all components.
     POUTRE_CONSTEXPR size_type size( ) const POUTRE_NOEXCEPT
        {
        size_t volume = m_size_list[0]; //at least rank==1
        for (size_t i = 1; i < rank; i++) volume *= m_size_list[i];
        return volume;
        }

      //!checks whether the passed index  is contained within  bounds  
      //returns true if every component of idx is equal or greater than zero and is less than the corresponding component of *this.
      POUTRE_CXX14_CONSTEXPR bool  contains(const index<rank>& idx) const POUTRE_NOEXCEPT
          {
          for (size_t i = 0; i < rank; i++)
            {
            if (idx[i] < 0) return false;
            if (idx[i] >= (*this)[i]) return false; 
            }
          return true;
          }

      const_iterator begin( ) const POUTRE_NOEXCEPT
        {
        return const_iterator(*this);
        }

      const_iterator end( ) const POUTRE_NOEXCEPT
        {
        //FIXME
        return const_iterator(*this);
        }

   };

   
/**
 * @brief  index is a type that represents an offset or a point in such space
 */
   template <int Rank> class index
   {
   static_assert(Rank > 0,
     "index requires a Rank>0");
   public:
      static /*constexpr*/ const int rank = Rank;

      using size_type = size_t;
      using value_type = ptrdiff_t;
      using const_value_type = std::add_const_t<value_type>;
      using pointer = std::add_pointer_t < value_type >;
      using reference = typename std::add_lvalue_reference < value_type >::type;
      using const_pointer = std::add_pointer_t < const_value_type >;
      using const_reference = typename std::add_lvalue_reference < const_value_type >::type;
      using difference_type = std::ptrdiff_t;
      using size_type = std::size_t;

      using array_storage = std::array < value_type, rank >;
      using self_type = index < rank >;

      array_storage m_size_list;

   public:
      /** @name Construction and Assignment
       */

      /**@{*/ 
     POUTRE_CONSTEXPR index( ) POUTRE_NOEXCEPT : m_size_list( )
      {
      }

      template <int Rank, typename std::enable_if<Rank == 1, void>::type* = nullptr>
      POUTRE_CONSTEXPR index(const value_type& a)  POUTRE_NOEXCEPT
      {
         m_size_list.assign(a);
      }

     template <int Rank, typename std::enable_if<Rank == 2, void>::type* = nullptr>
      POUTRE_CONSTEXPR index(const value_type& a0, const value_type& a1)  POUTRE_NOEXCEPT
       {
       m_size_list[0] = a0;
       m_size_list[1] = a1;
       }

     template <int Rank, typename std::enable_if<Rank == 3, void>::type* = nullptr>
       POUTRE_CONSTEXPR index(const value_type& a0, const value_type& a1, const value_type& a2)  POUTRE_NOEXCEPT
       {
        m_size_list[0] = a0;
        m_size_list[1] = a1;
        m_size_list[2] = a2;
       }

      POUTRE_CONSTEXPR index(const std::initializer_list<value_type>& rhs) POUTRE_NOEXCEPT
      {
      //!FIXME add check on rhs size for non c++14 constexpr
      #if !defined(BOOST_NO_CXX14_CONSTEXPR)
         static_assert(rhs.size( ) == Rank, "Ill formed initializer list");
      #endif 
         //details::helper_assign_container_op<self_type, AssignOpType::AssignOp, Rank>::op(rhs, *this);
         //safe but silly behavior if no static assertion
         std::copy_n(rhs.begin( ), std::min<ptrdiff_t>((ptrdiff_t)rank, rhs.size( )), m_size_list.begin( ));
      }

      POUTRE_CXX14_CONSTEXPR explicit index(const value_type(&rhs)[Rank]) POUTRE_NOEXCEPT
        {
        details::helper_assign_container_op<self_type, AssignOpType::AssignOp, Rank>::op(rhs, *this);
        }

      POUTRE_CONSTEXPR index(const self_type& rhs) POUTRE_NOEXCEPT
      {
      details::helper_assign_container_op<self_type, AssignOpType::AssignOp, Rank>::op(rhs, *this);
      }

      POUTRE_CXX14_CONSTEXPR self_type& operator=(const self_type &rhs) POUTRE_NOEXCEPT
      {
         if (this != &rhs)
         {
           details::helper_assign_container_op<self_type, AssignOpType::AssignOp, Rank>::op(rhs, *this);
         }
         return *this;
      }

      //is this really needed ?
      index(self_type&& rhs) POUTRE_NOEXCEPT
      {
         *this = std::move(rhs);
      }

      //is this really needed ?
      self_type& operator= (self_type&& rhs) POUTRE_NOEXCEPT
      {
         if (this != &rhs) // ?? http://scottmeyers.blogspot.fr/2014/06/the-drawbacks-of-implementing-move.html
         {
            m_size_list = std::move(rhs.m_size_list);
         }
         return *this;
      }
      /**@}*/


      /** @name Accessing Components
       */
      /**@{*/   
      POUTRE_ALWAYS_INLINE POUTRE_CXX14_CONSTEXPR reference operator[] (size_type n) POUTRE_NOEXCEPT
      {
        /* POUTRE_ASSERTCHECK(n >= 0, "index operator[n] n must be in >=0");
         POUTRE_ASSERTCHECK(n < rank, "index operator[n] n must be in [0,m_numdims[");
         return m_size_list[n];*/
        #if !defined(BOOST_NO_CXX14_CONSTEXPR)
        static_assert(rhs.size( ) == Rank, "Ill formed initializer list");
        #endif 
        return m_size_list[n];
      }

      POUTRE_ALWAYS_INLINE POUTRE_CXX14_CONSTEXPR const_reference operator[] (size_type n) const POUTRE_NOEXCEPT
      {
         //POUTRE_ASSERTCHECK(n >= 0, "index operator[n] n must be in >=0");
         //POUTRE_ASSERTCHECK(n < rank, "index operator[n] n must be in [0,m_numdims[");
         //return m_size_list[n];
         #if !defined(BOOST_NO_CXX14_CONSTEXPR)
         static_assert(rhs.size( ) == Rank, "Ill formed initializer list");
         #endif 
         return m_size_list[n];
      }
      /**@}*/      


      /** @name Comparison Operators
      */
      /**@{*/
      POUTRE_CXX14_CONSTEXPR bool operator==(const self_type& rhs) const POUTRE_NOEXCEPT
        {
        return  details::helper_comp_equal_container_op<self_type, Rank>::op(rhs, *this);
        }
      POUTRE_CXX14_CONSTEXPR bool operator!=(const self_type& rhs) const POUTRE_NOEXCEPT
        {
        return  !details::helper_comp_equal_container_op<self_type, Rank>::op(rhs, *this);
        }
      /**@}*/
      

      friend std::ostream& operator<<(std::ostream& out, const index <rank>& rhs) POUTRE_NOEXCEPT
      {
         for (const auto& val : rhs.m_size_list)
         {
            out << val;
            out << ",";
         }
         return out;
      }
      
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
           (*this)[0]+=1;
           return *this;
           }

      template <int Rank, typename std::enable_if<Rank == 1, void>::type* = nullptr>
         POUTRE_CXX14_CONSTEXPR self_type  operator++(int)POUTRE_NOEXCEPT
             {
             self_type tmp(*this); //return a fresh new object
             tmp[0] += 1;
             return tmp;
             }

      template <int Rank, typename std::enable_if<Rank == 1, void>::type* = nullptr>
         POUTRE_CXX14_CONSTEXPR self_type& operator--() POUTRE_NOEXCEPT
               {
               (*this)[0]-=1;
               return *this;
               }

      template <int Rank, typename std::enable_if<Rank == 1, void>::type* = nullptr>
               POUTRE_CXX14_CONSTEXPR self_type  operator--(int)POUTRE_NOEXCEPT
                 {
                 self_type tmp(*this); //return a fresh new object
                 tmp[0] -= 1;
                 return tmp;
                 }
 
                   
                   POUTRE_CXX14_CONSTEXPR self_type operator+() const POUTRE_NOEXCEPT
                   {
                   return *this;
                   }

                     POUTRE_CXX14_CONSTEXPR self_type operator-() const POUTRE_NOEXCEPT
                     {
                     auto tmp = *this; //return a fresh new object
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

      /** @name Scaling Operators
       */
       /**@{*/  
       

      template <typename ArithmeticType, typename std::enable_if<std::is_arithmetic<ArithmeticType>::value>::type* = nullptr>
                     POUTRE_CXX14_CONSTEXPR self_type  operator*(ArithmeticType v) const POUTRE_NOEXCEPT
      {
         self_type tmp(*this); //return a fresh new object
         details::helper_assign_container_valueop<self_type, AssignOpType::AssignOpMul, Rank>::op(v, tmp);
         return tmp;
      }

      template <typename ArithmeticType, typename std::enable_if<std::is_arithmetic<ArithmeticType>::value>::type* = nullptr>
      POUTRE_CXX14_CONSTEXPR self_type  operator/(ArithmeticType v) const POUTRE_NOEXCEPT
      {
         self_type tmp(*this); //return a fresh new object
         details::helper_assign_container_valueop<self_type, AssignOpType::AssignOpDiv, Rank>::op(v, tmp);
         return tmp;
      }

      template <typename ArithmeticType, typename std::enable_if<std::is_arithmetic<ArithmeticType>::value>::type* = nullptr>
      POUTRE_CXX14_CONSTEXPR self_type& operator*=(ArithmeticType v) POUTRE_NOEXCEPT
      {
         details::helper_assign_container_valueop<self_type, AssignOpType::AssignOpMul, Rank>::op(v, *this);
         return *this;
      }
      template <typename ArithmeticType, typename std::enable_if<std::is_arithmetic<ArithmeticType>::value>::type* = nullptr>
      POUTRE_CXX14_CONSTEXPR self_type& operator/=(ArithmeticType v) POUTRE_NOEXCEPT
      {
         details::helper_assign_container_valueop<self_type, AssignOpType::AssignOpDiv, Rank>::op(v, *this);
         return *this;
      }
      /**@}*/
   };

     template <int rank>
     POUTRE_CXX14_CONSTEXPR bounds<rank>  operator+(const index<rank>& lhs, const bounds<rank>& rhs) POUTRE_NOEXCEPT
     {
     bounds<rank> tmp(rhs);
     return (tmp + lhs);
     }

     template <int rank,typename ArithmeticType, typename std::enable_if<std::is_arithmetic<ArithmeticType>::value>::type* = nullptr>
     POUTRE_CXX14_CONSTEXPR bounds<rank> operator*(ArithmeticType v, const bounds<rank>& rhs) POUTRE_NOEXCEPT
     {
     bounds<rank> tmp(rhs);
     return (tmp*v);
     }

    template <int rank,typename ArithmeticType, typename std::enable_if<std::is_arithmetic<ArithmeticType>::value>::type* = nullptr>
     POUTRE_CXX14_CONSTEXPR index<rank>  operator*(ArithmeticType v, const index<rank>& rhs) POUTRE_NOEXCEPT
      {
      index<rank> tmp(rhs);
      return (tmp*v);
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
      class bounds_iterator : public std::iterator < std::random_access_iterator_tag,
        const index<Rank>,
        std::ptrdiff_t,
        const index<Rank>*,
        const index<Rank>& >
        {
        static_assert(Rank > 0,
          "bounds_iterator requires a Rank>0");
        public:
          using parent = std::iterator < std::random_access_iterator_tag, const index<Rank>, std::ptrdiff_t, const index<Rank>*, const index<Rank>&>;
          using self_type = bounds_iterator < Rank >;

        protected:
          bounds<Rank> m_bounds;
          bounds<Rank> m_bounds_orig;
          index<Rank>  m_currentidx;
          ptrdiff_t current_subrank;

        public:
          explicit bounds_iterator(bounds<Rank> bnd, index<Rank> curr = index < Rank > {}) POUTRE_NOEXCEPT : m_bounds(bnd), m_bounds_orig(bnd), m_currentidx(curr)
            {
            //The precondition is that bnd.contains(curr) unless bnd.size() = 0.
            if (m_bounds.size( ) != 0 && m_bounds.contains(m_currentidx))
              {
              m_bounds += m_currentidx;
              }
            else
              {//invalidate bounds 
              m_bounds = bounds < Rank >();
              }
            }

          bounds_iterator(const self_type& rhs) = default;
          self_type& operator=(const self_type& rhs) = default;

          bounds_iterator(self_type &&) POUTRE_NOEXCEPT
            {
            *this = std::move(rhs);
            }

            self_type& operator=(self_type &&)POUTRE_NOEXCEPT
            {
            if (this != &rhs) // ?? http://scottmeyers.blogspot.fr/2014/06/the-drawbacks-of-implementing-move.html
              {
              m_bounds_orig = std::move(rhs.m_bounds_orig);
              m_bounds = std::move(rhs.m_bounds);
              m_currentidx = std::move(rhs.m_currentidx);
              }
            return *this;
            }

          ~bounds_iterator( ) {}

          operator bool( ) const
            {
            return (0!=m_bounds.size());
            }

          bool         operator==(const self_type& rhs) const POUTRE_NOEXCEPT{ return ((m_bounds_orig == rhs.m_bounds_orig) && (m_bounds == rhs.m_bounds) && (m_currentidx == rhs.m_currentidx)); }
          bool         operator!=(const self_type& rhs) const POUTRE_NOEXCEPT{ return ((m_bounds_orig != rhs.m_bounds_orig) || (m_bounds != rhs.m_bounds) || (m_currentidx != rhs.m_currentidx)); }

          //self_type&                  operator+=(const difference_type& movement) POUTRE_NOEXCEPT{ m_ptr += movement; return (*this); }
          //self_type&                  operator-=(const difference_type& movement) POUTRE_NOEXCEPT{ m_ptr -= movement; return (*this); }


          //self_type&                  operator++() POUTRE_NOEXCEPT{ ++m_ptr; return (*this); }
          //self_type&                  operator--() POUTRE_NOEXCEPT{ --m_ptr; return (*this); }

          //self_type                   operator++(int)POUTRE_NOEXCEPT{ auto temp(*this); ++m_ptr; return temp; }
          //self_type                   operator--(int)POUTRE_NOEXCEPT{ auto temp(*this); --m_ptr; return temp; }

          //difference_type             operator-(const self_type& rhs) POUTRE_NOEXCEPT { return std::distance( ); }

          //!Note here a return by value
          value_type                  operator*() POUTRE_NOEXCEPT{ return ; }

          //!Note here a return by value
          value_type                  operator*() const  POUTRE_NOEXCEPT{ return ; }


          //const offset                              getOffset( ) const POUTRE_NOEXCEPT { return m_ptr - m_ptrorig; }

        private:

          void increment(index<Rank>& idx)
            {
            while (--current_rank >= 0)
              {
              if (++idx[current_rank] < m_bounds[current_rank])
                break;	// increment done, quit
              else
                idx[current_rank] = 0;	// carry to more-significant 
              }
            }
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

//! @} doxygroup: multidimensionnale_group   

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


}

#ifdef _MSC_VER
#pragma warning( pop ) 
#endif

#endif //POUTREMULTIDIMBOUNDINDEX_HPP__
