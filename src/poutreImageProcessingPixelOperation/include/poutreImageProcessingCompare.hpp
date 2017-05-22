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

#ifndef POUTRE_IMAGEPROCESSING_UNARYOP_HPP__
#include <poutreImageProcessingPixelOperation/include/poutreImageProcessingUnaryOp.hpp>
#endif
#ifndef POUTRE_IMAGEPROCESSING_BINARYOP_HPP__
#include <poutreImageProcessingPixelOperation/include/poutreImageProcessingBinaryOp.hpp>
#endif
#ifndef POUTRE_IMAGEPROCESSING_TERNARYOP_HPP__
#include <poutreImageProcessingPixelOperation/include/poutreImageProcessingTernaryOp.hpp>
#endif

#ifndef POUTRE_IMAGEPROCESSING_QUATERNARYOP_HPP__
#include <poutreImageProcessingPixelOperation/include/poutreImageProcessingQuaternaryOp.hpp>
#endif

//simd part
//https://developer.numscale.com/boost.simd/documentation/develop/group__group-predicates.html
#include <boost/simd/function/is_less.hpp>
#include <boost/simd/function/is_less_equal.hpp>
#include <boost/simd/function/is_greater.hpp>
#include <boost/simd/function/is_greater_equal.hpp>
#include <boost/simd/function/is_equal.hpp>
#include <boost/simd/function/is_not_equal.hpp>
#include <boost/simd/algorithm.hpp>//"simd" transform
#include <boost/simd/logical.hpp>

namespace poutre
{
    namespace bs = boost::simd;

    /******************************************Operator base ******************************************************/
    //!Operator equal
    template< typename T1, class tag = tag_SIMD_disabled>
    struct OpCompEqualValue
    {
        T1 m_val;
        OpCompEqualValue(T1 ival) :m_val(ival) {}
        POUTRE_ALWAYS_INLINE POUTRE_CXX14_CONSTEXPR bool operator()(T1 lhs) const POUTRE_NOEXCEPT
        {
            return  lhs == m_val;
        }
    };

    template< typename T1>
    struct OpCompEqualValue<T1, tag_SIMD_enabled>
    {
        using pack_t = bs::pack<T1>;
        using logical_t = bs::pack<bs::logical<T1>>;
        pack_t m_val_pack;
        OpCompEqualValue(T1 ival) :m_val_pack(ival) {}
        template< typename U>
        POUTRE_ALWAYS_INLINE POUTRE_CXX14_CONSTEXPR logical_t operator()(U lhs) const POUTRE_NOEXCEPT
        {
            return  bs::is_equal(lhs, m_val_pack);
        }
    };

    //!Operator diff
    template< typename T1, class tag = tag_SIMD_disabled>
    struct OpCompDiffValue
    {
        T1 m_val;
        OpCompDiffValue(T1 ival) :m_val(ival) {}
        POUTRE_ALWAYS_INLINE POUTRE_CXX14_CONSTEXPR bool operator()(T1 lhs) const POUTRE_NOEXCEPT
        {
            return  lhs != m_val;
        }
    };

    template< typename T1>
    struct OpCompDiffValue<T1, tag_SIMD_enabled>
    {
        using pack_t = bs::pack<T1>;
        using logical_t = bs::pack<bs::logical<T1>>;
        pack_t m_val_pack;
        OpCompDiffValue(T1 ival) :m_val_pack(ival) {}
        template< typename U>
        POUTRE_ALWAYS_INLINE POUTRE_CXX14_CONSTEXPR logical_t operator()(U lhs) const POUTRE_NOEXCEPT
        {
            return  bs::is_not_equal(lhs, m_val_pack);
        }
    };

    //!Operator Sup
    template< typename T1, class tag = tag_SIMD_disabled>
    struct OpCompSupValue
    {
        T1 m_val;
        OpCompSupValue(T1 ival) :m_val(ival) {}
        POUTRE_ALWAYS_INLINE POUTRE_CXX14_CONSTEXPR bool operator()(T1 lhs) const POUTRE_NOEXCEPT
        {
            return  lhs > m_val;
        }
    };

