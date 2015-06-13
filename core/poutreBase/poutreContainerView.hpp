
//==============================================================================
//                  Copyright (c) 2015 - Thomas Retornaz                      //
//                     thomas.retornaz@mines-paris.org                        //
//          Distributed under the Boost Software License, Version 1.0.        //
//                 See accompanying file LICENSE.txt or copy at               //
//                     http://www.boost.org/LICENSE_1_0.txt                   //
//==============================================================================

#ifndef POUTRE_CONTAINER_VIEW_HPP__
#define POUTRE_CONTAINER_VIEW_HPP__

/**
 * @file   poutreContainerView.hpp
 * @author Thomas Retornaz
 * @brief Implement View and StridedView as defined in http://www.open-std.org/jtc1/sc22/wg21/docs/papers/2014/n3851.pdf
 *
 *
 */

#ifndef POUTRE_COORDINATE_HPP__
#include <poutreBase/poutreCoordinate.hpp>
#endif


#ifdef _MSC_VER
#pragma warning( push )
#pragma warning( disable : 4425 )//4425 improper support constexpr
#endif

#include <vector>

////http://stackoverflow.com/questions/26606865/static-for-cycle
//#include <tuple>
//
//namespace details
//  {
//  using std::tuple;
//  using std::tuple_cat;
//
//  template< int i >
//  struct Number_as_type_
//    {
//    };
//
//  template< int... values >
//  struct Int_sequence_
//    {
//    using As_tuple = tuple< Number_as_type_<values>... >;
//    };
//
//  template< int... values >
//  auto int_seq_from(tuple< Number_as_type_<values>... >)
//    ->Int_sequence_< values... >;
//
//  template< class Int_seq_a, class Int_seq_b >
//  using Concat_ = decltype(
//    int_seq_from(tuple_cat(
//    typename Int_seq_a::As_tuple( ), typename Int_seq_b::As_tuple( )
//    ))
//    );
//
//  template< int n_indices >
//  struct Index_sequence_t_
//    {
//    using T = Concat_<
//      typename Index_sequence_t_<n_indices - 1>::T, Int_sequence_<n_indices - 1>
//    >;
//    };
//
//  template<>
//  struct Index_sequence_t_<1>
//    {
//    using T = Int_sequence_<0>;
//    };
//
//  template< int n_indices >
//  using Index_sequence_ = typename Index_sequence_t_<n_indices>::T;
//  }  // namespace details
//
//template <size_t I, typename Functor, std::size_t... Indices>
//struct Apply<I, Functor, details::Index_sequence_<Indices...>> :
//  private std::tuple<Functor>
//  {
//  Apply(Functor f) : std::tuple<Functor>(f)
//    {
//    }
//  Apply( ) = default;
//
//  template <typename InputRange1, typename InputRange2, typename OutputRange>
//  void operator()(OutputRange& dst, const InputRange1& lhs, const InputRange2& rhs) const
//    {
//    (void) std::initializer_list<int>
//      { (dst[Indices] = std::get<0>(*this)(lhs[Indices], rhs[Indices]), 0)... };
//    }
//  };
//
////static for loop
//template <ptrdiff_t First, ptrdiff_t Last>
//struct static_for
//  {
//  template <typename Fn>
//  void operator()(Fn const& fn) const
//    {
//    if ( First < Last )
//      {
//      fn(First);
//      static_for<First + 1, Last>( )(fn);
//      }
//    }
//  };
//
////stop loop
//template <int N>
//struct static_for<N, N>
//  {
//  template <typename Fn>
//  void operator()(Fn const& fn) const
//    {
//    }
//  };


namespace poutre
{




   /*!
    * @defgroup view_group Container View and Strided View
    * Implement View and StridedView as defined in http://www.open-std.org/jtc1/sc22/wg21/docs/papers/2014/n3851.pdf
    * @ingroup coordinate_group_group
    * @{
    */

#define VIEW_ACESS(data,idx,stride,rank)           \
                   {                                             \
                  ptrdiff_t offset = 0;           \
                  for ( auto i = 0; i < Rank; ++i )          \
                                      {          \
                  offset += idx[i] * stride[i];          \
                                      }          \
                  return *(data + offset);       \
                   }

  /**
  * @brief An array_view is a potentially multidimensional view on a sequence of uniformly strided objects of a uniform type, contiguous in the least significant dimension.
  */
  template <class T, ptrdiff_t Rank = 1>
  class array_view
    {
    //convenient friendship
    template<class T, ptrdiff_t Rank> friend class strided_array_view;
    public:
      POUTRE_STATIC_CONSTEXPR ptrdiff_t rank = Rank;
#ifndef DOXYGEN_SHOULD_SKIP_THIS
      using index_type = index<Rank>;
      using bounds_type = bounds<Rank>;


