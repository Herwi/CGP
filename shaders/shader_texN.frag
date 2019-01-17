#version 430 core

uniform sampler2D colorMap;
uniform sampler2D normalMap;

in vec3 interpNormal;
in vec3 interpPos;
in vec2 interpTexCoord;
in vec3 interpLightDir;
in vec3 interpCameraPos;

void main()
{
	vec2 modifiedTexCoord = vec2(interpTexCoord.x, 1.0 - interpTexCoord.y);
	vec3 color = texture2D(colorMap, modifiedTexCoord).rgb;
	
	vec3 normal = texture2D(normalMap, modifiedTexCoord).rgb;
	normal = normalize((normal-0.5f)*2);
	float diffuse = max(dot(normal, -interpLightDir), 0.0);
	
	vec3 toEye = normalize(interpCameraPos - interpPos);
	float specular = pow(max(dot(toEye, reflect(interpLightDir, normal)), 0.0), 30.0);

	gl_FragColor = vec4(color * diffuse, 1.0) + vec4(vec3(1.0) * specular, 0.0);
}
