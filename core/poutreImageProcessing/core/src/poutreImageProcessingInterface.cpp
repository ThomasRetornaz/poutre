#include <poutreImageProcessing/core/poutreImageProcessingInterface.hpp>
#ifndef POUTRE_TRACE__HPP__
#include <poutreBase/poutreTrace.hpp>
#endif

namespace poutre
  {

  template class std::vector<std::size_t>;

  std::ostream& operator<<(std::ostream& stream, const IInterface& img)
    {
    stream<<img.str( );
    return stream;
    }

  bool AsSizesCompatible(const IInterface& i_img1, const IInterface& i_img2)
    {
    if (i_img1.GetNumDims( ) != i_img2.GetNumDims( ))
      return false;
    const auto& coord1=i_img1.GetCoords( );
    const auto& coord2 = i_img2.GetCoords( );
    return std::equal(coord1.begin( ), coord1.end( ), coord2.begin( ));
    }

  bool AsTypesCompatible(const IInterface& i_img1, const IInterface& i_img2)
    {
    if (i_img1.GetPType( ) != i_img2.GetPType( ))
      return false;
    return true;
    }

  void AssertSizesCompatible(const IInterface& i_img1, const IInterface& i_img2, const std::string& i_msg)
    {
    if (!AsSizesCompatible(i_img1, i_img2)) POUTRE_RUNTIME_ERROR(i_msg);
    }


  void AssertAsTypesCompatible(const IInterface& i_img1, const IInterface& i_img2, const std::string& i_msg)
    {
    if (!AsTypesCompatible(i_img1, i_img2)) POUTRE_RUNTIME_ERROR(i_msg);
    }

  }
