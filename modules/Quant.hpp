/**
 * Please take this out once you are dealing with a
 * reasonable gpgpu provider.
 **/
#pragma once
#ifndef QUANT_QUANT_HPP
  #define QUANT_QUANT_HPP

  #ifndef QUANT_CL
    #define QUANT_CL
    
    #include <CL/cl.h>
    #undef CL_VERSION_1_2
    #define CL_EXT_SUFFIX__VERSION_1_1_DEPRECATED
    #include <CL/cl.hpp>
  #endif

  #include <memory>
  #include <vector>
  #include <iostream>
  #include "DeviceStats.hpp"

  class Quant {
    private:
      static cl::Platform getPlatform();
    protected:
      static std::vector<cl::Device> devices;
      static std::vector <std::shared_ptr<DeviceStats>> devices_stats;
      static cl::Context context;
    public:
      static void initialize();
  };
#endif