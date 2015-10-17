
//==============================================================================
//                  Copyright (c) 2015 - Thomas Retornaz                      //
//                     thomas.retornaz@mines-paris.org                        //
//          Distributed under the Boost Software License, Version 1.0.        //
//                 See accompanying file LICENSE.txt or copy at               //
//                     http://www.boost.org/LICENSE_1_0.txt                   //
//==============================================================================

#include "main.hpp"
#include "poutreBase/poutreGeometry.hpp"

#include <iostream>
#include <poutreImageProcessing/core/include/poutreImageProcessingContainer.hpp>
#include <boost/lexical_cast.hpp>

namespace poutre
{

  enum class DispatchView
  {
    DispatchViewUndef = 0, //!< Undefined dispatch
    DispatchViewCompatibleOffsetSamePtrType = 1 << 0, //!< Offset Compatible. Ptr arthimetic compatible
    DispatchViewCompatibleOffset = 1 << 1, //!< Offset Compatible. Iterate with one offset
    DispatchViewCompatibleArrayView = 1 << 2, //!< Ptr Compatible. Iterate with two ptr 
    DispatchViewAtLeastOneIsAStridedView = 1 << 3, //!< One is trided fallback to index iteration
  };

  std::ostream& operator<<(std::ostream& os, DispatchView dispatchview)
  {
    switch (dispatchview)
      {
      case DispatchView::DispatchViewCompatibleOffsetSamePtrType:
        os << "DispatchViewCompatibleOffsetSamePtrType";
        break;
      case DispatchView::DispatchViewCompatibleOffset:
        os << "DispatchViewCompatibleOffset";
        break;
      case DispatchView::DispatchViewCompatibleArrayView:
        os << "DispatchViewCompatibleArrayView";
        break;
      case DispatchView::DispatchViewAtLeastOneIsAStridedView:
        os << "DispatchViewAtLeastOneIsAStridedView";
        break;
      default://  enumDispatchView::DispatchViewUndef; 
        os << "Unknown dispatchView";
        break;
      }
    return os;
  }

  std::istream& operator>>(std::istream& is, DispatchView& dispatchview)
  {
    dispatchview = DispatchView::DispatchViewUndef;

    if (!is.good())
      return is;

    std::string strType;
    is >> strType;
    if (is.bad())
      return is;
    if (strType == "DispatchViewCompatibleOffsetSamePtrType")
      dispatchview = DispatchView::DispatchViewCompatibleOffsetSamePtrType;
    else if (strType == "DispatchViewCompatibleOffset")
      dispatchview = DispatchView::DispatchViewCompatibleOffset;
    else if (strType == "DispatchViewCompatibleArrayView")
      dispatchview = DispatchView::DispatchViewCompatibleArrayView;
    else if (strType == "DispatchViewAtLeastOneIsAStridedView")
      dispatchview = DispatchView::DispatchViewAtLeastOneIsAStridedView;
    else
      {
        POUTRE_RUNTIME_ERROR("Unable to read dispatchview from stream");
      }
    return is;
  }

  template <poutre::DispatchView dispatchview>
  struct pApplyImageUnaryViewOp_dispatcher
  {
  };

  template <>
  struct pApplyImageUnaryViewOp_dispatcher<DispatchView::DispatchViewCompatibleOffsetSamePtrType>
  {

    template </*class UnOp,*/ class ViewIn, class ViewOut, class ImageIn, class ImageOut>
    void operator()(/*UnOp& op,*/ const ViewIn vIn, ViewOut vOut, ImageIn&, ImageOut&) const
    {
      std::cout << "\n" << "call pApplyImageUnaryViewOp<DispatchView::DispatchViewCompatibleOffsetSamePtrType>";
    }
  };

  struct pApplyImageUnaryViewOp
  {

    template </*class UnOp,*/ class ViewIn, class ViewOut, class ImageIn, class ImageOut>
    void operator()(/*UnOp& op,*/ const ViewIn vIn, ViewOut vOut, ImageIn&, ImageOut&) const
    {
      //debug precondition
      POUTRE_ASSERTCHECK(vIn.size() == vOut.size(), "pApplyImageUnaryIterOp size of view are not compatible");

      //extract appropriate tag dispatch from view properties
      auto dispatch = DispatchView::DispatchViewUndef;
      auto vInbound = vIn.bound();
      auto vOutbound = vOut.bound();
      auto stridevIN = vIn.stride();
      auto stridevOut = vOut.stride();

      if ((vInbound == vOutbound) && (stridevIN == stridevOut))
        {
          if (std::is_same<ViewIn, ViewOut>::value)
            {
              //fallback ptr + simd 
              dispatch = poutre::DispatchView::DispatchViewCompatibleOffsetSamePtrType;
            }
          else
            {
              //fallback ptr 
              dispatch = poutre::DispatchView::DispatchViewCompatibleOffset;
            }
        }
      else
        {
          dispatch = poutre::DispatchView::DispatchViewAtLeastOneIsAStridedView;
        }
      std::cout << "\n***********************";
      std::cout << "\n" << dispatch;
      std::cout << "\n***********************";
    }
  };

}//namespace poutre

