#include "main.hpp"

#include <iostream>
#include <poutreImageProcessing/core/poutreImageProcessingType.hpp>

BOOST_AUTO_TEST_SUITE(poutreImageProcessingType)

BOOST_AUTO_TEST_CASE(ptype)
  {
      {
      auto ct = poutre::PType::PType_Bin;
      std::ostringstream os;
      os << ct;
      BOOST_CHECK_EQUAL("Bin", os.str( ));
      }
      {
      auto ct = poutre::PType::PType_GrayUINT8;
      std::ostringstream os;
      os << ct;
      BOOST_CHECK_EQUAL("GUINT8", os.str( ));
        }
        {
        auto ct = poutre::PType::PType_GrayINT32;
        std::ostringstream os;
        os << ct;
        BOOST_CHECK_EQUAL("GINT32", os.str( ));
          }
          {
          auto ct = poutre::PType::PType_GrayINT64;
          std::ostringstream os;
          os << ct;
          BOOST_CHECK_EQUAL("GINT64", os.str( ));
            }
            {
            auto ct = poutre::PType::PType_F32;
            std::ostringstream os;
            os << ct;
            BOOST_CHECK_EQUAL("F32", os.str( ));
              }

      {
      std::istringstream stream("Bin");
      auto ct = poutre::PType::PType_Undef;
      stream >> ct;
      BOOST_CHECK_EQUAL(ct, poutre::PType::PType_Bin);
        }

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
  }

BOOST_AUTO_TEST_CASE(imgtype)
  {
      {
      auto ct = poutre::ImgType::ImgType_Dense;
      std::ostringstream os;
      os << ct;
      BOOST_CHECK_EQUAL("Dense", os.str( ));
      }

          {
          auto ct = poutre::ImgType::ImgType_Sparse;
          std::ostringstream os;
          os << ct;
          BOOST_CHECK_EQUAL("Sparse", os.str( ));
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
      BOOST_CHECK_EQUAL("Scalar", os.str( ));
      }
          {
          auto ct = poutre::CompoundType::CompoundType_3Planes;
          std::ostringstream os;
          os << ct;
          BOOST_CHECK_EQUAL("3Planes", os.str( ));
            }
          {
          auto ct = poutre::CompoundType::CompoundType_4Planes;
          std::ostringstream os;
          os << ct;
          BOOST_CHECK_EQUAL("4Planes", os.str( ));
            }
          {
          auto ct = poutre::CompoundType::CompoundType_Container;
          std::ostringstream os;
          os << ct;
          BOOST_CHECK_EQUAL("Container", os.str( ));
            }

          {
          std::istringstream stream("Scalar");
          auto ct = poutre::CompoundType::CompoundType_Undef;
          stream >> ct;
          BOOST_CHECK_EQUAL(ct, poutre::CompoundType::CompoundType_Scalar);
            }

          {
          std::istringstream stream("Container");
          auto ct = poutre::CompoundType::CompoundType_Undef;
          stream >> ct;
          BOOST_CHECK_EQUAL(ct, poutre::CompoundType::CompoundType_Container);
            }

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

BOOST_AUTO_TEST_SUITE_END( )



