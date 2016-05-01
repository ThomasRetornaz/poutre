
//==============================================================================
//                  Copyright (c) 2015 - Thomas Retornaz                      //
//                     thomas.retornaz@mines-paris.org                        //
//          Distributed under the Boost Software License, Version 1.0.        //
//                 See accompanying file LICENSE.txt or copy at               //
//                     http://www.boost.org/LICENSE_1_0.txt                   //
//==============================================================================

#include "main.hpp"

#include <iostream>
#include <poutreImageProcessing/core/include/poutreImageProcessingContainer.hpp>
#include <poutreImageProcessing/core/poutreImageProcessingInterfaceCopieConvert.hpp>

BOOST_AUTO_TEST_SUITE(poutreImageProcessingCopyConvert)

BOOST_AUTO_TEST_CASE(Clone)
{
  using ImageType = poutre::DenseImage < poutre::pUINT8 >;
  using ImageTypeInterface = std::unique_ptr < poutre::IInterface >;
  ImageType img({3, 4});
  BOOST_CHECK_EQUAL(img.GetPType(), poutre::PType::PType_GrayUINT8);
  img.assign(10);

  ImageTypeInterface getSameii = poutre::Clone(img);

  ImageType* getSame = dynamic_cast<ImageType*> (getSameii.get());
  BOOST_CHECK(getSame);
  //!= address
  BOOST_CHECK_NE(getSame, &(img));
  //!= data address
  BOOST_CHECK_NE(&(*((*getSame).datas())), &(*(img.datas())));
  BOOST_CHECK_EQUAL((*getSame).GetCType(), poutre::CompoundType::CompoundType_Scalar);
  BOOST_CHECK_EQUAL((*getSame).GetPType(), poutre::PType::PType_GrayUINT8);
  BOOST_CHECK_EQUAL((*getSame).GetImgType(), poutre::ImgType::ImgType_Dense);
  BOOST_CHECK_EQUAL((*getSame).size(), 12);
  BOOST_CHECK_EQUAL((*getSame).max_size(), 12);
  BOOST_CHECK_EQUAL((*getSame).GetNumDims(), 2);
  BOOST_CHECK(!(*getSame).empty());
  auto getSamecoords = (*getSame).GetCoords();
  auto getSameexpectedcoords = {3, 4};
  BOOST_CHECK_EQUAL_COLLECTIONS(getSamecoords.begin(), getSamecoords.end(), getSameexpectedcoords.begin(), getSameexpectedcoords.end());

  for (const auto& var : (*getSame))
    {
      BOOST_CHECK_EQUAL(var, 10);
    }

}

BOOST_AUTO_TEST_CASE(getsamecoord)
{
  using ImageTypeBase = poutre::DenseImage < poutre::pUINT8 >;
  using ImageTypeInterface = std::unique_ptr < poutre::IInterface >;
  ImageTypeBase img({3, 4});
  BOOST_CHECK_EQUAL(img.GetPType(), poutre::PType::PType_GrayUINT8);
  img.assign(10);


  using ImageType = poutre::DenseImage < poutre::pFLOAT>;
  ImageTypeInterface getSameii = poutre::CloneGeometry(img, poutre::PType::PType_F32);

  ImageType* getSame = dynamic_cast<ImageType*> (getSameii.get());
  BOOST_CHECK_EQUAL((*getSame).GetCType(), poutre::CompoundType::CompoundType_Scalar);
  BOOST_CHECK_EQUAL((*getSame).GetPType(), poutre::PType::PType_F32);
  BOOST_CHECK_EQUAL((*getSame).GetImgType(), poutre::ImgType::ImgType_Dense);
  BOOST_CHECK_EQUAL((*getSame).size(), 12);
  BOOST_CHECK_EQUAL((*getSame).max_size(), 12);
  BOOST_CHECK_EQUAL((*getSame).GetNumDims(), 2);
  auto getSamecoords = (*getSame).GetCoords();
  auto getSameexpectedcoords = {3, 4};
  BOOST_CHECK_EQUAL_COLLECTIONS(getSamecoords.begin(), getSamecoords.end(), getSameexpectedcoords.begin(), getSameexpectedcoords.end());
}
BOOST_AUTO_TEST_SUITE_END()



