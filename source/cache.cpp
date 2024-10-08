#include <cassert>
#include <iostream>
#include <deque>
#include <chrono>

#include "../include/s3_fifo.hpp"
#include "../include/ideal.hpp"

int slow_get_page_int (int key) { return key; }

int main() {
  size_t hits_fifo = 0;
  size_t hits_ideal = 0;
  size_t count_of_elements;
  size_t cache_size;
  std::deque<int> request;

  std::cin >> count_of_elements >> cache_size;
  assert(std::cin.good());

  cache_fifo<int> c_fifo{cache_size};
  cache_ideal<int> c_ideal{cache_size};

  auto start_all = std::chrono::high_resolution_clock::now();

  for (int i = 0; i < count_of_elements; ++i) {
    int q;
    std::cin >> q;
    assert(std::cin.good());

    hits_fifo += c_fifo.fifo_update(q, slow_get_page_int);
    request.push_back(q);

    #ifdef DEBUG
      c_fifo.dump(q);  
    #endif
  }

  auto start_idl = std::chrono::high_resolution_clock::now();

  // hits_ideal = c_ideal.ideal_update(request, slow_get_page_int);

  auto stop_idl = std::chrono::high_resolution_clock::now();
  auto duration_idl = std::chrono::duration_cast<std::chrono::milliseconds>(stop_idl - start_idl);

  auto stop_all = std::chrono::high_resolution_clock::now();
  auto duration_all = std::chrono::duration_cast<std::chrono::milliseconds>(stop_all - start_all);

  std::cout << "fifo: " << hits_fifo << " ideal: " << hits_ideal << "\n";
  std::cout << "Time taken by function: " << duration_all.count() << " miliseconds; \n" 
                          << "By ideal: " << duration_idl.count() << " miliseconds " << std::endl;
}