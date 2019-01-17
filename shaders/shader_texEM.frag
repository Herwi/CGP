#version 430 core

uniform sampler2D textureSampler;
uniform vec3 lightDir;

uniform samplerCube cubeSampler;
uniform vec3 cameraPos;

in vec3 interpNormal;
in vec2 interpTexCoord;

in vec3 vertexPositionC;

float fresnelSchlick(float cosTheta, float F0)
{
    return F0 + (1.0 - F0) * pow(1.0 - cosTheta, 5.0);
}  


void main()
{
	vec3 normal = normalize(interpNormal);
	vec3 viewDir = normalize(vertexPositionC -cameraPos);
	vec3 reflection = reflect(viewDir, normal);
	
	float opacity = 1.0f;
	//doesn't seem to work
	opacity = max(0.0f, dot(-viewDir, normal));

	//reflaction
	float eta = 1.00f/1.33f;
	vec3 refraction = -refract(viewDir, normal, eta);

	//chromatic aberration
	vec3 refractionRed = -refract(viewDir, normal, eta - 0.02f);
	vec3 refractionGreen = -refract(viewDir, normal, eta + 0.02f);
	vec3 refractionBlue = -refract(viewDir, normal, eta - 0.01f);

	refraction = vec3(texture(cubeSampler, refractionRed).r, texture(cubeSampler, refractionGreen).g, texture(cubeSampler, refractionBlue).b);

	//combine
	float F0 = (((1-eta)*(1-eta))/((1+eta)*(1+eta)));
	float cosTheta = max(0.0f, dot(lightDir, normal));
	float Schlick = fresnelSchlick(cosTheta, F0);
	//vec3 colorMix = mix(texture(cubeSampler, refraction).rgb, texture(cubeSampler, reflection).rgb, vec3(Schlick, Schlick, Schlick));
	vec3 colorMix = mix(refraction.rgb, texture(cubeSampler, reflection).rgb, vec3(Schlick, Schlick, Schlick));

	gl_FragColor = vec4(colorMix, opacity);
	//gl_FragColor = vec4(texture(cubeSampler, reflection).rgb, opacity);
	//gl_FragColor = vec4(texture(cubeSampler, refraction).rgb, opacity);
	//kind of ca
	//gl_FragColor = vec4(refraction.rgb, opacity);
}
