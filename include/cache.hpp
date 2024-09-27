#pragma once

#include <iterator>
#include <list>
#include <unordered_map>
#include <vector>

int slow_get_page_int (int key) { return key; }


template <typename T, typename KeyT = int, typename frec = int> class cache_t {
  public:
    size_t sz_;

    std::list<std::tuple<KeyT, T, frec>> small_cache;
    std::list<std::tuple<KeyT, T, frec>> main_cache;
    std::list<std::tuple<KeyT, T, frec>> ghost_cache;

    using ListIt = typename std::list<std::tuple<KeyT, T, frec>>::iterator;
    
    std::unordered_map<KeyT, ListIt> small_hash;
    std::unordered_map<KeyT, ListIt> main_hash;
    std::unordered_map<KeyT, ListIt> ghost_hash;

    cache_t(size_t sz) : sz_(sz) {}

    template <typename F> bool lookup_update(KeyT key, F slow_get_page) {
      auto ghost_hit = ghost_hash.find(key);
      auto main_hit = main_hash.find(key);
      auto small_hit = small_hash.find(key);

      if (small_hit == small_hash.end() || main_hit == main_hash.end() || ghost_hit == ghost_hash.end()){
        if (small_cache.size() == sz_){                                                                       
          if (std::get<2>(small_cache.back()) > 0){                                                                            
            if (main_cache.size() == sz_){
              main_hash.erase(std::get<0>(main_cache.back()));
              main_cache.pop_back();
            }                    

            main_cache.emplace_front(small_cache.back());                                                       
            main_hash.emplace(std::get<0>(*(main_cache.begin())), main_cache.begin());               
                                 
            small_cache.pop_back();
          }      
          else {
            if (ghost_cache.size() == sz_){
              ghost_hash.erase(std::get<0>(ghost_cache.back()));
              ghost_cache.pop_back();
            }

            ghost_cache.emplace_front(small_cache.back());
            ghost_hash.emplace(std::get<0>(*(ghost_cache.begin())), ghost_cache.begin());        
            small_cache.pop_back();
            small_hash.erase(std::get<0>(small_cache.back()));
          }  
        }
        else {
          small_cache.emplace_front(key, slow_get_page(key), 0);     
          small_hash.emplace(key, small_cache.begin());
        }
        return false;
      }

      if (small_hit != small_hash.end())
      {
        std::get<2>(*(small_hit->second)) += 1; 
        return true; 
      }
      if (main_hit != main_hash.end())
      { 
        std::get<2>(*(main_hit->second)) += 1; 
        return true; 
      }
  
      // main_hash.emplace(ghost_hash.erase(ghost_hit));                      //!!!!
      // main_cache.emplace_front(ghost_cache.erase((ghost_hit->second)));    //!!!!
      return true; 
    }
};




// template <typename T, typename KeyT = int> struct cache_t {
//   size_t sz_;
//   std::list<std::pair<KeyT, T>> cache_;

//   using ListIt = typename std::list<std::pair<KeyT, T>>::iterator;
//   std::unordered_map<KeyT, ListIt> hash_;


//   cache_t(size_t sz) : sz_(sz) {}

//   bool full() const { return (cache_.size() == sz_); }

//   template <typename F> bool lookup_update(KeyT key, F slow_get_page) {
//     auto hit = hash_.find(key);
//     if (hit == hash_.end()) {
//       if (full()) {
//         hash_.erase(cache_.back().first);
//         cache_.pop_back();
//       }
//       cache_.emplace_front(key, slow_get_page(key));
//       hash_.emplace(key, cache_.begin());
//       return false;
//     }

//     auto eltit = hit->second;
//     if (eltit != cache_.begin())
//       cache_.splice(cache_.begin(), cache_, eltit, std::next(eltit));
//     return true;
//   }
// };



// how to move elements from the midle of list and unorderd map(for movement from ghost)? --- the biggest question rn
// how te send size of cache from main to hpp?
// how to add frecquency