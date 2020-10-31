
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

#include <queue>
#include <vector>

namespace poutre
{
  /**
   * @addtogroup image_processing_pqueue_group Priority queue facilities
   * @ingroup image_processing_group
   *@{
   */

  template<class T> struct stable_element
  {
    stable_element(T &&o, std::size_t c) : object_(std::move(o)), insertion_order_(c) {}
    stable_element(const T &o, std::size_t c) : object_(o), insertion_order_(c) {}
    operator T() { return object_; }

    T           object_;
    std::size_t insertion_order_;
  };

  template<class T> bool operator<(const stable_element<T> &lhs, const stable_element<T> &rhs)
  {
    return (lhs.object_ < rhs.object_) || (!(rhs.object_ < lhs.object_) && (rhs.insertion_order_ < lhs.insertion_order_));
  }

  template<class T,
           class Container = std::vector<stable_element<T>>,
           class Compare   = std::less<typename Container::value_type>>
  class stable_priority_queue : public std::priority_queue<stable_element<T>, Container, Compare>
  {
    using stableT = stable_element<T>;
    using std::priority_queue<stableT, Container, Compare>::priority_queue;

    public:
    const T &top() { return this->c.front().object_; }
    void     push(const T &value)
    {
      this->c.push_back(stableT(value, counter_++));
      std::push_heap(this->c.begin(), this->c.end(), this->comp);
    }
    void push(T &&value)
    {
      this->c.push_back(stableT(std::move(value), counter_++));
      std::push_heap(this->c.begin(), this->c.end(), this->comp);
    }
    template<class... Args> void emplace(Args &&... args)
    {
      this->c.emplace_back(T(std::forward<Args>(args)...), counter_++);
      std::push_heap(this->c.begin(), this->c.end(), this->comp);
    }
    void pop()
    {
      std::pop_heap(this->c.begin(), this->c.end(), this->comp);
      this->c.pop_back();
      if( this->empty() )
        counter_ = 0;
    }

    protected:
    std::size_t counter_ = 0;
  };

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

  template<class key,
           class value,
           class Container = std::vector<std::pair<key, value>>,
           class order     = lesserKey<key, value>,
           typename        = void>
  class PriorityQueue : public std::priority_queue<std::pair<key, value>, Container, order>
  {
    public:
    using base = std::priority_queue<std::pair<key, value>, Container, order>;

    PriorityQueue(size_t SizeReserve = 0) : base() { (this->c).reserve(SizeReserve); }

    // see this later
    PriorityQueue(const PriorityQueue &rhs) = delete;
    PriorityQueue &operator=(const PriorityQueue &rhs) = delete;
    // see this later
    PriorityQueue(PriorityQueue &&other) = delete;
    PriorityQueue &operator=(PriorityQueue &&other) = delete;
    ~PriorityQueue() {}
  };

  template<typename key, typename value>
  bool operator<(const stable_element<std::pair<key, value>> &lhs, const stable_element<std::pair<key, value>> &rhs)
  {
    return (lhs.object_.first < rhs.object_.first)
           || (!(rhs.object_.first < lhs.object_.first) && (rhs.insertion_order_ < lhs.insertion_order_));
  }

  template<typename key, typename value>
  bool operator>(const stable_element<std::pair<key, value>> &lhs, const stable_element<std::pair<key, value>> &rhs)
  {
    return (lhs.object_.first > rhs.object_.first)
           || (!(rhs.object_.first > lhs.object_.first) && (rhs.insertion_order_ < lhs.insertion_order_));
  }

  template<class key,
           class value,
           class container = std::vector<stable_element<std::pair<key, value>>>,
           class order     = std::less<typename container::value_type>,
           typename        = void>
  class PriorityQueueStable : public stable_priority_queue<std::pair<key, value>, container, order>
  {
    public:
    using base = stable_priority_queue<std::pair<key, value>, container, order>;

    PriorityQueueStable(size_t SizeReserve = 0) : base() { (this->c).reserve(SizeReserve); }

    // see this later
    PriorityQueueStable(const PriorityQueueStable &rhs) = delete;
    PriorityQueueStable &operator=(const PriorityQueueStable &rhs) = delete;
    // see this later
    PriorityQueueStable(PriorityQueueStable &&other) = delete;
    PriorityQueueStable &operator=(PriorityQueueStable &&other) = delete;
    ~PriorityQueueStable() {}
  };

  #  if(0) //TODO FIXME
  // specialize for small integral types
  template<class key,
           class value,
           class container /*= std::vector<stable_element<std::pair<key, value>>>*/,
           class order     /*= std::less<typename container::value_type>*/>
  class PriorityQueueStable<key, value, container, order, std::enable_if_t<(TypeTraits<key>::quant <= 16)>>
  {
    public:
    using queueValue = std::queue<value>; // FIFO queue for one level
    using Key_Type   = key;
    using value_type = std::pair<Key_Type, value>;

    PriorityQueueStable()
        : m_maxPriority(static_cast<int>(TypeTraits<Key_Type>::max()))
        , p_stack(m_maxPriority + 1, 0)
        , stack(m_maxPriority + 1)
        , m_current_highestPriority(0)
    {
    }
    // see this later
    PriorityQueueStable(const PriorityQueueStable &rhs) = delete;
    PriorityQueueStable &operator=(const PriorityQueueStable &rhs) = delete;
    // see this later
    PriorityQueueStable(PriorityQueueStable &&other) = delete;
    PriorityQueueStable &operator=(PriorityQueueStable &&other) = delete;
    ~PriorityQueueStable() {}

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
#  ifdef _DEBUG
      if( p_stack[m_current_highestPriority] == 0 ) // not element found ?
      {
        throw std::runtime_error("Don't call top on empty priority stack");
      }
#  endif
      return value_type(static_cast<Key_Type>(m_current_highestPriority),
                        stack[static_cast<size_t>(m_current_highestPriority)].front());
    }

    //! removes the top element of a priority queue.The top element is greater or at least as great
    //! as any other element in the priority queue.
    void pop()
    {
      getHigherPriority();
#  ifdef _DEBUG
      // ask current current_highestPriority
      if( p_stack[m_current_highestPriority] == 0 ) // not element found ?
      {
        throw std::runtime_error("Don't call pop on empty priority stack"); //
      }
#  endif
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
  #endif

  template<typename key, typename value> using poutre_pq = PriorityQueue<key, value>;

  template<typename key, typename value>
  using poutre_rpq = PriorityQueue<key, value, std::vector<std::pair<key, value>>, greaterKey<key, value>>;

  template<typename key, typename value> using poutre_pq_stable = PriorityQueueStable<key, value>;

  template<typename key, typename value>
  using poutre_rpq_stable = PriorityQueueStable<key,
                                                value,
                                                std::vector<stable_element<std::pair<key, value>>>,
                                                std::greater<stable_element<std::pair<key, value>>>>;

  //! @} doxygroup: image_processing_pqueue_group
} // namespace poutre
#endif // POUTRE_PRIORITYQUEUE_HPP__
