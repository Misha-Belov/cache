#pragma once

#include <iterator>
#include <list>
#include <unordered_map>
#include <vector>
#include <cmath>



// template <typename T1, typename T2, typename T3> 
// std::ostream &operator<< (std::ostream &os, const std::list<std::tuple<T1, T2, T3>> &cache) {
//   for (auto const &i: cache) {
//     os << std::get<1>(i) << " ";
//   }
//   return os;
// }

template <typename T, typename KeyT = int, typename frec = int> class cache_fifo {
  private:
    size_t sz_;

    std::list<std::tuple<KeyT, T, frec>> small_cache;
    std::list<std::tuple<KeyT, T, frec>> main_cache;
    std::list<std::tuple<KeyT, T, frec>> ghost_cache;

    using ListIt = typename std::list<std::tuple<KeyT, T, frec>>::iterator;
    using List   = typename std::list<std::tuple<KeyT, T, frec>>;
    using Hash   = typename std::unordered_map<KeyT, ListIt>;
    
    std::unordered_map<KeyT, ListIt> small_hash;
    std::unordered_map<KeyT, ListIt> main_hash;
    std::unordered_map<KeyT, ListIt> ghost_hash;

    void move (Hash& hash_src, Hash& hash_dst, List& cache_src, List& cache_dst) {
      cache_dst.emplace_front(cache_src.back());                                                       
      hash_dst.emplace(std::get<0>(*(cache_dst.begin())), cache_dst.begin());     

      hash_src.erase(std::get<0>(cache_src.back()));                     
      cache_src.pop_back();
    }

    void add_element (List& cache, Hash& hash, KeyT key, T elem) {
      cache.emplace_front(key, elem, 0);     
      hash.emplace(key, cache.begin());
    }

    void erase_back (List& cache, Hash& hash) {
      hash.erase(std::get<0>(cache.back()));
      cache.pop_back();
    }
    

  public:
    cache_fifo(size_t sz) : sz_(sz) {}

    size_t main_size  = ceil(sz_ * 0.4); 
    size_t small_size = ceil(sz_ * 0.3); 
    size_t ghost_size = 2 + sz_ - main_size - small_size;

    void dump(KeyT key) { 
      std::cout << " key: "   << key 
                << " small: " << small_cache 
                << " main: "  << main_cache 
                << " ghost: " << ghost_cache << "\n"; 
    }

    template <typename F> bool fifo_update(KeyT key, F slow_get_page) {
      auto ghost_hit = ghost_hash.find(key);
      auto main_hit  = main_hash.find(key);
      auto small_hit = small_hash.find(key);

      
      int small_frec = std::get<2>(small_cache.back());
      int main_frec  = std::get<2>(main_cache.back());

      if (small_hit == small_hash.end() && main_hit == main_hash.end() && ghost_hit == ghost_hash.end()){      //key not found
        if (small_cache.size() >= small_size){                                                                       
          if (small_frec > 0){                                                                            
            if (main_cache.size() >= main_size){
              erase_back(main_cache, main_hash);
            }                    

            move(small_hash, main_hash, small_cache, main_cache);           // move element from small to main
          }      
          else {
            if (ghost_cache.size() >= ghost_size){
              erase_back(ghost_cache, ghost_hash);
            }
            
            move(small_hash, ghost_hash, small_cache, ghost_cache);         // move element from small to ghost
          }  
        }
        
        add_element(small_cache, small_hash, key, slow_get_page(key));
        return false;
      }
      else {

        if (small_hit != small_hash.end()) {
          small_frec += 1; 
        }
        else if (main_hit != main_hash.end()) { 
          main_frec  += 1; 
        }
        else {
          if (main_cache.size() == main_size){
            erase_back(main_cache, main_hash);
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