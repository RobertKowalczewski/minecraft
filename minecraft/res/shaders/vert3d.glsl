#version 440 core
layout(location = 0) in uint data;

out vec3 texCoord;
out vec3 normal;
out vec3 fragPos;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;
uniform float normals[18];

void main()
{
    texCoord = vec3(
        float(data & 0x1u),
        float((data & 0x2u) >> 1u),
        float((data & 0xFCu) >> 2u)
    );

    vec4 aPos = vec4(
        float((data & 0x3F00u) >> 8u),
        float((data & 0x7FC000u) >> 14u),
        float((data & 0x1F800000u) >> 23u),
        1.0
    );
    int normalIndex = int((data & 0xE0000000u) >> 29u);
    vec3 aNormal = vec3(normals[normalIndex * 3], normals[normalIndex * 3 + 1], normals[normalIndex * 3 + 2]);


    // note that we read the multiplication from right to left
    gl_Position = projection * view * model * aPos;
    fragPos = vec3(model * aPos);
    //normal = mat3(transpose(inverse(model))) * aNormal;
    //normal = vec3(abs(aNormal.x), abs(normal.y),abs(aNormal.z));
    normal = aNormal;
}