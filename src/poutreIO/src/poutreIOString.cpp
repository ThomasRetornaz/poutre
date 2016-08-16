
//==============================================================================
//                  Copyright (c) 2015 - Thomas Retornaz                      //
//                     thomas.retornaz@mines-paris.org                        //
//          Distributed under the Boost Software License, Version 1.0.        //
//                 See accompanying file LICENSE.txt or copy at               //
//                     http://www.boost.org/LICENSE_1_0.txt                   //
//==============================================================================


#include <boost/lexical_cast.hpp>
#include <iostream>
#include <sstream>

#include "poutreIO/poutreIOString.hpp"
#include <poutreBase/poutreTrace.hpp>
#include <poutreImageProcessingCore/include/poutreImageProcessingContainer.hpp>



namespace poutre
{
    //TODO HAVE A LOOK AT boost::dispatch for dispatch ...

    /***********************************************************************************************************************/
    /*                                       IMAGE FROM STRING                                                             */
    /***********************************************************************************************************************/
    
    
    template<size_t dims,typename ptype> 
        void ImageFromStringDenseScalarDispatchPTypeHelper(poutre::IInterface& img,std::istringstream& istrm) 
    {
        using ImgType =  DenseImage<typename TypeTraits<ptype>::storage_type,dims>;
        ImgType* img_t = dynamic_cast<ImgType*>(&img);
        typename TypeTraits<ptype>::str_type tmp;
        if(!img_t) POUTRE_RUNTIME_ERROR("ImageFromStringDenseScalarDispatchPType:Dynamic cast failure");
        for (auto& it:(*img_t))
                {
                    istrm>>tmp;
                    //todo check range validity
                    if (istrm.fail())
                    {
                        std::string err = "ImageFromStringDenseScalarDispatchPType: unable to read current pixel from " + istrm.str();
                        POUTRE_RUNTIME_ERROR(err);
                    }
                    it=tmp;
                }
    }
    
    template<size_t dims,typename ptype> 
        void ImageFromStringDense3PlanesDispatchPTypeHelper(poutre::IInterface& img,std::istringstream& istrm) 
    {
        using ImgType =  DenseImage<typename TypeTraits<compound_pixel<ptype,3>>::storage_type,dims>;
        ImgType* img_t = dynamic_cast<ImgType*>(&img);
        typename TypeTraits<ptype>::str_type tmp;
        if(!img_t) POUTRE_RUNTIME_ERROR("ImageFromStringDenseScalarDispatchPType:Dynamic cast failure");
        for (auto& it:(*img_t))
                {
                    istrm>>tmp;
                    //todo check range validity ?
                    if (istrm.fail()) { std::string err = "ImageFromStringDenseScalarDispatchPType: unable to read current pixel from " + istrm.str(); POUTRE_RUNTIME_ERROR(err); }
                    it[0]=static_cast<typename TypeTraits<ptype>::storage_type>(tmp);
                    istrm>>tmp;
                    //todo check range validity ?
                    if (istrm.fail()) { std::string err = "ImageFromStringDenseScalarDispatchPType: unable to read current pixel from " + istrm.str(); POUTRE_RUNTIME_ERROR(err); }
                    it[1]=static_cast<typename TypeTraits<ptype>::storage_type>(tmp);
                    //todo check range validity ?
                    istrm>>tmp;
                    if (istrm.fail()) { std::string err = "ImageFromStringDenseScalarDispatchPType: unable to read current pixel from " + istrm.str(); POUTRE_RUNTIME_ERROR(err); }
                    it[2]=static_cast<typename TypeTraits<ptype>::storage_type>(tmp);
                    }
    }

    
    template<size_t dims,typename ptype> 
        void ImageFromStringDense4PlanesDispatchPTypeHelper(poutre::IInterface& img,std::istringstream& istrm) 
    {
        using ImgType =  DenseImage<typename TypeTraits<compound_pixel<ptype,4>>::storage_type,dims>;
        ImgType* img_t = dynamic_cast<ImgType*>(&img);
        typename TypeTraits<ptype>::str_type tmp;
        if(!img_t) POUTRE_RUNTIME_ERROR("ImageFromStringDenseScalarDispatchPType:Dynamic cast failure");
        for (auto& it:(*img_t))
                {
                    istrm>>tmp;
                    //todo check range validity ?
                    if (istrm.fail()) 
                    {
                        std::string err="ImageFromStringDenseScalarDispatchPType: unable to read current pixel from " + istrm.str();
                        POUTRE_RUNTIME_ERROR(err);
                    }
                    it[0] = static_cast<typename TypeTraits<ptype>::storage_type>(tmp);
                    istrm>>tmp;
                    //todo check range validity ?
                    if (istrm.fail()) { std::string err = "ImageFromStringDenseScalarDispatchPType: unable to read current pixel from " + istrm.str(); POUTRE_RUNTIME_ERROR(err); }
                    it[1]=static_cast<typename TypeTraits<ptype>::storage_type >(tmp);
                    //todo check range validity ?
                    istrm>>tmp;
                    if (istrm.fail()) { std::string err = "ImageFromStringDenseScalarDispatchPType: unable to read current pixel from " + istrm.str(); POUTRE_RUNTIME_ERROR(err); }
                    it[2]=static_cast<typename TypeTraits<ptype>::storage_type >(tmp);
                    //todo check range validity ?
                    istrm>>tmp;
                    if (istrm.fail()) { std::string err = "ImageFromStringDenseScalarDispatchPType: unable to read current pixel from " + istrm.str(); POUTRE_RUNTIME_ERROR(err); }
                    it[3]=static_cast<typename TypeTraits<ptype>::storage_type >(tmp);
                    }
    }
    
