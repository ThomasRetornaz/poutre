
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

#include <poutreBase/poutreTrace.hpp>
#include <poutreIO/include/poutreIOTIFF.hpp>
#include <poutreImageProcessing/core/include/poutreImageProcessingContainer.hpp>

namespace poutre
{
    namespace details
    {
        namespace bf = boost::filesystem;
        std::unique_ptr<IInterface> IOLoadTiff(const bf::path &i_path)
        {
            return std::unique_ptr<IInterface>();
        }
        void IOSaveTiff(const bf::path &i_path, const poutre::IInterface &i_image)
        {
        }
    } // namespace details

} // namespace poutre
