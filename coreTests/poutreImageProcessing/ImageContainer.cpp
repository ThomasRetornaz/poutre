#include "main.hpp"

#include <iostream>
#include <poutreImageProcessing/core/include/poutreImageProcessingContainer.hpp>

BOOST_AUTO_TEST_SUITE(poutreImageProcessingContainer)

BOOST_AUTO_TEST_CASE(ctor)
  {
  poutre::DenseImage<poutre::PType::PType_GrayUINT8> img({ 3, 4 });
  BOOST_CHECK_EQUAL(img.GetImgType( ), poutre::ImgType::ImgType_Dense);
  BOOST_CHECK_EQUAL(img.GetPType( ), poutre::PType::PType_GrayUINT8);
  BOOST_CHECK_EQUAL(img.size( ), 12);
  BOOST_CHECK_EQUAL(img.max_size( ), 12);
  BOOST_CHECK_EQUAL(img.GetNumDims( ), 2);
  BOOST_CHECK(!img.empty( ));
  auto coords = img.GetCoords( );
  auto expectedcoords = { 3, 4 };
  BOOST_CHECK_EQUAL_COLLECTIONS(coords.begin( ), coords.end( ), expectedcoords.begin( ), expectedcoords.end( ));

  /*std::cout << "**************************************************" << std::endl;
  std::cout << img << std::endl;
  std::cout << "**************************************************" << std::endl;*/
  }

BOOST_AUTO_TEST_CASE(clone)
  {
  using ImageType = poutre::DenseImage < poutre::PType::PType_GrayUINT8 > ;
  using ImageTypeInterface = std::unique_ptr < poutre::IInterface > ;
  ImageType img({ 3, 4 });
  BOOST_CHECK_EQUAL(img.GetPType( ), poutre::PType::PType_GrayUINT8);
  img.assign(10);

  ImageTypeInterface clonedinterface = img.Clone( );

  ImageType* cloned = dynamic_cast<ImageType*>(clonedinterface.get( ));
  BOOST_CHECK(cloned);
  //!= address
  BOOST_CHECK_NE(&(*cloned), &(img));
  //!= data address
  BOOST_CHECK_NE(&(*((*cloned).datas( ))), &(*(img.datas( ))));
  BOOST_CHECK_EQUAL((*cloned).GetPType( ), poutre::PType::PType_GrayUINT8);
  BOOST_CHECK_EQUAL((*cloned).GetImgType( ), poutre::ImgType::ImgType_Dense);
  BOOST_CHECK_EQUAL((*cloned).size( ), 12);
  BOOST_CHECK_EQUAL((*cloned).max_size( ), 12);
  BOOST_CHECK_EQUAL((*cloned).GetNumDims( ), 2);
  BOOST_CHECK(!(*cloned).empty( ));
  auto coords = (*cloned).GetCoords( );
  auto expectedcoords = { 3, 4 };
  BOOST_CHECK_EQUAL_COLLECTIONS(coords.begin( ), coords.end( ), expectedcoords.begin( ), expectedcoords.end( ));

  for (const auto& var : (*cloned))
    {
    BOOST_CHECK_EQUAL(var, 10);
    }

 /* std::cout << "**************************************************" << std::endl;
  std::cout << (*cloned) << std::endl;
  std::cout << "**************************************************" << std::endl;*/
  }

BOOST_AUTO_TEST_CASE(Factory)
  {
      {
      auto img = poutre::CreateDense({ 3, 4 }, poutre::PType::PType_Bin);
      BOOST_REQUIRE(img);
      BOOST_CHECK_EQUAL((*img).GetImgType( ), poutre::ImgType::ImgType_Dense);
      BOOST_CHECK_EQUAL((*img).GetPType( ), poutre::PType::PType_Bin);
      auto coords = (*img).GetCoords( );
      auto expectedcoords = { 3, 4 };
      BOOST_CHECK_EQUAL_COLLECTIONS(coords.begin( ), coords.end( ), expectedcoords.begin( ), expectedcoords.end( ));
      BOOST_CHECK(dynamic_cast<poutre::DenseImage < poutre::PType::PType_Bin >*>(img.get( )));
      }

              {
              auto img = poutre::CreateDense({ 3, 4 }, poutre::PType::PType_GrayUINT8);
              BOOST_REQUIRE(img);
              BOOST_CHECK_EQUAL((*img).GetImgType( ), poutre::ImgType::ImgType_Dense);
              BOOST_CHECK_EQUAL((*img).GetPType( ), poutre::PType::PType_GrayUINT8);
              auto coords = (*img).GetCoords( );
              auto expectedcoords = { 3, 4 };
              BOOST_CHECK_EQUAL_COLLECTIONS(coords.begin( ), coords.end( ), expectedcoords.begin( ), expectedcoords.end( ));
              BOOST_CHECK(dynamic_cast<poutre::DenseImage < poutre::PType::PType_GrayUINT8 >*>(img.get( )));
                }
                        {
                        auto img = poutre::CreateDense({ 3, 4 }, poutre::PType::PType_GrayINT32);
                        BOOST_REQUIRE(img);
                        BOOST_CHECK_EQUAL((*img).GetImgType( ), poutre::ImgType::ImgType_Dense);
                        BOOST_CHECK_EQUAL((*img).GetPType( ), poutre::PType::PType_GrayINT32);
                        auto coords = (*img).GetCoords( );
                        auto expectedcoords = { 3, 4 };
                        BOOST_CHECK_EQUAL_COLLECTIONS(coords.begin( ), coords.end( ), expectedcoords.begin( ), expectedcoords.end( ));
                        BOOST_CHECK(dynamic_cast<poutre::DenseImage < poutre::PType::PType_GrayINT32 >*>(img.get( )));
                          }
                        {
                        auto img = poutre::CreateDense({ 3, 4 }, poutre::PType::PType_F32);
                        BOOST_REQUIRE(img);
                        BOOST_CHECK_EQUAL((*img).GetImgType( ), poutre::ImgType::ImgType_Dense);
                        BOOST_CHECK_EQUAL((*img).GetPType( ), poutre::PType::PType_F32);
                        auto coords = (*img).GetCoords( );
                        auto expectedcoords = { 3, 4 };
                        BOOST_CHECK_EQUAL_COLLECTIONS(coords.begin( ), coords.end( ), expectedcoords.begin( ), expectedcoords.end( ));
                        BOOST_CHECK(dynamic_cast<poutre::DenseImage < poutre::PType::PType_F32 >*>(img.get( )));
                          }
                            {
                            auto img = poutre::CreateDense({ 3, 4 }, poutre::PType::PType_GrayINT64);
                            BOOST_REQUIRE(img);
                            BOOST_CHECK_EQUAL((*img).GetImgType( ), poutre::ImgType::ImgType_Dense);
                            BOOST_CHECK_EQUAL((*img).GetPType( ), poutre::PType::PType_GrayINT64);
                            auto coords = (*img).GetCoords( );
                            auto expectedcoords = { 3, 4 };
                            BOOST_CHECK_EQUAL_COLLECTIONS(coords.begin( ), coords.end( ), expectedcoords.begin( ), expectedcoords.end( ));
                            BOOST_CHECK(dynamic_cast<poutre::DenseImage < poutre::PType::PType_GrayINT64 >*>(img.get( )));
                              }
                              {
                              BOOST_CHECK_THROW(poutre::CreateDense({ 3, 4 }, poutre::PType::PType_Undef), std::runtime_error);
                                }

  }

