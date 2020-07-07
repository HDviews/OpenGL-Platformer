#version 330 core
#extension GL_ARB_explicit_uniform_location : require

layout (location = 0) in vec3 aPos;   // the position variable has attribute position 0
layout (location = 1) in vec3 aColor; // the color variable has attribute position 1
layout (location = 2) in vec2 aTexCoord; // the texture variable has attribute position 2
  
out vec3 ourColor; // output a color to the fragment shader
out vec2 TexCoord; // holds the texture data

layout (location = 0) uniform mat4 proj;
layout (location = 1) uniform mat4 view;
layout (location = 2) uniform mat4 scalar;
layout (location = 3) uniform mat4 model;

void main()
{
    gl_Position = proj * view * scalar * model * vec4(aPos, 1.0);
    ourColor = aColor; // set ourColor to the input color we got from the vertex data
    TexCoord = aTexCoord;
}       
