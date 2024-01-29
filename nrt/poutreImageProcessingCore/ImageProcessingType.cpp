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

TEST(poutreImageProcessingType,NativeType)
{
  EXPECT_TRUE(poutre::get_dim<poutre::pUINT8>::dim == 1);
}

TEST(poutreImageProcessingType,ptype)
{
  //{
  //auto ct = poutre::PType::PType_Bin;
  //std::ostringstream os;
  //os << ct;
  //EXPECT_EQ("Bin", os.str( ));
  //}
  {
    auto ct = poutre::PType::PType_GrayUINT8;
    std::ostringstream os;
    os << ct;
    EXPECT_EQ("GUINT8", os.str());
  }
  {
    auto ct = poutre::PType::PType_GrayINT32;
    std::ostringstream os;
    os << ct;
    EXPECT_EQ("GINT32", os.str());
  }
  {
    auto ct = poutre::PType::PType_GrayINT64;
    std::ostringstream os;
    os << ct;
    EXPECT_EQ("GINT64", os.str());
  }
  {
    auto ct = poutre::PType::PType_F32;
    std::ostringstream os;
    os << ct;
    EXPECT_EQ("F32", os.str());
  }
  {
    auto ct = poutre::PType::PType_D64;
    std::ostringstream os;
    os << ct;
    EXPECT_EQ("D64", os.str());
  }

  /*   {
     std::istringstream stream("Bin");
     auto ct = poutre::PType::PType_Undef;
     stream >> ct;
     EXPECT_EQ(ct, poutre::PType::PType_Bin);
       }*/

  {
    std::istringstream stream("GUINT8");
    auto ct = poutre::PType::PType_Undef;
    stream >> ct;
    EXPECT_EQ(ct, poutre::PType::PType_GrayUINT8);
  }

  {
    std::istringstream stream("GINT32");
    auto ct = poutre::PType::PType_Undef;
    stream >> ct;
    EXPECT_EQ(ct, poutre::PType::PType_GrayINT32);
  }
  {
    std::istringstream stream("GINT64");
    auto ct = poutre::PType::PType_Undef;
    stream >> ct;
    EXPECT_EQ(ct, poutre::PType::PType_GrayINT64);
  }
  {
    std::istringstream stream("F32");
    auto ct = poutre::PType::PType_Undef;
    stream >> ct;
    EXPECT_EQ(ct, poutre::PType::PType_F32);
  }

  {
    std::istringstream stream("D64");
    auto ct = poutre::PType::PType_Undef;
    stream >> ct;
    EXPECT_EQ(ct, poutre::PType::PType_D64);
  }

}

TEST(poutreImageProcessingType,imgtype)
{
  {
    auto ct = poutre::ImgType::ImgType_Dense;
    std::ostringstream os;
    os << ct;
    EXPECT_EQ("Dense", os.str());
  }

  {
    auto ct = poutre::ImgType::ImgType_Sparse;
    std::ostringstream os;
    os << ct;
    EXPECT_EQ("Sparse", os.str());
  }

  {
    std::istringstream stream("Dense");
    auto ct = poutre::ImgType::ImgType_Undef;
    stream >> ct;
    EXPECT_EQ(ct, poutre::ImgType::ImgType_Dense);
  }
  {
    std::istringstream stream("Sparse");
    auto ct = poutre::ImgType::ImgType_Undef;
    stream >> ct;
    EXPECT_EQ(ct, poutre::ImgType::ImgType_Sparse);
  }
}

TEST(poutreImageProcessingType,basictypecompound)
{
  {
    auto ct = poutre::CompoundType::CompoundType_Scalar;
    std::ostringstream os;
    os << ct;
    EXPECT_EQ("Scalar", os.str());
  }
  {
    auto ct = poutre::CompoundType::CompoundType_3Planes;
    std::ostringstream os;
    os << ct;
    EXPECT_EQ("3Planes", os.str());
  }
  {
    auto ct = poutre::CompoundType::CompoundType_4Planes;
    std::ostringstream os;
    os << ct;
    EXPECT_EQ("4Planes", os.str());
  }
  /*  {
    auto ct = poutre::CompoundType::CompoundType_Container;
    std::ostringstream os;
    os << ct;
    EXPECT_EQ("Container", os.str( ));
      }*/

  {
    std::istringstream stream("Scalar");
    auto ct = poutre::CompoundType::CompoundType_Undef;
    stream >> ct;
    EXPECT_EQ(ct, poutre::CompoundType::CompoundType_Scalar);
  }

  /*  {
    std::istringstream stream("Container");
    auto ct = poutre::CompoundType::CompoundType_Undef;
    stream >> ct;
    EXPECT_EQ(ct, poutre::CompoundType::CompoundType_Container);
      }*/

  {
    std::istringstream stream("3Planes");
    auto ct = poutre::CompoundType::CompoundType_Undef;
    stream >> ct;
    EXPECT_EQ(ct, poutre::CompoundType::CompoundType_3Planes);
  }

  {
    std::istringstream stream("4Planes");
    auto ct = poutre::CompoundType::CompoundType_Undef;
    stream >> ct;
    EXPECT_EQ(ct, poutre::CompoundType::CompoundType_4Planes);
  }

}

