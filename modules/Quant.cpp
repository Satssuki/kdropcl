/**
 * Please take this out once you are dealing with a
 * reasonable gpgpu provider.
 **/
#pragma once
#ifndef QUANT_QUANT_CPP
  #define QUANT_QUANT_CPP

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
  #include "QuantEnums.hpp"
  #include "Quant.hpp"
  #include "DeviceStats.hpp"

  std::vector <cl::CommandQueue> Quant::queues;
  std::vector <cl::Device> Quant::devices;
  std::vector <std::shared_ptr<DeviceStats>> Quant::devices_stats;
  cl::Context Quant::context;
  cl::Platform Quant::platform;

  void Quant::checkErr (cl_int err, const char * name) {
    if (err != CL_SUCCESS) {
      std::cerr << "ERROR: " << name << " (" << err << ")" << std::endl;
      exit(EXIT_FAILURE);
    }
  }

  // We need to find a platform that has the GPU devices that
  // we want. Once we find our gpu devices, we stuff them and
  // create a device stats
  QERRORS Quant::getPlatform () {
    std::vector <cl::Platform> platformList;
    cl::Platform::get(&platformList);
    QERRORS code = QERRORS::GPGPU_PLATFORM_NOT_FOUND;

    // Iterate through our platforms to find one that has
    // GPU Devices...
    size_t plat_size = platformList.size();
    
    for (int i = 0; i < plat_size; i ++) {
      // Get a particular platform
      platform = platformList[i];

      // Extract devices
      platform.getDevices( (cl_device_type)CL_DEVICE_TYPE_GPU, &devices );

      size_t dev_size = devices.size();
      
      for (int j = 0; j < dev_size; j++)
        devices_stats.push_back(mapDeviceStats(devices[j]));

      if (devices_stats.size() > 0) {
        code = QERRORS::SUCCESS;
      }
    }

    printDeviceInfo(devices_stats);

    // Release Memory
    platformList.clear();

    return code;
  }

  void Quant::buildQueues() {
    // Error tracker
    cl_int err;

    // Get number of devices
    int size = devices.size();

    // Iterate through each device and build a queue
    for (int i = 0; i < size; i ++) {
      queues.push_back(cl::CommandQueue(context, devices[0], 0, &err));
      checkErr(err, "Building Queue");
    }
  }

  void Quant::initialize() {
    cl_int err;
    
    // Get platform and devices
    checkErr((cl_int)getPlatform(), "Quant::getPlatform()");

    // Context configuration
    cl_context_properties cprops[3] = {
      CL_CONTEXT_PLATFORM,
      (cl_context_properties)platform(),
      0
    };

    // Create our context
    context = cl::Context(
      devices,
      cprops,
      NULL,
      NULL,
      &err
    );

    checkErr(err, "Quant::initialize -> cl::Context()");

    // Build out command queues
    buildQueues();
  }
#endif