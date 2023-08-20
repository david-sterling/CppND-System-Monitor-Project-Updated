#include "format.h"

#include <fmt/format.h>

#include <cmath>
#include <string>

using namespace std;

// TODO: Complete this helper function
// INPUT: Long int measuring seconds
// OUTPUT: HH:MM:SS
// REMOVE: [[maybe_unused]] once you define the function
string Format::ElapsedTime(long seconds) {
  long H, M, S{0};
  H = std::floor(seconds / 3600);
  M = std::floor((seconds - ((H * 3600))) / 60);
  S = seconds - (H * 3600) - (M * 60);
  return fmt::format("{:02d}:{:02d}:{:02d}", H, M, S);
}