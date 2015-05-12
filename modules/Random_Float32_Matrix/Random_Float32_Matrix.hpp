#pragma once
#ifndef RANDOM_FLOAT32_MATRIX_HPP
	#define RANDOM_FLOAT32_MATRIX_HPP
	#include "Quant.hpp"

	class Random_Float32_Matrix: public Quant {
		private:
			// Private properties
			cl::Kernel kernel;
			std::vector <cl::CommandQueue> queues;
			// Private methods
			void buildKernel();
			void buildCommandQueues();
		public:
			Random_Float32_Matrix();
	};
#endif