BOOST_AUTO_TEST_SUITE(poutreImageProcessingContainer)


BOOST_AUTO_TEST_CASE(unaryopviewcompatibleoffset)
{
  poutre::DenseImage<poutre::pUINT8> img1({3, 4});
  poutre::DenseImage<poutre::pUINT8> img2({3, 4});
  auto v_img1 = poutre::view(img1);
  auto v_img2 = poutre::view(img2);
  poutre::pApplyImageUnaryViewOp op1;
  op1(v_img1, v_img2, img1, img2);
  //throw in debug
  //#ifndef NDEBUG   
  //    BOOST_CHECK_THROW(poutre::view(img3),std::runtime_error);
  //#endif    

}

BOOST_AUTO_TEST_CASE(unaryopviewcompatibleviewDifferenttype)
{
  poutre::DenseImage<poutre::pUINT8> img1({5, 6});
  poutre::DenseImage<poutre::pINT32> img2({5, 6});
  auto v_img1 = poutre::view(img1);
  auto v_img2 = poutre::view(img2);
  poutre::pApplyImageUnaryViewOp op1;
  op1(v_img1, v_img2, img1, img2);
  //throw in debug
  //#ifndef NDEBUG   
  //    BOOST_CHECK_THROW(poutre::view(img3),std::runtime_error);
  //#endif    

}

BOOST_AUTO_TEST_CASE(ctor)
{
  poutre::DenseImage<poutre::pUINT8> img({3, 4});
  BOOST_CHECK_EQUAL(img.GetImgType(), poutre::ImgType::ImgType_Dense);
  BOOST_CHECK_EQUAL(img.GetPType(), poutre::PType::PType_GrayUINT8);
  BOOST_CHECK_EQUAL(img.GetCType(), poutre::CompoundType::CompoundType_Scalar);
  BOOST_CHECK_EQUAL(img.size(), 12);
  BOOST_CHECK_EQUAL(img.max_size(), 12);
  BOOST_CHECK_EQUAL(img.GetNumDims(), 2);
  BOOST_CHECK(!img.empty());
  auto coords = img.GetCoords();
  auto expectedcoords = {3, 4};
  BOOST_CHECK_EQUAL_COLLECTIONS(coords.begin(), coords.end(), expectedcoords.begin(), expectedcoords.end());

  /*std::cout << "**************************************************" << std::endl;
  std::cout << img << std::endl;
  std::cout << "**************************************************" << std::endl;*/
}

BOOST_AUTO_TEST_CASE(clone)
{
  using ImageType = poutre::DenseImage < poutre::pUINT8 >;
  using ImageTypeInterface = std::unique_ptr < poutre::IInterface >;
  ImageType img({3, 4});
  BOOST_CHECK_EQUAL(img.GetPType(), poutre::PType::PType_GrayUINT8);
  img.assign(10);

  ImageTypeInterface clonedinterface = img.Clone();

  ImageType* cloned = dynamic_cast<ImageType*> (clonedinterface.get());
  BOOST_CHECK(cloned);
  //!= address
  BOOST_CHECK_NE(cloned, &(img));
  //!= data address
  BOOST_CHECK_NE(&(*((*cloned).datas())), &(*(img.datas())));
  BOOST_CHECK_EQUAL((*cloned).GetPType(), poutre::PType::PType_GrayUINT8);
  BOOST_CHECK_EQUAL((*cloned).GetCType(), poutre::CompoundType::CompoundType_Scalar);
  BOOST_CHECK_EQUAL((*cloned).GetImgType(), poutre::ImgType::ImgType_Dense);
  BOOST_CHECK_EQUAL((*cloned).size(), 12);
  BOOST_CHECK_EQUAL((*cloned).max_size(), 12);
  BOOST_CHECK_EQUAL((*cloned).GetNumDims(), 2);
  BOOST_CHECK(!(*cloned).empty());
  auto coords = (*cloned).GetCoords();
  auto expectedcoords = {3, 4};
  BOOST_CHECK_EQUAL_COLLECTIONS(coords.begin(), coords.end(), expectedcoords.begin(), expectedcoords.end());

  for (const auto& var : (*cloned))
    {
      BOOST_CHECK_EQUAL(var, 10);
    }

  //  std::cout << "**************************************************" << std::endl;
  //  std::cout << (*cloned) << std::endl;
  //  std::cout << "**************************************************" << std::endl;
}

