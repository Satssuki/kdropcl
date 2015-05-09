#include <CL/cl.h>
#undef CL_VERSION_1_2
#define CL_EXT_SUFFIX__VERSION_1_1_DEPRECATED
#include <CL/cl.hpp>
#include <iostream>
#include <fstream>
#include "CLNeuralNetworkConfig.h"

#define SIZE 4000

inline void checkErr(cl_int err, const char * name) {
    if (err != CL_SUCCESS) {
      std::cerr << "ERROR: " << name  << " (" << err << ")" << std::endl;
      exit(EXIT_FAILURE);
   }
}

int main(void) {

  // Error code
  cl_int err;

  // Get platform
  std::vector <cl::Platform> platformList;
  cl::Platform::get(&platformList);
  
  // Get various platform information

  // Get number of opencl platform implementations
  checkErr(platformList.size()!=0 ? CL_SUCCESS : -1, "cl::Platform::get");
  std::cerr << "Platform number is: " << platformList.size() << std::endl;
  
  // Get platform vendor
  std::string platformVendor;
  platformList[0].getInfo( (cl_platform_info) CL_PLATFORM_VENDOR, &platformVendor );
  std::cerr << "Platform is by: " << platformVendor << "\n";

  // Get devices for first platform
  std::vector <cl::Device> devices;
  platformList[0].getDevices( (cl_device_type) CL_DEVICE_TYPE_GPU, &devices);
  std::cout << devices.size() << std::endl;

  // Just get the first device
  cl::Device device = devices[0];

  // Get device name
  std::string device_name;
  device.getInfo( (cl_device_info) CL_DEVICE_NAME, &device_name);
  std::cout << "Device name: " << device_name << std::endl;

  // Get device availability
  cl_bool availability;
  device.getInfo( (cl_device_info) CL_DEVICE_AVAILABLE, &availability);
  std::cout << "Availability: " << availability << std::endl;

  // Get max clock frequency in MHz
  cl_uint clock_freq;
  device.getInfo( (cl_device_info) CL_DEVICE_MAX_CLOCK_FREQUENCY, &clock_freq);
  std::cout << "Max clock frequency: " << clock_freq << std::endl;

  // Get max clock frequency in MHz
  char device_extensions[400];
  device.getInfo( (cl_device_info) CL_DEVICE_EXTENSIONS, &device_extensions);
  std::cout << "Device extentions: " << device_extensions << std::endl;

  // Get Endianess
  // Get max clock frequency in MHz
  cl_bool little_endian;
  device.getInfo( (cl_device_info)  CL_DEVICE_ENDIAN_LITTLE , &little_endian);
  std::cout << "Little endian: " << little_endian << std::endl;

  // Get device max mem alloc size
  cl_ulong mx_mem_alloc;
  device.getInfo( (cl_device_info) CL_DEVICE_MAX_MEM_ALLOC_SIZE, &mx_mem_alloc);
  std::cout << "Max alloc mem: " << mx_mem_alloc << std::endl;

  // Get compute units
  cl_uint mx_comp_unit;
  device.getInfo( (cl_device_info) CL_DEVICE_MAX_COMPUTE_UNITS, &mx_comp_unit);
  std::cout << "Max compute units: " << mx_comp_unit << std::endl;

  // Get work group size
  size_t mx_wkgp_size;
  device.getInfo( (cl_device_info) CL_DEVICE_MAX_WORK_GROUP_SIZE, &mx_wkgp_size);
  std::cout << "Max group sizes: " << mx_wkgp_size << std::endl;

  // Get max work item dimensions
  cl_uint mx_item_dim;
  device.getInfo( (cl_device_info) CL_DEVICE_MAX_WORK_ITEM_DIMENSIONS, &mx_item_dim);
  std::cout << "Max item dimmensions: " << mx_item_dim << std::endl;

  // Get max work item sizes                          
  size_t mx_item_sizes[3] = { 0, 0, 0};
  device.getInfo( (cl_device_info) CL_DEVICE_MAX_WORK_ITEM_SIZES, &mx_item_sizes);
  std::cout << "Max item sizes for 0: " << mx_item_sizes[0] << std::endl;
  std::cout << "Max item sizes for 1: " << mx_item_sizes[1] << std::endl;
  std::cout << "Max item sizes for 2: " << mx_item_sizes[2] << std::endl;


  // Defime the properties of our platform so that
  // we may associate it with a context
  cl_context_properties cprops[3] = {
  	CL_CONTEXT_PLATFORM,
  	(cl_context_properties)(platformList[0])(),
  	0
  };

  // Create a context associated with first platform
  // and some gpu device
  cl::Context context(
    devices,
    cprops,
    NULL,
    NULL,
    &err);

  // Any errors?
  checkErr(err, "Context::Context()");

  // Allocate shared memory for gpu
  size_t mem_size = sizeof(float)*SIZE;

  float *src_h = new float[SIZE];

  // Since we have a context we can associate a
  // pointer to device using it's context.
  //
  // CL_MEM_READ_ONLY == This flag specifies
  // that the memory object is a read-only memory
  // object when used inside a kernel.
  //
  // CL_MEM_WRITE_ONLY == This flags specifies
  // that the memory object will be written
  // but not read by a kernel.
  // Reading from a buffer or image object
  // created with CL_MEM_WRITE_ONLY inside a
  // kernel is undefined.
  //
  // CL_MEM_COPY_HOST_PTR == This flag is valid
  // only if host_ptr is not NULL. If specified,
  // it indicates that the application wants the
  // OpenCL implementation to allocate memory for
  // the memory object and copy the data from memory
  // referenced by host_ptr.
  // Writing to a buffer or image object created with
  // CL_MEM_READ_ONLY inside a kernel is undefined.
  cl::Buffer src_d(
    context,
    CL_MEM_READ_WRITE | CL_MEM_COPY_HOST_PTR,
    mem_size,
    src_h,
    &err);

   checkErr(err, "Buffer::Buffer()");

   // std::ifstream hello_cl("../kernels/hello.cl");
   // checkErr(hello_cl.is_open() ? CL_SUCCESS:-1, "hello.cl");

   // std::string hello_str(
   //    std::istreambuf_iterator<char>(hello_cl),
   //    (std::istreambuf_iterator<char>())
   //  );

   // cl::Program::Sources sources(
   //    1,
   //    std::make_pair(hello_str.c_str(), hello_str.length() + 1)
   //  );

   std::ifstream random_cl("../kernels/random.cl");
   checkErr(random_cl.is_open() ? CL_SUCCESS:-1, "random.cl");

   std::ifstream rnd_float32_matrix_cl("../kernels/rnd_float32_matrix.cl");
   checkErr(rnd_float32_matrix_cl.is_open() ? CL_SUCCESS:-1, "rnd_float32_matrix.cl");

   // Create our strings
   std::string random_cl_str(
      std::istreambuf_iterator<char>(random_cl),
      (std::istreambuf_iterator<char>())
    );

   std::string rnd_float32_matrix_cl_str(
      std::istreambuf_iterator<char>(rnd_float32_matrix_cl),
      (std::istreambuf_iterator<char>())
    );

   random_cl_str += rnd_float32_matrix_cl_str;

   std::cout << random_cl_str << std::endl;

   // Create our programs from source
   cl::Program::Sources sources(
      1,
      std::make_pair(random_cl_str.c_str(), random_cl_str.length() + 1)
    );

   // sources.push_back(
   //    std::make_pair(rnd_float32_matrix_cl_str.c_str(), rnd_float32_matrix_cl_str.length() + 1)
   //  );

   cl::Program program(context, sources, &err);

   checkErr(err, "Program::Program()");

   err = program.build(devices, NULL, NULL, NULL);

   checkErr(err, "Program::build()");

   cl::Kernel kernel(program, "rnd_float32_matrix", &err);

   checkErr(err, "Kernel::Kernel()");

   err = kernel.setArg(0, SIZE);
   checkErr(err, "Kernel::SetArg");

   err = kernel.setArg(1, 29304958);
   checkErr(err, "Kernel::SetArg");

   err = kernel.setArg(2, src_d);
   checkErr(err, "Kernel::SetArg");

   cl::CommandQueue queue(context, devices[0], 0, &err);
   checkErr(err, "CommandQueue::CommandQueue()");

   cl::Event event;

   err = queue.enqueueNDRangeKernel(
      kernel,
      cl::NDRange(0),
      cl::NDRange(1024),
      cl::NDRange(1),
      NULL,
      &event
    );

   checkErr(err, "CommandQueue::enqueueNDRangeKernel()");

   event.wait();

   err = queue.enqueueReadBuffer(
      src_d,
      CL_TRUE,
      0,
      mem_size,
      src_h);

   for(int i = 0; i < SIZE; i ++) {
    std::cout << src_h[i] << " ";
   }

   std::cout << std::endl;
  
  return EXIT_SUCCESS;
}