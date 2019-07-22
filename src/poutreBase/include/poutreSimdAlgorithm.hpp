
//==============================================================================
//                  Copyright (c) 2015 - Thomas Retornaz                      //
//                     thomas.retornaz@mines-paris.org                        //
//          Distributed under the Boost Software License, Version 1.0.        //
//                 See accompanying file LICENSE.txt or copy at               //
//                     http://www.boost.org/LICENSE_1_0.txt                   //
//==============================================================================

#ifndef POUTRE_SIMD_ALGORITHM_HPP__
#define POUTRE_SIMD_ALGORITHM_HPP__

/**
 * @file   poutreSimdAlgorithm.hpp
 * @author Thomas Retornaz
 * @brief  Add STL like algorithm on top of nsimd
 * 
 * 
 */

#ifndef POUTRE_CONFIG__HPP__
#include <poutreBase/poutreConfig.hpp>
#endif

#ifndef POUTRE_SIMD_HPP__
#include <poutreBase/include/poutreSimd.hpp>
#endif
namespace poutre
  {
	namespace simd
	{
        template<typename T, typename U, typename UnOp>
        U* transform(T const* first, T const* last, U* out, UnOp f) POUTRE_NOEXCEPTONLYNDEBUG
        {
            POUTRE_ASSERTCHECK(!first,"null ptr");
            POUTRE_ASSERTCHECK(!last,"null ptr");
            POUTRE_ASSERTCHECK(!out,"null ptr");

            using simd_type_T = typename nsimd::pack<T>;
            using simd_type_U = typename nsimd::pack<U>;

            static_assert (nsimd::max_len_t<T>::value == nsimd::max_len_t<U>::value
                , "mismatch length between T and U"
                );

            const auto alignment = SIMD_IDEAL_MAX_ALIGN_BYTES;//NSIMD_MAX_ALIGNMENT;

            //Define loop counter
            const auto simd_size = nsimd::len(simd_type_T());
            const auto size = std::distance(first, last);
            //note enforce that input is aligned when we start the main simd loop
            const auto range = simd::t_SIMDInputRange(first, last);
            const auto size_prologue_loop = range.first;
            const auto size_simd_loop = range.second;

            auto i = 0;

            //---prologue
            for (; i < size_prologue_loop; ++i)
            {
                *out++ = f(*first++);
            }

            //---main simd loop
            if (simd::IsAligned(out, alignment))
            {
                for (; i < size_simd_loop; i += simd_size)
                {
                    auto element = nsimd::loada<simd_type_T>(first);
                    nsimd::storea(out, f(element));
                    first += simd_size;
                    out += simd_size;
                }
            }
            else
            {
                for (; i < size_simd_loop; i += simd_size)
                {
                    auto element = nsimd::loada<simd_type_T>(first);
                    nsimd::storeu(out, f(element));
                    first += simd_size;
                    out += simd_size;
                }
            }
            //---epilogue
            for (; i < size; ++i)
            {
                *out++ = f(*first++);
            }
            return out;
        }

        template<typename T1, typename T2, typename U, typename BinOp>
        U* transform(T1 const* first1, T1 const* last1, T2 const* first2, U* out, BinOp f) POUTRE_NOEXCEPTONLYNDEBUG
        {
            POUTRE_ASSERTCHECK(!first1,"null ptr");
            POUTRE_ASSERTCHECK(!last1,"null ptr");
            POUTRE_ASSERTCHECK(!first2,"null ptr");
            POUTRE_ASSERTCHECK(!out,"null ptr");


            using simd_type_T1 = typename nsimd::pack<T1>;
            using simd_type_T2 = typename nsimd::pack<T2>;
            using simd_type_U = typename nsimd::pack<U>;

            static_assert (nsimd::max_len_t<T1>::value == nsimd::max_len_t<T2>::value
                , "mismatch length between T1 and T2"
                );
            static_assert (nsimd::max_len_t<T1>::value == nsimd::max_len_t<U>::value
                , "mismatch length between T1 and U"
                );
            static_assert (nsimd::max_len_t<T2>::value == nsimd::max_len_t<U>::value
                , "mismatch length between T2 and U"
                );

            auto alignment = SIMD_IDEAL_MAX_ALIGN_BYTES;////NSIMD_MAX_ALIGNMENT; 

            //Define loop counter
            const auto simd_size = nsimd::len(simd_type_T1());
            const auto size = std::distance(first1, last1);
            const auto range = simd::t_SIMDInputRange(first1, last1);
            const auto size_prologue_loop = range.first;
            const auto size_simd_loop = range.second;


            auto i = 0;

            //---prologue
            for (; i < size_prologue_loop; ++i)
            {
                *out++ = f(*first1++, *first2++);
            }

            //---main simd loop
            if (simd::IsAligned(first2, alignment) && simd::IsAligned(out, alignment))
            {
                for (; i < size_simd_loop; i += simd_size)
                {
                    simd_type_T1 element1 = nsimd::loada<simd_type_T1>(first1);
                    simd_type_T2 element2 = nsimd::loada<simd_type_T2>(first2);
                    nsimd::storea(out, f(element1, element2));
                    first1 += simd_size;
                    first2 += simd_size;
                    out += simd_size;
                }
            }
            else
            {
                for (; i < size_simd_loop; i += simd_size)
                {
                    simd_type_T1 element1 = nsimd::loada<simd_type_T1>(first1);
                    simd_type_T2 element2 = nsimd::loadu<simd_type_T2>(first2);
                    nsimd::storeu(out, f(element1, element2));
                    first1 += simd_size;
                    first2 += simd_size;
                    out += simd_size;
                }
            }
            //---epilogue
            for (; i < size; ++i)
            {
                *out++ = f(*first1++, *first2++);
            }
            return out;
        }
	}
  }
#endif //POUTRE_SIMD_ALGORITHM_HPP__
