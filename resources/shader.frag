#version 330 core

in vec3 v_Normal;
in vec2 v_TextureUV;

out vec4 FragColor;

uniform vec4 Sal_TintColor;
uniform bool Sal_HasTexture;
uniform sampler2D Sal_Texture_0;

void main()
{
    vec4 baseColor = Sal_TintColor;

    if (Sal_HasTexture)
    {
        baseColor *= texture(Sal_Texture_0, v_TextureUV);
    }

    vec3 normal = normalize(v_Normal);
    vec3 lightDirection = normalize(vec3(0.5, 1.0, 0.3));

    float diffuse = max(dot(normal, lightDirection), 0.0);
    float lighting = 0.2 + diffuse * 0.8;

    FragColor = vec4(baseColor.rgb * lighting, baseColor.a);
}
