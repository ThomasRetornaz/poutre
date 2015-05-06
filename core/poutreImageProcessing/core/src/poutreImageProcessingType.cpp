#include <poutreImageProcessing/core/poutreImageProcessingType.hpp>
#include <PoutreBase/poutreTrace.hpp>

namespace poutre
  {

  
  template class compound_pixel < pUINT8, 3 >;
  template class compound_pixel < pINT32, 3 >;
  template class compound_pixel < pFLOAT, 3 >;
  template class compound_pixel < pINT64, 3 >;
  template class compound_pixel < pDOUBLE, 3 >;

  template class compound_pixel < pUINT8, 4 >;
  template class compound_pixel < pINT32, 4 >;
  template class compound_pixel < pFLOAT, 4 >;
  template class compound_pixel < pINT64, 4 >;
  template class compound_pixel < pDOUBLE, 4 >;



  std::ostream& operator<<(std::ostream& os, CompoundType ct)
    {
    switch (ct)
      {
        case CompoundType::CompoundType_Scalar:
          {
          os << "Scalar";
          }break;
        case CompoundType::CompoundType_3Planes:
          {
          os << "3Planes";
          }break;
        case CompoundType::CompoundType_4Planes:
          {
          os << "4Planes";
          }break;
       /* case CompoundType::CompoundType_Container:
          {
          os << "Container";
          } break;*/
        default: //  CompoundType::CompoundType_Undef; 
          os << "Unknown PType";
      }//switch
    return os;
    }

  std::istream& operator>>(std::istream& is, CompoundType& ct)
    {
    ct=CompoundType::CompoundType_Undef;
    if (!is.good( ))
      return is;

    std::string strType;
    is >> strType;
    if (is.bad( ))
      return is;

    if (strType == "Scalar")
      ct = CompoundType::CompoundType_Scalar;
    else if (strType == "3Planes")
      ct = CompoundType::CompoundType_3Planes;
    else if (strType == "4Planes")
      ct = CompoundType::CompoundType_4Planes;
   /* else if (strType == "Container")
      ct = CompoundType::CompoundType_Container;*/
    else
      {
      POUTRE_RUNTIME_ERROR("Unable to read CompoundType from stream");
      }
    return is;
    }

  std::ostream& operator<<(std::ostream& os, ImgType imgType)
    {
    switch (imgType)
      {
        case ImgType::ImgType_Dense:
          os << "Dense";
          break;
        case ImgType::ImgType_Sparse:
          os << "Sparse";
          break;
        default://  ImgType::ImgType_Undef; 
          os << "Unknown PType";
          break;
      }
    return os;
    }

  
  std::istream& operator>>(std::istream& is, ImgType& imgType)
    {
    imgType = ImgType::ImgType_Undef;

    if (!is.good( ))
      return is;

    std::string strType;
    is >> strType;
    if (is.bad( ))
      return is;

    if (strType == "Dense")
      imgType = ImgType::ImgType_Dense;
    else if (strType == "Sparse")
      imgType = ImgType::ImgType_Sparse;
    else
      {
      POUTRE_RUNTIME_ERROR("Unable to read ImgType from stream");
      }
    return is;
    }

  std::ostream& operator<<(std::ostream& os, PType p)
    {
    switch (p)
      {
     /* case PType::PType_Bin:
        os << "Bin";
        break;*/
        /*      case PType_BinPack: // break;*/ //TODO ?
      case PType::PType_GrayUINT8:
        os << "GUINT8";
        break;
      case PType::PType_GrayINT32:
        os << "GINT32";
        break;
      case PType::PType_GrayINT64:
        os << "GINT64";
        break;
      case PType::PType_F32:
        os << "F32";
        break;
      case PType::PType_D64:
        os << "D64";
        break;
      default://  PType::PType_Undef; 
        os << "Unknown PType";
        break;
      }

    return os;
    }




  std::istream& operator>>(std::istream& is, PType& p)
    {
    p = PType::PType_Undef;

    if (!is.good())
      return is;

    std::string strType;
    is >> strType;
    if (is.bad())
      return is;

   /* if (strType == "Bin")
      p = PType::PType_Bin;*/
    /*      case PType_BinPack: // break;*/ //TODO ?
    else if (strType == "GUINT8")
      p = PType::PType_GrayUINT8;
    else if (strType == "GINT32")
      p = PType::PType_GrayINT32;
    else if (strType == "GINT64")
      p = PType::PType_GrayINT64;
    else if (strType == "F32")
      p = PType::PType_F32;
    else if (strType == "D64")
      p = PType::PType_D64;
    else
      {
      POUTRE_RUNTIME_ERROR("Unable to read PType from stream");
      }

    return is;
    }

  }