      using size_type = size_t;
      using value_type = T;
      using const_value_type = std::add_const_t < value_type >;
      using pointer = std::add_pointer_t < value_type >;
      using reference = typename std::add_lvalue_reference_t< value_type >;
      using const_pointer = std::add_pointer_t < const_value_type >;
      using const_reference = typename std::add_lvalue_reference_t< const_value_type >;
      using difference_type = std::ptrdiff_t;
      
      using self_type = array_view < value_type, rank >;
#endif
      
    private:
      bounds_type m_bnd;
      pointer m_data;
  
    public:
      /** @name Construction and Assignment
      */
      /**@{*/

      //!Default ctor
      POUTRE_CONSTEXPR array_view( ) :m_bnd( ), m_data(nullptr) POUTRE_NOEXCEPT_IF(m_bnd())
      {
      }

      template <class Viewable>
      //TODO fix SFINAE behavior!!!!! 
      //typename std::enable_if<std::is_same<typename std::remove_cv<T>::type, typename std::remove_cv<typename Viewable::value_type>::type>::value
      //&&std::is_convertible<typename std::add_pointer<typename Viewable::value_type>::type, pointer>::value
      //&& Rank == 1*/>::type* = nullptr>
      //!ctor from viewable object, mainly contiguous container C providing C.data(),C.size() interface and using the same value_type
      //@warning only available for rank==1
      POUTRE_CONSTEXPR array_view(Viewable&& vw) :m_bnd(vw.size( )), m_data(vw.data())
      {
      static_assert(Rank == 1, "array_view(Viewable&& vw) is only allowed for rank=1 view");
      }
      
      template <class U, int AnyRank,
      typename std::enable_if<std::is_same<std::remove_cv_t<T>, std::remove_cv_t<U> >::value 
      && std::is_convertible<std::add_pointer_t<U>, pointer>::value
      && Rank == 1>::type* = nullptr>
      //!ctor from other array_view with AnyRank is only allowed for flattened view (ie Rank=1)
      //@warning only available for rank==1
      POUTRE_CONSTEXPR array_view(const array_view<U, AnyRank>& rhs) :m_bnd(rhs.bounds( ).size()), m_data(rhs.data( )) POUTRE_NOEXCEPT_IF(m_bnd(rhs.bounds()))
      {
      }


     //TODO !
     /* template <class ArrayType,
      typename std::enable_if<
      std::rank<ArrayType>::value == Rank
      && std::is_same<std::remove_cv_t<std::remove_all_extents_t<ArrayType>>, std::remove_cv_t<value_type>>::value
      && std::is_convertible<std::add_pointer_t<std::remove_all_extents_t<ArrayType>>, pointer>::value
      >::type* = nullptr>
      POUTRE_CONSTEXPR array_view(ArrayType& arr) :m_bnd( ), m_data(nullptr) POUTRE_NOEXCEPT_IF(m_bnd( ))
        {
        
        Apply<4, std::plus<>>( )(dest, lhs, rhs);
        m_data = &arr;
        }*/

      template <class U,
      typename std::enable_if<
      std::is_convertible<std::add_pointer_t<U>, pointer>::value && 
      std::is_same<std::remove_cv_t<U>, std::remove_cv_t<value_type>>::value
      >::type* = nullptr>
      //!copy ctor      
      POUTRE_CONSTEXPR array_view(const array_view<U, Rank>& rhs) :m_bnd(rhs.bounds()), m_data(rhs.m_data) POUTRE_NOEXCEPT_IF(m_bnd(rhs.m_bnd))
        {

        }

      
      template <class Viewable>
      //TODO fix SFINAE behavior!!!!! 
      //typename std::enable_if<std::is_same<typename std::remove_cv<T>::type, typename std::remove_cv<typename Viewable::value_type>::type>::value
      //&&std::is_convertible<typename std::add_pointer<typename Viewable::value_type>::type, pointer>::value
      //>::type* = nullptr>
      //!ctor from an other view
      //@note This constructor may be used to create an array_view with a different rank and/or bounds than the original array_view, i.e. reshape the view.
      POUTRE_CONSTEXPR array_view(Viewable&& vw, bounds_type bounds) :m_bnd(bounds), m_data(vw.data())
      {
      if ( vw.size( ) < bounds.size( ) )
        {
        POUTRE_RUNTIME_ERROR("array_view(Viewable&& vw, bounds_type bounds) can't create view, provided bounds overfit vw");
        }
      }
      
