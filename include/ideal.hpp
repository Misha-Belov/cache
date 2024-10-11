#pragma once

#include <iterator>
#include <list>
#include <deque>
#include <unordered_map>
#include <vector>
#include <algorithm>
#include <iostream>

template <typename T1, typename T2, typename T3> 
std::ostream &operator<< (std::ostream &os, const std::list<std::tuple<T1, T2, T3>> &cache) {
  for (auto const &i: cache) {
    os << std::get<1>(i) << " -- " << std::get<2>(i) << "; ";
  }
  return os;
}

template <typename T, typename KeyT = int, typename dist = int> class cache_ideal {
  private:
    size_t sz_;

    std::list<std::tuple<KeyT, T, dist>> cache;

    using ListIt = typename std::list<std::tuple<KeyT, T, dist>>::iterator;
    using HashIt = typename std::unordered_map<KeyT, ListIt>::iterator;
    
    std::unordered_map<KeyT, ListIt> hash;

    

  public:
    cache_ideal(size_t sz) : sz_(sz) {}

    template <typename Req, typename F> int ideal_update(Req request, F slow_get_page) {
      int hits = 0;

      while (request.size() != 0) {
        KeyT key = request.front();
        request.pop_front();

        // std::cout << c << " ";
        // c++;
        // std::cout << "key: " << key << " cache: " << cache << "\n";

        int tmp = 0;
        ListIt far_iter;

        for (auto& hash_elem : hash) {                      //find the next element to delete
          auto cache_elem = hash_elem.second;

          int dst = std::get<2>(*cache_elem);
          std::get<2>(*cache_elem) -= 1;

          if (dst >= tmp) {
            tmp = dst;
            far_iter = cache_elem;
          }
        }

        auto f = std::find(request.begin(), request.end(), key);
        int elem_dist = std::distance(request.begin(), f);

        if (hash.find(key) == hash.end()) {   //key is not in cache
          if (cache.size() == sz_) {          //cache is full

            cache.erase(far_iter);
            hash.erase(std::get<0>(*far_iter));
          }

          cache.emplace_front(key, slow_get_page(key), elem_dist);     
          hash.emplace(key, cache.begin());
        }
        else
        {
          ++hits;

          std::get<2>(*((hash.find(key))->second)) = elem_dist;
        }
      }
      return hits;
    }
};