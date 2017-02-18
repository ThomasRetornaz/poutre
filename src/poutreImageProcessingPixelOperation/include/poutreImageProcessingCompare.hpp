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

    /*************************************************************************************************************************************/
    /*                                                      SSS                                                                          */
    /*************************************************************************************************************************************/
    template <typename Tin, typename Tout, class CompareOp>
    struct compare_sss
    {
        const CompareOp  cop;
        const Tout m_valtrue, m_valfalse;
        using param_type = typename boost::call_traits<Tin>::param_type;
        compare_sss(const Tout& i_valtrue, const Tout& i_valfalse) : cop(), m_valtrue(i_valtrue), m_valfalse(i_valfalse) {}
        compare_sss(const Tout& i_valtrue, const Tout& i_valfalse, const CompareOp& op) : cop(op), m_valtrue(i_valtrue), m_valfalse(i_valfalse) {}
        const Tout& operator()(param_type i_val) const POUTRE_NOEXCEPT //TODO inline ?
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
            OpCompDiffValue<Tin> myop(i_compval);
            return t_ViewCompare_sss(i_vin, myop, i_valtrue, i_valfalse, o_vout);
        }break;
        case CompOpType::CompOpSup:
        {
            OpCompSupValue<Tin> myop(i_compval);
            return t_ViewCompare_sss(i_vin, myop, i_valtrue, i_valfalse, o_vout);
        }break;
        case CompOpType::CompOpSupEqual:
        {
            OpCompSupEqualValue<Tin> myop(i_compval);
            return t_ViewCompare_sss(i_vin, myop, i_valtrue, i_valfalse, o_vout);
        }break;
        case CompOpType::CompOpInf:
        {
            OpCompInfValue<Tin> myop(i_compval);
            return t_ViewCompare_sss(i_vin, myop, i_valtrue, i_valfalse, o_vout);
        }break;
        case CompOpType::CompOpInfEqual:
        {
            OpCompInfEqualValue<Tin> myop(i_compval);
            return t_ViewCompare_sss(i_vin, myop, i_valtrue, i_valfalse, o_vout);
        }break;
        default:
        {
            POUTRE_RUNTIME_ERROR("ViewCompare_sss_dispatch CompOpType not implemented");
        }
        }
    }
    /*************************************************************************************************************************************/
    /*                                                      III                                                                          */
    /*************************************************************************************************************************************/
    template <typename Tin, typename TComp, typename Ttrue, typename Tfalse, typename Tout, class CompareOp>
    struct compare_iii
    {
        const CompareOp  cop;
        using param_typein = typename boost::call_traits<Tin>::param_type;
        using param_typecomp = typename boost::call_traits<TComp>::param_type;
        using param_typetrue = typename boost::call_traits<Ttrue>::param_type;
        using param_typefalse = typename boost::call_traits<Tfalse>::param_type;
        compare_iii() : cop() {}
        compare_iii(const CompareOp& op) : cop(op) {}
        Tout operator()(param_typein i_val, param_typecomp i_comp, param_typetrue i_true, param_typetrue i_false) const POUTRE_NOEXCEPT //TODO inline ?
        {
            return (cop(i_val, i_comp) ? i_true : i_false);
        }

    };
    template<typename Tin, typename Tcomp, typename Ttrue, typename Tfalse, typename Tout, ptrdiff_t Rank,
        template <typename, ptrdiff_t> class ViewIn,
        template <typename, ptrdiff_t> class ViewComp,
        template <typename, ptrdiff_t> class ViewTrue,
        template <typename, ptrdiff_t> class ViewFalse,
        class Op,
        template <typename, ptrdiff_t> class ViewOut>
    void t_ViewCompare_iii(
        const ViewIn<Tin, Rank>& i_vin,
        Op i_op,
        const ViewComp<Tcomp, Rank>& i_vcomp,
        const ViewTrue<Ttrue, Rank>& i_vtrue,
        const ViewFalse<Tfalse, Rank>& i_vfalse,
        ViewOut<Tout, Rank>& o_vout)
    {
        using myop = compare_iii<Tin, Tcomp, Ttrue, Tfalse, Tout, Op>;
        myop op(i_op);
        PixelWiseQuaternaryOp(i_vin, op, i_vcomp, i_vtrue, i_vfalse, o_vout);
    }

    template<typename Tin, typename Tcomp, typename Ttrue, typename Tfalse, typename Tout, ptrdiff_t Rank, template <typename, ptrdiff_t> class ViewIn, template <typename, ptrdiff_t> class ViewComp, template <typename, ptrdiff_t> class ViewTrue, template <typename, ptrdiff_t> class ViewFalse, template <typename, ptrdiff_t> class ViewOut>
    void ViewCompare_iii_dispatch(
        const ViewIn<Tin, Rank>& i_vin,
        CompOpType compOpType,
        const ViewComp<Tcomp, Rank>& i_vcomp,
        const ViewTrue<Ttrue, Rank>& i_vtrue,
        const ViewFalse<Tfalse, Rank>& i_vfalse,
        ViewOut<Tout, Rank>& o_vout)
    {
        static_assert(std::is_convertible<Ttrue, Tout>(), "ViewCompare_iii_dispatch Ttrue and Tout must be convertible to each other");
        static_assert(std::is_convertible<Tfalse, Tout>(), "ViewCompare_iii_dispatch Tfalse and Tout must be convertible to each other");
        static_assert(std::is_convertible<Tcomp, Tin>(), "ViewCompare_iii_dispatch Tcomp and Tin must be convertible to each other");
        switch (compOpType)
        {
        case CompOpType::CompOpEqual:
        {
            return t_ViewCompare_iii(i_vin, std::equal_to<Tin>(), i_vcomp, i_vtrue, i_vfalse, o_vout);
        }break;
        case CompOpType::CompOpDiff:
        {
            return t_ViewCompare_iii(i_vin, std::not_equal_to<Tin>(), i_vcomp, i_vtrue, i_vfalse, o_vout);
        }break;
        case CompOpType::CompOpSup:
        {
            return t_ViewCompare_iii(i_vin, std::greater<Tin>(), i_vcomp, i_vtrue, i_vfalse, o_vout);
        }break;
        case CompOpType::CompOpSupEqual:
        {
            return t_ViewCompare_iii(i_vin, std::greater_equal<Tin>(), i_vcomp, i_vtrue, i_vfalse, o_vout);
        }break;
        case CompOpType::CompOpInf:
        {
            return t_ViewCompare_iii(i_vin, std::less<Tin>(), i_vcomp, i_vtrue, i_vfalse, o_vout);
        }break;
        case CompOpType::CompOpInfEqual:
        {
            return t_ViewCompare_iii(i_vin, std::less_equal<Tin>(), i_vcomp, i_vtrue, i_vfalse, o_vout);
        }break;
        default:
        {
            POUTRE_RUNTIME_ERROR("ViewCompare_iii_dispatch CompOpType not implemented");
        }
        }
    }

    /*************************************************************************************************************************************/
    /*                                                      SII                                                                          */
    /*************************************************************************************************************************************/
    template <typename Tin, typename Ttrue, typename Tfalse, typename Tout, class CompareOp>
    struct compare_sii
    {
        const CompareOp  cop;
        const Tin m_compval;

        using param_typein = typename boost::call_traits<Tin>::param_type;
        using param_typetrue = typename boost::call_traits<Ttrue>::param_type;
        using param_typefalse = typename boost::call_traits<Tfalse>::param_type;
        compare_sii(const Tin& i_compval) : cop(), m_compval(i_compval) {}
        compare_sii(const Tin& i_compval, const CompareOp& op) : cop(op), m_compval(i_compval) {}
        Tout operator()(param_typein i_val, param_typetrue i_true, param_typefalse i_false) const POUTRE_NOEXCEPT //TODO inline ?
        {
            return (cop(i_val, m_compval) ? i_true : i_false);
        }

    };
    template<typename Tin, typename Ttrue, typename Tfalse, typename Tout, ptrdiff_t Rank, template <typename, ptrdiff_t> class ViewIn, template <typename, ptrdiff_t> class ViewTrue, template <typename, ptrdiff_t> class ViewFalse, class Op, template <typename, ptrdiff_t> class ViewOut>
    void t_ViewCompare_sii(
        const ViewIn<Tin, Rank>& i_vin,
        Op i_op,
        typename boost::call_traits<Tin>::param_type i_compval,
        const ViewTrue<Ttrue, Rank>& i_vtrue,
        const ViewFalse<Tfalse, Rank>& i_vfalse,
        ViewOut<Tout, Rank>& o_vout)
    {
        using myop = compare_sii<Tin, Ttrue, Tfalse, Tout, Op>;
        myop op(i_compval, i_op);
        PixelWiseTernaryOp(i_vin, op, i_vtrue, i_vfalse, o_vout);
    }

    template<typename Tin, typename Ttrue, typename Tfalse, typename Tout, ptrdiff_t Rank, template <typename, ptrdiff_t> class ViewIn, template <typename, ptrdiff_t> class ViewTrue, template <typename, ptrdiff_t> class ViewFalse, template <typename, ptrdiff_t> class ViewOut>
    void ViewCompare_sii_dispatch(
        const ViewIn<Tin, Rank>& i_vin,
        CompOpType compOpType,
        typename boost::call_traits<Tin>::param_type i_compval,
        const ViewTrue<Ttrue, Rank>& i_vtrue,
        const ViewFalse<Tfalse, Rank>& i_vfalse,
        ViewOut<Tout, Rank>& o_vout)
    {
        static_assert(std::is_convertible<Ttrue, Tout>(), "ViewCompare_sii_dispatch Ttrue and Tout must be convertible to each other");
        static_assert(std::is_convertible<Tfalse, Tout>(), "ViewCompare_sii_dispatch Tfalse and Tout must be convertible to each other");
        switch (compOpType)
        {
        case CompOpType::CompOpEqual:
        {
            return t_ViewCompare_sii(i_vin, std::equal_to<Tin>(), i_compval, i_vtrue, i_vfalse, o_vout);
        }break;
        case CompOpType::CompOpDiff:
        {
            return t_ViewCompare_sii(i_vin, std::not_equal_to<Tin>(), i_compval, i_vtrue, i_vfalse, o_vout);
        }break;
        case CompOpType::CompOpSup:
        {
            return t_ViewCompare_sii(i_vin, std::greater<Tin>(), i_compval, i_vtrue, i_vfalse, o_vout);
        }break;
        case CompOpType::CompOpSupEqual:
        {
            return t_ViewCompare_sii(i_vin, std::greater_equal<Tin>(), i_compval, i_vtrue, i_vfalse, o_vout);
        }break;
        case CompOpType::CompOpInf:
        {
            return t_ViewCompare_sii(i_vin, std::less<Tin>(), i_compval, i_vtrue, i_vfalse, o_vout);
        }break;
        case CompOpType::CompOpInfEqual:
        {
            return t_ViewCompare_sii(i_vin, std::less_equal<Tin>(), i_compval, i_vtrue, i_vfalse, o_vout);
        }break;
        default:
        {
            POUTRE_RUNTIME_ERROR("ViewCompare_sis_dispatch CompOpType not implemented");
        }
        }
    }

    /*************************************************************************************************************************************/
    /*                                                      SIS                                                                          */
    /*************************************************************************************************************************************/
    template <typename Tin, typename Ttrue, typename Tout, class CompareOp>
    struct compare_sis
    {
        const CompareOp  cop;
        const Tin m_compval;
        const Tout m_valfalse;

        using param_typein = typename boost::call_traits<Tin>::param_type;
        using param_typetrue = typename boost::call_traits<Ttrue>::param_type;
        compare_sis(const Tin& i_compval, const Tout& i_valfalse) : cop(), m_compval(i_compval), m_valfalse(i_valfalse) {}
        compare_sis(const Tin& i_compval, const Tout& i_valfalse, const CompareOp& op) : cop(op), m_compval(i_compval), m_valfalse(i_valfalse) {}
        Tout operator()(param_typein i_val, param_typetrue i_true) const POUTRE_NOEXCEPT //TODO inline ?
        {
            return (cop(i_val, m_compval) ? i_true : m_valfalse);
        }

    };

    template<typename Tin, typename Ttrue, typename Tout, ptrdiff_t Rank, template <typename, ptrdiff_t> class ViewIn, template <typename, ptrdiff_t> class ViewTrue, class Op, template <typename, ptrdiff_t> class ViewOut>
    void t_ViewCompare_sis(
        const ViewIn<Tin, Rank>& i_vin,
        Op i_op,
        typename boost::call_traits<Tin>::param_type i_compval,
        const ViewTrue<Ttrue, Rank>& i_vtrue,
        typename boost::call_traits<Tout>::param_type i_valfalse,
        ViewOut<Tout, Rank>& o_vout)
    {
        using myop = compare_sis<Tin, Ttrue, Tout, Op>;
        myop op(i_compval, i_valfalse, i_op);
        PixelWiseBinaryOp(i_vin, op, i_vtrue, o_vout);
    }

    template<typename Tin, typename Ttrue, typename Tout, ptrdiff_t Rank, template <typename, ptrdiff_t> class ViewIn, template <typename, ptrdiff_t> class ViewTrue, template <typename, ptrdiff_t> class ViewOut>
    void ViewCompare_sis_dispatch(
        const ViewIn<Tin, Rank>& i_vin,
        CompOpType compOpType,
        typename boost::call_traits<Tin>::param_type i_compval,
        const ViewTrue<Ttrue, Rank>& i_vtrue,
        typename boost::call_traits<Tout>::param_type i_valfalse,
        ViewOut<Tout, Rank>& o_vout)
    {
        static_assert(std::is_convertible<Ttrue, Tout>(), "ViewCompare_sis_dispatch Ttrue and Tout must be convertible to each other");
        switch (compOpType)
        {
        case CompOpType::CompOpEqual:
        {
            return t_ViewCompare_sis(i_vin, std::equal_to<Tin>(), i_compval, i_vtrue, i_valfalse, o_vout);
        }break;
        case CompOpType::CompOpDiff:
        {
            return t_ViewCompare_sis(i_vin, std::not_equal_to<Tin>(), i_compval, i_vtrue, i_valfalse, o_vout);
        }break;
        case CompOpType::CompOpSup:
        {
            return t_ViewCompare_sis(i_vin, std::greater<Tin>(), i_compval, i_vtrue, i_valfalse, o_vout);
        }break;
        case CompOpType::CompOpSupEqual:
        {
            return t_ViewCompare_sis(i_vin, std::greater_equal<Tin>(), i_compval, i_vtrue, i_valfalse, o_vout);
        }break;
        case CompOpType::CompOpInf:
        {
            return t_ViewCompare_sis(i_vin, std::less<Tin>(), i_compval, i_vtrue, i_valfalse, o_vout);
        }break;
        case CompOpType::CompOpInfEqual:
        {
            return t_ViewCompare_sis(i_vin, std::less_equal<Tin>(), i_compval, i_vtrue, i_valfalse, o_vout);
        }break;
        default:
        {
            POUTRE_RUNTIME_ERROR("ViewCompare_sis_dispatch CompOpType not implemented");
        }
        }
    }

    /*************************************************************************************************************************************/
    /*                                                      ISI                                                                          */
    /*************************************************************************************************************************************/
    template <typename Tin, typename Tcomp, typename Tfalse, typename Tout, class CompareOp>
    struct compare_isi
    {
        const CompareOp  cop;
        const Tout m_valtrue;

        using param_typein = typename boost::call_traits<Tin>::param_type;
        using param_typecomp = typename boost::call_traits<Tcomp>::param_type;
        using param_typefalse = typename boost::call_traits<Tfalse>::param_type;
        compare_isi(const Tout& i_valtrue) : cop(), m_valtrue(i_valtrue) {}
        compare_isi(const Tout& i_valtrue, const CompareOp& op) : cop(op), m_valtrue(i_valtrue) {}
        Tout operator()(param_typein i_val, param_typecomp i_comp, param_typefalse i_false) const POUTRE_NOEXCEPT //TODO inline ?
        {
            return (cop(i_val, i_comp) ? m_valtrue : i_false);
        }

    };

    template<typename Tin, typename Tcomp, typename Tfalse, typename Tout, ptrdiff_t Rank, template <typename, ptrdiff_t> class ViewIn, template <typename, ptrdiff_t> class ViewComp, template <typename, ptrdiff_t> class ViewFalse, class Op, template <typename, ptrdiff_t> class ViewOut>
    void t_ViewCompare_isi(
        const ViewIn<Tin, Rank>& i_vin,
        Op i_op,
        const ViewComp<Tcomp, Rank>& i_vcomp,
        typename boost::call_traits<Tout>::param_type i_valtrue,
        const ViewFalse<Tfalse, Rank>& i_vfalse,
        ViewOut<Tout, Rank>& o_vout)
    {
        using myop = compare_isi<Tin, Tcomp, Tfalse, Tout, Op>;
        myop op(i_valtrue, i_op);
        PixelWiseTernaryOp(i_vin, op, i_vcomp, i_vfalse, o_vout);
    }

    template<typename Tin, typename Tcomp, typename Tfalse, typename Tout, ptrdiff_t Rank, template <typename, ptrdiff_t> class ViewIn, template <typename, ptrdiff_t> class ViewComp, template <typename, ptrdiff_t> class ViewTrue, template <typename, ptrdiff_t> class ViewOut>
    void ViewCompare_isi_dispatch(
        const ViewIn<Tin, Rank>& i_vin,
        CompOpType compOpType,
        const ViewComp<Tcomp, Rank>& i_vcomp,
        typename boost::call_traits<Tout>::param_type i_valtrue,
        const ViewTrue<Tfalse, Rank>& i_vfalse,
        ViewOut<Tout, Rank>& o_vout)
    {
        static_assert(std::is_convertible<Tfalse, Tout>(), "ViewCompare_isi_dispatch Tfalse and Tout must be convertible to each other");
        static_assert(std::is_convertible<Tin, Tcomp>(), "ViewCompare_isi_dispatch Tin and Tcomp must be convertible to each other");
        switch (compOpType)
        {
        case CompOpType::CompOpEqual:
        {
            return t_ViewCompare_isi(i_vin, std::equal_to<Tin>(), i_vcomp, i_valtrue, i_vfalse, o_vout);
        }break;
        case CompOpType::CompOpDiff:
        {
            return t_ViewCompare_isi(i_vin, std::not_equal_to<Tin>(), i_vcomp, i_valtrue, i_vfalse, o_vout);
        }break;
        case CompOpType::CompOpSup:
        {
            return t_ViewCompare_isi(i_vin, std::greater<Tin>(), i_vcomp, i_valtrue, i_vfalse, o_vout);
        }break;
        case CompOpType::CompOpSupEqual:
        {
            return t_ViewCompare_isi(i_vin, std::greater_equal<Tin>(), i_vcomp, i_valtrue, i_vfalse, o_vout);
        }break;
        case CompOpType::CompOpInf:
        {
            return t_ViewCompare_isi(i_vin, std::less<Tin>(), i_vcomp, i_valtrue, i_vfalse, o_vout);
        }break;
        case CompOpType::CompOpInfEqual:
        {
            return t_ViewCompare_isi(i_vin, std::less_equal<Tin>(), i_vcomp, i_valtrue, i_vfalse, o_vout);
        }break;
        default:
        {
            POUTRE_RUNTIME_ERROR("ViewCompare_isi_dispatch CompOpType not implemented");
        }
        }
    }

    /*************************************************************************************************************************************/
    /*                                                      IIS                                                                          */
    /*************************************************************************************************************************************/
    template <typename Tin, typename Tcomp, typename Ttrue, typename Tout, class CompareOp>
    struct compare_iis
    {
        const CompareOp  cop;
        const Tout m_valfalse;

        using param_typein = typename boost::call_traits<Tin>::param_type;
        using param_typecomp = typename boost::call_traits<Tcomp>::param_type;
        using param_typetrue = typename boost::call_traits<Ttrue>::param_type;
        compare_iis(const Tout& i_valfalse) : cop(), m_valfalse(i_valfalse) {}
        compare_iis(const Tout& i_valfalse, const CompareOp& op) : cop(op), m_valfalse(i_valfalse) {}
        Tout operator()(param_typein i_val, param_typecomp i_comp, param_typetrue i_true) const POUTRE_NOEXCEPT //TODO inline ?
        {
            return (cop(i_val, i_comp) ? i_true : m_valfalse);
        }

    };

    template<typename Tin, typename Tcomp, typename Ttrue, typename Tout, ptrdiff_t Rank, template <typename, ptrdiff_t> class ViewIn, template <typename, ptrdiff_t> class ViewComp, template <typename, ptrdiff_t> class ViewTrue, class Op, template <typename, ptrdiff_t> class ViewOut>
    void t_ViewCompare_iis(
        const ViewIn<Tin, Rank>& i_vin,
        Op i_op,
        const ViewComp<Tcomp, Rank>& i_vcomp,
        const ViewTrue<Ttrue, Rank>& i_vtrue,
        typename boost::call_traits<Tout>::param_type i_valfalse,
        ViewOut<Tout, Rank>& o_vout)
    {
        using myop = compare_iis<Tin, Tcomp, Ttrue, Tout, Op>;
        myop op(i_valfalse, i_op);
        PixelWiseTernaryOp(i_vin, op, i_vcomp, i_vtrue, o_vout);
    }

    template<typename Tin, typename Tcomp, typename Ttrue, typename Tout, ptrdiff_t Rank, template <typename, ptrdiff_t> class ViewIn, template <typename, ptrdiff_t> class ViewComp, template <typename, ptrdiff_t> class ViewTrue, template <typename, ptrdiff_t> class ViewOut>
    void ViewCompare_iis_dispatch(
        const ViewIn<Tin, Rank>& i_vin,
        CompOpType compOpType,
        const ViewComp<Tcomp, Rank>& i_vcomp,
        const ViewTrue<Ttrue, Rank>& i_vtrue,
        typename boost::call_traits<Tout>::param_type i_valfalse,
        ViewOut<Tout, Rank>& o_vout)
    {
        static_assert(std::is_convertible<Ttrue, Tout>(), "ViewCompare_iis_dispatch Ttrue and Tout must be convertible to each other");
        static_assert(std::is_convertible<Tin, Tcomp>(), "ViewCompare_iis_dispatch Tin and Tcomp must be convertible to each other");
        switch (compOpType)
        {
        case CompOpType::CompOpEqual:
        {
            return t_ViewCompare_iis(i_vin, std::equal_to<Tin>(), i_vcomp, i_vtrue, i_valfalse, o_vout);
        }break;
        case CompOpType::CompOpDiff:
        {
            return t_ViewCompare_iis(i_vin, std::not_equal_to<Tin>(), i_vcomp, i_vtrue, i_valfalse, o_vout);
        }break;
        case CompOpType::CompOpSup:
        {
            return t_ViewCompare_iis(i_vin, std::greater<Tin>(), i_vcomp, i_vtrue, i_valfalse, o_vout);
        }break;
        case CompOpType::CompOpSupEqual:
        {
            return t_ViewCompare_iis(i_vin, std::greater_equal<Tin>(), i_vcomp, i_vtrue, i_valfalse, o_vout);
        }break;
        case CompOpType::CompOpInf:
        {
            return t_ViewCompare_iis(i_vin, std::less<Tin>(), i_vcomp, i_vtrue, i_valfalse, o_vout);
        }break;
        case CompOpType::CompOpInfEqual:
        {
            return t_ViewCompare_iis(i_vin, std::less_equal<Tin>(), i_vcomp, i_vtrue, i_valfalse, o_vout);
        }break;
        default:
        {
            POUTRE_RUNTIME_ERROR("ViewCompare_iis_dispatch CompOpType not implemented");
        }
        }
    }



    /*************************************************************************************************************************************/
    /*                                                      SSI                                                                          */
    /*************************************************************************************************************************************/
    template <typename Tin, typename Tfalse, typename Tout, class CompareOp>
    struct compare_ssi
    {
        const CompareOp  cop;
        const Tin m_compval;
        const Tout m_valtrue;

        using param_typein = typename boost::call_traits<Tin>::param_type;
        using param_typetrue = typename boost::call_traits<Tfalse>::param_type;
        compare_ssi(const Tin& i_compval, const Tout& i_valtrue) : cop(), m_compval(i_compval), m_valtrue(i_valtrue) {}
        compare_ssi(const Tin& i_compval, const Tout& i_valtrue, const CompareOp& op) : cop(op), m_compval(i_compval), m_valtrue(i_valtrue) {}
        Tout operator()(param_typein i_val, param_typetrue i_false) const POUTRE_NOEXCEPT //TODO inline ?
        {
            return (cop(i_val, m_compval) ? m_valtrue : i_false);
        }

    };

    template<typename Tin, typename Tfalse, typename Tout, ptrdiff_t Rank, template <typename, ptrdiff_t> class ViewIn, template <typename, ptrdiff_t> class ViewFalse, class Op, template <typename, ptrdiff_t> class ViewOut>
    void t_ViewCompare_ssi(
        const ViewIn<Tin, Rank>& i_vin,
        Op i_op,
        typename boost::call_traits<Tin>::param_type i_compval,
        typename boost::call_traits<Tout>::param_type i_valtrue,
        const ViewFalse<Tfalse, Rank>& i_vfalse,
        ViewOut<Tout, Rank>& o_vout)
    {
        using myop = compare_ssi<Tin, Tfalse, Tout, Op>;
        myop op(i_compval, i_valtrue, i_op);
        PixelWiseBinaryOp(i_vin, op, i_vfalse, o_vout);
    }

    template<typename Tin, typename Tfalse, typename Tout, ptrdiff_t Rank, template <typename, ptrdiff_t> class ViewIn, template <typename, ptrdiff_t> class ViewFalse, template <typename, ptrdiff_t> class ViewOut>
    void ViewCompare_ssi_dispatch(
        const ViewIn<Tin, Rank>& i_vin,
        CompOpType compOpType,
        typename boost::call_traits<Tin>::param_type i_compval,
        typename boost::call_traits<Tout>::param_type i_valtrue,
        const ViewFalse<Tfalse, Rank>& i_vfalse,
        ViewOut<Tout, Rank>& o_vout)
    {
        static_assert(std::is_convertible<Tfalse, Tout>(), "ViewCompare_ssi_dispatch Ttrue and Tout must be convertible to each other");
        switch (compOpType)
        {
        case CompOpType::CompOpEqual:
        {
            return t_ViewCompare_ssi(i_vin, std::equal_to<Tin>(), i_compval, i_valtrue, i_vfalse, o_vout);
        }break;
        case CompOpType::CompOpDiff:
        {
            return t_ViewCompare_ssi(i_vin, std::not_equal_to<Tin>(), i_compval, i_valtrue, i_vfalse, o_vout);
        }break;
        case CompOpType::CompOpSup:
        {
            return t_ViewCompare_ssi(i_vin, std::greater<Tin>(), i_compval, i_valtrue, i_vfalse, o_vout);
        }break;
        case CompOpType::CompOpSupEqual:
        {
            return t_ViewCompare_ssi(i_vin, std::greater_equal<Tin>(), i_compval, i_valtrue, i_vfalse, o_vout);
        }break;
        case CompOpType::CompOpInf:
        {
            return t_ViewCompare_ssi(i_vin, std::less<Tin>(), i_compval, i_valtrue, i_vfalse, o_vout);
        }break;
        case CompOpType::CompOpInfEqual:
        {
            return t_ViewCompare_ssi(i_vin, std::less_equal<Tin>(), i_compval, i_valtrue, i_vfalse, o_vout);
        }break;
        default:
        {
            POUTRE_RUNTIME_ERROR("ViewCompare_ssi_dispatch CompOpType not implemented");
        }
        }
    }

    /*************************************************************************************************************************************/
    /*                                                      ISS                                                                          */
    /*************************************************************************************************************************************/
    template <typename Tin1, typename Tin2, typename Tout, class CompareOp>
    struct compare_iss
    {
        const CompareOp  cop;
        const Tout m_valtrue, m_valfalse;
        using param_type1 = typename boost::call_traits<Tin1>::param_type;
        using param_type2 = typename boost::call_traits<Tin2>::param_type;
        compare_iss(const Tout& i_valtrue, const Tout& i_valfalse) : cop(), m_valtrue(i_valtrue), m_valfalse(i_valfalse) {}
        compare_iss(const Tout& i_valtrue, const Tout& i_valfalse, const CompareOp& op) : cop(op), m_valtrue(i_valtrue), m_valfalse(i_valfalse) {}
        const Tout& operator()(param_type1 i_val1, param_type2 i_val2) const POUTRE_NOEXCEPT //TODO inline ?
        {
            return (cop(i_val1, i_val2) ? m_valtrue : m_valfalse);
        }

    };

    template<typename Tin1, typename Tin2, typename Tout, ptrdiff_t Rank, template <typename, ptrdiff_t> class ViewIn1, template <typename, ptrdiff_t> class ViewIn2, class Op, template <typename, ptrdiff_t> class ViewOut>
    void t_ViewCompare_iss(
        const ViewIn1<Tin1, Rank>& i_vin1,
        Op i_op,
        const ViewIn2<Tin2, Rank>& i_vin2,
        typename boost::call_traits<Tout>::param_type i_valtrue,
        typename boost::call_traits<Tout>::param_type i_valfalse,
        ViewOut<Tout, Rank>& o_vout)
    {
        using myop = compare_iss<Tin1, Tin2, Tout, Op>;
        myop op(i_valtrue, i_valfalse, i_op);
        PixelWiseBinaryOp(i_vin1, op, i_vin2, o_vout);
    }

    template<typename Tin1, typename Tin2, typename Tout, ptrdiff_t Rank, template <typename, ptrdiff_t> class ViewIn1, template <typename, ptrdiff_t> class ViewIn2, template <typename, ptrdiff_t> class ViewOut>
    void ViewCompare_iss_dispatch(
        const ViewIn1<Tin1, Rank>& i_vin1,
        CompOpType compOpType,
        const ViewIn2<Tin2, Rank>& i_vin2,
        typename boost::call_traits<Tout>::param_type i_valtrue,
        typename boost::call_traits<Tout>::param_type i_valfalse,
        ViewOut<Tout, Rank>& o_vout)
    {
        static_assert(std::is_convertible<Tin1, Tin2>(), "ViewCompare_iss_dispatch Tin1 and Tin2 must be convertible to each other");
        switch (compOpType)
        {
        case CompOpType::CompOpEqual:
        {
            return t_ViewCompare_iss(i_vin1, std::equal_to<Tin1>(), i_vin2, i_valtrue, i_valfalse, o_vout);
        }break;
        case CompOpType::CompOpDiff:
        {
            return t_ViewCompare_iss(i_vin1, std::not_equal_to<Tin1>(), i_vin2, i_valtrue, i_valfalse, o_vout);
        }break;
        case CompOpType::CompOpSup:
        {
            return t_ViewCompare_iss(i_vin1, std::greater<Tin1>(), i_vin2, i_valtrue, i_valfalse, o_vout);
        }break;
        case CompOpType::CompOpSupEqual:
        {
            return t_ViewCompare_iss(i_vin1, std::greater_equal<Tin1>(), i_vin2, i_valtrue, i_valfalse, o_vout);
        }break;
        case CompOpType::CompOpInf:
        {
            return t_ViewCompare_iss(i_vin1, std::less<Tin1>(), i_vin2, i_valtrue, i_valfalse, o_vout);
        }break;
        case CompOpType::CompOpInfEqual:
        {
            return t_ViewCompare_iss(i_vin1, std::less_equal<Tin1>(), i_vin2, i_valtrue, i_valfalse, o_vout);
        }break;
        default:
        {
            POUTRE_RUNTIME_ERROR("ViewCompare_iss_dispatch CompOpType not implemented");
        }
        }
    }
}
#endif //POUTRE_IMAGEPROCESSING_COMPARE_HPP__