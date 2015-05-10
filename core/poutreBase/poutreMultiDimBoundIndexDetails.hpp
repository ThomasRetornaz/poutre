#ifndef POUTREMULTIDIMBOUNDINDEX_DETAILS_HPP__
#define POUTREMULTIDIMBOUNDINDEX_DETAILS_HPP__

/**
 * @file   poutreMultiDimBoundIndexDetails.hpp
 * @author Thomas Retornaz
 * @brief  
 * 
 * 
 */

namespace poutre
{

namespace details
  {
  //unrolling helper for small rank eg from 1 to 4
  //All of this may be over engineered depends on stl implementation and compiler capabilities
  //BUT for MSVC2013 you could have a look on std::array implementation, no special optimisation seems present for low rank 
  //NOTE c++14 we need return void see http://en.cppreference.com/w/cpp/language/constexpr 
  //NOTE c++14 we need return assignment see http://en.cppreference.com/w/cpp/language/constexpr
  //Those specialization and design was inspired by ms amp tricks

  template <typename value_type, ArithOpType op>
  struct helper_arith_op {};

  //todo macro ?
  template <typename value_type>
  struct helper_arith_op<value_type, ArithOpType::ArithOpPlus>
    {   
    POUTRE_ALWAYS_INLINE POUTRE_STATIC_CONSTEXPR void op(const value_type &A0, const value_type&A1, value_type &res) POUTRE_NOEXCEPT
      {
       res=(A0 + A1);
      }
    };

  template <typename value_type>
  struct helper_arith_op<value_type, ArithOpType::ArithOpMinus>
    {
    POUTRE_ALWAYS_INLINE POUTRE_STATIC_CONSTEXPR void op(const value_type &A0, const  value_type&A1, value_type &res) POUTRE_NOEXCEPT
      {
       res=(A0 - A1);
      }
    };

  template <typename value_type>
  struct helper_arith_op<value_type, ArithOpType::ArithOpMul>
    {
    POUTRE_ALWAYS_INLINE POUTRE_STATIC_CONSTEXPR void op(const value_type &A0, const value_type&A1, value_type &res) POUTRE_NOEXCEPT
      {
      res=(A0 * A1);
      }
    };

  template <typename value_type>
  struct helper_arith_op<value_type, ArithOpType::ArithOpDiv>
    {
    POUTRE_ALWAYS_INLINE POUTRE_STATIC_CONSTEXPR void op(const  value_type &A0, const  value_type&A1, value_type &res) POUTRE_NOEXCEPT      
      {
      res=(A0 / A1);
      }
    };



  template <typename value_type, AssignOpType op>
  struct helper_assign_op {};

  //todo macro ?
  template <typename value_type>
  struct helper_assign_op<value_type, AssignOpType::AssignOp>
    {
    POUTRE_ALWAYS_INLINE POUTRE_STATIC_CONSTEXPR void op(const value_type &A0, value_type &result) POUTRE_NOEXCEPT
      {
      result = A0;
      }
    };

  template <typename value_type>
  struct helper_assign_op<value_type, AssignOpType::AssignOpAdd>
    {
      POUTRE_ALWAYS_INLINE POUTRE_STATIC_CONSTEXPR void op(const value_type &A0, value_type &result) POUTRE_NOEXCEPT
      {
      result += A0;
      }
    };

  template <typename value_type>
  struct helper_assign_op<value_type, AssignOpType::AssignOpMinus>
    {
      POUTRE_ALWAYS_INLINE POUTRE_STATIC_CONSTEXPR void op(const value_type &A0, value_type &result) POUTRE_NOEXCEPT
      {
      result -= A0;
      }
    };

  template <typename value_type>
  struct helper_assign_op<value_type, AssignOpType::AssignOpMul>
    {
    POUTRE_ALWAYS_INLINE POUTRE_STATIC_CONSTEXPR void op(const value_type &A0, value_type &result) POUTRE_NOEXCEPT
      {
      result *= A0;
      }
    };

  template <typename value_type>
  struct helper_assign_op<value_type, AssignOpType::AssignOpDiv>
    {
      POUTRE_ALWAYS_INLINE POUTRE_STATIC_CONSTEXPR void op(const value_type &A0, value_type &result) POUTRE_NOEXCEPT
      {
      result /= A0
      }
    };