      //!ctor from nude ptr 
      //@warning Requires: [ptr, ptr + bounds.size()) is a valid range. 
      POUTRE_CONSTEXPR array_view(pointer ptr, bounds_type bounds) : m_bnd(bounds), m_data(ptr)
        {
        if ( !m_data && bounds.size( ) != 0 )
          {
          POUTRE_RUNTIME_ERROR("array_view(pointer ptr, bounds_type bounds) null ptr is allowed only if bound.size()==0");
          }
        }
      
      template <class U,
        typename std::enable_if<
        std::is_convertible<std::add_pointer_t<U>, pointer>::value &&
        std::is_same<std::remove_cv_t<U>, std::remove_cv_t<value_type>>::value
      >::type* = nullptr>
      //!assignment operator
      POUTRE_CXX14_CONSTEXPR self_type& operator=(const array_view<U, Rank>& rhs) POUTRE_NOEXCEPT
        {
        m_bnd = rhs.bounds( );
        m_data = rhs.data( );
        return *this
        }

      /**@}*/

      /** @name Observers
      */
      /**@{*/
      //!Getter bounds of view
      POUTRE_CONSTEXPR bounds_type bounds( ) const POUTRE_NOEXCEPT
      {
      return m_bnd;
      }

      //!Getter size of wiew
      POUTRE_CONSTEXPR size_type   size( ) const POUTRE_NOEXCEPT
      {
      return m_bnd.size();
      }

      //!Getter stride of view
      POUTRE_CXX14_CONSTEXPR index_type  stride( ) const POUTRE_NOEXCEPT
        {
        //todo caching in ctor ?
        index_type stride;
        stride[Rank - 1] = 1;
        for (ptrdiff_t i=Rank-2;i>=0;i--)
          {
          stride[i] = stride[i + 1] * m_bnd[i + 1];
          }
        return stride;
        }

      //!Getter raw access to underlying data ptr
      POUTRE_CONSTEXPR pointer data( ) const POUTRE_NOEXCEPT
      {
      return m_data;
      }
      /**@}*/


      /** @name Access
      */
      /**@{*/
      //!Accessor to underlying data through indexing
      POUTRE_CXX14_CONSTEXPR reference operator[](const index_type& idx) const /*POUTRE_NOEXCEPTONLYNDEBUG*/
        {
        POUTRE_ASSERTCHECK(m_bnd.contains(idx) == true,"Out of bound");
        VIEW_ACESS(this->data(), idx, this->stride(),Rank);
        }
      /**@}*/

      /** @name Slicing and sectioning
      */
      /**@{*/
      //! Return a view such that the initial element is (*this)[{slice, 0, 0, …..., 0}], and the bounds are {bounds()[1], bounds()[2], …..., bounds()[Rank - 1]}. 
      POUTRE_CXX14_CONSTEXPR array_view<T, Rank - 1>
      operator[](difference_type slice) const
          {
          static_assert(Rank>1, "Slicing is only allowed if Rank>1");
          POUTRE_CHECK(0 <= slice,"Slicing slice must be  >=0");
          POUTRE_CHECK(slice<m_bnd[0], "Slicing slice must be < bounds()[0]");

          //Slice bound
          bounds<Rank-1> bnd_slice;
          for ( auto i = 0; i<Rank - 2; i++ )
            {
            bnd_slice[i] = m_bnd[i + 1];
            }

          //Compute prt shift
          index_type idxslice;
          idxslice[0] = slice;          
          pointer data_slice = datas( ) + details::get_offset_from_coord(m_bnd, idxslice);

          return array_view<T, Rank - 1>(data_slice,bnd_slice);
          }


      POUTRE_CONSTEXPR strided_array_view<T, Rank>
        //!A strided view such that the initial element is (*this)[origin], the stride is stride(), and the bounds are section_bnd. 
        //@warning Requires: bounds().contains(origin + idx) == true for any index_type idx such that (bounds() - origin).contains(idx) == true. 
        section(const index_type& origin, const bounds_type& section_bnd) const
        {
        POUTRE_CHECK(m_bnd.contains(origin), "section(origin,section_bnd) origin is Out of bound");
        POUTRE_CHECK(m_bnd.contains((section_bnd + origin) - 1), "section(origin,section_bnd) section_bnd shifted by origins is out of bound");

        pointer data_section = datas( ) + details::get_offset_from_coord(m_bnd, origin);
        return strided_array_view<T, Rank>(data_section, section_bnd, stride());
        }