BOOST_AUTO_TEST_CASE(Factory)
{
  {
    auto img = poutre::CreateDense({3, 4}, poutre::CompoundType::CompoundType_Scalar, poutre::PType::PType_GrayUINT8);
    BOOST_REQUIRE(img);
    BOOST_CHECK_EQUAL((*img).GetImgType(), poutre::ImgType::ImgType_Dense);
    BOOST_CHECK_EQUAL((*img).GetCType(), poutre::CompoundType::CompoundType_Scalar);
    BOOST_CHECK_EQUAL((*img).GetPType(), poutre::PType::PType_GrayUINT8);
    auto coords = (*img).GetCoords();
    auto expectedcoords = {3, 4};
    BOOST_CHECK_EQUAL_COLLECTIONS(coords.begin(), coords.end(), expectedcoords.begin(), expectedcoords.end());
    BOOST_CHECK(dynamic_cast<poutre::DenseImage < poutre::pUINT8 >*> (img.get()));
  }

  {
    auto img = poutre::CreateDense({3, 4}, poutre::CompoundType::CompoundType_Scalar, poutre::PType::PType_GrayINT32);
    BOOST_REQUIRE(img);
    BOOST_CHECK_EQUAL((*img).GetImgType(), poutre::ImgType::ImgType_Dense);
    BOOST_CHECK_EQUAL((*img).GetCType(), poutre::CompoundType::CompoundType_Scalar);
    BOOST_CHECK_EQUAL((*img).GetPType(), poutre::PType::PType_GrayINT32);
    auto coords = (*img).GetCoords();
    auto expectedcoords = {3, 4};
    BOOST_CHECK_EQUAL_COLLECTIONS(coords.begin(), coords.end(), expectedcoords.begin(), expectedcoords.end());
    BOOST_CHECK(dynamic_cast<poutre::DenseImage < poutre::pINT32>*> (img.get()));
  }
  {
    auto img = poutre::CreateDense({3, 4}, poutre::CompoundType::CompoundType_Scalar, poutre::PType::PType_F32);
    BOOST_REQUIRE(img);
    BOOST_CHECK_EQUAL((*img).GetImgType(), poutre::ImgType::ImgType_Dense);
    BOOST_CHECK_EQUAL((*img).GetCType(), poutre::CompoundType::CompoundType_Scalar);
    BOOST_CHECK_EQUAL((*img).GetPType(), poutre::PType::PType_F32);
    auto coords = (*img).GetCoords();
    auto expectedcoords = {3, 4};
    BOOST_CHECK_EQUAL_COLLECTIONS(coords.begin(), coords.end(), expectedcoords.begin(), expectedcoords.end());
    BOOST_CHECK(dynamic_cast<poutre::DenseImage < poutre::pFLOAT>*> (img.get()));
  }
  {
    auto img = poutre::CreateDense({3, 4}, poutre::CompoundType::CompoundType_Scalar, poutre::PType::PType_GrayINT64);
    BOOST_REQUIRE(img);
    BOOST_CHECK_EQUAL((*img).GetImgType(), poutre::ImgType::ImgType_Dense);
    BOOST_CHECK_EQUAL((*img).GetPType(), poutre::PType::PType_GrayINT64);
    BOOST_CHECK_EQUAL((*img).GetCType(), poutre::CompoundType::CompoundType_Scalar);
    auto coords = (*img).GetCoords();
    auto expectedcoords = {3, 4};
    BOOST_CHECK_EQUAL_COLLECTIONS(coords.begin(), coords.end(), expectedcoords.begin(), expectedcoords.end());
    BOOST_CHECK(dynamic_cast<poutre::DenseImage < poutre::pINT64>*> (img.get()));
  }
  {
    BOOST_CHECK_THROW(poutre::CreateDense({3, 4}, poutre::CompoundType::CompoundType_Scalar, poutre::PType::PType_Undef), std::runtime_error);
    BOOST_CHECK_THROW(poutre::CreateDense({3, 4}, poutre::CompoundType::CompoundType_Undef, poutre::PType::PType_GrayINT64), std::runtime_error);
  }

}

