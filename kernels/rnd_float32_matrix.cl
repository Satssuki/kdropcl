/**
  * The idea is to map 1-d buffer to a
  * 2-d array or matrix. This methodology
  * can be generalized to tensors.
  **/
__kernel void rnd_float32_matrix (
  const uint size,
  const uint inSeed,
  __global float * res
) {
  // Matrices are rank 2 tensors.
  // We're going to adapt this for more general use.
  const uint rank = 2;

  // Get our dimension and global id
  const uint dim = get_work_dim() - 1;
  const uint idx = get_global_id(dim);
  const size_t dim_wk_size = get_global_size(dim);

  int seed = (int)inSeed;

  uint offset = dim;

  uint index = idx + (dim_wk_size * offset);

  //Get the coordinates of our matrix
  while(index < size) {
    res[index] =  rnd((int)res[index] + index);
    offset ++;
    index = idx + (dim_wk_size * offset);
  }
}