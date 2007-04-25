float VsmLookup(in vec4 map, in float dist, in float epsilon) 
{
#if 1 // Vsm smoothing version
  float mapdist = map.x;

  // replace 0.0 with some factor > 0.0 to make the light affect even parts in shadow
  float lit_factor = dist <= mapdist ? 1.0 : 0.0;
  float E_x2 = map.y;
  float Ex_2 = mapdist * mapdist;
  float variance = min(max(E_x2 - Ex_2, 0.0) + epsilon, 1.0);
  float m_d = mapdist - dist;
  float p_max = variance / (variance + m_d * m_d);
  return max(lit_factor, p_max);
#else // use this version for old fashioned shadow mapping with no variance
  float mapdist = map.x;
  return dist - epsilon <= mapdist ? 1.0 : 0.0;
#endif
}
