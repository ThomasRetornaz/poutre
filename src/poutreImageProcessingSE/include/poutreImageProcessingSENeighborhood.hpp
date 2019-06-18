
//==============================================================================
//                  Copyright (c) 2018 - Thomas Retornaz                      //
//                     thomas.retornaz@mines-paris.org                        //
//          Distributed under the Boost Software License, Version 1.0.        //
//                 See accompanying file LICENSE.txt or copy at               //
//                     http://www.boost.org/LICENSE_1_0.txt                   //
//==============================================================================

#ifndef POUTRE_IPSENL_HPP__
#define POUTRE_IPSENL_HPP__

#ifndef POUTRE_IPSEINTERFACE_HPP__
#include <poutreImageProcessingSE/poutreImageProcessingSEInterface.hpp>
#endif
#ifndef POUTRE_COORDINATE_HPP__
#include <poutreBase/poutreCoordinate.hpp>
#endif
/**
 * @file   poutreImageProcessingSENeighborList.hpp
 * @author Thomas Retornaz
 * @brief  Define Neighborlist Structuring element type
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

      template <std::ptrdiff_t NumDims = 2>
      struct NeighborlistSE :public IStructuringElement
      {
         static_assert(NumDims > 0, "NumDims must be >0");
      public:
         using self_type = NeighborlistSE<NumDims>;
         using parent_type = IStructuringElement;
         using coordinate_type = index<NumDims>;
         //using surrounding_extent = bounds<NumDims>;
         using storage_type = std::vector<coordinate_type>;
         using iterator = typename storage_type::iterator;
         using const_iterator = typename storage_type::const_iterator;

      private:
         constexpr std::size_t m_numdims = NumDims;
         static const SEType m_setype = SEType::SEType_NL;
         storage_type m_coordinates;

      public:
         //! Default constructor
         NeighborlistSE() : storage()
         {}

         //! CTOR with explicit set of list of 
         NeighborlistSE(const storage_type& coordinates) :
            m_coordinates(coordinates)
         {}

         const_iterator begin() const {
            return m_coordinates.begin();
         }

         const_iterator end() const {
            return m_coordinates.end();
         }

         void SetCoordinates(const storage_type& coordinates) {
            m_coordinates = coordinates;
         }

         const storage_type& GetCoordinates() const noexcept {
            return m_coordinates;
         }

         //! Returns a transposed copy of this structuring element
         self_type transpose() const {
            const auto& coord = this->GetCoordinates();
            coordinate_type outcoord; out.reserve(coord.size());
            for (const auto& c : coord)
            {
               out.push_back(-c);
            }
            self_type ret(outcoord);
            return ret;
         }

         //! Returns the size of the structuring element
         size_t size() const {
            return m_coordinates.size();
         }

         /*!Returns the maximal extension of this structuring element
          * The parameter @c dimension controls the dimension on which the extension is queried, while @c direction
          * specifies the direction (true for forward, false for backward).
          */
         ptrdiff_t MaxExtent(const ptrdiff_t rank, const bool direction) const {
            if (direction) {
               ptrdiff_t extension = 0;
               for (const auto& c : m_coordinates) {
                  extension = std::max(extension, c[i][dimension]);
               }
               return extension;
            }
            else {
               typename element_t::scalar_coordinate_type extension = 0;
               for (const auto& c : m_coordinates) {
                  extension = std::max(extension, -c[i][dimension]);
               }
               return extension;
            }
         }

         /*!
          *@name  Virtual methods inherited from IStructuringElement
          *@{
          */
         virtual SEType GetType() const override {
            return m_setype;
         }
         virtual size_t GetSize() const override {
            return m_coordinates.size();
         }

         virtual std::unique_ptr<IStructuringElement> Transpose() const override {
            return std::unique_ptr<IStructuringElement>(new self_type(this->transpose()));
         }

         virtual std::unique_ptr<IStructuringElement> Clone() const override {
            return std::unique_ptr<IStructuringElement>(new self_type(*this));
         }

         //! @}
      };



      //! @} // se_nl_grp
   }

}
#endif //POUTRE_IPSENL_HPP__