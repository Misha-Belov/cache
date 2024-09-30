#include <cassert>
#include <iostream>

#include "cache.hpp"



int main() {
  int hits = 0;
  int count_of_elements;
  size_t cache_size;

  std::cin >> count_of_elements >> cache_size;
  assert(std::cin.good());

  cache_t<int> c{cache_size};

  for (int i = 0; i < count_of_elements; ++i) {
    int q;
    std::cin >> q;
    assert(std::cin.good());

    hits += c.lookup_update(q, slow_get_page_int);
  }

  std::cout << hits << std::endl;
}