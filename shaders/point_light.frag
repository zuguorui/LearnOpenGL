#version 330 core

struct Material {
    sampler2D diffuse;
    sampler2D specular;
    float shininess;
};

struct Light {
    vec3 position;
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;

    float constant;
    float linear;
    float quadratic;
};

uniform Material material;
uniform Light light;
uniform vec3 viewPos;


in vec2 TexCoords;
in vec3 Normal;
in vec3 FragPos;

out vec4 fragColor;

void main() {
    
    

    vec3 ambient = light.ambient * texture(material.diffuse, TexCoords).rgb;

    vec3 norm = normalize(Normal);

    vec3 lightDir = normalize(light.position - FragPos);
    
    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = light.diffuse * diff * texture(material.diffuse, TexCoords).rgb;

    vec3 viewDir = normalize(viewPos - FragPos);
    vec3 reflectDir = reflect(-lightDir, norm);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
    vec3 specular = light.specular * spec * texture(material.specular, TexCoords).rgb;

    float dis = length(light.position - FragPos);
    float attenuation = 1.0f / (light.constant + light.linear * dis + light.quadratic * dis * dis);

    ambient *= attenuation;
    diffuse *= attenuation;
    specular *= attenuation;

    fragColor = vec4((ambient + diffuse + specular), 1.0);
}


