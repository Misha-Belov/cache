#include <cassert>
#include <iostream>
#include <deque>
#include <chrono>

#include "../include/ideal.hpp"

int slow_get_page_int (int key) { return key; }

int main() {
  size_t hits_ideal        = 0;
  size_t count_of_elements = 0;
  size_t cache_size        = 0;

  std::deque<int> request;

  std::cin >> cache_size >> count_of_elements;
  assert(std::cin.good());

  cache_ideal<int> c_ideal{cache_size};

  auto start_all = std::chrono::high_resolution_clock::now();

  for (size_t i = 0; i < count_of_elements; ++i) {
    int q;
    std::cin >> q;
    assert(std::cin.good());

    request.push_back(q);

    #ifdef DEBUG
      c_fifo.dump(q);  
    #endif
  }

  hits_ideal = c_ideal.ideal_update(request, slow_get_page_int);

  auto stop_all = std::chrono::high_resolution_clock::now();
  auto duration_all = std::chrono::duration_cast<std::chrono::milliseconds>(stop_all - start_all);

  std::cout << "ideal hits: " << hits_ideal << "\n";
  std::cout << "Time taken by function: " << duration_all.count() << " miliseconds; \n" << std::endl;
}