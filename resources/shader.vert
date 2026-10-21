#version 330 core

layout(location = 0) in vec3 a_Position;
layout(location = 1) in vec3 a_Normal;
layout(location = 2) in vec2 a_TextureUV;
layout(location = 3) in mat4 a_InstanceModel;

uniform mat4 Sal_Model;
uniform mat4 Sal_View;
uniform mat4 Sal_Projection;
uniform bool Sal_Instanced;

out vec3 v_Normal;
out vec2 v_TextureUV;

void main()
{
    mat4 model = Sal_Instanced ? a_InstanceModel : Sal_Model;

    vec4 worldPosition = model * vec4(a_Position, 1.0);

    v_Normal = normalize(
            transpose(inverse(mat3(model))) * a_Normal
        );
    v_TextureUV = a_TextureUV;

    gl_Position = Sal_Projection * Sal_View * worldPosition;
}
