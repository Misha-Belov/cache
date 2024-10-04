#pragma once

#include <iterator>
#include <list>
#include <deque>
#include <unordered_map>
#include <vector>
#include <algorithm>
#include <iostream>

template <typename T, typename KeyT = int, typename frec = int> class cache_ideal {
  private:
    size_t sz_;

    std::list<std::tuple<KeyT, T, frec>> cache;

    using ListIt = typename std::list<std::tuple<KeyT, T, frec>>::iterator;
    
    std::unordered_map<KeyT, ListIt> hash;

  public:
    cache_ideal(size_t sz) : sz_(sz) {}

    template <typename Tp, typename F> int ideal_update(std::deque<Tp> request, F slow_get_page) {
      int hits = 0;
      for (int n : request) {

        KeyT key = request.back();

        if (hash.find(key) == hash.end()) {
          if (cache.size() == sz_) {
            bool is_exist = 1;
            size_t appear_num = 0;
            ListIt appear_iter;

            for (auto& hash_elem : hash) {
              size_t t = 0;
              auto f = std::find(request.begin(), request.end(), hash_elem.first);
              
              if (f == request.end() || appear_num < t) {
                appear_num = t;
                appear_iter = hash_elem.second;
              }
              if (f == request.end()) {
                break;
              }

              ++t;
            }    

            cache.erase(appear_iter);
            hash.erase(std::get<0>(*(appear_iter)));                                               
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