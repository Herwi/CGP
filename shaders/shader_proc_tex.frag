#version 430 core

uniform vec3 lightDir;

in vec3 interpNormal;




void main()
{
	vec3 normal = normalize(interpNormal);
	float diffuse = max(dot(normal, -lightDir), 0.0);
	vec3 objectColor;
	if(sin(vertexPosition.y)>0){
		objectColor = vec3(1,1,1);

		}else{
			objectColor = vec3(0,0,0);
	}



}
