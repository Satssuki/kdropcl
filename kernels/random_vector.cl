__kernel void vector_add (
  __global const unsigned int array_size,
  __global float* res
)
{
   const int idx = get_global_id(0);

   if (idx < array_size)
      res[idx] = 5;
}