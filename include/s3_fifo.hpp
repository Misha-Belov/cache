#pragma once

#include <iterator>
#include <list>
#include <unordered_map>
#include <vector>
#include <cmath>



template <typename T1, typename T2, typename T3> 
std::ostream &operator<< (std::ostream &os, const std::list<std::tuple<T1, T2, T3>> &cache) {
  for (auto const &i: cache) {
    os << std::get<1>(i) << " ";
  }
  return os;
}

template <typename T, typename KeyT = int, typename frec = int> class cache_fifo {
  private:
    size_t sz_;

    std::list<std::tuple<KeyT, T, frec>> small_cache;
    std::list<std::tuple<KeyT, T, frec>> main_cache;
    std::list<std::tuple<KeyT, T, frec>> ghost_cache;

    using ListIt = typename std::list<std::tuple<KeyT, T, frec>>::iterator;
    
    std::unordered_map<KeyT, ListIt> small_hash;
    std::unordered_map<KeyT, ListIt> main_hash;
    std::unordered_map<KeyT, ListIt> ghost_hash;

  public:
    cache_fifo(size_t sz) : sz_(sz) {}

    size_t main_size = ceil(sz_ * 0.4); 
    size_t small_size = ceil(sz_ * 0.3); 
    size_t ghost_size = 2 + sz_ - main_size - small_size;


    void dump(KeyT key) { 
      std::cout << "key: " << key << " small: " << small_cache << " main: " << main_cache << " ghost: " << ghost_cache << "\n"; 
    }

    template <typename F> bool fifo_update(KeyT key, F slow_get_page) {
      auto ghost_hit = ghost_hash.find(key);
      auto main_hit = main_hash.find(key);
      auto small_hit = small_hash.find(key);

      if (small_hit == small_hash.end() && main_hit == main_hash.end() && ghost_hit == ghost_hash.end()){      //key not found
        if (small_cache.size() == small_size){                                                                       
          if (std::get<2>(small_cache.back()) > 0){                                                                            
            if (main_cache.size() == main_size){
              main_hash.erase(std::get<0>(main_cache.back()));
              main_cache.pop_back();
            }                    

            main_cache.emplace_front(small_cache.back());                                                       
            main_hash.emplace(std::get<0>(*(main_cache.begin())), main_cache.begin());               
            small_hash.erase(std::get<0>(small_cache.back()));                     
            small_cache.pop_back();
          }      
          else {
            if (ghost_cache.size() == ghost_size){
              ghost_hash.erase(std::get<0>(ghost_cache.back()));
              ghost_cache.pop_back();
            }

            ghost_cache.emplace_front(small_cache.back());
            ghost_hash.emplace(std::get<0>(*(ghost_cache.begin())), ghost_cache.begin()); 
            small_hash.erase(std::get<0>(small_cache.back()));       
            small_cache.pop_back();
          }  
        }
        
        small_cache.emplace_front(key, slow_get_page(key), 0);     
        small_hash.emplace(key, small_cache.begin());
        return false;
      }
      else {
        if (small_hit != small_hash.end()){
          std::get<2>(*(small_hit->second)) += 1; 
        }
        else if (main_hit != main_hash.end()) { 
          std::get<2>(*(main_hit->second)) += 1; 
        }
        else {
          if (main_cache.size() == main_size){
            main_hash.erase(std::get<0>(main_cache.back()));
            main_cache.pop_back();
          }
          main_cache.emplace_front(*(ghost_hit->second));   
          main_hash.emplace(std::get<0>(*(main_cache.begin())), main_cache.begin());                    
          ghost_cache.erase((ghost_hit->second));
          ghost_hash.erase(ghost_hit);
        }
        return true; 
      }
    }
};