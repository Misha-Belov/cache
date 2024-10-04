#pragma once

#include <iterator>
#include <list>
#include <deque>
#include <unordered_map>
#include <vector>
#include <algorithm>
#include <iostream>

template <typename T1, typename T2> 
std::ostream &operator<< (std::ostream &os, const std::list<std::pair<T1, T2>> &cache) {
  for (auto const &i: cache) {
    os << i.second << " ";
  }
  return os;
}

template <typename T, typename KeyT = int, typename frec = int> class cache_ideal {
  private:
    size_t sz_;

    std::list<std::pair<KeyT, T>> cache;

    using ListIt = typename std::list<std::pair<KeyT, T>>::iterator;
    
    std::unordered_map<KeyT, ListIt> hash;

  public:
    cache_ideal(size_t sz) : sz_(sz) {}

    template <typename Tp, typename F> int ideal_update(std::deque<Tp> request, F slow_get_page) {
      int hits = 0;

      for (int n : request) {
        KeyT key = request.front();

        // std::cout << "key: " << key << " cache: " << cache << "\n";

        if (hash.find(key) == hash.end()) {   //key is not in cache
          if (cache.size() == sz_) {
            size_t appear_num = 0;
            ListIt appear_iter = cache.begin();

            for (auto& hash_elem : hash) {
              auto f = std::find(request.begin(), request.end(), hash_elem.first);
              int dist = std::distance(request.begin(), f);

              if (f == request.end() || appear_num < dist) {
                appear_num = dist;
                appear_iter = hash_elem.second;
              }
              if (f == request.end()) {
                break;
              }
            }    

            cache.erase(appear_iter);
            hash.erase(appear_iter->first);  
            request.pop_front();                                             
          }
          else {
            request.pop_front();
          }
          cache.emplace_front(key, slow_get_page(key));     
          hash.emplace(key, cache.begin());
        }
        else
        {
          request.pop_front();
          ++hits;
        }
      }
      return hits;
    }
};