
float SpotLight(in int i,
                in vec3 eye,
                in vec3 ecPosition3,
                in vec3 normal,
                inout vec4 ambient,
                inout vec4 diffuse,
                inout vec4 specular)
{
  float nDotVP;
  float nDotHV;
  float pf;
  float att;
  float spotDot;
  float spotAtt;
  float d;
  vec3 VP;
  vec3 halfvec;

  VP = vec3(gl_LightSource[i].position) - ecPosition3;
  d = length(VP);
  VP = normalize(VP);

  att = 1.0 / (gl_LightSource[i].constantAttenuation +
               gl_LightSource[i].linearAttenuation * d +
               gl_LightSource[i].quadraticAttenuation * d * d);

  spotDot = dot(-VP, normalize(gl_LightSource[i].spotDirection));

  // need to read this variable outside the if statment to work around ATi driver issues
  float spotexp = gl_LightSource[i].spotExponent;

  if (spotDot < gl_LightSource[i].spotCosCutoff)
    spotAtt = 0.0;
  else
    spotAtt = pow(spotDot, spotexp);

  att *= spotAtt;

  halfvec = normalize(VP + eye);
  nDotVP = max(0.0, dot(normal, VP));
  nDotHV = max(0.0, dot(normal, halfvec));

  // need to read this variable outside the if statment to work around ATi driver issues
  float shininess =  gl_FrontMaterial.shininess;

  if (nDotVP == 0.0)
    pf = 0.0;
  else
    pf = pow(nDotHV, shininess);

  ambient += gl_LightSource[i].ambient * att;
  diffuse += gl_LightSource[i].diffuse * nDotVP * att;
  specular += gl_LightSource[i].specular * pf * att;

  return d;
}

