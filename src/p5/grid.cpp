#include "grid.h"
#include <algorithm>

#include <ctime>
int random(int max) {
  static bool call_once = true;
  if (call_once) {
    std::srand(std::time(nullptr));
    call_once = false;
  }
  return std::rand() / ((RAND_MAX + 1u) / max);
}