    template<size_t dims> 
    void ImageFromStringDense3PlanesDispatchPType(poutre::IInterface& img, PType ptype,std::istringstream& istrm) 
        {
         switch (ptype) {
            case PType::PType_GrayUINT8 :
            {
                ImageFromStringDense3PlanesDispatchPTypeHelper<dims,pUINT8>(img,istrm);
                
            }break;
            case PType::PType_GrayINT32 :
            {
                ImageFromStringDense3PlanesDispatchPTypeHelper<dims,pINT32>(img,istrm);
            }break;
            case PType::PType_GrayINT64 :
            {
                ImageFromStringDense3PlanesDispatchPTypeHelper<dims,pINT64>(img,istrm);                
                 
            }break;
            case PType::PType_F32 :
            {
                ImageFromStringDense3PlanesDispatchPTypeHelper<dims,pFLOAT>(img,istrm);                
            }break;            
            case PType::PType_D64 :
            {
                ImageFromStringDense3PlanesDispatchPTypeHelper<dims,pDOUBLE>(img,istrm);                
            }break;                        
            default:
            {
                POUTRE_RUNTIME_ERROR(("ImageFromStringDenseScalarDispatchPType::Unsupported ptype:" + boost::lexical_cast<std::string>(ptype)));
            }
        }//switch (ptype)
    }
    
    template<size_t dims> 
    void ImageFromStringDense4PlanesDispatchPType(poutre::IInterface& img, PType ptype,std::istringstream& istrm) 
        {
         switch (ptype) {
            case PType::PType_GrayUINT8 :
            {
                ImageFromStringDense4PlanesDispatchPTypeHelper<dims,pUINT8>(img,istrm);
                
            }break;
            case PType::PType_GrayINT32 :
            {
                ImageFromStringDense4PlanesDispatchPTypeHelper<dims,pINT32>(img,istrm);
            }break;
            case PType::PType_GrayINT64 :
            {
                ImageFromStringDense4PlanesDispatchPTypeHelper<dims,pINT64>(img,istrm);                
                 
            }break;
            case PType::PType_F32 :
            {
                ImageFromStringDense4PlanesDispatchPTypeHelper<dims,pFLOAT>(img,istrm);                
            }break;            
            case PType::PType_D64 :
            {
                ImageFromStringDense4PlanesDispatchPTypeHelper<dims,pDOUBLE>(img,istrm);                
            }break;                        
            default:
            {
                POUTRE_RUNTIME_ERROR(("ImageFromStringDenseScalarDispatchPType::Unsupported ptype:" + boost::lexical_cast<std::string>(ptype)));
            }
        }//switch (ptype)
    }
    
