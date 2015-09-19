//==============================================================================
//                  Copyright (c) 2015 - Thomas Retornaz                      //
//                     thomas.retornaz@mines-paris.org                        //
//          Distributed under the Boost Software License, Version 1.0.        //
//                 See accompanying file LICENSE.txt or copy at               //
//                     http://www.boost.org/LICENSE_1_0.txt                   //
//==============================================================================

#ifndef POUTREIMAGEPROCESSINGITERATORDISPATCH_HPP
#define	POUTREIMAGEPROCESSINGITERATORDISPATCH_HPP

/* 
 * File:   poutreImageProcessingIteratorDispatch.hpp
 * Author: thomas
 *
 * Created on 29 août 2015, 09:26
 */

namespace poutre
{
  /*!
  * @brief Runtime iterator dispatch type
  */
  enum class IteratorDispatchOnViewType
    {
    _IteratorDispatchOnViewType_Max = 1 << 5 //sync with the max value
    };

//  //! operator<< for CompOpType
//  BASE_API std::ostream& operator<<(std::ostream&, IteratorDispatchOnViewType);
//
//  //! operator>> for CompOpType
//  BASE_API std::istream& operator>>(std::istream&, IteratorDispatchOnViewType&);
  
}//poutre


#endif	/* POUTREIMAGEPROCESSINGITERATORDISPATCH_HPP */

