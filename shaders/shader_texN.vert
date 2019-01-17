#version 430 core

layout(location = 0) in vec3 vertexPosition;
layout(location = 1) in vec2 vertexTexCoord;
layout(location = 2) in vec3 vertexNormal;
layout(location = 3) in vec3 vertexTangent;

uniform mat4 modelViewProjectionMatrix;
uniform mat4 modelMatrix;
uniform vec3 lightDir;
uniform vec3 cameraPos;

out vec3 interpNormal;
out vec3 interpPos;
out vec2 interpTexCoord;
out vec3 interpLightDir;
out vec3 interpCameraPos;

vec3 interpTangent;
vec3 interpBitangent;

void main()
{
	gl_Position = modelViewProjectionMatrix * vec4(vertexPosition, 1.0);
	interpNormal = normalize((modelMatrix * vec4(vertexNormal, 0.0)).xyz);
	interpTangent = normalize((modelMatrix * vec4(vertexTangent, 0.0)).xyz);
	interpBitangent = normalize(cross(interpNormal, interpTangent));
	interpTexCoord = vertexTexCoord;
	vec3 tempInterpPos = (modelMatrix * vec4(vertexPosition, 1.0)).xyz;
	
	interpPos.x = dot(tempInterpPos, interpTangent);
	interpPos.y = dot(tempInterpPos, interpBitangent);
	interpPos.z = dot(tempInterpPos, interpNormal);
	
	interpLightDir.x = dot(lightDir, interpTangent);
	interpLightDir.y = dot(lightDir, interpBitangent);
	interpLightDir.z = dot(lightDir, interpNormal);
	
	interpCameraPos.x = dot(cameraPos, interpTangent);
	interpCameraPos.y = dot(cameraPos, interpBitangent);
	interpCameraPos.z = dot(cameraPos, interpNormal);
}