    template<size_t dims> 
        void ImageFromStringDenseScalarDispatchPType(poutre::IInterface& img, PType ptype,std::istringstream& istrm) 
    {
         switch (ptype) {
            case PType::PType_GrayUINT8 :
            {
                ImageFromStringDenseScalarDispatchPTypeHelper<dims,pUINT8>(img,istrm);
                
            }break;
            case PType::PType_GrayINT32 :
            {
                ImageFromStringDenseScalarDispatchPTypeHelper<dims,pINT32>(img,istrm);
            }break;
            case PType::PType_GrayINT64 :
            {
                ImageFromStringDenseScalarDispatchPTypeHelper<dims,pINT64>(img,istrm);                
                 
            }break;
            case PType::PType_F32 :
            {
                ImageFromStringDenseScalarDispatchPTypeHelper<dims,pFLOAT>(img,istrm);                
            }break;            
            case PType::PType_D64 :
            {
                ImageFromStringDenseScalarDispatchPTypeHelper<dims,pDOUBLE>(img,istrm);                
            }break;                        
            default:
            {
                POUTRE_RUNTIME_ERROR(("ImageFromStringDenseScalarDispatchPType::Unsupported ptype:" + boost::lexical_cast<std::string>(ptype)));
            }
        }//switch (ptype)
    }
    
    template<size_t dims>  
    void ImageFromStringDispatchDense(poutre::IInterface& img,CompoundType cptype, PType ptype,std::istringstream& istrm) 
    {
        switch (cptype) {
            case CompoundType::CompoundType_Scalar:
            {
                ImageFromStringDenseScalarDispatchPType<dims>(img,ptype,istrm);
            }
            break;
            
            case CompoundType::CompoundType_3Planes:
            {
                ImageFromStringDense3PlanesDispatchPType<dims>(img,ptype,istrm);
            }
            break;
            
            case CompoundType::CompoundType_4Planes:
            {
                ImageFromStringDense4PlanesDispatchPType<dims>(img,ptype,istrm);
            }
            break;
            default:
            {
                POUTRE_RUNTIME_ERROR(("ImageFromStringDispatch::Unsupported compound type:" + boost::lexical_cast<std::string>(cptype)));
            }
        }//switch (cptype)
    }
    
    
    std::unique_ptr<IInterface> ImageFromString(const std::string& i_str) 
    {
        std::istringstream istrm(i_str);
        if (!istrm.good()) {
            POUTRE_RUNTIME_ERROR("ImageFromString: unable to read input string");
        }

        ImgType imgtype=ImgType::ImgType_Undef;
        CompoundType cptype=CompoundType::CompoundType_Undef;
        PType ptype=PType::PType_Undef;
        std::ptrdiff_t NumDims= -1;
        std::vector<std::size_t> dims;

        //tokenize here instead?
        istrm>>imgtype;
        if (istrm.fail()) {
            std::string err="ImageFromString: unable to read ImgType from " + i_str;
            POUTRE_RUNTIME_ERROR(err);
        }
        //TODO dispatch sparse type ?
        if (imgtype != ImgType::ImgType_Dense) {
            std::string err="ImageFromString: ImgType unknown or not supported yet from " + i_str;
            POUTRE_RUNTIME_ERROR(err);

        }
        
        istrm>>cptype;
        if (istrm.fail()) {
            std::string err="ImageFromString: unable to read CompoundType from " + i_str;
            POUTRE_RUNTIME_ERROR(err);
        }
        istrm>>ptype;
        if (istrm.fail()) {
            std::string err="ImageFromString: unable to read PType from " + i_str;
            POUTRE_RUNTIME_ERROR(err);
        }
        istrm>>NumDims;

        if (istrm.fail()) {
            std::string err="ImageFromString: unable to read NumDims from " + i_str;
            POUTRE_RUNTIME_ERROR(err);
        }

        if (!(NumDims > 0)) {
            std::string err="ImageFromString: wrong value from NumDims " + i_str;
            POUTRE_RUNTIME_ERROR(err);
        }

        //from NumDims we know howto fill Dims
        dims.reserve(NumDims);
        ptrdiff_t current_dim=0;
        for (auto i=0; i < NumDims; i++) {
            istrm>>current_dim;
            if (istrm.fail()) {
                std::string err="ImageFromString: unable to read dim from " + i_str;
                POUTRE_RUNTIME_ERROR(err);
            }
            dims.push_back(current_dim);
        }

        //TODO dispatch sparse type ?
        if (imgtype != ImgType::ImgType_Dense) {
            std::string err="ImageFromString: ImgType unknown or not supported yet from " + i_str;
            POUTRE_RUNTIME_ERROR(err);

        }

        //create image 
        std::unique_ptr<poutre::IInterface> img=CreateDense(dims, cptype, ptype);
        
             
        //Dispatch
        switch (NumDims)
          {
        case 0:
          {
          POUTRE_RUNTIME_ERROR("Unsupported number of dims:0"); return img;
          }break;
        case 1:
          {
            ImageFromStringDispatchDense<1>(*img,cptype,ptype,istrm);
          }break;
        case 2:
          {
            ImageFromStringDispatchDense<2>(*img,cptype,ptype,istrm);
          }break;
        case 3:
          {
            ImageFromStringDispatchDense<3>(*img,cptype, ptype,istrm);
          }break;
        case 4:
          {
            ImageFromStringDispatchDense<4>(*img,cptype, ptype,istrm);
          }break;
        default:
          {
          POUTRE_RUNTIME_ERROR("Unsupported number of dims");
          }
      }

      return std::move(img);
    }

