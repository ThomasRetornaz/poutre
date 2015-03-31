#ifndef POUTREIMAGEPROCESSINGINTERFACE_HPP__
#define POUTREIMAGEPROCESSINGINTERFACE_HPP__

#ifndef POUTREIMAGEPROCESSING_HPP__
#include <poutreImageProcessing/poutreImageProcessing.hpp>
#endif

#ifndef POUTREIMAGEPROCESSINGTYPE_HPP__
#include <poutreImageProcessing/core/poutreImageProcessingType.hpp>
#endif

#include <memory>
#include <vector> 

namespace poutre
  {
  extern template class std::vector<std::size_t>;

  class IInterface
    {
    public:
      virtual std::unique_ptr<IInterface> DeepClone() const = 0;
      virtual PType GetPType() const = 0;
      virtual std::vector<std::size_t> GetCoords( ) const = 0;
      virtual std::size_t GetNumDims( ) const = 0;
      virtual ~IInterface(){}
      virtual std::string str( ) const = 0;
    };


  //! operator<< for IInterface
  BASE_API std::ostream& operator<<(std::ostream&, const IInterface& img);

  BASE_API bool AsSizesCompatible(const IInterface& i_img1, const IInterface& i_img2);

  BASE_API bool AsTypesCompatible(const IInterface& i_img1, const IInterface& i_img2);

  BASE_API void AssertSizesCompatible(const IInterface& i_img1, const IInterface& i_img2, const std::string& i_msg);

  BASE_API void AssertAsTypesCompatible(const IInterface& i_img1, const IInterface& i_img2, const std::string& i_msg);


  }
#endif POUTREIMAGEPROCESSINGINTERFACE_HPP__