  template <typename value_type>
  struct helper_assign_op<value_type, AssignOpType::AssignOpMod>
    {
    POUTRE_ALWAYS_INLINE POUTRE_STATIC_CONSTEXPR void op(const value_type &A0, value_type &result) POUTRE_NOEXCEPT
      {
      result %= A0
      }
    };

  
  template <typename value_type, CompOpType op>
  struct helper_comp_op {};


  //todo macro ?
  template <typename value_type>
  struct helper_comp_op<value_type, CompOpType::CompOpEqual>
    {
    POUTRE_ALWAYS_INLINE POUTRE_STATIC_CONSTEXPR bool op(const value_type &A0, const value_type &A1) POUTRE_NOEXCEPT
      {
      return (A0==A1);
      }
    };

  template <typename value_type>
  struct helper_comp_op<value_type, CompOpType::CompOpDiff>
    {
    POUTRE_ALWAYS_INLINE POUTRE_STATIC_CONSTEXPR bool op(const value_type &A0, const value_type &A1) POUTRE_NOEXCEPT
      {
      return (A0 != A1);
      }
    };

  template <typename value_type>
  struct helper_comp_op<value_type, CompOpType::CompOpSup>
    {
    POUTRE_ALWAYS_INLINE POUTRE_STATIC_CONSTEXPR bool op(const value_type &A0, const value_type &A1) POUTRE_NOEXCEPT
      {
      return (A0 > A1);
      }
    };

  template <typename value_type>
  struct helper_comp_op<value_type, CompOpType::CompOpSupEqual>
    {
    POUTRE_ALWAYS_INLINE POUTRE_STATIC_CONSTEXPR bool op(const value_type &A0, const value_type &A1) POUTRE_NOEXCEPT
      {
      return (A0 >= A1);
      }
    };

  template <typename value_type>
  struct helper_comp_op<value_type, CompOpType::CompOpInfEqual>
    {
     POUTRE_ALWAYS_INLINE POUTRE_STATIC_CONSTEXPR bool op(const value_type &A0, const value_type &A1) POUTRE_NOEXCEPT
      {
      return (A0 <= A1);
      }
    };

  
  template <typename value_type, int Rank, class container, ArithOpType>
  struct helper_arith_container_op
    {
    POUTRE_ALWAYS_INLINE POUTRE_STATIC_CONSTEXPR void op(container const & A0, container const & A1, container &res) POUTRE_NOEXCEPT
      {
      for (size_t i = 0; i < rank; i++) {helper_comp_op<value_type, ArithOpType>(A0[i], A1[i], res[i]);}
      }
    };


  /*template <typename value_type, int Rank, class container<value_type, Rank>, CompOpType>
  struct helper_comp_container_op
    {
    static POUTRE_CONSTEXPR bool op(container const & A0, container const & A1) POUTRE_NOEXCEPT
      { 
      for (size_t i = 0; i < rank; i++) {helper_comp_op<value_type, ArithOpType>(A0[i], A1[i], res[i]);}
      }
    };
  */

  //with proper support of constexpr may above specialization are unnecessary
  template <typename value_type,class container, ArithOpType op>
  struct helper_arith_container_op<value_type,1,container,op>
    {
    POUTRE_ALWAYS_INLINE POUTRE_STATIC_CONSTEXPR void op(container const & A0, container const & A1, container &res) POUTRE_NOEXCEPT
      {
      helper_arith_op<value_type, op>(A0[0], A1[0], res[0]);
      }
    };

  template <typename value_type, class container, ArithOpType op>
  struct helper_arith_container_op<value_type, 2, container, op>
    {
    POUTRE_ALWAYS_INLINE POUTRE_STATIC_CONSTEXPR void op(container const & A0, container const & A1, container &res) POUTRE_NOEXCEPT
      {
       helper_arith_op<value_type, op>(A0[0], A1[0], res[0]);
       helper_arith_op<value_type, op>(A0[1], A1[1], res[1]);
      }
    };

