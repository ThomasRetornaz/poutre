// This is an open source non-commercial project. Dear PVS-Studio, please check it.

// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com

//==============================================================================
//                  Copyright (c) 2015 - Thomas Retornaz                      //
//                     thomas.retornaz@mines-paris.org                        //
//          Distributed under the Boost Software License, Version 1.0.        //
//                 See accompanying file LICENSE.txt or copy at               //
//                     http://www.boost.org/LICENSE_1_0.txt                   //
//==============================================================================

#include "main.hpp"

#include <iostream>
#include <poutreImageProcessingCore/poutreImageProcessingType.hpp>

BOOST_AUTO_TEST_SUITE(poutreImageProcessingType)

BOOST_AUTO_TEST_CASE(NativeType)
{
  BOOST_CHECK(poutre::get_dim<poutre::pUINT8>::dim == 1);
}

BOOST_AUTO_TEST_CASE(ptype)
{
  //{
  //auto ct = poutre::PType::PType_Bin;
  //std::ostringstream os;
  //os << ct;
  //BOOST_CHECK_EQUAL("Bin", os.str( ));
  //}
  {
    auto ct = poutre::PType::PType_GrayUINT8;
    std::ostringstream os;
    os << ct;
    BOOST_CHECK_EQUAL("GUINT8", os.str());
  }
  {
    auto ct = poutre::PType::PType_GrayINT32;
    std::ostringstream os;
    os << ct;
    BOOST_CHECK_EQUAL("GINT32", os.str());
  }
  {
    auto ct = poutre::PType::PType_GrayINT64;
    std::ostringstream os;
    os << ct;
    BOOST_CHECK_EQUAL("GINT64", os.str());
  }
  {
    auto ct = poutre::PType::PType_F32;
    std::ostringstream os;
    os << ct;
    BOOST_CHECK_EQUAL("F32", os.str());
  }
  {
    auto ct = poutre::PType::PType_D64;
    std::ostringstream os;
    os << ct;
    BOOST_CHECK_EQUAL("D64", os.str());
  }

  /*   {
     std::istringstream stream("Bin");
     auto ct = poutre::PType::PType_Undef;
     stream >> ct;
     BOOST_CHECK_EQUAL(ct, poutre::PType::PType_Bin);
       }*/

  {
    std::istringstream stream("GUINT8");
    auto ct = poutre::PType::PType_Undef;
    stream >> ct;
    BOOST_CHECK_EQUAL(ct, poutre::PType::PType_GrayUINT8);
  }

  {
    std::istringstream stream("GINT32");
    auto ct = poutre::PType::PType_Undef;
    stream >> ct;
    BOOST_CHECK_EQUAL(ct, poutre::PType::PType_GrayINT32);
  }
  {
    std::istringstream stream("GINT64");
    auto ct = poutre::PType::PType_Undef;
    stream >> ct;
    BOOST_CHECK_EQUAL(ct, poutre::PType::PType_GrayINT64);
  }
  {
    std::istringstream stream("F32");
    auto ct = poutre::PType::PType_Undef;
    stream >> ct;
    BOOST_CHECK_EQUAL(ct, poutre::PType::PType_F32);
  }

  {
    std::istringstream stream("D64");
    auto ct = poutre::PType::PType_Undef;
    stream >> ct;
    BOOST_CHECK_EQUAL(ct, poutre::PType::PType_D64);
  }

}

BOOST_AUTO_TEST_CASE(imgtype)
{
  {
    auto ct = poutre::ImgType::ImgType_Dense;
    std::ostringstream os;
    os << ct;
    BOOST_CHECK_EQUAL("Dense", os.str());
  }

  {
    auto ct = poutre::ImgType::ImgType_Sparse;
    std::ostringstream os;
    os << ct;
    BOOST_CHECK_EQUAL("Sparse", os.str());
  }

  {
    std::istringstream stream("Dense");
    auto ct = poutre::ImgType::ImgType_Undef;
    stream >> ct;
    BOOST_CHECK_EQUAL(ct, poutre::ImgType::ImgType_Dense);
  }
  {
    std::istringstream stream("Sparse");
    auto ct = poutre::ImgType::ImgType_Undef;
    stream >> ct;
    BOOST_CHECK_EQUAL(ct, poutre::ImgType::ImgType_Sparse);
  }
}

BOOST_AUTO_TEST_CASE(basictypecompound)
{
  {
    auto ct = poutre::CompoundType::CompoundType_Scalar;
    std::ostringstream os;
    os << ct;
    BOOST_CHECK_EQUAL("Scalar", os.str());
  }
  {
    auto ct = poutre::CompoundType::CompoundType_3Planes;
    std::ostringstream os;
    os << ct;
    BOOST_CHECK_EQUAL("3Planes", os.str());
  }
  {
    auto ct = poutre::CompoundType::CompoundType_4Planes;
    std::ostringstream os;
    os << ct;
    BOOST_CHECK_EQUAL("4Planes", os.str());
  }
  /*  {
    auto ct = poutre::CompoundType::CompoundType_Container;
    std::ostringstream os;
    os << ct;
    BOOST_CHECK_EQUAL("Container", os.str( ));
      }*/

  {
    std::istringstream stream("Scalar");
    auto ct = poutre::CompoundType::CompoundType_Undef;
    stream >> ct;
    BOOST_CHECK_EQUAL(ct, poutre::CompoundType::CompoundType_Scalar);
  }

  /*  {
    std::istringstream stream("Container");
    auto ct = poutre::CompoundType::CompoundType_Undef;
    stream >> ct;
    BOOST_CHECK_EQUAL(ct, poutre::CompoundType::CompoundType_Container);
      }*/

  {
    std::istringstream stream("3Planes");
    auto ct = poutre::CompoundType::CompoundType_Undef;
    stream >> ct;
    BOOST_CHECK_EQUAL(ct, poutre::CompoundType::CompoundType_3Planes);
  }

  {
    std::istringstream stream("4Planes");
    auto ct = poutre::CompoundType::CompoundType_Undef;
    stream >> ct;
    BOOST_CHECK_EQUAL(ct, poutre::CompoundType::CompoundType_4Planes);
  }

}

