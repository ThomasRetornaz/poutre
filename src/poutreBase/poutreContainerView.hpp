
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
 * @brief Implement View and StridedView as defined in https://isocpp.org/files/papers/N4494.html
 *
 *
 */

#ifndef POUTRE_CONFIG__HPP__
#include <poutreBase/poutreConfig.hpp>
#endif

#ifndef POUTRE_COORDINATE_HPP__
#include <poutreBase/poutreCoordinate.hpp>
#endif

#include <type_traits>

#ifdef _MSC_VER
#pragma warning( push )
#pragma warning( disable : 4425 )//4425 improper support constexpr
#endif


namespace poutre
{

#ifndef DOXYGEN_SHOULD_SKIP_THIS
  //forward declare
  template <class T, ptrdiff_t Rank> class array_view;
  template <class T, ptrdiff_t Rank> class strided_array_view;

  // Views alias templates
  template <class T, ptrdiff_t Rank>
  using carray_view = array_view<const T, Rank>;
  template <class T, ptrdiff_t Rank>
  using cstrided_array_view = strided_array_view<const T, Rank>;
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
    struct extract_value_type<X<T, Args...>> //specialization
    {
      typedef T value_type;
    };

    template <typename T, ptrdiff_t Rank>
    POUTRE_CXX14_CONSTEXPR T& view_access(T* data, const index<Rank>& idx, const index<Rank>& stride)
    {
        ptrdiff_t offset=0;
        for (size_t i = 0; i<Rank; ++i)
        {
            offset += idx[i] * stride[i];
        }
        return data[offset];
    }

    template <ptrdiff_t Rank>
    POUTRE_CXX14_CONSTEXPR ptrdiff_t view_offset(const index<Rank>& idx, const index<Rank>& stride)
    {
        ptrdiff_t offset{};
        for (size_t i = 0; i<Rank; ++i)
        {
            offset += idx[i] * stride[i];
        }
        return offset;
    }

   template <ptrdiff_t Rank>
   POUTRE_CXX14_CONSTEXPR void init_default_stride(const bounds<Rank>& bound, index<Rank>& stride)
   {
    stride[Rank - 1] = 1;
    for ( ptrdiff_t i = Rank - 2; i >= 0; i-- )
    {                                          
    stride[i] = stride[i + 1] * bound[i + 1];  
    }
   }

   template <ptrdiff_t rank>
   struct IsRankEqual1
   {
       POUTRE_STATIC_CONSTEXPR bool value = false;
   };

   template<>
   struct IsRankEqual1<1>
   {
       POUTRE_STATIC_CONSTEXPR bool value = true;
   };

   template <ptrdiff_t rank1, ptrdiff_t rank2>
   struct UseSameRank
   {
       POUTRE_STATIC_CONSTEXPR bool value = false;
   };

   template<ptrdiff_t rank>
   struct UseSameRank<rank, rank>
   {
       POUTRE_STATIC_CONSTEXPR bool value = true;
   };

 

   //stolen https://github.com/wardw/array_view/blob/master/array_view/array_view.h
   template <typename Viewable, typename U, typename View = std::remove_reference_t<Viewable>>
   using is_viewable_on_u = std::integral_constant<bool,
       std::is_convertible<typename View::size_type, ptrdiff_t>::value &&
       std::is_convertible<typename View::value_type*, std::add_pointer_t<U>>::value &&
       std::is_same<std::remove_cv_t<typename View::value_type>, std::remove_cv_t<U>>::value

   >;

