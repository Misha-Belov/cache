#include <cassert>
#include <iostream>
#include <deque>
#include <chrono>

#include "../include/s3_fifo.hpp"

int slow_get_page_int (int key) { return key; }

int main() {
  size_t hits_fifo         = 0;
  size_t count_of_elements = 0;
  size_t cache_size        = 0;

  std::cin >> cache_size >> count_of_elements;
  assert(std::cin.good());

  cache_fifo <int> c_fifo {cache_size};

  auto start_all = std::chrono::high_resolution_clock::now();

  for (size_t i = 0; i < count_of_elements; ++i) {
    int q;
    std::cin >> q;
    assert(std::cin.good());



    hits_fifo += c_fifo.fifo_update(q, slow_get_page_int);

    #ifdef DEBUG
      c_fifo.dump(q);  
    #endif
  }

  auto stop_all = std::chrono::high_resolution_clock::now();
  auto duration_all = std::chrono::duration_cast<std::chrono::milliseconds>(stop_all - start_all);

  std::cout << "fifo hits: " << hits_fifo << "\n";
  std::cout << "Time taken by function: " << duration_all.count() << " miliseconds; " << std::endl;
}