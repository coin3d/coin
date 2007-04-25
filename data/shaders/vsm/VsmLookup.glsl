float VsmLookup(in vec4 map, in float dist, in float epsilon) 
{
  float mapdist = map.x;
  float lit_factor = dist <= mapdist ? 1.0 : 0.0;
  float E_x2 = map.y;
  float Ex_2 = mapdist * mapdist;
  float variance = min(max(E_x2 - Ex_2, 0.0) + epsilon, 1.0);
  float m_d = mapdist - dist;
  float p_max = variance / (variance + m_d * m_d);
  return max(lit_factor, p_max);
}
