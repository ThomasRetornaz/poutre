
//==============================================================================
//                  Copyright (c) 2015 - Thomas Retornaz                      //
//                     thomas.retornaz@mines-paris.org                        //
//          Distributed under the Boost Software License, Version 1.0.        //
//                 See accompanying file LICENSE.txt or copy at               //
//                     http://www.boost.org/LICENSE_1_0.txt                   //
//==============================================================================

#ifndef POUTRE_IMAGEPROCESSING_INTERFACE_HPP__
#define POUTRE_IMAGEPROCESSING_INTERFACE_HPP__

/**
 * @file   poutreImageProcessingInterface.hpp
 * @author Thomas Retornaz
 * @brief  Define Image interface
 * 
 * 
 */

#ifndef POUTRE_IMAGEPROCESSINGCORE_HPP__
#include <poutreImageProcessingCore/poutreImageProcessingCore.hpp>
#endif

#ifndef POUTRE_IMAGEPROCESSING_TYPE_HPP__
#include <poutreImageProcessingCore/poutreImageProcessingType.hpp>
#endif

#include <memory>
#include <vector> 


namespace poutre
{
/**
 * @addtogroup image_processing_group Image Processing Group
 *
 *@{
 */

/**
 * @addtogroup image_processing_interface_group Image Processing Interface API
 * @ingroup image_processing_group 
 *@{
 */
   
   /**
    * @brief Pure interface class for all "Image" object
    */

   class IMP_CORE_API IInterface
   {
   public:
      //! Provid deep copy
      virtual std::unique_ptr<IInterface> Clone() const = 0;
      //! Get @c PType belong to @c IInterface
      virtual PType GetPType() const = 0 ;
      //! Get @c CompoundType belong to @c IInterface
      virtual CompoundType GetCType( ) const = 0 ;
      //! Get @c ImgType belong to @c IInterface
      virtual ImgType GetImgType( ) const = 0 ;
      //! Get size over dimensions 
      virtual std::vector<std::size_t> GetCoords( ) const = 0 ; //@TODO
                                                               //may
                                                               //change this
      //! Get size num of dimensions 
      virtual std::size_t GetNumDims( ) const = 0 ;
      //!Dtor
      virtual ~IInterface() = default;
      //!Stringification 
      virtual std::string str( ) const = 0 ;
   };


   //! operator<< for IInterface
   //@warning Serialize a description of image. Doesn't serialize data. Use @c poutreIO::ImageFromString,@c poutreIO::ImageToString 
   IMP_CORE_API std::ostream& operator<<(std::ostream&, const IInterface& img);

/**
 * @brief Check if images are compatible regarding sizes
 * 
 * @param i_img1 [in] input image
 * @param i_img2 [in] input image
 *  
 * @return true if sizes of provided images are compatible 
 */
   IMP_CORE_API bool AsSizesCompatible(const IInterface& i_img1, const IInterface& i_img2);

/**
 * @brief Check if images are compatible regarding type ( @c CompoundType, @c PType)
 * 
 * @param i_img1 [in] input image
 * @param i_img2 [in] input image
 *  
 * @return true if sizes of provided images are compatible 
 */
   IMP_CORE_API bool AsTypesCompatible(const IInterface& i_img1, const IInterface& i_img2);

   //! @throw runtime_error if input images  are not compatible @see @c AsSizesCompatible
   IMP_CORE_API void AssertSizesCompatible(const IInterface& i_img1, const IInterface& i_img2, const std::string& i_msg);

   //! @throw runtime_error if input images  are not compatible @see @c AsTypesCompatible     
   IMP_CORE_API void AssertAsTypesCompatible(const IInterface& i_img1, const IInterface& i_img2, const std::string& i_msg);

   //TODO dispatch with sparse type
   //BASE_API std::unique_ptr<IInterface> Create(const std::initializer_list<size_t>& dims, ImgType imgType, PType ptype);

   //! Factory to build contiguous dense image
   IMP_CORE_API std::unique_ptr<IInterface> CreateDense(const std::vector<std::size_t>& dims, CompoundType ctype, PType ptype);
//! @} doxygroup: image_processing_group
     
}
#endif//POUTRE_IMAGEPROCESSING_INTERFACE_HPP__


