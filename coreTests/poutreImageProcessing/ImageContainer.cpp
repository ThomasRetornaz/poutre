#include "main.hpp"

#include <iostream>
#include <poutreImageProcessing/core/include/poutreImageProcessingContainer.hpp>

BOOST_AUTO_TEST_SUITE(poutreImageProcessingContainer)

BOOST_AUTO_TEST_CASE(ctor)
  {
  poutre::DenseImage<poutre::PType::PType_GrayUINT8> img({ 3, 4 });
  BOOST_CHECK_EQUAL(img.GetPType(), poutre::PType::PType_GrayUINT8);
  std::cout << "**************************************************"<<std::endl;
  std::cout << img << std::endl;
  std::cout << "**************************************************" << std::endl;
  }

BOOST_AUTO_TEST_CASE(iterate)
  {
  poutre::DenseImage<poutre::PType::PType_GrayUINT8> img({ 3, 4 });
  BOOST_CHECK_EQUAL(img.GetPType(), poutre::PType::PType_GrayUINT8);
  img.assign(10);

  for (const auto& var : img)
    {
    BOOST_CHECK_EQUAL(var, 10);
    }

  }

BOOST_AUTO_TEST_SUITE_END()



