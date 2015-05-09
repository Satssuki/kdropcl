/**
 * Please take this out once you are dealing with a
 * reasonable gpgpu provider.
 **/
#pragma once
#ifndef QUANT_DEVICE_STATS
  #define QUANT_DEVICE_STATS

  #define DIM_SIZE 100
  #define QUANT_SPDS std::shared_ptr<DeviceStats>

  struct DeviceStats {
    std::string name;
    cl_bool availability;
    cl_uint clock_freq;
    std::string device_extensions;
    cl_bool little_endian;
    cl_ulong mx_mem_alloc;
    cl_uint mx_comp_unit;
    size_t mx_wkgp_size;
    cl_uint mx_item_dim;
    std::vector <size_t> mx_item_sizes;
  };

  inline std::shared_ptr <DeviceStats> mapDeviceStats (cl::Device &device) {

    size_t mx_item_sizes[DIM_SIZE];
    std::shared_ptr <DeviceStats> ds(new DeviceStats);

    // Initialize array to zero
    for (int i = 0; i < DIM_SIZE; i++) {
      mx_item_sizes[i] = 0;
    }

    device.getInfo( (cl_device_info) CL_DEVICE_NAME , &ds->name );
    device.getInfo( (cl_device_info) CL_DEVICE_AVAILABLE, &ds->availability);
    device.getInfo( (cl_device_info) CL_DEVICE_MAX_CLOCK_FREQUENCY, &ds->clock_freq);
    device.getInfo( (cl_device_info) CL_DEVICE_EXTENSIONS, &ds->device_extensions);
    device.getInfo( (cl_device_info) CL_DEVICE_ENDIAN_LITTLE , &ds->little_endian);
    device.getInfo( (cl_device_info) CL_DEVICE_MAX_MEM_ALLOC_SIZE, &ds->mx_mem_alloc);
    device.getInfo( (cl_device_info) CL_DEVICE_MAX_COMPUTE_UNITS, &ds->mx_comp_unit);
    device.getInfo( (cl_device_info) CL_DEVICE_MAX_WORK_GROUP_SIZE, &ds->mx_wkgp_size);
    device.getInfo( (cl_device_info) CL_DEVICE_MAX_WORK_ITEM_DIMENSIONS, &ds->mx_item_dim);

    device.getInfo( (cl_device_info) CL_DEVICE_MAX_WORK_ITEM_SIZES, &mx_item_sizes);

    for (int i = 0; i < DIM_SIZE; i ++) {
      if (mx_item_sizes[i] == 0)
        break;
      ds->mx_item_sizes.push_back(mx_item_sizes[i]);
    }

    return ds;
  }

  inline void printDeviceInfo(std::vector <std::shared_ptr<DeviceStats>> dv) {
    int size = dv.size();
    for(int i = 0; i < size; i ++) {
      std::cout << "------------------------------------------------------" << std::endl;
      std::cout << "Device Name: " << dv[i]->name << std::endl;
      std::cout << "Availability: " << dv[i]->availability << std::endl;
      std::cout << "Clock Freq(MHz): " << dv[i]->clock_freq << std::endl;
      std::cout << "Device Extensions: " << dv[i]->device_extensions << std::endl;
      std::cout << "Little Endian: " << dv[i]->little_endian << std::endl;
      std::cout << "Max Memory Alloc(Bytes): " << dv[i]->mx_mem_alloc << std::endl;
      std::cout << "Max Compute Unit: " << dv[i]->mx_comp_unit << std::endl;
      std::cout << "Max Workgroup Size: " << dv[i]->mx_wkgp_size << std::endl;
      std::cout << "Total Work Dimensions: " << dv[i]->mx_item_dim << std::endl;

      std::vector <size_t> mx_item_sizes = dv[i]->mx_item_sizes;
      
      int size_it = mx_item_sizes.size();

      for(int j = 0; j < size_it; j++)
        std::cout << "Total Available Work Items Dim(" << j << "): " << mx_item_sizes[j] << std::endl;
    }
    std::cout << "------------------------------------------------------" << std::endl;
  }
#endif