    /***********************************************************************************************************************/
    /*                                       IMAGE TO STRING                                                               */
    /***********************************************************************************************************************/
    
    //IMAGE TO STRING DISPATCH PTYPE
    template<size_t dims,typename ptype> 
    void ImageToStringDenseCompoundType_4PlanesDispatchPTypeHelper(const poutre::IInterface& img,std::ostringstream& ostrm) 
    {
        using ImgType =  DenseImage<typename TypeTraits<compound_pixel<ptype,4>>::storage_type,dims>;
        const ImgType* img_t = dynamic_cast<const ImgType*>(&img);
        if(!img_t) POUTRE_RUNTIME_ERROR("ImageFromStringDenseCompoundType_4PlanesDispatchPType:Dynamic cast failure");
        const ptrdiff_t len=img_t->size();
        for(ptrdiff_t i=0;i<len-1;++i)
        {
            for(auto val : (*img_t)[i])
            {
                ostrm<<(typename poutre::TypeTraits<ptype>::str_type)val<<" ";
            }
        }
        if (len>0)
        {
            ostrm<<(typename poutre::TypeTraits<ptype>::str_type)(*img_t)[len-1][0]<<" ";
            ostrm<<(typename poutre::TypeTraits<ptype>::str_type)(*img_t)[len-1][1]<<" ";
            ostrm<<(typename poutre::TypeTraits<ptype>::str_type)(*img_t)[len-1][2]<<" ";
            ostrm<<(typename poutre::TypeTraits<ptype>::str_type)(*img_t)[len-1][3];
        }
    }
    
    template<size_t dims,typename ptype> 
    void ImageToStringDenseCompoundType_3PlanesDispatchPTypeHelper(const poutre::IInterface& img,std::ostringstream& ostrm) 
    {
        using ImgType =  DenseImage<typename TypeTraits<compound_pixel<ptype,3>>::storage_type,dims>;
        const ImgType* img_t = dynamic_cast<const ImgType*>(&img);
        if(!img_t) POUTRE_RUNTIME_ERROR("ImageFromStringDenseCompoundType_3PlanesDispatchPType:Dynamic cast failure");
        const ptrdiff_t len=img_t->size();
        for(ptrdiff_t i=0;i<len-1;++i)
        {
            for(auto val : (*img_t)[i])
            {
                ostrm<<(typename poutre::TypeTraits<ptype>::str_type)val<<" ";
            }
        }
        if (len>0)
        {
            ostrm<<(typename poutre::TypeTraits<ptype>::str_type)(*img_t)[len-1][0]<<" ";
            ostrm<<(typename poutre::TypeTraits<ptype>::str_type)(*img_t)[len-1][1]<<" ";
            ostrm<<(typename poutre::TypeTraits<ptype>::str_type)(*img_t)[len-1][2];
        }
    }
    
