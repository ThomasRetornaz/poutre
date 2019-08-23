
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

#include <stdalign.h>
#include <stddef.h>
#include <stdio.h>

namespace poutre {
namespace simd {
template <typename T, typename U, typename UnOp>
U *transform(T const *first, T const *last, U *out,
             UnOp f) POUTRE_NOEXCEPTONLYNDEBUG {
  POUTRE_ASSERTCHECK(!first, "null ptr");
  POUTRE_ASSERTCHECK(!last, "null ptr");
  POUTRE_ASSERTCHECK(!out, "null ptr");

  using simd_type_T = typename TypeTraits<T>::simd_type;
  using simd_type_U = typename TypeTraits<U>::simd_type;

  static_assert(simd_type_T::size == simd_type_U::size,
                "mismatch length between T and U");

  static_assert(sizeof(T) >= alignof(T),
                "can't generate simd code for this kind of type");

  static_assert(sizeof(U) >= alignof(U),
                "can't generate simd code for this kind of type");

  const auto alignment = SIMD_IDEAL_MAX_ALIGN_BYTES;

  // Define loop counter
  const auto simd_size = TypeTraits<T>::simd_loop_step;
  const auto size = std::distance(first, last);
  // note enforce that input is aligned when we start the main simd loop
  const auto range = simd::t_SIMDInputRange(first, last);
  const auto size_prologue_loop = range.first;
  const auto size_simd_loop = range.second;

  auto i = 0;

  //---prologue
  for (; i < size_prologue_loop; ++i) {
    *out++ = f(*first++);
  }

  //---main simd loop
  if (simd::IsAligned(out, alignment)) {
    for (; i < size_simd_loop; i += simd_size) {
      auto element = xs::load_aligned(first);
      xs::store_aligned(out, f(element));
      first += simd_size;
      out += simd_size;
    }
  } else {
    for (; i < size_simd_loop; i += simd_size) {
      auto element = xs::load_aligned(first);
      xs::store_unaligned(out, f(element));
      first += simd_size;
      out += simd_size;
    }
  }
  //---epilogue
  for (; i < size; ++i) {
    *out++ = f(*first++);
  }
  return out;
}

template <typename T1, typename T2, typename U, typename BinOp>
U *transform(T1 const *first1, T1 const *last1, T2 const *first2, U *out,
             BinOp f) POUTRE_NOEXCEPTONLYNDEBUG {
  POUTRE_ASSERTCHECK(!first1, "null ptr");
  POUTRE_ASSERTCHECK(!last1, "null ptr");
  POUTRE_ASSERTCHECK(!first2, "null ptr");
  POUTRE_ASSERTCHECK(!out, "null ptr");

  using simd_type_T1 = typename TypeTraits<T1>::simd_type;
  using simd_type_T2 = typename TypeTraits<T2>::simd_type;
  using simd_type_U = typename TypeTraits<U>::simd_type;

  static_assert(simd_type_T1::size == simd_type_T2::size,
                "mismatch length between T1 and T2");
  static_assert(simd_type_T1::size == simd_type_U::size,
                "mismatch length between T1 and U");
  static_assert(simd_type_T2::size == simd_type_U::size,
                "mismatch length between T2 and U");
  static_assert(sizeof(T1) >= alignof(T1),
                "can't generate simd code for this kind of type");
  static_assert(sizeof(T2) >= alignof(T2),
                "can't generate simd code for this kind of type");
  static_assert(sizeof(U) >= alignof(U),
                "can't generate simd code for this kind of type");
  auto alignment = SIMD_IDEAL_MAX_ALIGN_BYTES;

  // Define loop counter
  const auto simd_size = TypeTraits<T1>::simd_loop_step;
  const auto size = std::distance(first1, last1);
  const auto range = simd::t_SIMDInputRange(first1, last1);
  const auto size_prologue_loop = range.first;
  const auto size_simd_loop = range.second;

  auto i = 0;

  //---prologue
  for (; i < size_prologue_loop; ++i) {
    *out++ = f(*first1++, *first2++);
  }

  //---main simd loop
  if (simd::IsAligned(first2, alignment) && simd::IsAligned(out, alignment)) {
    for (; i < size_simd_loop; i += simd_size) {
      simd_type_T1 element1 = xs::load_aligned(first1);
      simd_type_T2 element2 = xs::load_aligned(first2);
      xs::store_aligned(out, f(element1, element2));
      first1 += simd_size;
      first2 += simd_size;
      out += simd_size;
    }
  } else {
    for (; i < size_simd_loop; i += simd_size) {
      simd_type_T1 element1 = xs::load_aligned(first1);
      simd_type_T2 element2 = xs::load_unaligned(first2);
      xs::store_unaligned(out, f(element1, element2));
      first1 += simd_size;
      first2 += simd_size;
      out += simd_size;
    }
  }
  //---epilogue
  for (; i < size; ++i) {
    *out++ = f(*first1++, *first2++);
  }
  return out;
}
} // namespace simd
} // namespace poutre
#endif // POUTRE_SIMD_ALGORITHM_HPP__
