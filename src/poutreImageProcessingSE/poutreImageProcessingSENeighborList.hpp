
//==============================================================================
//                  Copyright (c) 2018 - Thomas Retornaz                      //
//                     thomas.retornaz@mines-paris.org                        //
//          Distributed under the Boost Software License, Version 1.0.        //
//                 See accompanying file LICENSE.txt or copy at               //
//                     http://www.boost.org/LICENSE_1_0.txt                   //
//==============================================================================

#ifndef POUTRE_IPSENL_HPP__
#define POUTRE_IPSENL_HPP__

#ifndef POUTRE_IPSECORE_HPP__
#include <poutreImageProcessingSE/poutreImageProcessingSEInterface.hpp>
#endif
#ifndef POUTRE_COORDINATE_HPP__
#include <poutreBase/poutreCoordinate.hpp>
#endif
/**
 * @file   poutreImageProcessingSENeighborList.hpp
 * @author Thomas Retornaz
 * @brief  Static Neighborlist Structuring element type 
 *
 *
 */

namespace poutre
{
    namespace se
    {
        /**
         * @addtogroup se_nl_grp NeighborList Structuring Element Group
         * @ingroup se_grp
         *@{
         */
       enum class NeighborListStaticSE
       {
          NeighborListStaticSE2DCTUndefined,          //!< Undefined type 
          NeighborListStaticSE2DCT8,     //!< Connection 8 SE  
          NeighborListStaticSE2DCT4,    //!< Connection 4 SE
       };
         //! @} // se_nl_grp

       template<NeighborListStaticSE>
       struct NeighborListStaticSETraits {};

       template<>
       struct NeighborListStaticSETraits< NeighborListStaticSE::NeighborListStaticSE2DCT8>
       {
          POUTRE_STATIC_CONSTEXPR ptrdiff_t Rank = 2;
          const static ptrdiff_t HalfSize; //FXIME should be constexpr
		  const static std::array<idx2d, 9> NlList;
		  const static std::array<idx2d, 9> NlListTransposed;
       };
       
       const ptrdiff_t NeighborListStaticSETraits< NeighborListStaticSE::NeighborListStaticSE2DCT8>::HalfSize = 1;
	   const std::array<idx2d, 9> NeighborListStaticSETraits< NeighborListStaticSE::NeighborListStaticSE2DCT8>::NlList = {
	   idx2d{ 0,0 },
	   //
	   idx2d{ -1,-1 },
	   idx2d{ -1,0 },
	   idx2d{ -1,1 },
	   //
	   idx2d{ 0,-1 },
	   idx2d{ 0,1 },
	   //
	   idx2d{ 1,-1 },
	   idx2d{ 1,0 },
	   idx2d{ 1,1 },
	   };
	   const std::array<idx2d, 9> NeighborListStaticSETraits< NeighborListStaticSE::NeighborListStaticSE2DCT8>::NlListTransposed = {
	   idx2d{ 0,0 },
	   //
	   idx2d{ -1,-1 },
	   idx2d{ -1,0 },
	   idx2d{ -1,1 },
	   //
	   idx2d{ 0,-1 },
	   idx2d{ 0,1 },
	   //
	   idx2d{ 1,-1 },
	   idx2d{ 1,0 },
	   idx2d{ 1,1 },
	   };
	   template<>
	   struct NeighborListStaticSETraits< NeighborListStaticSE::NeighborListStaticSE2DCT4>
	   {
		   POUTRE_STATIC_CONSTEXPR ptrdiff_t Rank = 2;
		   const static ptrdiff_t HalfSize; //FXIME should be constexpr
		   const static std::array<idx2d, 5> NlList;
		   const static std::array<idx2d, 5> NlListTransposed;
	   };

	   const ptrdiff_t NeighborListStaticSETraits< NeighborListStaticSE::NeighborListStaticSE2DCT4>::HalfSize = 1;
	   const std::array<idx2d, 5> NeighborListStaticSETraits< NeighborListStaticSE::NeighborListStaticSE2DCT4>::NlList = {
	   idx2d{ 0,0 },
	   //
	   idx2d{ -1,0 },
	   //
	   idx2d{ 0,-1 },
	   idx2d{ 0,1 },
	   //
	   idx2d{ 1,0 }
	   };

	   const std::array<idx2d, 5> NeighborListStaticSETraits< NeighborListStaticSE::NeighborListStaticSE2DCT4>::NlListTransposed = {
	   idx2d{ 0,0 },
	   //
	   idx2d{ -1,0 },
	   //
	   idx2d{ 0,-1 },
	   idx2d{ 0,1 },
	   //
	   idx2d{ 1,0 }
	   };
    }
    
}
#endif //POUTRE_IPSENL_HPP__