    template<size_t dims,typename ptype> 
        void ImageToStringDenseScalarDispatchPTypeHelper(const poutre::IInterface& img,std::ostringstream& ostrm) 
    {
        using ImgType =  DenseImage<typename TypeTraits<ptype>::storage_type,dims>;
        const ImgType* img_t = dynamic_cast<const ImgType*>(&img);
        if(!img_t) POUTRE_RUNTIME_ERROR("ImageToStringDenseScalarDispatchPTypeHelper:Dynamic cast failure");
        const ptrdiff_t len=img_t->size();
        for(ptrdiff_t i=0;i<len-1;++i)
        {
            ostrm<<(typename poutre::TypeTraits<ptype>::str_type)(*img_t)[i]<<" ";                    
        }
        if (len>0)
            ostrm<<(typename poutre::TypeTraits<ptype>::str_type)(*img_t)[len-1];
    }
    
    //IMAGE TO STRING DISPATCH CTYPE
    template<size_t dims> 
        void ImageToStringDenseScalarDispatchPType(const poutre::IInterface& img, PType ptype,std::ostringstream& ostrm) 
    {
         switch (ptype) {
            case PType::PType_GrayUINT8 :
            {   
               ImageToStringDenseScalarDispatchPTypeHelper<dims,poutre::pUINT8>(img,ostrm);      
            }break;
            
            case PType::PType_GrayINT32 :
            {
                ImageToStringDenseScalarDispatchPTypeHelper<dims,poutre::pINT32>(img,ostrm);      
            }break;
            case PType::PType_GrayINT64 :
            {
                ImageToStringDenseScalarDispatchPTypeHelper<dims,poutre::pINT64>(img,ostrm);                      
            }break;
            case PType::PType_F32 :
            {
                ImageToStringDenseScalarDispatchPTypeHelper<dims,poutre::pFLOAT>(img,ostrm);                                      
            }break;            
            case PType::PType_D64 :
            {
                ImageToStringDenseScalarDispatchPTypeHelper<dims,poutre::pDOUBLE>(img,ostrm);                                                      
            }break;
            
            default:
            {
                POUTRE_RUNTIME_ERROR(("ImageToStringDenseScalarDispatchPType::Unsupported pixel type:" + boost::lexical_cast<std::string>(ptype)));
            }
        }//switch (ptype)
    }
    
     template<size_t dims> 
        void ImageToStringDenseCompoundType_3PlanesDispatchPType(const poutre::IInterface& img, PType ptype,std::ostringstream& ostrm) 
    {
         switch (ptype) {
            case PType::PType_GrayUINT8 :
            {   
               ImageToStringDenseCompoundType_3PlanesDispatchPTypeHelper<dims,poutre::pUINT8>(img,ostrm);      
            }break;
            
            case PType::PType_GrayINT32 :
            {
                ImageToStringDenseCompoundType_3PlanesDispatchPTypeHelper<dims,poutre::pINT32>(img,ostrm);      
            }break;
            case PType::PType_GrayINT64 :
            {
                ImageToStringDenseCompoundType_3PlanesDispatchPTypeHelper<dims,poutre::pINT64>(img,ostrm);                      
            }break;
            case PType::PType_F32 :
            {
                ImageToStringDenseCompoundType_3PlanesDispatchPTypeHelper<dims,poutre::pFLOAT>(img,ostrm);                                      
            }break;            
            case PType::PType_D64 :
            {
                ImageToStringDenseCompoundType_3PlanesDispatchPTypeHelper<dims,poutre::pDOUBLE>(img,ostrm);                                                      
            }break;
            
            default:
            {
                POUTRE_RUNTIME_ERROR(("ImageToStringDenseScalarDispatchPType::Unsupported pixel type:" + boost::lexical_cast<std::string>(ptype)));
            }
        }//switch (ptype)
    }
    
