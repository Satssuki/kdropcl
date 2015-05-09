#pragma OPENCL EXTENSION cl_khr_byte_addressable_store : enable
float rnd(int seed) {
	int den = seed;
	seed = (seed << (seed % 32)) ^ seed;
	return ((float)seed) / ((float)den);
}
