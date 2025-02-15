#include "Core/Core.hpp"

int main() {
  try {
    Core app;
    app.run();
  } catch (const std::exception& e) {
    return -1;
  }
  return 0;
}
