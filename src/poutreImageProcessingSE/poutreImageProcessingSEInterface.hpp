
//==============================================================================
//                  Copyright (c) 2018 - Thomas Retornaz                      //
//                     thomas.retornaz@mines-paris.org                        //
//          Distributed under the Boost Software License, Version 1.0.        //
//                 See accompanying file LICENSE.txt or copy at               //
//                     http://www.boost.org/LICENSE_1_0.txt                   //
//==============================================================================

#ifndef POUTRE_IPSEINTERFACE_HPP__
#define POUTRE_IPSEINTERFACE_HPP__

#include <poutreImageProcessingSE/include/poutreImageProcessingSETypesAndTags.hpp>
#include <poutreImageProcessingSE/poutreImageProcessingSECore.hpp>

/**
 * @file   poutreImageProcessingSEInterface.hpp
 * @author Thomas Retornaz
 * @brief  Define Structuring element type and interface
 *
 *
 */

namespace poutre
{
    namespace se
    {
        /**
         * @addtogroup se_grp Structuring Element Group
         *
         *@{

         */

        /**
         * @brief Pure interface class for all "SE" object
         */
        class IPPSE_API IStructuringElement
        {
          public:
            //! Get @c SEType belong to @c IStructuringElement
            virtual SEType GetSEType() const = 0;

            //! Provide deep copy
            virtual std::unique_ptr<IStructuringElement> Clone() const = 0;

            //! Returns a new structuring element that is a transposed copy
            virtual std::unique_ptr<IStructuringElement> Transpose() const = 0;

            //! Returns the size of the structuring element in number of neighbors (the center, if defined, is included)
            virtual size_t GetSize() const = 0;

            //! Dtor
            virtual ~IStructuringElement() = default;

            const IStructuringElement &operator=(const IStructuringElement &) = delete; // see this later
        };

        //! operator<< for IStructuringElement
        //@warning Serialize a description of StructuringElement. Doesn't serialize data. Use @c TODO,@c TODO
        IPPSE_API std::ostream &operator<<(std::ostream &, const IStructuringElement &ise);

    } // namespace se
    //! @} // se_grp
} // namespace poutre
#endif // POUTRE_IPSEINTERFACE_HPP__