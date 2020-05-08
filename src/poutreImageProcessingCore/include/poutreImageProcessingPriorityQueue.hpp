
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

#include <boost/heap/priority_queue.hpp>
#include <queue>
#include <vector>

#ifndef POUTRE_CONFIG__HPP__
#include <poutreBase/poutreConfig.hpp>
#endif

#ifndef POUTRE_IMAGEPROCESSING_TYPE_HPP__
#include <poutreImageProcessingCore/poutreImageProcessingType.hpp>
#endif

namespace poutre
{
    /**
     * @addtogroup image_processing_pqueue_group Priority queue facilities
     * @ingroup image_processing_group
     *@{
     */
    template <typename key, typename value> struct lesserKey
    {
        bool operator()(const std::pair<key, value> &lhs, const std::pair<key, value> &rhs) const
        {
            return std::less<key>()(lhs.first, rhs.first);
        }
    };

    template <typename key, typename value> struct greaterKey
    {
        bool operator()(const std::pair<key, value> &lhs, const std::pair<key, value> &rhs) const
        {
            return std::greater<key>()(lhs.first, rhs.first);
        }
    };

    template <class key, class value, class order = lesserKey<key, value>, bool IsStable = false,
              typename Enable = void>
    class PriorityQueue : public boost::heap::priority_queue<std::pair<key, value>, boost::heap::compare<order>,
                                                             boost::heap::stable<IsStable>>
    {
      public:
        using base = boost::heap::priority_queue<std::pair<key, value>, boost::heap::compare<order>,
                                                 boost::heap::stable<IsStable>>;

        PriorityQueue() : base()
        {
        }
    };

    // specialize for small integral types
    // TODO
    // template <class key, class value, class order = lesserKey<key, value>,bool IsStable = false, typename
    // std::enable_if< (TypeTraits<key>::quant <= 16) >::type> class PriorityQueue
    //{
    // public:
    //    PriorityQueue(){}
    //};
    //! @} doxygroup: image_processing_pqueue_group
} // namespace poutre
#endif // POUTRE_PRIORITYQUEUE_HPP__
