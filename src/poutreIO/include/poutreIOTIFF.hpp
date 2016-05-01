
//==============================================================================
//                  Copyright (c) 2015 - Thomas Retornaz                      //
//                     thomas.retornaz@mines-paris.org                        //
//          Distributed under the Boost Software License, Version 1.0.        //
//                 See accompanying file LICENSE.txt or copy at               //
//                     http://www.boost.org/LICENSE_1_0.txt                   //
//==============================================================================

/*
* File:   poutreIOPNG.hpp
* Author: thomas retornaz
*
* Created on 19 mars 2016
*/

#ifndef POUTREIOTIFF_HPP
#define	POUTREIOTIFF_HPP

#include <string>
#include <memory>

#ifndef POUTRE_IO_HPP__
#include <poutreIO/poutreIO.hpp>
#endif

#ifndef POUTRE_IMAGEPROCESSING_INTERFACE_HPP__
#include <poutreImageProcessing/core/poutreImageProcessingInterface.hpp>
#endif

#include <boost/filesystem.hpp>

namespace poutre
{
	namespace details
	{
		namespace bf = boost::filesystem;
		IO_API  std::unique_ptr<IInterface> IOLoadTiff(const bf::path& i_path);
		IO_API  void IOSaveTiff(const poutre::IInterface& i_image);

	}
}
#endif	/* POUTREIOTIFF_HPP */