BOOST_AUTO_TEST_CASE(BasicIteration)
  {
  poutre::DenseImage<poutre::PType::PType_GrayUINT8> img({ 3, 4 });
  BOOST_CHECK_EQUAL(img.GetPType( ), poutre::PType::PType_GrayUINT8);
  img.assign(10);

  for (const auto& var : img)
    {
    BOOST_CHECK_EQUAL(var, 10);
    }
  auto count = 0;
  for (auto it = img.begin( ); it != img.end( ); ++it, ++count)
    {
    BOOST_CHECK_EQUAL(*it, 10);
    }
  BOOST_CHECK_EQUAL(count, 12);
  count = 0;
  for (auto it = img.cbegin( ); it != img.cend( ); ++it, ++count)
    {
    BOOST_CHECK_EQUAL(*it, 10);
    }
  BOOST_CHECK_EQUAL(count, 12);

  count = 0;
  auto rit = img.rbegin( );
  auto rend = img.rend( );
  for (; rit != rend; ++rit, ++count)
    {
    BOOST_CHECK_EQUAL(*rit, 10);
    }
  BOOST_CHECK_EQUAL(count, 12);

  count = 0;
  auto crit = img.crbegin( );
  auto crend = img.crend( );
  for (; crit != crend; ++crit, ++count)
    {
    BOOST_CHECK_EQUAL(*crit, 10);
    }
  BOOST_CHECK_EQUAL(count, 12);

  for (auto i = 0; i < img.size( ); i++)
    {
    BOOST_CHECK_EQUAL(img[i], 10);
    BOOST_CHECK_EQUAL(img.at(i), 10);
    }
  BOOST_CHECK_EQUAL(img.front( ), 10);
  BOOST_CHECK_EQUAL(img.back( ), 10);
  }

BOOST_AUTO_TEST_CASE(move)
  {
  //todo check assembly
  auto img(poutre::DenseImage<poutre::PType::PType_GrayUINT8>{ 3, 4 });
  BOOST_CHECK_EQUAL(img.GetImgType( ), poutre::ImgType::ImgType_Dense);
  BOOST_CHECK_EQUAL(img.GetPType( ), poutre::PType::PType_GrayUINT8);
  auto coords = img.GetCoords( );
  auto expectedcoords = { 3, 4 };
  BOOST_CHECK_EQUAL_COLLECTIONS(coords.begin( ), coords.end( ), expectedcoords.begin( ), expectedcoords.end( ));

  poutre::DenseImage<poutre::PType::PType_GrayUINT8> imgbin({ 2, 3 });
  BOOST_CHECK_EQUAL(imgbin.GetImgType( ), poutre::ImgType::ImgType_Dense);
  BOOST_CHECK_EQUAL(imgbin.GetPType( ), poutre::PType::PType_GrayUINT8);
  auto coords2 = imgbin.GetCoords( );
  auto expectedcoords2 = { 2, 3 };
  BOOST_CHECK_EQUAL_COLLECTIONS(coords2.begin( ), coords2.end( ), expectedcoords2.begin( ), expectedcoords2.end( ));

  //move 
  imgbin = std::move(img);
  BOOST_CHECK_EQUAL(imgbin.GetImgType( ), poutre::ImgType::ImgType_Dense);
  BOOST_CHECK_EQUAL(imgbin.GetPType( ), poutre::PType::PType_GrayUINT8);
  auto coordsmove = imgbin.GetCoords( );
  auto expectedcoordsmove = { 3, 4 };
  BOOST_CHECK_EQUAL_COLLECTIONS(coordsmove.begin( ), coordsmove.end( ), expectedcoordsmove.begin( ), expectedcoordsmove.end( ));

  }
BOOST_AUTO_TEST_SUITE_END( )



