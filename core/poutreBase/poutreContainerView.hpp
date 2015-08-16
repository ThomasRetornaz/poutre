
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

#ifndef DOXYGEN_SHOULD_SKIP_THIS
   //forward declare
   template <class T, ptrdiff_t Rank> class array_view;
   template <class T, ptrdiff_t Rank> class strided_array_view;
#endif
	
	namespace details
	{
		//from http://stackoverflow.com/questions/11056714/c-type-traits-to-extract-template-parameter-class
		template<typename T>
			struct extract_value_type
		{
			typedef T value_type;
		};

		template<template<typename, typename ...> class X, typename T, typename ...Args>
			struct extract_value_type<X<T, Args...>>   //specialization
		{
			typedef T value_type;
		};
	} //namespace details
	
	



   /*!
    * @defgroup view_group Container View and Strided View
    * Implement View and StridedView as defined in http://www.open-std.org/jtc1/sc22/wg21/docs/papers/2014/n3851.pdf
    * @ingroup coordinate_group_group
    * @{
    */

//@todo unroll
#define VIEW_ACESS(data,idx,stride,rank)            \
                   {                                \
                  ptrdiff_t offset = 0;             \
                  for ( auto i = 0; i < Rank; ++i ) \
                                      {             \
                  offset += idx[i] * stride[i];     \
                                      }             \
                  return *(data + offset);          \
                   }

