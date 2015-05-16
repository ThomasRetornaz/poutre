
//==============================================================================
//                  Copyright (c) 2015 - Thomas Retornaz                      //
//                     thomas.retornaz@mines-paris.org                        //
//          Distributed under the Boost Software License, Version 1.0.        //
//                 See accompanying file LICENSE.txt or copy at               //
//                     http://www.boost.org/LICENSE_1_0.txt                   //
//==============================================================================

#ifndef POUTRE_IMAGEPROCESSING_INTERFACE_HPP__
#define POUTRE_IMAGEPROCESSING_INTERFACE_HPP__

#ifndef POUTRE_IMAGEPROCESSING_HPP__
#include <poutreImageProcessing/poutreImageProcessing.hpp>
#endif

#ifndef POUTRE_IMAGEPROCESSING_TYPE_HPP__
#include <poutreImageProcessing/core/poutreImageProcessingType.hpp>
#endif

#include <memory>
#include <vector> 

namespace poutre
  {
  extern template class std::vector<std::size_t>;

  class IMP_API IInterface
    {
    public:
      virtual std::unique_ptr<IInterface> Clone() const = 0;
      virtual PType GetPType() const = 0;
      virtual CompoundType GetCType( ) const = 0;
      virtual ImgType GetImgType( ) const = 0;
      virtual std::vector<std::size_t> GetCoords( ) const = 0;
      virtual std::size_t GetNumDims( ) const = 0;
      virtual ~IInterface(){}
      virtual std::string str( ) const = 0;
    };


  //! operator<< for IInterface
  IMP_API std::ostream& operator<<(std::ostream&, const IInterface& img);

  IMP_API bool AsSizesCompatible(const IInterface& i_img1, const IInterface& i_img2);

  IMP_API bool AsTypesCompatible(const IInterface& i_img1, const IInterface& i_img2);

  IMP_API void AssertSizesCompatible(const IInterface& i_img1, const IInterface& i_img2, const std::string& i_msg);

  IMP_API void AssertAsTypesCompatible(const IInterface& i_img1, const IInterface& i_img2, const std::string& i_msg);

  //TODO dispatch with sparse type
  //BASE_API std::unique_ptr<IInterface> Create(const std::initializer_list<size_t>& dims, ImgType imgType, PType ptype);

  IMP_API std::unique_ptr<IInterface> CreateDense(const std::vector<std::size_t>& dims, CompoundType ctype, PType ptype);

  }
#endif//POUTRE_IMAGEPROCESSING_INTERFACE_HPP__


