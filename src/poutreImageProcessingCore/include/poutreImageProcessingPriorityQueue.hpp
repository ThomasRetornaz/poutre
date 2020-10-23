
//==============================================================================
//                  Copyright (c) 2015 - Thomas Retornaz                      //
//                     thomas.retornaz@mines-paris.org                        //
//          Distributed under the Boost Software License, Version 1.0.        //
//                 See accompanying file LICENSE.txt or copy at               //
//                     http://www.boost.org/LICENSE_1_0.txt                   //
//==============================================================================

#ifndef POUTRE_PRIORITYQUEUE_HPP__
#define POUTRE_PRIORITYQUEUE_HPP__

/**
 * @file poutreImageProcessingPriorityQueue.hpp
 * @author thomas.retornaz@mines-paris.org
 * @brief Define common pqueue data structures
 * @version 0.1
 * @date 2020-05-08
 *
 * @copyright Copyright (c) 2020
 *
 */

#include <poutreBase/poutreConfig.hpp>
#include <poutreImageProcessingCore/poutreImageProcessingType.hpp>

#include <boost/heap/priority_queue.hpp>
#include <queue>
#include <vector>

namespace poutre
{
  /**
   * @addtogroup image_processing_pqueue_group Priority queue facilities
   * @ingroup image_processing_group
   *@{
   */
  template<typename key, typename value> struct lesserKey
  {
    bool operator()(const std::pair<key, value> &lhs, const std::pair<key, value> &rhs) const
    {
      return std::less<key>()(lhs.first, rhs.first);
    }
  };

  template<typename key, typename value> struct greaterKey
  {
    bool operator()(const std::pair<key, value> &lhs, const std::pair<key, value> &rhs) const
    {
      return std::greater<key>()(lhs.first, rhs.first);
    }
  };

  template<class key, class value, class order = lesserKey<key, value>, bool IsStable = false, typename = void>
  class PriorityQueue
      : public boost::heap::priority_queue<std::pair<key, value>, boost::heap::compare<order>, boost::heap::stable<IsStable>>
  {
    public:
    using base =
        boost::heap::priority_queue<std::pair<key, value>, boost::heap::compare<order>, boost::heap::stable<IsStable>>;

    PriorityQueue() : base() {}

    // see this later
    PriorityQueue(const PriorityQueue &rhs) = delete;
    PriorityQueue &operator=(const PriorityQueue &rhs) = delete;
    // see this later
    PriorityQueue(PriorityQueue &&other) = delete;
    PriorityQueue &operator=(PriorityQueue &&other) = delete;
    ~PriorityQueue() {}
  };

  // specialize for small integral types
  template<class key, class value, class order>
  class PriorityQueue<key, value, order, true, std::enable_if_t<(TypeTraits<key>::quant <= 16), key>>
  {
    public:
    using queueValue = std::queue<value>; // FIFO queue for one level
    using Key_Type   = key;
    using value_type = std::pair<Key_Type, value>;

    PriorityQueue()
        : m_maxPriority(static_cast<int>(TypeTraits<Key_Type>::max()))
        , p_stack(m_maxPriority + 1, 0)
        , stack(m_maxPriority + 1)
        , m_current_highestPriority(0)
    {
    }
    // see this later
    PriorityQueue(const PriorityQueue &rhs) = delete;
    PriorityQueue &operator=(const PriorityQueue &rhs) = delete;
    // see this later
    PriorityQueue(PriorityQueue &&other) = delete;
    PriorityQueue &operator=(PriorityQueue &&other) = delete;
    ~PriorityQueue() {}

    //! true if the priority queue has no elements
    bool empty() const { return (p_stack[(size_t)getHigherPriority()] == 0); }

    //! adds an element to the priority queue
    void push(const value_type &rhs)
    {
      Key_Type priority = rhs.first;

      if( m_current_highestPriority < priority )
      {
        m_current_highestPriority = priority; // update current_highestPriority
      }
      p_stack[priority] += 1;           // record number of element at this level
      stack[priority].push(rhs.second); // push element
    }

    //! returns the top element of the priority queue.The top element is greater or at least as
    //! great as any other element in the priority queue.
    value_type top() const
    {
      // ask current current_highestPriority
      getHigherPriority();
#ifdef _DEBUG
      if( p_stack[m_current_highestPriority] == 0 ) // not element found ?
      {
        throw std::runtime_error("Don't call top on empty priority stack");
      }
#endif
      return value_type(static_cast<Key_Type>(m_current_highestPriority),
                        stack[static_cast<size_t>(m_current_highestPriority)].front());
    }

    //! removes the top element of a priority queue.The top element is greater or at least as great
    //! as any other element in the priority queue.
    void pop()
    {
      getHigherPriority();
#ifdef _DEBUG
      // ask current current_highestPriority
      if( p_stack[m_current_highestPriority] == 0 ) // not element found ?
      {
        throw std::runtime_error("Don't call pop on empty priority stack"); //
      }
#endif
      p_stack[static_cast<size_t>(m_current_highestPriority)] -= 1; // record number of element at this level
      stack[static_cast<size_t>(m_current_highestPriority)].pop();
    }

    private:
    inline int getHigherPriority() const
    {
      if( p_stack[static_cast<size_t>(m_current_highestPriority)] == 0 )
      {
        int i;
        for( i = m_current_highestPriority; p_stack[(size_t)i] == 0 && i > 0; i-- ) //-V781
          ;
        m_current_highestPriority = i;
      }
      return m_current_highestPriority;
    }
    mutable int             m_current_highestPriority;
    int                     m_maxPriority;
    std::vector<int>        p_stack;
    std::vector<queueValue> stack;
  };
  //! @} doxygroup: image_processing_pqueue_group
} // namespace poutre
#endif // POUTRE_PRIORITYQUEUE_HPP__