      POUTRE_CONSTEXPR strided_array_view<T, Rank>
        //! A strided view such that the initial element is (*this)[origin], the stride is stride(), and the bounds are (bounds() - origin). 
        //@warning Requires: bounds().contains(origin + idx) == true for any index_type idx such that (bounds() - origin).contains(idx) == true. 
        section(const index_type& origin) const
        {
        POUTRE_CHECK(m_bnd.contains(origin), "section(origin,section_bnd) origin is Out of bound");

        bounds_type section_bnd = m_bnd - origin;
        pointer data_section = datas( ) + details::get_offset_from_coord(m_bnd, origin);
        return strided_array_view<T, Rank>(data_section, section_bnd, stride( ));
        }
      /**@}*/
    };


  /**
  * @brief An strided_array_view is a potentially multidimensional view on a sequence of  strided objects of a uniform type
  */
  template <class T, ptrdiff_t Rank = 1>
  class strided_array_view
    {
    //convenient friendship
    template<class T, ptrdiff_t Rank> friend class array_view;
    public:
      POUTRE_STATIC_CONSTEXPR ptrdiff_t rank = Rank;
      using index_type = index<Rank>;
      using bounds_type = bounds<Rank>;

      using size_type = size_t;
      using value_type = T;
      using const_value_type = std::add_const_t < value_type >;
      using pointer = std::add_pointer_t < value_type >;
      using reference = typename std::add_lvalue_reference < value_type >::type;
      using const_pointer = std::add_pointer_t < const_value_type >;
      using const_reference = typename std::add_lvalue_reference < const_value_type >::type;
      using difference_type = std::ptrdiff_t;

      using self_type = strided_array_view < value_type, rank >;
    private:
      bounds_type m_bnd;
      index_type m_stride_idx;
      pointer m_data;

    public:

      /** @name Construction and Assignment
      */
      /**@{*/
      //!Empty ctor
      POUTRE_CONSTEXPR strided_array_view( ) :m_bnd( ), m_stride_idx( ), m_data(nullptr) POUTRE_NOEXCEPT_IF(POUTRE_NOEXCEPT_IF(m_bnd( )) && POUTRE_NOEXCEPT_IF(m_stride_idx( )))
        {
        }

      template <class U,
      typename std::enable_if<
        std::is_convertible<std::add_pointer_t<U>, pointer>::value &&
        std::is_same<std::remove_cv_t<U>, std::remove_cv_t<value_type>>::value
      >::type* = nullptr>
      //!Ctor from array_view
      POUTRE_CONSTEXPR strided_array_view(const array_view<U, Rank>& rhs) :m_bnd(rhs.m_bnd), m_stride_idx( ), m_data(rhs.m_data) POUTRE_NOEXCEPT_IF(POUTRE_NOEXCEPT_IF(m_bnd(rhs.m_bnd)) && POUTRE_NOEXCEPT_IF(m_stride_idx( )))
        {
        }

      template <class U,
        typename std::enable_if<
        std::is_convertible<std::add_pointer_t<U>, pointer>::value &&
        std::is_same<std::remove_cv_t<U>, std::remove_cv_t<value_type>>::value
      >::type* = nullptr>
      //!Copy ctor
      POUTRE_CONSTEXPR strided_array_view(const strided_array_view<U, Rank>& rhs) :m_bnd(rhs.m_bnd), m_stride_idx(rhs.m_stride_idx), m_data(rhs.m_data) POUTRE_NOEXCEPT_IF(POUTRE_NOEXCEPT_IF(m_sview_bnd(rhs.m_bnd)) && POUTRE_NOEXCEPT_IF(m_stride_idx(rhs.m_stride_idx)))
       {
       }

      //!ctor from nude ptr 
      //@warning Requires: [ptr, ptr + bounds.size()) is a valid range. 
      POUTRE_CONSTEXPR strided_array_view(pointer ptr, bounds_type bounds, index_type stride) : m_bnd(bounds), m_stride_idx(stride), m_data(ptr)
        {
        if ( !m_data && bounds.size( ) != 0 )
          {
          POUTRE_RUNTIME_ERROR("array_view(pointer ptr, bounds_type bounds) null ptr is allowed only if bound.size()==0");
          }
        if ( !bounds.contains(stride))
          {
          POUTRE_RUNTIME_ERROR("strided_array_view(pointer ptr, bounds_type bounds, index_type stride) provided stride is out of bounds");
          }
        }