    template< typename T1>
    struct OpCompSupValue<T1, tag_SIMD_enabled>
    {
        using pack_t = bs::pack<T1>;
        using logical_t = bs::pack<bs::logical<T1>>;
        pack_t m_val_pack;
        OpCompSupValue(T1 ival) :m_val_pack(ival) {}
        template< typename U>
        POUTRE_ALWAYS_INLINE POUTRE_CXX14_CONSTEXPR logical_t operator()(U lhs) const POUTRE_NOEXCEPT
        {
            return  bs::is_greater(lhs, m_val_pack);
        }
    };

    //!Operator SupEqual
    template< typename T1, class tag = tag_SIMD_disabled>
    struct OpCompSupEqualValue
    {
        T1 m_val;
        OpCompSupEqualValue(T1 ival) :m_val(ival) {}
        POUTRE_ALWAYS_INLINE POUTRE_CXX14_CONSTEXPR bool operator()(T1 lhs) const POUTRE_NOEXCEPT
        {
            return lhs >= m_val;
        }
    };

    template< typename T1>
    struct OpCompSupEqualValue<T1, tag_SIMD_enabled>
    {
        using pack_t = bs::pack<T1>;
        using logical_t = bs::pack<bs::logical<T1>>;
        pack_t m_val_pack;
        OpCompSupEqualValue(T1 ival) :m_val_pack(ival) {}
        template< typename U>
        POUTRE_ALWAYS_INLINE POUTRE_CXX14_CONSTEXPR logical_t operator()(U lhs) const POUTRE_NOEXCEPT
        {
            return  bs::is_greater_equal(lhs, m_val_pack);
        }
    };

    //!Operator Inf
    template< typename T1, class tag = tag_SIMD_disabled>
    struct OpCompInfValue
    {
        T1 m_val;
        OpCompInfValue(T1 ival) :m_val(ival) {}
        POUTRE_ALWAYS_INLINE POUTRE_CXX14_CONSTEXPR bool operator()(T1 lhs) const POUTRE_NOEXCEPT
        {
            return lhs < m_val;
        }
    };

    template< typename T1>
    struct OpCompInfValue<T1, tag_SIMD_enabled>
    {
        using pack_t = bs::pack<T1>;
        using logical_t = bs::pack<bs::logical<T1>>;
        pack_t m_val_pack;
        OpCompInfValue(T1 ival) :m_val_pack(ival) {}
        template< typename U>
        POUTRE_ALWAYS_INLINE POUTRE_CXX14_CONSTEXPR logical_t operator()(U lhs) const POUTRE_NOEXCEPT
        {
            return  bs::is_less(lhs, m_val_pack);
        }
    };

    //!Operator InfEqual
    template< typename T1, class tag = tag_SIMD_disabled>
    struct OpCompInfEqualValue
    {
        T1 m_val;
        OpCompInfEqualValue(T1 ival) :m_val(ival) {}
        POUTRE_ALWAYS_INLINE POUTRE_CXX14_CONSTEXPR bool operator()(T1 lhs) const POUTRE_NOEXCEPT
        {
            return lhs <= m_val;
        }
    };

