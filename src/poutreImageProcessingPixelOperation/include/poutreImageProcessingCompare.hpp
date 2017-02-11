//==============================================================================
//                  Copyright (c) 2015 - Thomas Retornaz                      //
//                     thomas.retornaz@mines-paris.org                        //
//          Distributed under the Boost Software License, Version 1.0.        //
//                 See accompanying file LICENSE.txt or copy at               //
//                     http://www.boost.org/LICENSE_1_0.txt                   //
//==============================================================================

#ifndef POUTRE_IMAGEPROCESSING_COMPARE_HPP__
#define POUTRE_IMAGEPROCESSING_COMPARE_HPP__

#ifndef POUTRE_IMAGEPROCESSINGCORE_HPP__
#include <poutreImageProcessingCore/poutreImageProcessingCore.hpp>
#endif

#ifndef POUTRE_IMAGEPROCESSING_TYPE_HPP__
#include <poutreImageProcessingCore/poutreImageProcessingType.hpp>
#endif

#ifndef POUTRE_TRACE__HPP__
#include <poutreBase/poutreTrace.hpp>
#endif

#ifndef POUTRE_CONTAINER_VIEW_HPP__
#include <poutreBase/poutreContainerView.hpp>
#endif

#include <poutreImageProcessingPixelOperation/include/poutreImageProcessingUnaryOp.hpp>
#include <poutreImageProcessingPixelOperation/include/poutreImageProcessingBinaryOp.hpp>
#include <poutreImageProcessingPixelOperation/include/poutreImageProcessingTernaryOp.hpp>
#include <poutreImageProcessingPixelOperation/include/poutreImageProcessingQuaternaryOp.hpp>



namespace poutre
{
    /******************************************Operator base ******************************************************/
    //!Operator equal
    template< typename T1>
    struct OpCompEqualValue
    {
        T1 m_val;
        OpCompEqualValue(T1 ival) :m_val(ival) {}
        POUTRE_ALWAYS_INLINE POUTRE_CXX14_CONSTEXPR bool operator()(T1 lhs) const POUTRE_NOEXCEPT
        {
            return  lhs == m_val;
        }
    };

    //!Operator diff
    template< typename T1>
    struct OpCompDiffValue
    {
        T1 m_val;
        OpCompDiffValue(T1 ival) :m_val(ival) {}
        POUTRE_ALWAYS_INLINE POUTRE_CXX14_CONSTEXPR bool operator()(T1 lhs) const POUTRE_NOEXCEPT
        {
            return  lhs != m_val;
        }
    };

    //!Operator Sup
    template< typename T1>
    struct OpCompSupValue
    {
        T1 m_val;
        OpCompSupValue(T1 ival) :m_val(ival) {}
        POUTRE_ALWAYS_INLINE POUTRE_CXX14_CONSTEXPR bool operator()(T1 lhs) const POUTRE_NOEXCEPT
        {
            return  lhs > m_val;
        }
    };

    //!Operator SupEqual
    template< typename T1>
    struct OpCompSupEqualValue
    {
        T1 m_val;
        OpCompSupEqualValue(T1 ival) :m_val(ival) {}
        POUTRE_ALWAYS_INLINE POUTRE_CXX14_CONSTEXPR bool operator()(T1 lhs) const POUTRE_NOEXCEPT
        {
            return lhs >= m_val;
        }
    };

    //!Operator Inf
    template< typename T1>
    struct OpCompInfValue
    {
        T1 m_val;
        OpCompInfValue(T1 ival) :m_val(ival) {}
        POUTRE_ALWAYS_INLINE POUTRE_CXX14_CONSTEXPR bool operator()(T1 lhs) const POUTRE_NOEXCEPT
        {
            return lhs < m_val;
        }
    };

    //!Operator InfEqual
    template<typename T1>
    struct OpCompInfEqualValue
    {
        T1 m_val;
        OpCompInfEqualValue(T1 ival) :m_val(ival) {}
        POUTRE_ALWAYS_INLINE POUTRE_CXX14_CONSTEXPR bool operator()(T1 lhs) const POUTRE_NOEXCEPT
        {
            return lhs <= m_val;
        }
    };


    template <typename Tin, typename Tout, class CompareOp>
    struct compare_sss
    {
        const CompareOp  cop;
        const Tout m_valtrue, m_valfalse;
        using param_type = typename boost::call_traits<Tin>::param_type;
        compare_sss(const Tout& i_valtrue, const Tout& i_valfalse) : cop(), m_valtrue(i_valtrue), m_valfalse(i_valfalse) {}
        compare_sss(const Tout& i_valtrue, const Tout& i_valfalse, const CompareOp& op) : cop(op), m_valtrue(i_valtrue), m_valfalse(i_valfalse) {}
        const Tout operator()(param_type i_val) const POUTRE_NOEXCEPT //TODO optimize return type ?
        {
            return (cop(i_val) ? m_valtrue : m_valfalse);
        }

    };

    template<typename Tin, typename Tout, ptrdiff_t Rank, template <typename, ptrdiff_t> class View1, class Op, template <typename, ptrdiff_t> class View2>
    void t_ViewCompare_sss(
        const View1<Tin, Rank>& i_vin,
        Op i_op,
        typename boost::call_traits<Tout>::param_type i_valtrue,
        typename boost::call_traits<Tout>::param_type i_valfalse,
        View2<Tout, Rank>& o_vout)
    {
        using myop = compare_sss<Tin, Tout, Op>;
        myop op(i_valtrue, i_valfalse, i_op);
        PixelWiseUnaryOp(i_vin, op, o_vout);
    }

    template<typename Tin, typename Tout, ptrdiff_t Rank, template <typename, ptrdiff_t> class View1, template <typename, ptrdiff_t> class View2>
    void ViewCompare_sss_dispatch(
        const View1<Tin, Rank>& i_vin,
        CompOpType compOpType,
        typename boost::call_traits<Tin>::param_type i_compval,
        typename boost::call_traits<Tout>::param_type i_valtrue,
        typename boost::call_traits<Tout>::param_type i_valfalse,
        View2<Tout, Rank>& o_vout)
    {
        switch (compOpType)
        {
        case CompOpType::CompOpEqual:
        {
            OpCompEqualValue<Tin> myop(i_compval);
            return t_ViewCompare_sss(i_vin, myop, i_valtrue, i_valfalse, o_vout);
        }break;
        case CompOpType::CompOpDiff:
        {
        }break;
        case CompOpType::CompOpSup:
        {
        }break;
        case CompOpType::CompOpSupEqual:
        {
        }break;
        case CompOpType::CompOpInf:
        {
        }break;
        case CompOpType::CompOpInfEqual:
        {

        }break;
        default:
        {
            POUTRE_RUNTIME_ERROR("ViewCompare_sss_dispatch CompOpType not implemented");
        }
        }
    }
}
#endif //POUTRE_IMAGEPROCESSING_COMPARE_HPP__