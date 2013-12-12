// Fragment Shader

static const char* PhongFSH = STRINGIFY
(
 
 // Varying
 varying highp vec3 vNormal;
 
 // Uniforms
 uniform highp vec3 uDiffuse;
 uniform highp vec3 uSpecular;
 
 void main(void)
{
    highp vec3 material = (0.5*uDiffuse) + (0.5*uSpecular);
    gl_FragColor = vec4(material, 1.0);
}
 
 );