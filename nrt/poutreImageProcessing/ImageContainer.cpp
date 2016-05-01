
//==============================================================================
//                  Copyright (c) 2015 - Thomas Retornaz                      //
//                     thomas.retornaz@mines-paris.org                        //
//          Distributed under the Boost Software License, Version 1.0.        //
//                 See accompanying file LICENSE.txt or copy at               //
//                     http://www.boost.org/LICENSE_1_0.txt                   //
//==============================================================================

#include "main.hpp"
#include "poutreBase/poutreGeometry.hpp"
#include <vector>
#include <iostream>
#include <poutreImageProcessing/core/include/poutreImageProcessingContainer.hpp>
#include <poutreImageProcessing/core/include/poutreImageProcessingUnaryOp.hpp>
#include <boost/lexical_cast.hpp>


template <typename value_type1, typename value_type2>
struct dummy
{
private:
    size_t count;
    std::vector<value_type1> m_vect;

public:
    dummy() :count(0), m_vect() {}

    value_type2 operator()(const value_type1 &val) /*const*/
    {
        m_vect.pushback(val);
        count++;
        return static_cast<value_type2>(val);
    }

    //void operator()()
    //{
    //}

    size_t GetCount() const
    {
        return count;
    }

    std::vector<value_type1> GetValues() const
    {
        return m_vect;
    }

};

BOOST_AUTO_TEST_SUITE(poutreImageProcessingContainer)


BOOST_AUTO_TEST_CASE(unaryopviewInPlace)
{
  poutre::DenseImage<poutre::pUINT8> img1({3, 4});
  auto v_img1 = poutre::view(img1);
  poutre::UnaryInPlaceOp(v_img1,dummy<poutre::pUINT8, poutre::pUINT8>());
}

BOOST_AUTO_TEST_CASE(unaryopviewDispatchContiguousDifferentPtrType)
{
  poutre::DenseImage<poutre::pUINT8> img1({5, 6});
  poutre::DenseImage<poutre::pINT32> img2({5, 6});
  auto v_img1 = poutre::view(img1);
  auto v_img2 = poutre::view(img2);
  poutre::UnaryOp(v_img1, v_img2, dummy<poutre::pUINT8,poutre::pINT32>());
}

BOOST_AUTO_TEST_CASE(unaryopviewDispatchContiguousSamePtrType)
{
    poutre::DenseImage<poutre::pUINT8> img1({ 5, 6 });
    poutre::DenseImage<poutre::pUINT8> img2({ 5, 6 });
    auto v_img1 = poutre::view(img1);
    auto v_img2 = poutre::view(img2);
    poutre::UnaryOp(v_img1, v_img2, dummy<poutre::pUINT8, poutre::pUINT8>());
}



BOOST_AUTO_TEST_CASE(unaryopviewDispatchOneIdxSamePtrType)
{
	poutre::DenseImage<poutre::pUINT8> img1({ 5, 6 });
	poutre::DenseImage<poutre::pUINT8> img2({ 5, 6 });
	auto v_img1 = poutre::view(img1);
	auto v_img2 = poutre::view(img2);
	//Extract section
	auto s_img1 = v_img1.section({1,1});
	auto s_img2 = v_img2.section({1,1});

	poutre::UnaryOp(s_img1, s_img2, dummy<poutre::pUINT8, poutre::pUINT8>());
}

BOOST_AUTO_TEST_CASE(unaryopviewDispatchOneIdxDiferentPtrType)
{
    poutre::DenseImage<poutre::pUINT8> img1({ 5, 6 });
    poutre::DenseImage<poutre::pINT32> img2({ 5, 6 });
    auto v_img1 = poutre::view(img1);
    auto v_img2 = poutre::view(img2);
    //Extract section
    auto s_img1 = v_img1.section({ 1,1 });
    auto s_img2 = v_img2.section({ 1,1 });

    poutre::UnaryOp(s_img1, s_img2, dummy<poutre::pUINT8, poutre::pINT32>());
}

/*
BOOST_AUTO_TEST_CASE(unaryopviewDispatchViewCompatibleShiftedOffset)
{
	poutre::DenseImage<poutre::pUINT8> img1({ 5, 6 });
	poutre::DenseImage<poutre::pUINT8> img2({ 5, 6 });
	auto v_img1 = poutre::view(img1);
	auto v_img2 = poutre::view(img2);
	
	//Extract section
	
	auto s_img1 = v_img1.section({ 1,1 }, { 2,2 });
	auto s_img2 = v_img2.section({ 3,3 }, { 2,2 });

	poutre::pApplyImageUnaryViewOp_dispatcher op1;
    op1.dispatch(dummy(), s_img1, s_img2);
}

BOOST_AUTO_TEST_CASE(unaryopviewDispatchViewAllDiff)
{
	poutre::DenseImage<poutre::pUINT8> img1({ 5, 6 });
	poutre::DenseImage<poutre::pINT32> img2({ 5, 6 });
	auto v_img1 = poutre::view(img1);
	auto v_img2 = poutre::view(img2);
	//Extract section
	auto s_img1 = v_img1.section({ 1,1 }, { 3,2 });
	auto s_img2 = v_img2.section({ 1,1 }, { 2,3 });

	poutre::pApplyImageUnaryViewOp_dispatcher op1;
	op1.dispatch(dummy(), s_img1, s_img2);
}
*/

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



