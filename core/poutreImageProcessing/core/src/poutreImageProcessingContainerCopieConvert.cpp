#include <poutreImageProcessing/core/include/poutreImageProcessingContainerCopieConvert.hpp>
#include <poutreImageProcessing/core/poutreImageProcessingInterfaceCopieConvert.hpp>

namespace poutre
  {

  std::unique_ptr<IInterface> GetSame(const IInterface& i_img1)
    {
    return i_img1.Clone( );
    }

  std::unique_ptr<IInterface> GetSameCoord(const IInterface& i_img1, PType ptype)
    {
    auto imgType=i_img1.GetImgType( );
    switch (imgType)
      {
        case ImgType::ImgType_Dense:
          {
          return CreateDense(i_img1.GetCoords( ), ptype);
          }break;
        default:
          POUTRE_RUNTIME_ERROR(("GetSameCoordinates:: unsupported img type" + boost::lexical_cast<std::string>(imgType)));
      }
    }
  }
