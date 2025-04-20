// test.cpp
#include <fmt/core.h>
#include <sqlite3.h>

#include <cstdio>

// main function!
int main() {
  fmt::print("test!\n");
  fmt::print("{}\n", sqlite3_libversion());
  return 0;
}