    template<size_t dims> 
        void ImageToStringDenseCompoundType_4PlanesDispatchPType(const poutre::IInterface& img, PType ptype,std::ostringstream& ostrm) 
    {
         switch (ptype) {
            case PType::PType_GrayUINT8 :
            {   
               ImageToStringDenseCompoundType_4PlanesDispatchPTypeHelper<dims,poutre::pUINT8>(img,ostrm);      
            }break;
            
            case PType::PType_GrayINT32 :
            {
                ImageToStringDenseCompoundType_4PlanesDispatchPTypeHelper<dims,poutre::pINT32>(img,ostrm);      
            }break;
            case PType::PType_GrayINT64 :
            {
                ImageToStringDenseCompoundType_4PlanesDispatchPTypeHelper<dims,poutre::pINT64>(img,ostrm);                      
            }break;
            case PType::PType_F32 :
            {
                ImageToStringDenseCompoundType_4PlanesDispatchPTypeHelper<dims,poutre::pFLOAT>(img,ostrm);                                      
            }break;            
            case PType::PType_D64 :
            {
                ImageToStringDenseCompoundType_4PlanesDispatchPTypeHelper<dims,poutre::pDOUBLE>(img,ostrm);                                                      
            }break;
            
            default:
            {
                POUTRE_RUNTIME_ERROR(("ImageToStringDenseScalarDispatchPType::Unsupported pixel type:" + boost::lexical_cast<std::string>(ptype)));
            }
        }//switch (ptype)
    }
      
    template<size_t dims>  
    void ImageToStringDispatchDense(const poutre::IInterface& img,CompoundType cptype, PType ptype,std::ostringstream& ostrm) 
    {
        switch (cptype) {
            case CompoundType::CompoundType_Scalar:
            {
                ImageToStringDenseScalarDispatchPType<dims>(img,ptype,ostrm);
            }
            break;
            
            case CompoundType::CompoundType_3Planes:
            {
                ImageToStringDenseCompoundType_3PlanesDispatchPType<dims>(img,ptype,ostrm);
            }
            break;
            
            case CompoundType::CompoundType_4Planes:
            {
                ImageToStringDenseCompoundType_3PlanesDispatchPType<dims>(img,ptype,ostrm);
            }
            break;            
            
            default:
            {
                POUTRE_RUNTIME_ERROR(("ImageFromStringDispatch::Unsupported compound type:" + boost::lexical_cast<std::string>(cptype)));
            }
        }//switch (cptype)
    }
    
    //IMAGE TO STRING 
    std::string ImageToString(const poutre::IInterface& i_image) 
    {

        std::ostringstream ostrm;
        if (!ostrm.good()) {
            POUTRE_RUNTIME_ERROR("ImageFromString: unable to create ostrm");
        }
        auto imgtype =i_image.GetImgType();
        //TODO dispatch sparse type ?
        if (imgtype != ImgType::ImgType_Dense) {
            std::string err="ImageToString: ImgType unknown or not supported yet from"+boost::lexical_cast<std::string>(imgtype);
            POUTRE_RUNTIME_ERROR(err);
        }
        
        auto cptype = i_image.GetCType(); 
        auto ptype =  i_image.GetPType();
        auto numdim = i_image.GetNumDims();
        
        ostrm << imgtype << " ";
        ostrm << cptype<< " ";
        ostrm << ptype<< " ";
        ostrm << numdim << " ";
        for (auto dim : i_image.GetCoords()) {
            ostrm << dim << " ";
        }
        
        
        //Dispatch
        switch (numdim)
          {
        case 0:
          {
          POUTRE_RUNTIME_ERROR("Unsupported number of dims:0"); return std::move(ostrm.str());
          }break;
        case 1:
          {
            ImageToStringDispatchDense<1>(i_image,cptype,ptype,ostrm);
          }break;
        case 2:
          {
            ImageToStringDispatchDense<2>(i_image,cptype,ptype,ostrm);
          }break;
        case 3:
          {
            ImageToStringDispatchDense<3>(i_image,cptype, ptype,ostrm);
          }break;
        case 4:
          {
            ImageToStringDispatchDense<4>(i_image,cptype, ptype,ostrm);
          }break;
        default:
          {
          POUTRE_RUNTIME_ERROR("Unsupported number of dims");
          }
        }  
        return std::move(ostrm.str());
    }

} //namespace
