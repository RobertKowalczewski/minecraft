#version 440 core

out vec4 FragColor;

in vec3 texCoord;
in vec3 normal;
in vec3 fragPos;

uniform vec3 lightPos;
uniform vec3 lightColor;
uniform vec3 viewPos;

uniform sampler2DArray texture1;

void main()
{
    //ambient
    float ambientStrength = 0.2;
    vec3 ambient = ambientStrength * lightColor;

    vec4 pixelColor = texture(texture1, vec3(texCoord.xy, texCoord.z-1));
    //diffuse
    vec3 lightDir = normalize(lightPos - fragPos);
    float diff = max(dot(normal, lightDir), 0.0);
    vec3 diffuse = diff * lightColor;

    //specular
    /*float specularStrength = 0.2;
    vec3 viewDir = normalize(viewPos - fragPos);
    vec3 reflectDir = reflect(-lightDir, normal);

    int shineness = 32;
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), shineness);
    vec3 specular = specularStrength * spec * lightColor;*/

    FragColor = vec4((ambient + diffuse) * pixelColor.rgb, pixelColor.a);
    //FragColor = texture(texture1, vec3(texCoord.x, texCoord.y, 0));
}
