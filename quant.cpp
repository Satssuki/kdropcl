// Include all of our modules
#include "modules/Quant.hpp"
#include "modules/Random_Float32_Matrix/Random_Float32_Matrix.hpp"

int main() {
  Quant::initialize();
  Random_Float32_Matrix *rndf32m = new Random_Float32_Matrix();
  return 0;
}