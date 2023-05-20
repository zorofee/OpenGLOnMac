#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;

out vec3 FragPos;
out vec3 Normal;

uniform mat4 u_ModelMat;
uniform mat4 u_ViewMat;
uniform mat4 u_ProjMat;

void main()
{
    FragPos = vec3(u_ModelMat * vec4(aPos, 1.0));
    Normal = mat3(transpose(inverse(u_ModelMat))) * aNormal;  
    
    gl_Position = u_ProjMat * u_ViewMat * vec4(FragPos, 1.0);
}