BOOST_AUTO_TEST_CASE(compound_pixel_container)
{
  using c5G8 = poutre::compound_pixel<poutre::pUINT8, 5>;
  BOOST_CHECK(poutre::get_dim<c5G8>::dim == 5);
  c5G8 p00;
  p00.assign(10);
  c5G8 p0 = {10, 10, 10, 10, 10};
  BOOST_CHECK_EQUAL(p00, p0);
  BOOST_CHECK_EQUAL(p00[0], 10);
  BOOST_CHECK_EQUAL(p00[3], 10);
  p00[4] = 11;
  BOOST_CHECK_EQUAL(p00[4], 11);
  BOOST_CHECK_NE(p00, p0);

  //c5G8 pc = p00;
  //BOOST_CHECK(pc == p00);
  //BOOST_CHECK(&pc != &p00);


}

BOOST_AUTO_TEST_CASE(compound_pixel_3)
{
  using c3G8 = poutre::compound_pixel<poutre::pUINT8, 3>;
  BOOST_CHECK(poutre::get_dim<c3G8>::dim == 3);
  c3G8 p00(0, 1, 2);
  BOOST_CHECK(p00[0] == 0);
  BOOST_CHECK(p00[1] == 1);
  BOOST_CHECK(p00[2] == 2);

  c3G8 p0; //default uninitialized
  p0.fill(12);
  BOOST_CHECK(p0[0] == 12);
  BOOST_CHECK(p0[1] == 12);
  BOOST_CHECK(p0[2] == 12);

  c3G8 p(1, 2, 3);
  BOOST_CHECK(p0 != p);
  BOOST_CHECK(p[0] == 1);
  BOOST_CHECK(p[1] == 2);
  BOOST_CHECK(p[2] == 3);

  c3G8 pc = p;
  BOOST_CHECK(pc == p);
  BOOST_CHECK(&pc != &p);
  BOOST_CHECK(pc[0] == 1);
  BOOST_CHECK(pc[1] == 2);
  BOOST_CHECK(pc[2] == 3);
  pc[0] = 3;
  pc[1] = 4; //-V112
  pc[2] = 5;
  BOOST_CHECK(pc[0] == 3);
  BOOST_CHECK(pc[1] == 4); //-V112
  BOOST_CHECK(pc[2] == 5);

  //p0 = std::move(pc);
  //BOOST_CHECK(p0[0] == 3);
  //BOOST_CHECK(p0[1] == 4);
  //BOOST_CHECK(p0[2] == 5);
}

BOOST_AUTO_TEST_CASE(compound_pixel_4)
{
  using c4G8 = poutre::compound_pixel<poutre::pUINT8, 4>;
  BOOST_CHECK(poutre::get_dim<c4G8>::dim == 4); //-V112
  c4G8 p00(0, 1, 2, 3);
  BOOST_CHECK(p00[0] == 0);
  BOOST_CHECK(p00[1] == 1);
  BOOST_CHECK(p00[2] == 2);
  BOOST_CHECK(p00[3] == 3);

  c4G8 p0; //default uninitialized
  p0.assign(0);
  p0 += 12;
  BOOST_CHECK(p0[0] == 12);
  BOOST_CHECK(p0[1] == 12);
  BOOST_CHECK(p0[2] == 12);
  BOOST_CHECK(p0[3] == 12);

  c4G8 p = {1, 2, 3, 4}; //-V112
  BOOST_CHECK(p0 != p);
  BOOST_CHECK(p[0] == 1);
  BOOST_CHECK(p[1] == 2);
  BOOST_CHECK(p[2] == 3);
  BOOST_CHECK(p[3] == 4); //-V112

  c4G8 pc = p;
  BOOST_CHECK(pc == p);
  BOOST_CHECK(&pc != &p);
  BOOST_CHECK(pc[0] == 1);
  BOOST_CHECK(pc[1] == 2);
  BOOST_CHECK(pc[2] == 3);
  BOOST_CHECK(pc[3] == 4); //-V112
  pc[0] = 3;
  pc[1] = 4; //-V112
  pc[2] = 5;
  pc[3] = 6;
  BOOST_CHECK(pc[0] == 3);
  BOOST_CHECK(pc[1] == 4); //-V112
  BOOST_CHECK(pc[2] == 5);
  BOOST_CHECK(pc[3] == 6);

  //p0 = std::move(pc);
  //BOOST_CHECK(p0[0] == 3);
  //BOOST_CHECK(p0[1] == 4);
  //BOOST_CHECK(p0[2] == 5);
  //BOOST_CHECK(p0[3] == 6);
}

BOOST_AUTO_TEST_SUITE_END()