      template <class U,
        typename std::enable_if<
        std::is_convertible<std::add_pointer_t<U>, pointer>::value &&
        std::is_same<std::remove_cv_t<U>, std::remove_cv_t<value_type>>::value
      >::type* = nullptr>
      //!Assignement operator
      POUTRE_CXX14_CONSTEXPR self_type& operator=(const strided_array_view<U, Rank>& rhs) POUTRE_NOEXCEPT
        {
        m_bnd = rhs.m_bnd;
        m_stride_idx = rhs.m_stride_idx
        m_data = rhs.m_data;
        return *this
        }

      /**@}*/

      /** @name Observers
      */
      /**@{*/
      //!Accessor bound
      POUTRE_CONSTEXPR bounds_type bounds( ) const POUTRE_NOEXCEPT
        {
        return m_bnd;
        }

      //!Accessor size eg underlying bound size
      POUTRE_CONSTEXPR size_type   size( ) const POUTRE_NOEXCEPT
        {
        return m_bnd.size( );
        }

      //!Accessor stride
      POUTRE_CONSTEXPR index_type  stride( ) const POUTRE_NOEXCEPT
        {
        return m_stride_idx;
        }
      /**@}*/

      /** @name Access
      */
      /**@{*/
      //!Accessor to underlying data through indexing
      POUTRE_CXX14_CONSTEXPR reference operator[](const index_type& idx) const  POUTRE_NOEXCEPTONLYNDEBUG
          {
          POUTRE_ASSERTCHECK(m_bnd.contains(idx) == true, "Out of bound");
          VIEW_ACESS(m_data, idx, m_stride_idx,Rank);
          }
      /**@}*/

      /** @name Slicing and sectioning
      */
      /**@{*/
      //! Return A strided view such that the initial element is(*this)[{slice, 0, 0, …..., 0}], the bounds are{bounds( )[1], bounds( )[2], …...,bounds()[Rank - 1] }, and the stride is{ stride( )[1], stride( )[2], …...,stride()[Rank - 1] }
      //@warning Requires: 0<=slice<bound()[0]
      POUTRE_CXX14_CONSTEXPR strided_array_view<T, Rank - 1>
        operator[](difference_type slice) const
        {
        static_assert(Rank>1, "Slicing is only allowed if Rank>1");
        POUTRE_CHECK(0 <= slice, "Slicing slice must be  >=0");
        POUTRE_CHECK(slice<m_bnd[0], "Slicing slice must be < bounds()[0]");

        //slice bound and stride
        bounds<Rank - 1> bnd_slice;
        index<Rank - 1>  stride_slice;
        for ( auto i = 0; i<Rank - 2; i++ )
          {
          bnd_slice[i] = m_bnd[i + 1];
          stride_slice[i] = m_stride_idx[i + 1];
          }

        //compute shift
        index_type idxslice;
        idxslice[0] = slice;
        pointer data_slice = datas( ) + details::get_offset_from_coord(m_bnd, idxslice);

        return strided_array_view<T, Rank - 1>(data_slice, bnd_slice, stride_slice);
        }

      POUTRE_CONSTEXPR strided_array_view<T, Rank>
        //! A strided view such that the initial element is (*this)[origin], the stride is stride(), and the bounds are section_bnd.
        //@warning Requires: bounds().contains(origin + idx) == true for any index_type idx such that (bounds() - origin).contains(idx) == true. 
        section(const index_type& origin, const bounds_type& section_bnd) const
        {
        POUTRE_CHECK(m_bnd.contains(origin), "section(origin,section_bnd) origin is Out of bound");
        POUTRE_CHECK(m_bnd.contains(section_bnd+origin-1), "section(origin,section_bnd) section_bnd shifted by origins is out of bound");

        pointer data_section = datas( ) + details::get_offset_from_coord(m_bnd, origin);
        return strided_array_view<T, Rank>(data_section, section_bnd, m_stride_idx);
        }

      POUTRE_CONSTEXPR strided_array_view<T, Rank>
        //! A strided view such that the initial element is (*this)[origin], the stride is stride(), and the bounds are (bounds() - origin). 
        // @warning Requires: bounds().contains(origin + idx) == true for any index_type idx such that (bounds() - origin).contains(idx) == true. 
        section(const index_type& origin) const
        {
        POUTRE_CHECK(m_bnd.contains(origin), "section(origin,section_bnd) origin is Out of bound");

        bounds_type section_bnd = m_bnd-origin;
        pointer data_section = datas( ) + details::get_offset_from_coord(m_bnd, origin);
        return strided_array_view<T, Rank>(data_section, section_bnd, m_stride_idx);
        }
      /**@}*/
    };
  //! @} doxygroup: coordinate_group   

} //namespace poutre

#ifdef _MSC_VER
#pragma warning( pop ) 
#endif

#endif //POUTRE_CONTAINER_VIEW_HPP__
