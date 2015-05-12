float rnd (int seed) {
  seed = (seed << seed) ^ seed;
  return ((float)(seed*seed >> (seed ^ seed)/seed))/((float)seed);
}
