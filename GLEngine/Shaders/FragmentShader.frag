#version 330 core
out vec4 FragColor;

in vec3 VertexColor;
in vec3 VertexNormal;
in vec3 fragPosition;

uniform vec3 viewPos;

float ambientStrength = 0.4;
float Shininess = 30;
float specularStrength = 0.5;
vec3 lightColor = vec3(1, 1, 1);
vec3 lightPos = vec3(0, 10, 0);


void main()
{
	vec3 viewDir = normalize(viewPos - fragPosition);

	vec3 Ambient = ambientStrength * lightColor;

	vec3 Normal = normalize(VertexNormal);
	vec3 lightDir = normalize(lightPos - fragPosition);

	float diffuseStrength = max(dot(Normal, lightDir), 0.0);
	vec3 Diffuse = diffuseStrength * lightColor;

	vec3 reflectDir = reflect(-lightDir, Normal);
	float spec = pow(max(dot(viewDir, reflectDir), 0.0), Shininess);
	vec3 Specular = specularStrength * spec * lightColor;

	vec3 Result = (Ambient + Diffuse + Specular) * VertexColor;
	FragColor = vec4(Result, 1.0);
}