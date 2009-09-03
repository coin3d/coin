
float DirSpotLight(in int i,
                   in vec3 eye,
                   in vec3 ecPosition3,
                   in vec3 normal,
                   inout vec4 diffuse,
                   inout vec4 specular)
{
  float nDotVP;
  float nDotHV;
  float pf;
  vec3 dir = -normalize(vec3(gl_LightSource[i].spotDirection));
  vec3 hv = normalize(eye + dir);
  nDotVP = max(0.0, dot(normal, dir));
  nDotHV = max(0.0, dot(normal, hv));
  float shininess = gl_FrontMaterial.shininess;
  if (nDotVP == 0.0)
    pf = 0.0;
  else
    pf = pow(nDotHV, shininess);

  diffuse *= nDotVP;
  specular *= pf;
  return length(vec3(gl_LightSource[i].position) - ecPosition3);
}

