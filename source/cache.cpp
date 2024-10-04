#include <cassert>
#include <iostream>
#include <deque>

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

  for (int i = 0; i < count_of_elements; ++i) {
    int q;
    std::cin >> q;
    assert(std::cin.good());

    hits_fifo += c_fifo.fifo_update(q, slow_get_page_int);
    request.push_front(q);

    #ifdef DEBUG
      c_fifo.dump(q);  
    #endif
  }

  hits_ideal = c_ideal.ideal_update(request, slow_get_page_int);

  std::cout << "fifo: " << hits_fifo << " ideal: " << hits_ideal << std::endl;
}