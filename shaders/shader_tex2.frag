#version 430 core

uniform sampler2D textureSampler;
uniform vec3 lightDir;
uniform vec3 lightDir1;
uniform vec3 lightDir2;

in vec3 interpNormal;
in vec2 interpTexCoord;

void main()
{
	vec3 color = texture2D(textureSampler, interpTexCoord).rgb;
	//vec3 normal = normalize(interpNormal);
	vec3 normal = normalize(color * 2.0 - 1.0) * normalize(interpNormal);
	float diffuse = max(dot(normal, -lightDir), 0.0);
	float diffuse1 = max(dot(normal, -lightDir1), 0.0);
	float diffuse2 = max(dot(normal, -lightDir2), 0.0);
	gl_FragColor = vec4(color * diffuse2*1.0f , 1.0) + vec4(color * diffuse1*2.8f , 1.0) + vec4(color * diffuse*1.8f, 1.0);
}