    template< typename T1>
    struct OpCompInfEqualValue<T1, tag_SIMD_enabled>
    {
        using pack_t = bs::pack<T1>;
        using logical_t = bs::pack<bs::logical<T1>>;
        pack_t m_val_pack;
        OpCompInfEqualValue(T1 ival) :m_val_pack(ival) {}
        template< typename U>
        POUTRE_ALWAYS_INLINE POUTRE_CXX14_CONSTEXPR logical_t operator()(U lhs) const POUTRE_NOEXCEPT
        {
            return  bs::is_less_equal(lhs, m_val_pack);
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

    template <typename T, class CompareOpSimd, class CompareOp>
    struct compare_sss_simd
    {
		const CompareOpSimd copsimd;
        const CompareOp  cop;
        using pack_t = bs::pack<T>;
        using logical_t = bs::pack<bs::logical<T>>;
        pack_t m_valtrue_pack, m_valfalse_pack;
		T m_valtrue, m_valfalse;
        compare_sss_simd(const T& i_valtrue, const T& i_valfalse) : copsimd(),cop(), m_valtrue_pack(i_valtrue), m_valfalse_pack(i_valfalse),m_valtrue(i_valtrue),m_valfalse(i_valfalse) {}
		compare_sss_simd(const T& i_valtrue, const T& i_valfalse, const CompareOpSimd& opsimd, const CompareOp& op) :copsimd(opsimd), cop(op), m_valtrue_pack(i_valtrue), m_valfalse_pack(i_valfalse), m_valtrue(i_valtrue), m_valfalse(i_valfalse) {}

        T operator()(T i_val) const POUTRE_NOEXCEPT 
        {
            std::cout << "call unary" << std::endl;
            return (cop(i_val) ? m_valtrue : m_valfalse);
        }

        template<typename U>
        U operator()(U i_val) const POUTRE_NOEXCEPT //TODO inline ?        
        {          
            std::cout << "call unary simd" << std::endl;
            logical_t v_res = copsimd(i_val);
            return bs::if_else(v_res, m_valtrue_pack, m_valfalse_pack);
        }

    };

    template<typename Tin, typename Tout, ptrdiff_t Rank, template <typename, ptrdiff_t> class ViewIn, class Op, template <typename, ptrdiff_t> class ViewOut>
    struct viewCompare_sss
    {
        void operator()(const ViewIn<Tin, Rank>& i_vin,
            Op i_op,
            typename boost::call_traits<Tout>::param_type i_valtrue,
            typename boost::call_traits<Tout>::param_type i_valfalse,
            ViewOut<Tout, Rank>& o_vout)
        {
            using myop = compare_sss<Tin, Tout, Op>;
            myop op(i_valtrue, i_valfalse, i_op);
            PixelWiseUnaryOp(i_vin, op, o_vout);
        }
    };

    template<typename T, ptrdiff_t Rank, class OpSimd,class Op>
    struct viewCompare_sss_simd
    {
        void operator()(const array_view<T, Rank>& i_vin,
			OpSimd i_opsimd,
            Op i_op,
            typename boost::call_traits<T>::param_type i_valtrue,
            typename boost::call_traits<T>::param_type i_valfalse,
            array_view<T, Rank>& o_vout)
        {
            using functor = compare_sss_simd<T, OpSimd, Op>;
            functor f(i_valtrue, i_valfalse, i_opsimd, i_op);
            auto begin = i_vin.data();
            auto end = i_vin.data() + i_vin.size();
            auto out = o_vout.data();
            bs::transform(begin, end, out, f);
        }

    };

    //primary template
    template<typename Tin, typename Tout, ptrdiff_t Rank, template <typename, ptrdiff_t> class ViewIn, template <typename, ptrdiff_t> class ViewOut,typename=void>
    struct ViewCompare_sss_dispatch
    {
        void operator()(
            const ViewIn<Tin, Rank>& i_vin,
            CompOpType compOpType,
            typename boost::call_traits<Tin>::param_type i_compval,
            typename boost::call_traits<Tout>::param_type i_valtrue,
            typename boost::call_traits<Tout>::param_type i_valfalse,
            ViewOut<Tout, Rank>& o_vout)
        {
            switch (compOpType)
            {
            case CompOpType::CompOpEqual:
            {
                OpCompEqualValue<Tin> myop(i_compval);
                return viewCompare_sss<Tin, Tout, Rank, ViewIn, OpCompEqualValue<Tin>, ViewOut>()(i_vin, myop, i_valtrue, i_valfalse, o_vout);
            }break;
            case CompOpType::CompOpDiff:
            {
                OpCompDiffValue<Tin> myop(i_compval);
                return viewCompare_sss<Tin, Tout, Rank, ViewIn, OpCompDiffValue<Tin>, ViewOut>()(i_vin, myop, i_valtrue, i_valfalse, o_vout);
            }break;
            case CompOpType::CompOpSup:
            {
                OpCompSupValue<Tin> myop(i_compval);
                return viewCompare_sss<Tin, Tout, Rank, ViewIn, OpCompSupValue<Tin>, ViewOut>()(i_vin, myop, i_valtrue, i_valfalse, o_vout);
            }break;
            case CompOpType::CompOpSupEqual:
            {
                OpCompSupEqualValue<Tin> myop(i_compval);
                return viewCompare_sss<Tin, Tout, Rank, ViewIn, OpCompSupEqualValue<Tin>, ViewOut>()(i_vin, myop, i_valtrue, i_valfalse, o_vout);
            }break;
            case CompOpType::CompOpInf:
            {
                OpCompInfValue<Tin> myop(i_compval);
                return viewCompare_sss<Tin, Tout, Rank, ViewIn, OpCompInfValue<Tin>, ViewOut>()(i_vin, myop, i_valtrue, i_valfalse, o_vout);
            }break;
            case CompOpType::CompOpInfEqual:
            {
                OpCompInfEqualValue<Tin> myop(i_compval);
                return viewCompare_sss<Tin, Tout, Rank, ViewIn, OpCompInfEqualValue<Tin>, ViewOut>()(i_vin, myop, i_valtrue, i_valfalse, o_vout);
            }break;
            default:
            {
                POUTRE_RUNTIME_ERROR("ViewCompare_sss_dispatch CompOpType not implemented");
            }
            }
        }
    };
	
    //SIMD Specialization arithmetic type,same type and both array_view
	template<typename T, ptrdiff_t Rank, template <typename, ptrdiff_t> class ViewIn, template <typename, ptrdiff_t> class ViewOut>
	struct ViewCompare_sss_dispatch<T, T, Rank, ViewIn, ViewOut,
		std::enable_if_t<
		std::is_arithmetic<T>::value &&
		(std::is_same< ViewIn<T,Rank>, array_view<T, Rank> >::value || std::is_same< ViewIn<T,Rank>, carray_view<T, Rank> >::value) 
		&& std::is_same< ViewOut<T, Rank>, array_view<T, Rank> >::value>
	>
    {
        void operator()(
            const ViewIn<T, Rank>& i_vin,
            CompOpType compOpType,
            typename boost::call_traits<T>::param_type i_compval,
            typename boost::call_traits<T>::param_type i_valtrue,
            typename boost::call_traits<T>::param_type i_valfalse,
			ViewOut<T, Rank>& o_vout)
        {
            switch (compOpType)
            {
            case CompOpType::CompOpEqual:
            {
                OpCompEqualValue<T, tag_SIMD_enabled> myopsimd(i_compval);
				OpCompEqualValue<T> myop(i_compval);
                return viewCompare_sss_simd<T, Rank, OpCompEqualValue<T, tag_SIMD_enabled>,OpCompEqualValue<T>>()(i_vin, myopsimd, myop,i_valtrue, i_valfalse, o_vout);
            }break;
            case CompOpType::CompOpDiff:
            {
                OpCompDiffValue<T, tag_SIMD_enabled> myopsimd(i_compval);
				OpCompDiffValue<T> myop(i_compval);
                return viewCompare_sss_simd<T, Rank, OpCompDiffValue<T, tag_SIMD_enabled>, OpCompDiffValue<T>>()(i_vin, myopsimd, myop, i_valtrue, i_valfalse, o_vout);
            }break;
            case CompOpType::CompOpSup:
            {
                OpCompSupValue<T, tag_SIMD_enabled> myopsimd(i_compval);
				OpCompSupValue<T> myop(i_compval);
                return viewCompare_sss_simd<T, Rank, OpCompSupValue<T, tag_SIMD_enabled>, OpCompSupValue<T>>()(i_vin, myopsimd, myop, i_valtrue, i_valfalse, o_vout);
            }break;
            case CompOpType::CompOpSupEqual:
            {
                OpCompSupEqualValue<T, tag_SIMD_enabled> myopsimd(i_compval);
				OpCompSupEqualValue<T> myop(i_compval);
                return viewCompare_sss_simd<T, Rank, OpCompSupEqualValue<T, tag_SIMD_enabled>,OpCompSupEqualValue<T>>()(i_vin, myopsimd, myop, i_valtrue, i_valfalse, o_vout);
            }break;
            case CompOpType::CompOpInf:
            {
                OpCompInfValue<T, tag_SIMD_enabled> myopsimd(i_compval);
				OpCompInfValue<T> myop(i_compval);
                return viewCompare_sss_simd<T, Rank, OpCompInfValue<T, tag_SIMD_enabled>, OpCompInfValue<T>>()(i_vin, myopsimd, myop, i_valtrue, i_valfalse, o_vout);
            }break;
            case CompOpType::CompOpInfEqual:
            {
                OpCompInfEqualValue<T, tag_SIMD_enabled> myopsimd(i_compval);
				OpCompInfEqualValue<T> myop(i_compval);
                return viewCompare_sss_simd<T, Rank, OpCompInfEqualValue<T, tag_SIMD_enabled>,OpCompInfEqualValue<T>>()(i_vin, myopsimd, myop, i_valtrue, i_valfalse, o_vout);
            }break;
            default:
            {
                POUTRE_RUNTIME_ERROR("ViewCompare_sss_dispatch CompOpType not implemented");
            }
            }
        }
    };
	
    template<typename Tin, typename Tout, ptrdiff_t Rank, template <typename, ptrdiff_t> class ViewIn, template <typename, ptrdiff_t> class ViewOut>
    void ViewCompare_sss_func_helper(
        const ViewIn<Tin, Rank>& i_vin,
        CompOpType compOpType,
        typename boost::call_traits<Tin>::param_type i_compval,
        typename boost::call_traits<Tout>::param_type i_valtrue,
        typename boost::call_traits<Tout>::param_type i_valfalse,
        ViewOut<Tout, Rank>& o_vout)
    {
		ViewCompare_sss_dispatch<Tin, Tout, Rank, ViewIn, ViewOut> op;
        op(i_vin, compOpType, i_compval, i_valtrue, i_valfalse, o_vout);
    }
    /*
    //default
    template<typename Tin, typename Tout, ptrdiff_t Rank, template <typename, ptrdiff_t> class ViewIn, template <typename, ptrdiff_t> class ViewOut>
    typename std::enable_if<!(std::is_arithmetic<Tin>::value && std::is_arithmetic<Tout>::value && std::is_same<Tin, Tout>::value),void>::type
        ViewCompare_sss_dispatch(
        const ViewIn<Tin, Rank>& i_vin,
        CompOpType compOpType,
        typename boost::call_traits<Tin>::param_type i_compval,
        typename boost::call_traits<Tout>::param_type i_valtrue,
        typename boost::call_traits<Tout>::param_type i_valfalse,
        ViewOut<Tout, Rank>& o_vout)
    {
        switch (compOpType)
        {
        case CompOpType::CompOpEqual:
        {
            OpCompEqualValue<Tin> myop(i_compval);
            return viewCompare_sss<Tin,Tout,Rank,ViewIn,OpCompEqualValue<Tin>,ViewOut>()(i_vin, myop, i_valtrue, i_valfalse, o_vout);
        }break;
        case CompOpType::CompOpDiff:
        {
            OpCompDiffValue<Tin> myop(i_compval);
            return viewCompare_sss<Tin, Tout, Rank, ViewIn, OpCompDiffValue<Tin>, ViewOut>()(i_vin, myop, i_valtrue, i_valfalse, o_vout);
        }break;
        case CompOpType::CompOpSup:
        {
            OpCompSupValue<Tin> myop(i_compval);
            return viewCompare_sss<Tin, Tout, Rank, ViewIn, OpCompSupValue<Tin>, ViewOut>()(i_vin, myop, i_valtrue, i_valfalse, o_vout);
        }break;
        case CompOpType::CompOpSupEqual:
        {
            OpCompSupEqualValue<Tin> myop(i_compval);
            return viewCompare_sss<Tin, Tout, Rank, ViewIn, OpCompSupEqualValue<Tin>, ViewOut>()(i_vin, myop, i_valtrue, i_valfalse, o_vout);
        }break;
        case CompOpType::CompOpInf:
        {
            OpCompInfValue<Tin> myop(i_compval);
            return viewCompare_sss<Tin, Tout, Rank, ViewIn, OpCompInfValue<Tin>, ViewOut>()(i_vin, myop, i_valtrue, i_valfalse, o_vout);
        }break;
        case CompOpType::CompOpInfEqual:
        {
            OpCompInfEqualValue<Tin> myop(i_compval);
            return viewCompare_sss<Tin, Tout, Rank, ViewIn, OpCompInfEqualValue<Tin>, ViewOut>()(i_vin, myop, i_valtrue, i_valfalse, o_vout);
        }break;
        default:
        {
            POUTRE_RUNTIME_ERROR("ViewCompare_sss_dispatch CompOpType not implemented");
        }
        }
    }



    //overload same type and arithmetic
    template<typename Tin, typename Tout, ptrdiff_t Rank, template <typename, ptrdiff_t> class ViewIn, template <typename, ptrdiff_t> class ViewOut>
    typename std::enable_if < (std::is_arithmetic<Tin>::value && std::is_arithmetic<Tout>::value && std::is_same<Tin, Tout>::value), void > ::type
        ViewCompare_sss_dispatch(
            const ViewIn<Tin, Rank>& i_vin,
            CompOpType compOpType,
            typename boost::call_traits<Tin>::param_type i_compval,
            typename boost::call_traits<Tout>::param_type i_valtrue,
            typename boost::call_traits<Tout>::param_type i_valfalse,
            ViewOut<Tout, Rank>& o_vout)
    {
        switch (compOpType)
        {
        case CompOpType::CompOpEqual:
        {
            OpCompEqualValue<Tin,tag_SIMD_enabled> myop(i_compval);
            return viewCompare_sss<Tin, Tout, Rank, ViewIn, OpCompEqualValue<Tin>, ViewOut>()(i_vin, myop, i_valtrue, i_valfalse, o_vout);
        }break;
        case CompOpType::CompOpDiff:
        {
            OpCompDiffValue<Tin, tag_SIMD_enabled> myop(i_compval);
            return viewCompare_sss<Tin, Tout, Rank, ViewIn, OpCompDiffValue<Tin>, ViewOut>()(i_vin, myop, i_valtrue, i_valfalse, o_vout);
        }break;
        case CompOpType::CompOpSup:
        {
            OpCompSupValue<Tin, tag_SIMD_enabled> myop(i_compval);
            return viewCompare_sss<Tin, Tout, Rank, ViewIn, OpCompSupValue<Tin>, ViewOut>()(i_vin, myop, i_valtrue, i_valfalse, o_vout);
        }break;
        case CompOpType::CompOpSupEqual:
        {
            OpCompSupEqualValue<Tin, tag_SIMD_enabled> myop(i_compval);
            return viewCompare_sss<Tin, Tout, Rank, ViewIn, OpCompSupEqualValue<Tin>, ViewOut>()(i_vin, myop, i_valtrue, i_valfalse, o_vout);
        }break;
        case CompOpType::CompOpInf:
        {
            OpCompInfValue<Tin, tag_SIMD_enabled> myop(i_compval);
            return viewCompare_sss<Tin, Tout, Rank, ViewIn, OpCompInfValue<Tin>, ViewOut>()(i_vin, myop, i_valtrue, i_valfalse, o_vout);
        }break;
        case CompOpType::CompOpInfEqual:
        {
            OpCompInfEqualValue<Tin, tag_SIMD_enabled> myop(i_compval);
            return viewCompare_sss<Tin, Tout, Rank, ViewIn, OpCompInfEqualValue<Tin>, ViewOut>()(i_vin, myop, i_valtrue, i_valfalse, o_vout);
        }break;
        default:
        {
            POUTRE_RUNTIME_ERROR("ViewCompare_sss_dispatch CompOpType not implemented");
        }
        }
    }*/


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