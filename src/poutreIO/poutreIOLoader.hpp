
//==============================================================================
//                  Copyright (c) 2015 - Thomas Retornaz                      //
//                     thomas.retornaz@mines-paris.org                        //
//          Distributed under the Boost Software License, Version 1.0.        //
//                 See accompanying file LICENSE.txt or copy at               //
//                     http://www.boost.org/LICENSE_1_0.txt                   //
//==============================================================================

/*
* File:   poutreIOLoader.hpp
* Author: thomas
*
* Created on 15 mars 2016
*/

#ifndef POUTREIOLOADER_HPP
#define	POUTREIOLOADER_HPP

#include <string>
#include <memory>

#ifndef POUTRE_IO_HPP__
#include <poutreIO/poutreIO.hpp>
#endif

#ifndef POUTRE_IMAGEPROCESSING_INTERFACE_HPP__
#include <poutreImageProcessingCore/poutreImageProcessingInterface.hpp>
#endif

#include <boost/filesystem.hpp>

namespace poutre
{
	class ImageProvider;
}

namespace poutre
{
	namespace bf = boost::filesystem;

	class IO_API ImageLoader
	{
		using self_type = ImageLoader;
		friend class ImageProvider;
		//bf::path m_imgPath;
        std::string m_imgPath;
		bool m_isready;
	public:
		ImageLoader() :m_imgPath(),m_isready(false){}
		
        //rule of 0
		//virtual ~ImageLoader() {}

		//ImageLoader(self_type &&rhs) = default;

		//self_type &operator=(self_type &&rhs) = default;

		//ImageLoader(const self_type& rhs) = default;
		//
		//self_type& operator=(const self_type& rhs) = default;
		
        self_type& SetPath(std::string&& i_imgpath);

        self_type& SetPath(const std::string& i_imgpath);

        std::unique_ptr<IInterface> Load() const;
	};
}//poutre

#endif	/* POUTREIOLOADER_HPP */

