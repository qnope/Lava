#include "SdlContext.h"

#include <iostream>

SdlContext::SdlContext() {
  std::cout << "SdlContext" << std::endl;
  auto l = []<typename T>(T){};
}