   //stolen https://github.com/wardw/array_view/blob/master/array_view/array_view.h
   template <typename T, typename U>
   using is_viewable_value = std::integral_constant<bool,
       std::is_convertible<std::add_pointer_t<T>, std::add_pointer_t<U>>::value &&
       std::is_same<std::remove_cv_t<T>, std::remove_cv_t<U>>::value
   >;
} //namespace details





  /*!
   * @defgroup view_group Container View and Strided View
   * Implement View and StridedView as defined in http://www.open-std.org/jtc1/sc22/wg21/docs/papers/2014/n3851.pdf
   * @ingroup coordinate_group_group
   * @{
   */

  /**
   * @brief An array_view is a potentially multidimensional view on a sequence of uniformly strided objects of a uniform type, contiguous in the least significant dimension.
   */
  template <class T, ptrdiff_t Rank
  >
  class array_view
  {
    //convenient friendship
    template<class, ptrdiff_t > friend class strided_array_view;
  public:
    POUTRE_STATIC_CONSTEXPR ptrdiff_t rank = Rank;
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
    pointer m_data;

  public:
    /** @name Construction and Assignment
     */
    /**@{*/

    //!Default ctor

    POUTRE_CXX14_CONSTEXPR array_view ()
    POUTRE_NOEXCEPT_IF (POUTRE_NOEXCEPT_EXPR (bounds_type ()) && POUTRE_NOEXCEPT_EXPR (index_type ()))
            : m_bnd (),m_data (nullptr)
    {

    }

    //template <typename Viewable, size_t R = Rank,
    //    typename = std::enable_if_t<R == 1 &&
    //    is_viewable_on_u<Viewable, value_type>::value
    //    >
    //    >
    template<typename Viewable, size_t R = Rank>
        //!ctor from viewable object, mainly contiguous container C providing C.data(),C.size() interface and using the same value_type
		// @warning only available for rank==1
		  POUTRE_CXX14_CONSTEXPR
		  array_view (Viewable&& vw) : m_bnd (vw.size()),m_data (vw.data())
	  {
		  static_assert(Rank == 1, "array_view(Viewable&& vw) is only allowed for rank=1 view");
	  }

    template <typename U, ptrdiff_t R = Rank,
        typename = std::enable_if_t<R == 1 &&
        std::is_convertible<std::add_pointer_t<U>, pointer>::value && 
        std::is_same<std::remove_cv_t<U>, std::remove_cv_t<value_type>>::value
        is_viewable_value<U, value_type>::value
        >
    >
    //!ctor from other array_view with AnyRank is only allowed for flattened view (ie Rank=1)
    //@warning only available for AnyRank==1
    POUTRE_CONSTEXPR
    array_view (const array_view<U, R>& rhs) POUTRE_NOEXCEPT
    : m_bnd (rhs.m_bnd.size ()),m_data (rhs.m_data) { }

    //!ctor from C_Array 
    //@warning only available for rank==1
    template <size_t Extent,ptrdiff_t rank=Rank,
    class = typename std::enable_if<details::IsRankEqual1<rank>::value>::type>
    POUTRE_CXX14_CONSTEXPR
    array_view (value_type (&arr)[Extent]) POUTRE_NOEXCEPT
    : m_bnd (Extent), m_data (nullptr)
    {
       m_bnd[0] = Extent;
       m_data = &arr[0];
    }

    template <class U=T>
    //!copy ctor      
    POUTRE_CONSTEXPR array_view (const array_view<U, Rank>& rhs)
    POUTRE_NOEXCEPT_IF (POUTRE_NOEXCEPT_EXPR ((m_bnd (rhs.m_bnd))) && POUTRE_NOEXCEPT_EXPR ((m_stride_idx (rhs.m_stride_idx))))
            : m_bnd (rhs.m_bnd),m_data (rhs.m_data) 
    {
    }

  
    /*template <typename Viewable,
        typename = std::enable_if_t<is_viewable_on_u<Viewable, value_type>::value>
    >*/
    template <typename Viewable>
    //!ctor from an other view
    //@note This constructor may be used to create an array_view with a different rank and/or bounds than the original array_view, i.e. reshape the view.
    POUTRE_CXX14_CONSTEXPR
    array_view (Viewable&& vw, bounds_type bound) : m_bnd (bound),m_data (vw.data ())
    {
      if (vw.size () < bound.size ())
        {
          POUTRE_RUNTIME_ERROR ("array_view(Viewable&& vw, bounds_type bound) can't create view, provided bounds overfit vw");
        }      
    }

    //!ctor from nude ptr 
    //@warning Requires: [ptr, ptr + bounds.size()) is a valid range. 

    POUTRE_CXX14_CONSTEXPR
    array_view (pointer ptr, bounds_type bound) : m_bnd (bound), m_data (ptr)
    {
      if (!m_data && bound.size () != 0)
        {
          POUTRE_RUNTIME_ERROR ("array_view(pointer ptr, bounds_type bound) null ptr is allowed only if bound.size()==0");
        }
      //details::helper_assign_container_valueop<index_type, AssignOpType::AssignOp, Rank>::op (1, m_stride_idx);
    }

    /*
    template <class U,
    typename std::enable_if<
    std::is_convertible<typename std::add_pointer<U>::type, pointer>::value &&
    std::is_same<typename std::remove_cv<U>::type, typename std::remove_cv<value_type>::type>::value
    >::type* = nullptr>
    //!assignment operator
    POUTRE_CXX14_CONSTEXPR self_type& operator= (const array_view<U, Rank>& rhs) POUTRE_NOEXCEPT
    {
      m_bnd = rhs.m_bnd;
      m_stride_idx = rhs.m_stride_idx;
      m_data = rhs.m_data;
      return *this;
    }
    */
    
    /**@}*/

    /** @name Observers
     */
    /**@{*/
    //!Getter bounds of view

    POUTRE_CONSTEXPR bounds_type
    bound () const POUTRE_NOEXCEPT
    {
      return m_bnd;
    }

    //!Getter size of wiew

    POUTRE_CONSTEXPR size_type
    size () const POUTRE_NOEXCEPT
    {
      return m_bnd.size ();
    }


    //!Getter stride of view
    POUTRE_CXX14_CONSTEXPR index_type
    stride () const POUTRE_NOEXCEPT
    {
      index_type stride;
      details::init_default_stride(m_bnd,stride);
      return stride;
    }

    //!Getter raw access to underlying data ptr to the contiguous sequence on which the view was created. 

    POUTRE_CONSTEXPR pointer
    data () const POUTRE_NOEXCEPT
    {
      return m_data;
    }
    /**@}*/


    /** @name Access
     */
    /**@{*/
    //!Accessor to underlying data through indexing

    POUTRE_CXX14_CONSTEXPR reference operator[] (const index_type& idx) const POUTRE_NOEXCEPTONLYNDEBUG
    {
      POUTRE_ASSERTCHECK (m_bnd.contains (idx) == true, "Out of bound");
      //TODO FIXME add auto stride at ctor level
      return details::view_access(m_data, idx, stride());
    }
    /**@}*/

    /** @name Slicing and sectioning
     */
    /**@{*/
    //! Return a view such that the initial element is (*this)[{slice, 0, 0, �..., 0}], and the bounds are {bounds()[1], bounds()[2], �..., bounds()[Rank - 1]}. 

    POUTRE_CXX14_CONSTEXPR array_view<T, Rank - 1>
    operator[] (difference_type slice) const POUTRE_NOEXCEPTONLYNDEBUG
    {
      static_assert (Rank > 1, "Slicing is only allowed if Rank>1");
      POUTRE_ASSERTCHECK (0 <= slice, "Slicing slice must be  >=0");
      POUTRE_ASSERTCHECK (slice < m_bnd[0], "Slicing slice must be < bounds()[0]");

      //Slice bound
      poutre::bounds < Rank - 1 > bnd_slice;
      for (auto i = 0; i < Rank - 1; i++)
        {
          bnd_slice[i] = m_bnd[i + 1];
        }

      //Compute prt shift
      index_type idxslice;
      details::helper_assign_container_valueop<index_type, AssignOpType::AssignOp, Rank>::op (0, idxslice);
      idxslice[0] = slice;
      pointer data_slice = m_data + details::get_offset_from_coord_nostride<bounds_type, index_type>::op (m_bnd, idxslice);

      return array_view<T, Rank - 1 > (data_slice, bnd_slice);
    }

    POUTRE_CXX14_CONSTEXPR strided_array_view<T, Rank>
    //!A strided view such that the initial element is (*this)[origin], the stride is stride(), and the bounds are section_bnd. 
    //@warning Requires: bounds().contains(origin + idx) == true for any index_type idx such that (bounds() - origin).contains(idx) == true. 
    section (const index_type& origin, const bounds_type& section_bnd) const
    {
      //precondition
      POUTRE_CHECK (m_bnd.contains (origin), "section(origin,section_bnd) origin is Out of bound");
      index_type shifted = section_bnd + origin;
      shifted -= 1;
      POUTRE_CHECK (m_bnd.contains (shifted), "section(origin,section_bnd) section_bnd shifted by origins is out of bound");

      pointer data_section = m_data + details::get_offset_from_coord_nostride<bounds_type, index_type>::op (m_bnd, origin);
      return strided_array_view<T, Rank>(data_section, section_bnd, stride());
    }

    POUTRE_CXX14_CONSTEXPR strided_array_view<T, Rank>
    //! A strided view such that the initial element is (*this)[origin], the stride is stride(), and the bounds are (bounds() - origin). 
    //@warning Requires: bounds().contains(origin + idx) == true for any index_type idx such that (bounds() - origin).contains(idx) == true. 
    section (const index_type& origin) const
    {
      POUTRE_CHECK (m_bnd.contains (origin), "section(origin,section_bnd) origin is Out of bound");

      bounds_type section_bnd = m_bnd - origin;
      //std::cout<<"\nstrided_array_view::section init "<<m_bnd<<" sub "<<section_bnd<<" stride"<<stride()<<"\n";
      pointer data_section = m_data + details::get_offset_from_coord_nostride<bounds_type, index_type>::op (m_bnd, origin);
      return strided_array_view<T, Rank>(data_section, section_bnd, stride());
    }

    /**@}*/
  };

  /**
   * @brief An strided_array_view is a potentially multidimensional view on a sequence of  strided objects of a uniform type
   */
  template <class T, ptrdiff_t Rank
  >
  class strided_array_view
  {
    //convenient friendship
    template<class, ptrdiff_t> friend class array_view;
  public:
    POUTRE_STATIC_CONSTEXPR ptrdiff_t rank = Rank;
    using index_type = index<Rank>;
    using bounds_type = bounds<Rank>;

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

    POUTRE_CXX14_CONSTEXPR strided_array_view ()
    POUTRE_NOEXCEPT_IF (POUTRE_NOEXCEPT_EXPR (bounds_type ()) && POUTRE_NOEXCEPT_EXPR (index_type ()))
            : m_bnd (), m_stride_idx (), m_data (nullptr)
    {
        details::init_default_stride(m_bnd, m_stride_idx);
    }

    template <class U=T,
    typename std::enable_if<
    std::is_convertible<typename std::add_pointer<U>::type, pointer>::value &&
    std::is_same<typename std::remove_cv<U>::type, typename std::remove_cv<value_type>::type>::value
    >::type* = nullptr>
    //!Ctor from strided_array_view
    POUTRE_CONSTEXPR strided_array_view (const array_view<U, Rank>& rhs)
    POUTRE_NOEXCEPT_IF (POUTRE_NOEXCEPT_EXPR (m_bnd (rhs.m_bnd)) && POUTRE_NOEXCEPT_EXPR (m_stride_idx ()))
            : m_bnd (rhs.m_bnd), m_stride_idx (rhs.stride()), m_data (rhs.m_data) { }

    template <class U=T,
    typename std::enable_if<
    std::is_convertible<typename std::add_pointer<U>::type, pointer>::value &&
    std::is_same<typename std::remove_cv<U>::type, typename std::remove_cv<value_type>::type>::value
    >::type* = nullptr>
    //!Copy ctor
    POUTRE_CONSTEXPR strided_array_view (const strided_array_view<U, Rank>& rhs)
    POUTRE_NOEXCEPT_IF (POUTRE_NOEXCEPT_EXPR (m_sview_bnd (rhs.m_bnd)) && POUTRE_NOEXCEPT_EXPR (m_stride_idx (rhs.m_stride_idx)))
            : m_bnd (rhs.m_bnd), m_stride_idx (rhs.stride()), m_data (rhs.m_data) { }

    //!ctor from nude ptr 
    //@warning Requires: [ptr, ptr + bounds.size()) is a valid range. 
    POUTRE_CXX14_CONSTEXPR
    strided_array_view (pointer ptr, bounds_type bounds, index_type stride)
            : m_bnd (bounds), m_stride_idx (stride), m_data (ptr)
    {
      if (!m_data && bounds.size () != 0)
        {
          POUTRE_RUNTIME_ERROR ("array_view(pointer ptr, bounds_type bounds) null ptr is allowed only if bound.size()==0");
        }
//      if (!bounds.contains (stride))
//        {
//          POUTRE_RUNTIME_ERROR ("strided_array_view(pointer ptr, bounds_type bounds, index_type stride) provided stride is out of bounds");
//        }
    }

    template <class U=T,
    typename std::enable_if<
    std::is_convertible<typename std::add_pointer<U>::type, pointer>::value &&
    std::is_same<typename std::remove_cv<U>::type, typename std::remove_cv<value_type>::type>::value
    >::type* = nullptr>
    //!Assignement operator
    POUTRE_CXX14_CONSTEXPR self_type& operator= (const strided_array_view<U, Rank>& rhs) POUTRE_NOEXCEPT
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

    POUTRE_CONSTEXPR bounds_type
    bound () const POUTRE_NOEXCEPT
    {
      return m_bnd;
    }

    //!Accessor size eg underlying bound size

    POUTRE_CONSTEXPR size_type
    size () const POUTRE_NOEXCEPT
    {
      return m_bnd.size ();
    }

    //!Accessor stride

    POUTRE_CONSTEXPR index_type
    stride () const POUTRE_NOEXCEPT
    {
      return m_stride_idx;
    }
    /**@}*/

    /** @name Access
     */
    /**@{*/
    //!Accessor to underlying data through indexing

    POUTRE_CXX14_CONSTEXPR reference operator[] (const index_type& idx) const POUTRE_NOEXCEPTONLYNDEBUG
    {
      POUTRE_ASSERTCHECK (m_bnd.contains (idx) == true, "Out of bound");
      return details::view_access(m_data,idx,m_stride_idx);
    }
    /**@}*/

    /** @name Slicing and sectioning
     */
    /**@{*/
    //! Return A strided view such that the initial element is(*this)[{slice, 0, 0, �..., 0}], the bounds are{bounds( )[1], bounds( )[2], �...,bounds()[Rank - 1] }, and the stride is{ stride( )[1], stride( )[2], �...,stride()[Rank - 1] }
    //@warning Requires: 0<=slice<bound()[0]

    POUTRE_CXX14_CONSTEXPR strided_array_view<T, Rank - 1>
    operator[] (difference_type slice) const
    {
      static_assert (Rank > 1, "Slicing is only allowed if Rank>1");
	  POUTRE_ASSERTCHECK(0 <= slice, "Slicing slice must be  >=0");
	  POUTRE_ASSERTCHECK(slice < m_bnd[0], "Slicing slice must be < bounds()[0]");

      //slice bound and stride
      bounds < Rank - 1 > bnd_slice;
      index < Rank - 1 > stride_slice;
      for (auto i = 0; i < Rank - 1; i++)
        {
          bnd_slice[i] = m_bnd[i + 1];
          stride_slice[i] = m_stride_idx[i + 1];
        }

      //Compute prt shift
      index_type idxslice;
      details::helper_assign_container_valueop<index_type, AssignOpType::AssignOp, Rank>::op (0, idxslice);
      idxslice[0] = slice;
      
      ptrdiff_t shift= details::view_offset(idxslice,m_stride_idx);
      pointer data_slice = m_data +shift;      
      
      return strided_array_view<T, Rank - 1 > (data_slice, bnd_slice, stride_slice);
    }

    POUTRE_CXX14_CONSTEXPR strided_array_view<T, Rank>
    //! A strided view such that the initial element is (*this)[origin], the stride is stride(), and the bounds are section_bnd.
    //@warning Requires: bounds().contains(origin + idx) == true for any index_type idx such that (bounds() - origin).contains(idx) == true. 
    section (const index_type& origin, const bounds_type& section_bnd) const
    {
      //TODO  A REPRENDRE
      POUTRE_ASSERTCHECK (m_bnd.contains(origin), "section(origin,section_bnd) origin is Out of bound"); 
      
      index_type shifted = section_bnd + origin;
      shifted -= 1;
      //std::cout<<"\n strided_array_view::section with bound shifted:"<<shifted<<" bnd:"<<m_bnd;
      //POUTRE_CHECK (m_bnd.contains (shifted), "section(origin,section_bnd) section_bnd shifted by origins is out of bound");
      ptrdiff_t shift = details::view_offset(origin, m_stride_idx);
      pointer data_section = m_data +shift;      
      
      //std::cout<<"\n strided_array_view::section with bound data_section:"<<*data_section;
      return strided_array_view<T, Rank>(data_section, section_bnd, m_stride_idx);
    }

    POUTRE_CXX14_CONSTEXPR strided_array_view<T, Rank>
    //! A strided view such that the initial element is (*this)[origin], the stride is stride(), and the bounds are (bounds() - origin). 
    // @warning Requires: bounds().contains(origin + idx) == true for any index_type idx such that (bounds() - origin).contains(idx) == true. 
    section (const index_type& origin) const
    {      
      POUTRE_ASSERTCHECK(m_bnd.contains (origin), "section(origin,section_bnd) origin is Out of bound");
      bounds_type section_bnd = m_bnd - origin;
      //TODO take into account stride ?
      ptrdiff_t shift = details::view_offset(origin, m_stride_idx);
      pointer data_section = m_data +shift;
      //std::cout<<"\n"<<" bnd:"<<m_bnd<<" data:"<<*m_data<<" strided_array_view::section data_section:"<<*data_section<<" section_bnd:"<<section_bnd ;
      return strided_array_view<T, Rank>(data_section, section_bnd, m_stride_idx);
    }
    /**@}*/
  };
  //! @} doxygroup: coordinate_group   


  //Default to false 
  template<class ViewType>
  struct is_strided : std::false_type {};

  template<typename T, std::ptrdiff_t Rank >
  struct is_strided<strided_array_view<T, Rank>> : std::true_type {};

  template<class ViewType1,class ViewType2>
  struct is_same_view_type : std::false_type {};

  template<class ViewType>
  struct is_same_view_type<ViewType, ViewType> : std::true_type {};



} //namespace poutre

#ifdef _MSC_VER
#pragma warning( pop ) 
#endif

#endif //POUTRE_CONTAINER_VIEW_HPP__
