#version 330 core
layout(location = 0) in uint data;

out vec2 texCoord;
out vec3 normal;
out vec3 fragPos;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;
uniform float normals[18];

void main()
{
    //przetestuj przez uniform vec3 i zmien kolor jeœli siê nie zgadzas


    int normalIndex = int(data & 0x7u) - 1;
    vec3 aNormal = vec3(normals[normalIndex * 3], normals[normalIndex * 3 + 1], normals[normalIndex * 3 + 2]);

    texCoord = vec2(float((data & 0x10u) >> 4u), float((data & 0x8u) >> 3u));

    vec4 aPos = vec4(float((data & 0x1F80000u) >> 19u),
                     float((data & 0x7F800u) >> 11u),
                     float((data & 0x7E0u) >> 5u),
                     1.0);


    // note that we read the multiplication from right to left
    gl_Position = projection * view * model * aPos;
    fragPos = vec3(model * aPos);
    //normal = mat3(transpose(inverse(model))) * aNormal;
    //normal = vec3(abs(aNormal.x), abs(normal.y),abs(aNormal.z));
    normal = aNormal;
}