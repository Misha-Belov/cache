#pragma once

#include <iterator>
#include <list>
#include <deque>
#include <unordered_map>
#include <vector>
#include <algorithm>

template <typename T, typename KeyT = int, typename frec = int> class cache_ideal {
  private:
    size_t sz_, num;

    std::list<std::tuple<KeyT, T, frec>> cache;

    using ListIt = typename std::list<std::tuple<KeyT, T, frec>>::iterator;
    
    std::unordered_map<KeyT, ListIt> hash;
    std::pair<int, ListIt> appear;

  public:
    cache_ideal(size_t sz, size_t count_of_elements) { sz_ = sz; num = count_of_elements; }

    template <typename Tp, typename F> int ideal_update(std::deque<Tp> request, F slow_get_page) {
      int hits = 0;
      for (int n : request) {
        KeyT key = request.back();
        if (hash.find(key) == hash.end()) {
          if (cache.size() == sz_) {
            bool is_exist = 1;
            for (int t = 0; t < sz_, is_exist == 1; ++t) {
              auto f = std::find(request.begin(), request.end(), std::get<0>(*(hash[t])));
              if (f != request.end()) {
                if (appear.first < t) {
                  appear.first = t;
                  appear.second = hash[t];
                }
              }
              else {
                is_exist = 0;
                appear.first = t;
                appear.second = hash[t];
              }
              
            }    
            // delete hash[t], cache[t]                                                
          }
          else {
            request.pop_back();
          }

          cache.emplace_front(key, slow_get_page(key), 0);     
          hash.emplace(key, cache.begin());
        }
        else
        {
          request.pop_back();
          ++hits;
        }
      }
      return hits;
    }
};