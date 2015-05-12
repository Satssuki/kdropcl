#pragma once
#ifndef RANDOM_FLOAT32_MATRIX_CPP
  #define RANDOM_FLOAT32_MATRIX_CPP
  #define KERNEL_HELPERS "../kernel_helpers"
  #define KERNELS "../kernels"

  #include <fstream>

  #include "Random_Float32_Matrix.hpp"

  void printProgram(std::string prog_str) {
    std::cout << "-----------------------------" << std::endl;
    std::cout << "-------Program String--------" << std::endl;
    std::cout << "-----------------------------" << std::endl;
    std::cout << prog_str << std::endl;
    std::cout << "-----------------------------" << std::endl;
  }

  void Random_Float32_Matrix::buildKernel () {
    cl_int err;

    // Header for opencl compiler
    std::string cl_header = "#pragma OPENCL EXTENSION cl_khr_byte_addressable_store : enable\n";
    
    std::cout << "Building program" << std::endl;

    // Get our kernel helpers
    std::ifstream random_cl(std::string(KERNEL_HELPERS) + std::string("/random.cl"));
    checkErr(random_cl.is_open() ? CL_SUCCESS:-1, "ifstream(random.cl)");

    // Get our kernel
    std::ifstream rnd_float32_matrix_cl(std::string(KERNELS) + std::string("/rnd_float32_matrix.cl"));
    checkErr(random_cl.is_open() ? CL_SUCCESS:-1, "ifstream(rnd_float32_matrix.cl)");

    // We must build out our string sources
    std::string random_cl_str(
        std::istreambuf_iterator <char> (random_cl),
        (std::istreambuf_iterator <char> ())
    );

    std::string rnd_float32_matrix_cl_str(
      std::istreambuf_iterator <char> (rnd_float32_matrix_cl),
      (std::istreambuf_iterator <char> ())
    );

    std::string prog_str = cl_header +
                           random_cl_str +
                           rnd_float32_matrix_cl_str;

    // Print program string
    printProgram(prog_str);

    // Create sources
    cl::Program::Sources sources(
      1,
      std::make_pair(prog_str.c_str(), prog_str.length() + 1)
    );

    // Define our program
    cl::Program program(context, sources, &err);
    checkErr(err, "Program()");

    // Build on device
    err = program.build(devices, NULL, NULL, NULL);
    checkErr(err, "Program.build()");

    kernel = cl::Kernel(program, "rnd_float32_matrix", &err);
    checkErr(err, "Building kernel");

    std::cout << "Kernel Wrapper Built Successful!" << std::endl;
  }

  void Random_Float32_Matrix::buildCommandQueues () {
    cl_int err;
    int size = devices.size();
    queues = std::vector <cl::CommandQueue> ();
    for (int i = 0; i < size; i ++) {
      queues.push_back(cl::CommandQueue(context, devices[0], 0, &err));
      checkErr(err, "Building Queue");
    }
  }

  Random_Float32_Matrix::Random_Float32_Matrix () {
    buildKernel();
    buildCommandQueues();
  }
#endif