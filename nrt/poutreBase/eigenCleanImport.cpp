#include "main.hpp"
#include <iostream>
#include <Eigen/Dense>

BOOST_AUTO_TEST_SUITE(eigenImport)

BOOST_AUTO_TEST_CASE(tutorial)
  {
  Eigen::Matrix3f A;
  Eigen::Vector3f b;
  A << 1, 2, 3, 4, 5, 6, 7, 8, 10;
  b << 3, 3, 4;
  //std::cout << "Here is the matrix A:\n" << A << std::endl;
  //std::cout << "Here is the vector b:\n" << b << std::endl;
  Eigen::Vector3f x = A.colPivHouseholderQr().solve(b);
  std::vector<float> expected = { -2.f, 0.999997f, 1.f };
  for (size_t i = 0; i < expected.size(); i++)
    {
      BOOST_CHECK_CLOSE(x[i] ,expected[i],1e-3f);
    }
    
  }
BOOST_AUTO_TEST_SUITE_END()