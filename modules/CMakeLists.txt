add_library(Quant STATIC
	Quant.cpp
	Random_Float32_Matrix/Random_Float32_Matrix.cpp
)

# Include all our header files
include_directories("${CMAKE_CURRENT_SOURCE_DIR}/Random_Float32_Matrix")