  template <typename value_type, class container, ArithOpType op>
  struct helper_arith_container_op<value_type, 3, container, op>
    {
    POUTRE_ALWAYS_INLINE POUTRE_STATIC_CONSTEXPR void op(container const & A0, container const & A1, container &res) POUTRE_NOEXCEPT
      {
      helper_arith_op<value_type, op>(A0[0], A1[0],res[0]);
      helper_arith_op<value_type, op>(A0[1], A1[1],res[1]);
      helper_arith_op<value_type, op>(A0[2], A1[2],res[2]);
      }
    };

  template <typename value_type, class container, ArithOpType op>
  struct helper_arith_container_op<value_type, 4, container, op>
    {
    POUTRE_ALWAYS_INLINE POUTRE_STATIC_CONSTEXPR void op(container const & A0, container const & A1, container &res) POUTRE_NOEXCEPT
      {
      helper_arith_op<value_type, op>(A0[0], A1[0], res[0]);
      helper_arith_op<value_type, op>(A0[1], A1[1], res[1]);
      helper_arith_op<value_type, op>(A0[2], A1[2], res[2]);
      helper_arith_op<value_type, op>(A0[3], A1[3], res[3]);
      }
    };

 /* template <typename value_type, int Rank, class container<value_type, Rank>, CompOpType op>
  struct helper_comp_container_op
    {
    static POUTRE_ALWAYS_INLINE bool op(container const & A0, container const & A1) POUTRE_NOEXCEPT
      {
      for (size_t i = 0; i < Rank; i++) { helper_comp_op<value_type, op>(A0[i], A1[i], res[i]); }
      }
    }; */


  template <typename value_type, int Rank, class container, AssignOpType op>
  struct helper_assign_container_op
    {
    POUTRE_ALWAYS_INLINE POUTRE_STATIC_CONSTEXPR void op(container const & A0, container const & A1, container &res) POUTRE_NOEXCEPT
      {
      for (size_t i = 0; i < Rank; i++)
        {
        res[i] = helper_assign_op<value_type, op>(A0[i], A1[i]);
        }
      }
    };

  //with proper support of constexpr may above specialization are unnecessary
  template <typename value_type, class container, AssignOpType op>
  struct helper_assign_container_op<value_type, 1, container, op>
    {
    POUTRE_ALWAYS_INLINE POUTRE_STATIC_CONSTEXPR void op(container const & A0, container const & A1, container &res) POUTRE_NOEXCEPT
      {
      helper_assing_op<value_type, op>(A0[0], A1[0], res[0]);
      }
    };

  template <typename value_type, class container, AssignOpType op>
  struct helper_assign_container_op<value_type, 2, container, op>
    {
    POUTRE_ALWAYS_INLINE POUTRE_STATIC_CONSTEXPR void op(container const & A0, container const & A1, container &res) POUTRE_NOEXCEPT
      {
      helper_assing_op<value_type, op>(A0[0], A1[0], res[0]);
      helper_assing_op<value_type, op>(A0[1], A1[1], res[1]);
      }
    };

  template <typename value_type, class container, AssignOpType op>
  struct helper_assign_container_op<value_type, 3, container, op>
    {
    POUTRE_ALWAYS_INLINE POUTRE_STATIC_CONSTEXPR void op(container const & A0, container const & A1, container &res) POUTRE_NOEXCEPT
      {
      helper_assing_op<value_type, op>(A0[0], A1[0], res[0]);
      helper_assing_op<value_type, op>(A0[1], A1[1], res[1]);
      helper_assing_op<value_type, op>(A0[2], A1[2], res[2]);
      }
    };

  template <typename value_type, class container, AssignOpType op>
  struct helper_assign_container_op<value_type, 4, container, op>
    {
    POUTRE_ALWAYS_INLINE POUTRE_STATIC_CONSTEXPR void op(container const & A0, container const & A1, container &res) POUTRE_NOEXCEPT
      {
      helper_assing_op<value_type, op>(A0[0], A1[0], res[0]);
      helper_assing_op<value_type, op>(A0[1], A1[1], res[1]);
      helper_assing_op<value_type, op>(A0[2], A1[2], res[2]);
      helper_assing_op<value_type, op>(A0[3], A1[3], res[3]);
      }
    };
    
  }//namespace details
   
   

}


#endif //POUTREMULTIDIMBOUNDINDEX_DETAILS_HPP__