//@todo unroll
#define INIT_DEFAULT_STRIDE(bound,stride,rank) \
   stride[rank - 1] = 1;                       \
   for ( ptrdiff_t i = Rank - 2; i >= 0; i-- ) \
    {                                          \
    stride[i] = stride[i + 1] * bound[i + 1];  \
    }

  /**
  * @brief An array_view is a potentially multidimensional view on a sequence of uniformly strided objects of a uniform type, contiguous in the least significant dimension.
  */
  template <class T, ptrdiff_t Rank // = 1
	  >
  class array_view
    {
    //convenient friendship
    template<class , ptrdiff_t > friend class strided_array_view;
    public:
      POUTRE_STATIC_CONSTEXPR ptrdiff_t rank= Rank;
#ifndef DOXYGEN_SHOULD_SKIP_THIS
      using index_type = index<Rank>;
      using bounds_type = bounds<Rank>;


      using size_type = size_t;
      using value_type = T;
	  using const_value_type = typename std::add_const<value_type>::type;
	  using pointer = typename std::add_pointer< value_type>::type;
	  using reference = typename std::add_lvalue_reference<value_type>::type;
	  using const_pointer = typename std::add_pointer<const_value_type>::type;
	  using const_reference = typename std::add_lvalue_reference<const_value_type>::type;
      using difference_type = std::ptrdiff_t;
      
      using self_type = array_view < value_type, rank >;
#endif
      
    private:
      bounds_type m_bnd;
      index_type m_stride_idx;
      pointer m_data;
		
    public:
      /** @name Construction and Assignment
      */
      /**@{*/

      //!Default ctor
      POUTRE_CXX14_CONSTEXPR array_view( ) POUTRE_NOEXCEPT_IF(POUTRE_NOEXCEPT_EXPR(bounds_type( )) && POUTRE_NOEXCEPT_EXPR(index_type( )))
        :m_bnd( ), m_stride_idx( ), m_data(nullptr)
        {
        INIT_DEFAULT_STRIDE(m_bnd, m_stride_idx, Rank);
        }
      template <class Viewable>
      //template <class Viewable,
      //  typename std::enable_if<
      //  std::is_same<typename std::remove_cv<T>::type, typename std::remove_cv<typename details::extract_value_type<Viewable>::value_type>::type>::value
      //  && std::is_convertible<typename std::add_pointer<typename details::extract_value_type<Viewable>::value_type>::type, pointer>::value
      //  && Rank == 1 /*,int */>::type* = nullptr>
      //!ctor from viewable object, mainly contiguous container C providing C.data(),C.size() interface and using the same value_type
      //@warning only available for rank==1
      POUTRE_CXX14_CONSTEXPR array_view(Viewable&& vw) :m_bnd(vw.size( )), m_stride_idx( ), m_data(vw.data( ))
      {
      static_assert(Rank == 1, "array_view(Viewable&& vw) is only allowed for rank=1 view");
      INIT_DEFAULT_STRIDE(m_bnd, m_stride_idx, Rank);
      }
      
      template <class U, ptrdiff_t AnyRank,
      typename std::enable_if<
		  std::is_same<typename std::remove_cv<U>::type, typename std::remove_cv<value_type>::type>::value &&
		  std::is_convertible<typename std::add_pointer<U>::type, pointer>::value &&
		  Rank == 1>::type* = nullptr>
      //!ctor from other array_view with AnyRank is only allowed for flattened view (ie Rank=1)
      //@warning only available for rank==1
      POUTRE_CONSTEXPR array_view(const array_view<U, AnyRank>& rhs) POUTRE_NOEXCEPT
      :m_bnd(rhs.m_bnd.size( )), m_stride_idx(rhs.m_stride_idx), m_data(rhs.m_data)
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
		  std::is_convertible<typename std::add_pointer<U>::type, pointer>::value &&
		  std::is_same<typename std::remove_cv<U>::type, typename std::remove_cv<value_type>::type>::value
      >::type* = nullptr>
      //!copy ctor      
      POUTRE_CONSTEXPR array_view(const array_view<U, Rank>& rhs) POUTRE_NOEXCEPT_IF(POUTRE_NOEXCEPT_EXPR((m_bnd(rhs.m_bnd))) && POUTRE_NOEXCEPT_EXPR((m_stride_idx(rhs.m_stride_idx))))
      :m_bnd(rhs.m_bnd), m_stride_idx(rhs.m_stride_idx), m_data(rhs.m_data)
        {

        }

      
      template <class Viewable>
      //TODO fix SFINAE behavior!!!!! 
      /*  typename std::enable_if<
        std::is_convertible<typename std::add_pointer<typename details::extract_value_type<Viewable>::value_type>::type, pointer>::value &&
        std::is_same<typename std::remove_cv<typename details::extract_value_type<Viewable>::value_type>::type, typename std::remove_cv<value_type>::type>::value
      >::type* = nullptr>*/

      //!ctor from an other view
      //@note This constructor may be used to create an array_view with a different rank and/or bounds than the original array_view, i.e. reshape the view.
      POUTRE_CXX14_CONSTEXPR array_view(Viewable&& vw, bounds_type bound) :m_bnd(bound), m_stride_idx(), m_data(vw.data( ))
      {
      if ( vw.size( ) < bound.size( ) )
        {
        POUTRE_RUNTIME_ERROR("array_view(Viewable&& vw, bounds_type bound) can't create view, provided bounds overfit vw");
        }
      INIT_DEFAULT_STRIDE(m_bnd, m_stride_idx, Rank);
      }
      
      //!ctor from nude ptr 
      //@warning Requires: [ptr, ptr + bounds.size()) is a valid range. 
      POUTRE_CXX14_CONSTEXPR array_view(pointer ptr, bounds_type bound) : m_bnd(bound), m_stride_idx( ), m_data(ptr)
        {
        if ( !m_data && bound.size( ) != 0 )
          {
          POUTRE_RUNTIME_ERROR("array_view(pointer ptr, bounds_type bound) null ptr is allowed only if bound.size()==0");
          }
        INIT_DEFAULT_STRIDE(m_bnd, m_stride_idx, Rank);
        }
      
      template <class U,
        typename std::enable_if<
			std::is_convertible<typename std::add_pointer<U>::type, pointer>::value &&
			std::is_same<typename std::remove_cv<U>::type, typename std::remove_cv<value_type>::type>::value
      >::type* = nullptr>
      //!assignment operator
      POUTRE_CXX14_CONSTEXPR self_type& operator=(const array_view<U, Rank>& rhs) POUTRE_NOEXCEPT
        {
        m_bnd = rhs.m_bnd;
        m_stride_idx = rhs.m_stride_idx;
        m_data = rhs.m_data;
        return *this;
		}

      /**@}*/

      /** @name Observers
      */
      /**@{*/
      //!Getter bounds of view
      POUTRE_CONSTEXPR bounds_type bound( ) const POUTRE_NOEXCEPT
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
        return m_stride_idx;
        }

      //!Getter raw access to underlying data ptr to the contiguous sequence on which the view was created. 
      POUTRE_CONSTEXPR pointer data( ) const POUTRE_NOEXCEPT
      {
      return m_data;
      }
      /**@}*/


      /** @name Access
      */
      /**@{*/
      //!Accessor to underlying data through indexing
      POUTRE_CXX14_CONSTEXPR reference operator[](const index_type& idx) const POUTRE_NOEXCEPTONLYNDEBUG
        {
        POUTRE_ASSERTCHECK(m_bnd.contains(idx) == true,"Out of bound");
        //VIEW_ACESS(this->data(), idx, this->stride(),Rank);
        VIEW_ACESS(m_data, idx, m_stride_idx, Rank);
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
          poutre::bounds<Rank-1> bnd_slice;
          for ( auto i = 0; i<Rank - 1; i++ )
            {
            bnd_slice[i] = m_bnd[i + 1];
            }

          //Compute prt shift
          index_type idxslice;
          idxslice[0] = slice;          
          pointer data_slice = m_data + details::get_offset_from_coord<bounds_type, index_type>::op(m_bnd, idxslice);

          return array_view<T, Rank - 1>(data_slice,bnd_slice);
          }


      POUTRE_CXX14_CONSTEXPR strided_array_view<T, Rank>
        //!A strided view such that the initial element is (*this)[origin], the stride is stride(), and the bounds are section_bnd. 
        //@warning Requires: bounds().contains(origin + idx) == true for any index_type idx such that (bounds() - origin).contains(idx) == true. 
        section(const index_type& origin, const bounds_type& section_bnd) const
        {
        //precondition
        POUTRE_CHECK(m_bnd.contains(origin), "section(origin,section_bnd) origin is Out of bound");
        index_type shifted = section_bnd + origin; shifted -= 1;
        POUTRE_CHECK(m_bnd.contains(shifted), "section(origin,section_bnd) section_bnd shifted by origins is out of bound");

        pointer data_section = m_data + details::get_offset_from_coord<bounds_type, index_type>::op(m_bnd, origin);
        return strided_array_view<T, Rank>(data_section, section_bnd, stride());
        }

      POUTRE_CXX14_CONSTEXPR strided_array_view<T, Rank>
        //! A strided view such that the initial element is (*this)[origin], the stride is stride(), and the bounds are (bounds() - origin). 
        //@warning Requires: bounds().contains(origin + idx) == true for any index_type idx such that (bounds() - origin).contains(idx) == true. 
        section(const index_type& origin) const
        {
        POUTRE_CHECK(m_bnd.contains(origin), "section(origin,section_bnd) origin is Out of bound");

        bounds_type section_bnd = m_bnd - origin;
        pointer data_section = m_data + details::get_offset_from_coord<bounds_type, index_type>::op(m_bnd, origin);
        return strided_array_view<T, Rank>(data_section, section_bnd, stride( ));
        }
      /**@}*/
    };


  /**
  * @brief An strided_array_view is a potentially multidimensional view on a sequence of  strided objects of a uniform type
  */
  template <class T, ptrdiff_t Rank // = 1
	  >
  class strided_array_view
    {
    //convenient friendship
    template<class, ptrdiff_t> friend class array_view;
    public:
      POUTRE_STATIC_CONSTEXPR ptrdiff_t rank= Rank;
      using index_type = index<Rank>;
      using bounds_type =  bounds<Rank>;

      using size_type = size_t;
      using value_type = T;
		using const_value_type = typename std::add_const < value_type >::type;
		using pointer = typename std::add_pointer < value_type >::type;
      using reference = typename std::add_lvalue_reference < value_type >::type;
		using const_pointer = typename std::add_pointer < const_value_type >::type;
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
		POUTRE_CXX14_CONSTEXPR strided_array_view( ) POUTRE_NOEXCEPT_IF(POUTRE_NOEXCEPT_EXPR(bounds_type( )) && POUTRE_NOEXCEPT_EXPR(index_type( )))
		  :m_bnd(), m_stride_idx(), m_data(nullptr) 
        {
        INIT_DEFAULT_STRIDE(m_bnd, m_stride_idx, Rank);
        }

	  template <class U,
      typename std::enable_if<
		  std::is_convertible<typename std::add_pointer<U>::type, pointer>::value &&
		  std::is_same<typename std::remove_cv<U>::type, typename std::remove_cv<value_type>::type>::value
      >::type* = nullptr>
//!Ctor from array_view
      POUTRE_CONSTEXPR strided_array_view(const array_view<U, Rank>& rhs) POUTRE_NOEXCEPT_IF(POUTRE_NOEXCEPT_EXPR(m_bnd(rhs.m_bnd)) && POUTRE_NOEXCEPT_EXPR(m_stride_idx( )))
		  :m_bnd(rhs.m_bnd), m_stride_idx(rhs.stride()), m_data(rhs.m_data) 
        {
        }

      template <class U,
      typename std::enable_if<
		  std::is_convertible<typename std::add_pointer<U>::type, pointer>::value &&
		  std::is_same<typename std::remove_cv<U>::type, typename std::remove_cv<value_type>::type>::value
      >::type* = nullptr>		  
      //!Copy ctor
      POUTRE_CONSTEXPR strided_array_view(const strided_array_view<U, Rank>& rhs) POUTRE_NOEXCEPT_IF(POUTRE_NOEXCEPT_EXPR(m_sview_bnd(rhs.m_bnd)) && POUTRE_NOEXCEPT_EXPR(m_stride_idx(rhs.m_stride_idx)))
		  :m_bnd(rhs.m_bnd), m_stride_idx(rhs.m_stride_idx), m_data(rhs.m_data) 
       {
       }

      //!ctor from nude ptr 
      //@warning Requires: [ptr, ptr + bounds.size()) is a valid range. 
      POUTRE_CXX14_CONSTEXPR strided_array_view(pointer ptr, bounds_type bounds, index_type stride)
		  : m_bnd(bounds), m_stride_idx(stride), m_data(ptr)
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
		  std::is_convertible<typename std::add_pointer<U>::type, pointer>::value &&
		  std::is_same<typename std::remove_cv<U>::type, typename std::remove_cv<value_type>::type>::value
      >::type* = nullptr>
      //!Assignement operator
      POUTRE_CXX14_CONSTEXPR self_type& operator=(const strided_array_view<U, Rank>& rhs) POUTRE_NOEXCEPT
        {
        m_bnd = rhs.m_bnd;
        m_stride_idx = rhs.m_stride_idx;
		m_data = rhs.m_data;
        return *this;
		}

      /**@}*/

      /** @name Observers
      */
      /**@{*/
      //!Accessor bound
      POUTRE_CONSTEXPR bounds_type bound( ) const POUTRE_NOEXCEPT
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
        bounds<Rank-1> bnd_slice;
        index<Rank-1>  stride_slice;
        for ( auto i = 0; i<Rank - 2; i++ )
          {
          bnd_slice[i] = m_bnd[i + 1];
          stride_slice[i] = m_stride_idx[i + 1];
          }

        //compute shift
        index_type idxslice;
        idxslice[0] = slice;
        pointer data_slice = m_data + details::get_offset_from_coord<bounds_type, index_type>::op(m_bnd, idxslice);

        return strided_array_view<T, Rank - 1>(data_slice, bnd_slice, stride_slice);
        }

      POUTRE_CXX14_CONSTEXPR strided_array_view<T, Rank>
        //! A strided view such that the initial element is (*this)[origin], the stride is stride(), and the bounds are section_bnd.
        //@warning Requires: bounds().contains(origin + idx) == true for any index_type idx such that (bounds() - origin).contains(idx) == true. 
        section(const index_type& origin, const bounds_type& section_bnd) const
        {
        POUTRE_CHECK(m_bnd.contains(origin), "section(origin,section_bnd) origin is Out of bound");
        POUTRE_CHECK(m_bnd.contains(section_bnd+origin-1), "section(origin,section_bnd) section_bnd shifted by origins is out of bound");

        pointer data_section = m_data + details::get_offset_from_coord<bounds_type, index_type>::op(m_bnd, origin);
        return strided_array_view<T, Rank>(data_section, section_bnd, m_stride_idx);
        }

      POUTRE_CXX14_CONSTEXPR strided_array_view<T, Rank>
        //! A strided view such that the initial element is (*this)[origin], the stride is stride(), and the bounds are (bounds() - origin). 
        // @warning Requires: bounds().contains(origin + idx) == true for any index_type idx such that (bounds() - origin).contains(idx) == true. 
        section(const index_type& origin) const
        {
        POUTRE_CHECK(m_bnd.contains(origin), "section(origin,section_bnd) origin is Out of bound");

        bounds_type section_bnd = m_bnd-origin;
        pointer data_section = m_data + details::get_offset_from_coord<bounds_type, index_type>::op(m_bnd, origin);
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
