// Vertex Shader

static const char* PhongVSH = STRINGIFY
(
 
 // Attributes
 attribute vec3 aPosition;
 attribute vec3 aNormal;
 
 // Uniforms
 uniform mat4 uProjectionMatrix;
 uniform mat4 uModelViewMatrix;
 uniform mat3 uNormalMatrix;
 
 // Varying
 varying vec3 vNormal;
 
 void main(void)
{
    vNormal = uNormalMatrix * aNormal;
    gl_Position = uProjectionMatrix * uModelViewMatrix * vec4(aPosition, 1.0);
}
 
 );