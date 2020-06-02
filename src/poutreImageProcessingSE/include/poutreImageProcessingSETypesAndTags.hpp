
//==============================================================================
//                  Copyright (c) 2018 - Thomas Retornaz                      //
//                     thomas.retornaz@mines-paris.org                        //
//          Distributed under the Boost Software License, Version 1.0.        //
//                 See accompanying file LICENSE.txt or copy at               //
//                     http://www.boost.org/LICENSE_1_0.txt                   //
//==============================================================================

#ifndef POUTRE_IPSETYPES_AND_TAGS_HPP__
#define POUTRE_IPSETYPES_AND_TAGS_HPP__

#ifndef POUTRE_IPSEINTERFACE_HPP__
#include <poutreImageProcessingSE/poutreImageProcessingSEInterface.hpp>
#endif
#ifndef POUTRE_IPSECORE_HPP__
#include <poutreImageProcessingSE/poutreImageProcessingSECore.hpp>
#endif
/**
 * @file   poutreImageProcessingSETypesAndTags.hpp
 * @author Thomas Retornaz
 * @brief  Define Structuring element type and tags
 *
 *
 */

namespace poutre
{
    /**
     * @addtogroup se_nl_grp
     * @ingroup se_grp
     *@{
     */
    namespace se
    {

        struct adaptative_neighbourhood_tag
        {
        };
        struct runtime_neighbourhood_tag : adaptative_neighbourhood_tag
        {
        };
        struct static_neighbourhood_tag : runtime_neighbourhood_tag
        {
        };

        /*! Type of structuring element
         *
         */
        enum class SEType
        {
            SEType_Undef = 0,      //!< Undefined type
            SEType_NL = 1 << 0,    //!< Neighborlist Type
            SEType_Image = 1 << 1, //!< Image Type
            // TODO
            // BOX,line,amibe
        };

        //! operator<< for SEType
        IPPSE_API std::ostream &operator<<(std::ostream &, SEType);

        //! operator>> for SEType
        IPPSE_API std::istream &operator>>(std::istream &, SEType &);

    } // namespace se

    //! @} // se_nl_grp
} // namespace poutre
#endif // POUTRE_IPSETYPES_AND_TAGS_HPP__