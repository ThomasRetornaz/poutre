
//==============================================================================
//                  Copyright (c) 2015 - Thomas Retornaz                      //
//                     thomas.retornaz@mines-paris.org                        //
//          Distributed under the Boost Software License, Version 1.0.        //
//                 See accompanying file LICENSE.txt or copy at               //
//                     http://www.boost.org/LICENSE_1_0.txt                   //
//==============================================================================

#ifndef POUTRE_IMAGEPROCESSING_CONTAINER_COPIECONVERT_HPP__
#define POUTRE_IMAGEPROCESSING_CONTAINER_COPIECONVERT_HPP__


#ifndef POUTRE_IMAGEPROCESSING_CONTAINER_HPP__
#include <poutreImageProcessingCore/include/poutreImageProcessingContainer.hpp>
#endif

namespace poutre
  {
	  
	  //template< class ViewIn, class ViewOut>
	  //struct CopyViewDispatcherHelper
	  //{
		 // void operator()(const ViewIn& i_viewin, ViewOut& i_viewout)
		 // {
			//  //todo
		 // }

	  //};

	  //template< class ViewInOut>
	  //struct CopyViewDispatcherHelper
	  //{
		 // void operator()(const ViewInOut& i_viewin, ViewInOut& i_viewout)
		 // {
			//  POUTRE_ASSERTCHECK(i_viewin.size() == i_viewout.size(), "CopyViewDispatcherHelper size of view are not compatible");
			//  auto vInbound = i_viewin.bound();
			//  auto vOutbound = i_viewout.bound();
			//  auto stridevIN = vIn.stride();
			//  auto stridevOut = vOut.stride();

			//  if (vInbound != vOutbound || stridevIN != stridevOut) //two index 
			//  {
			//	  auto idx1 = begin(vInbound);
			//	  auto idx2 = begin(vOutbound);
			//	  auto idx1end = end(vInbound);
			//	  for (; idx1 != idx1end; ++idx1, ++idx2)
			//	  {
			//		  i_viewout[idx2] = i_viewin[idx1];
			//	  }
			//	  return;
			//  }
			//  auto idx1 = begin(vInbound);
			//  auto idx1end = end(vInbound);
			//  for (; idx1 != idx1end; ++idx1)
			//  {
			//	  i_viewout[idx1] = i_viewin[idx1];
			//  }
			//  return;
		 // }
	  //};
  }
#endif //POUTRE_IMAGEPROCESSING_CONTAINER_COPIECONVERT_HPP__
