#shader vertex
#version 330 core
layout(location = 0) in vec3 aPos;
layout(location = 1) in vec3 aNormal;
layout(location = 2) in vec2 aTexCoords;

out vec2 TexCoords;
out vec3 Normal;
out vec3 WorldPos;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main()
{
	Normal = mat3(model) * aNormal;
	WorldPos = vec3(model * vec4(aPos, 1.0));
	TexCoords = aTexCoords;
	gl_Position = projection * view * vec4(WorldPos, 1.0);
}

#shader fragment
#version 330 core
out vec4 FragColor;

in vec2 TexCoords;
in vec3 Normal;
in vec3 WorldPos;

uniform sampler2D albedo;
uniform sampler2D normal;
uniform sampler2D metallic;
uniform sampler2D roughness;
uniform sampler2D ambient;

uniform float fMetallic;
uniform float fRoughness;
uniform float fAmbient;

uniform vec3 vAlbedo;

uniform vec3 camPos;

struct Light
{
	vec3 position;
	vec3 colour;
};
uniform Light lights[5]; // 5 Lights Max

const float PI = 3.14159265359;

vec3 getNormalFromMap()
{
	vec3 tangentNormal = texture(normal, TexCoords).xyz * 2.0 - 1.0;

	vec3 Q1 = dFdx(WorldPos);
	vec3 Q2 = dFdy(WorldPos);
	vec2 st1 = dFdx(TexCoords);
	vec2 st2 = dFdy(TexCoords);

	vec3 N = normalize(Normal);
	vec3 T = normalize(Q1*st2.t - Q2 * st1.t);
	vec3 B = -normalize(cross(N, T));
	mat3 TBN = mat3(T, B, N);

	return normalize(TBN * tangentNormal);
}

float DistributionGGX(vec3 N, vec3 H, float R)
{
	float a = R * R;
	float a2 = a * a;
	float NdotH = max(dot(N, H), 0.0);
	float NdotH2 = NdotH * NdotH;

	float nom = a2;
	float denom = (NdotH2 * (a2 - 1.0) + 1.0);
	denom = PI * denom * denom;

	return nom / denom;
}

float GeometrySchlickGGX(float NdotV, float R)
{
	float r = (R + 1.0);
	float k = (r*r) / 8.0;

	float nom = NdotV;
	float denom = NdotV * (1.0 - k) + k;

	return nom / denom;
}

float GeometrySmith(vec3 N, vec3 V, vec3 L, float R)
{
	float NdotV = max(dot(N, V), 0.0);
	float NdotL = max(dot(N, L), 0.0);
	float ggx2 = GeometrySchlickGGX(NdotV, R);
	float ggx1 = GeometrySchlickGGX(NdotL, R);

	return ggx1 * ggx2;
}

vec3 fresnelSchlick(float cosTheta, vec3 F0)
{
	return F0 + (1.0 - F0) * pow(1.0 - cosTheta, 5.0);
}
vec3 fresnelSchlickRoughness(float cosTheta, vec3 F0, float R)
{
	return F0 + (max(vec3(1.0 - R), F0) - F0) * pow(1.0 - cosTheta, 5.0);
}

void main()
{
	vec3 _albedo = pow(texture(albedo, TexCoords).rgb, vec3(2.2));
	float _metallic = texture(metallic, TexCoords).r * fMetallic;
	float _roughness = texture(roughness, TexCoords).r * fRoughness;
	float _ao = texture(ambient, TexCoords).r * fAmbient;

	vec3 N = getNormalFromMap();
	vec3 V = normalize(camPos - WorldPos);
	vec3 R = reflect(-V, N);

	vec3 F0 = vec3(0.05);
	F0 = mix(F0, _albedo, _metallic);

	vec3 Lo = vec3(0);
	for (int i = 0; i < 5; ++i)
	{
		vec3 L = normalize(lights[i].position - WorldPos);
		vec3 H = normalize(V + L);
		float distance = length(lights[i].position - WorldPos);
		float attenuation = (1.f / (distance * distance)) + 0.3;
		vec3 radiance = lights[i].colour * attenuation;

		float NDF = DistributionGGX(N, H, _roughness);
		float G = GeometrySmith(N, V, L, _roughness);
		vec3 F = fresnelSchlick(max(dot(H, V), 0.0), F0);

		vec3 nominator = NDF * G * F;
		float denominator = 5 * max(dot(N, V), 0.0) * max(dot(N, L), 0.0) + 0.001;
		vec3 specular = nominator / denominator;

		vec3 kS = F;
		vec3 kD = vec3(1.0) - kS;
		kD *= 1.0 - _metallic;

		float NdotL = max(dot(N, L), 0.0);
		Lo += (kD * _albedo / PI + specular) * radiance * NdotL;
	}

	vec3 colour = Lo;
	colour = colour / (colour + vec3(1.0));
	colour = pow(colour, vec3(1.0 / 2.5));
	FragColor = vec4(colour, 1.0);
}