TEST(poutreImageProcessingType,compound_pixel_container)
{
  using c5G8 = poutre::compound<poutre::pUINT8, 5>;
  EXPECT_TRUE(poutre::get_dim<c5G8>::dim == 5);
  c5G8 p00;
  std::fill(p00.begin(), p00.end(),10);
  c5G8 p0 = {10, 10, 10, 10, 10};
  EXPECT_EQ(p00, p0);
  EXPECT_EQ(p00[0], 10);
  EXPECT_EQ(p00[3], 10);
  p00[4] = 11;
  EXPECT_EQ(p00[4], 11);
  EXPECT_NE(p00, p0);

  //c5G8 pc = p00;
  //EXPECT_TRUE(pc == p00);
  //EXPECT_TRUE(&pc != &p00);


}

TEST(poutreImageProcessingType,compound_pixel_3)
{
  using c3G8 = poutre::compound<poutre::pUINT8, 3>;
  EXPECT_TRUE(poutre::get_dim<c3G8>::dim == 3);
  c3G8 p00 = {0, 1, 2};
  EXPECT_TRUE(p00[0] == 0);
  EXPECT_TRUE(p00[1] == 1);
  EXPECT_TRUE(p00[2] == 2);

  c3G8 p0; //default uninitialized
  p0.fill(12);
  EXPECT_TRUE(p0[0] == 12);
  EXPECT_TRUE(p0[1] == 12);
  EXPECT_TRUE(p0[2] == 12);

  c3G8 p = {1, 2, 3};
  EXPECT_TRUE(p0 != p);
  EXPECT_TRUE(p[0] == 1);
  EXPECT_TRUE(p[1] == 2);
  EXPECT_TRUE(p[2] == 3);

  c3G8 pc = p;
  EXPECT_TRUE(pc == p);
  EXPECT_TRUE(&pc != &p);
  EXPECT_TRUE(pc[0] == 1);
  EXPECT_TRUE(pc[1] == 2);
  EXPECT_TRUE(pc[2] == 3);
  pc[0] = 3;
  pc[1] = 4; //-V112
  pc[2] = 5;
  EXPECT_TRUE(pc[0] == 3);
  EXPECT_TRUE(pc[1] == 4); //-V112
  EXPECT_TRUE(pc[2] == 5);

  //p0 = std::move(pc);
  //EXPECT_TRUE(p0[0] == 3);
  //EXPECT_TRUE(p0[1] == 4);
  //EXPECT_TRUE(p0[2] == 5);
}

TEST(poutreImageProcessingType,compound_pixel_4)
{
  using c4G8 = poutre::compound<poutre::pUINT8, 4>;
  EXPECT_TRUE(poutre::get_dim<c4G8>::dim == 4); //-V112
  c4G8 p00 = {0, 1, 2, 3};
  EXPECT_TRUE(p00[0] == 0);
  EXPECT_TRUE(p00[1] == 1);
  EXPECT_TRUE(p00[2] == 2);
  EXPECT_TRUE(p00[3] == 3);

  c4G8 p0 = {0}; // default uninitialized
  p0.fill(0);

  c4G8 p = {1, 2, 3, 4}; //-V112
  EXPECT_TRUE(p0 != p);
  EXPECT_TRUE(p[0] == 1);
  EXPECT_TRUE(p[1] == 2);
  EXPECT_TRUE(p[2] == 3);
  EXPECT_TRUE(p[3] == 4); //-V112

  c4G8 pc = p;
  EXPECT_TRUE(pc == p);
  EXPECT_TRUE(&pc != &p);
  EXPECT_TRUE(pc[0] == 1);
  EXPECT_TRUE(pc[1] == 2);
  EXPECT_TRUE(pc[2] == 3);
  EXPECT_TRUE(pc[3] == 4); //-V112
  pc[0] = 3;
  pc[1] = 4; //-V112
  pc[2] = 5;
  pc[3] = 6;
  EXPECT_TRUE(pc[0] == 3);
  EXPECT_TRUE(pc[1] == 4); //-V112
  EXPECT_TRUE(pc[2] == 5);
  EXPECT_TRUE(pc[3] == 6);

  //p0 = std::move(pc);
  //EXPECT_TRUE(p0[0] == 3);
  //EXPECT_TRUE(p0[1] == 4);
  //EXPECT_TRUE(p0[2] == 5);
  //EXPECT_TRUE(p0[3] == 6);
}



