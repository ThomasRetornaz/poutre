#include <poutreImageProcessing/core/poutreImageProcessingType.hpp>
#include <PoutreBase/poutreTrace.hpp>

namespace poutre
  {

  std::ostream& operator<<(std::ostream& os, PType p)
    {
    switch (p)
      {
      case PType::PType_Bin:
        os << "Bin";
        break;
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

    if (strType == "Bin")
      p = PType::PType_Bin;
    /*      case PType_BinPack: // break;*/ //TODO ?
    else if (strType == "GUINT8")
      p = PType::PType_GrayUINT8;
    else if (strType == "GINT32")
      p = PType::PType_GrayINT32;
    else if (strType == "GINT64")
      p = PType::PType_GrayINT64;
    else if (strType == "F32")
      p = PType::PType_F32;
    else
      {
      POUTRE_RUNTIME_ERROR("Unable to read PType from stream");
      }

    return is;
    }

  }
