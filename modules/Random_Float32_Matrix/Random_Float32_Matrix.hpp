#pragma once
#ifndef RANDOM_FLOAT32_MATRIX_HPP
	#define RANDOM_FLOAT32_MATRIX_HPP
	#include "Quant.hpp"

	class Random_Float32_Matrix: public Quant {
		private:
			// Private properties
			cl::Kernel kernel;
		public:
			Random_Float32_Matrix();
	};
#endif