BOOST_AUTO_TEST_CASE(BasicIteration)
{
  poutre::DenseImage<poutre::pUINT8> img({3, 4});
  BOOST_CHECK_EQUAL(img.GetPType(), poutre::PType::PType_GrayUINT8);
  BOOST_CHECK_EQUAL(img.GetCType(), poutre::CompoundType::CompoundType_Scalar);
  img.assign(10);

  for (const auto& var : img)
    {
      BOOST_CHECK_EQUAL(var, 10);
    }
  auto count = 0;
  for (auto it = img.begin(); it != img.end(); ++it, ++count)
    {
      BOOST_CHECK_EQUAL(*it, 10);
    }
  BOOST_CHECK_EQUAL(count, 12);
  count = 0;
  for (auto it = img.cbegin(); it != img.cend(); ++it, ++count)
    {
      BOOST_CHECK_EQUAL(*it, 10);
    }
  BOOST_CHECK_EQUAL(count, 12);

  count = 0;
  auto rit = img.rbegin();
  auto rend = img.rend();
  for (; rit != rend; ++rit, ++count)
    {
      BOOST_CHECK_EQUAL(*rit, 10);
    }
  BOOST_CHECK_EQUAL(count, 12);

  count = 0;
  auto crit = img.crbegin();
  auto crend = img.crend();
  for (; crit != crend; ++crit, ++count)
    {
      BOOST_CHECK_EQUAL(*crit, 10);
    }
  BOOST_CHECK_EQUAL(count, 12);

  for (auto i = 0; i < img.size(); i++)
    {
      BOOST_CHECK_EQUAL(img[i], 10);
      BOOST_CHECK_EQUAL(img.at(i), 10);
    }
  BOOST_CHECK_EQUAL(img.front(), 10);
  BOOST_CHECK_EQUAL(img.back(), 10);
}

BOOST_AUTO_TEST_CASE(move)
{
  //todo check assembly
  auto img(poutre::DenseImage<poutre::pUINT8>{3, 4});
  BOOST_CHECK_EQUAL(img.GetImgType(), poutre::ImgType::ImgType_Dense);
  BOOST_CHECK_EQUAL(img.GetPType(), poutre::PType::PType_GrayUINT8);
  BOOST_CHECK_EQUAL(img.GetCType(), poutre::CompoundType::CompoundType_Scalar);

  auto coords = img.GetCoords();
  auto expectedcoords = {3, 4};
  BOOST_CHECK_EQUAL_COLLECTIONS(coords.begin(), coords.end(), expectedcoords.begin(), expectedcoords.end());

  poutre::DenseImage<poutre::pUINT8> img2({2, 3});
  BOOST_CHECK_EQUAL(img2.GetImgType(), poutre::ImgType::ImgType_Dense);
  BOOST_CHECK_EQUAL(img2.GetPType(), poutre::PType::PType_GrayUINT8);
  BOOST_CHECK_EQUAL(img2.GetCType(), poutre::CompoundType::CompoundType_Scalar);
  auto coords2 = img2.GetCoords();
  auto expectedcoords2 = {2, 3};
  BOOST_CHECK_EQUAL_COLLECTIONS(coords2.begin(), coords2.end(), expectedcoords2.begin(), expectedcoords2.end());

  //move 
  img2 = std::move(img);
  BOOST_CHECK_EQUAL(img2.GetImgType(), poutre::ImgType::ImgType_Dense);
  BOOST_CHECK_EQUAL(img2.GetPType(), poutre::PType::PType_GrayUINT8);
  BOOST_CHECK_EQUAL(img2.GetCType(), poutre::CompoundType::CompoundType_Scalar);
  auto coordsmove = img2.GetCoords();
  auto expectedcoordsmove = {3, 4};
  BOOST_CHECK_EQUAL_COLLECTIONS(coordsmove.begin(), coordsmove.end(), expectedcoordsmove.begin(), expectedcoordsmove.end());

}

BOOST_AUTO_TEST_CASE(viewoverimage)
{
  poutre::DenseImage<poutre::pUINT8> img1({3, 4});
  //start view
  auto vimg1 = poutre::view(img1);
  BOOST_CHECK_EQUAL(vimg1.size(), 12);
  BOOST_CHECK_EQUAL(vimg1.bound(), (poutre::bd2d{3, 4}));
  BOOST_CHECK_EQUAL(vimg1.stride(), (poutre::idx2d{4, 1}));


}


BOOST_AUTO_TEST_SUITE_END()



