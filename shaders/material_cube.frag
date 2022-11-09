#version 330 core

struct Material {
    // 环境光
    vec3 ambient;
    // 漫反射
    vec3 diffuse;
    // 镜面反射
    vec3 specular;

    // 反光强度
    float shininess;
};

uniform Material material;

in vec3 Normal;
in vec3 FragPos;

uniform vec3 lightPos;
uniform vec3 lightColor;
uniform vec3 viewPos;
uniform vec3 cubeColor;

out vec4 fragColor;

void main() {
    vec3 ambient = lightColor * material.ambient;

    vec3 norm = normalize(Normal);

    vec3 lightDir = normalize(lightPos - FragPos);
    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = lightColor * (diff * material.diffuse);

    vec3 viewDir = normalize(viewPos - FragPos);
    vec3 reflectDir = reflect(-lightDir, norm);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
    vec3 specular = lightColor * (diff * material.specular);

    fragColor = vec4((ambient + diffuse + specular), 1.0);
    
}

