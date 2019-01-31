#include "glew.h"
#include "freeglut.h"
#include "glm.hpp"
#include "ext.hpp"
#include <iostream>
#include <cmath>
#include <vector>
#include <string>
#include <cstdlib>

#include "Shader_Loader.h"
#include "Render_Utils.h"
#include "Camera.h"
#include "Texture.h"

#define ILOSCRYB 800
#define ILOSCWODOROSTOW 300
#define ILOSCBUBBLE 300
#define ILOSCSTATUL 2

GLuint programColor;
GLuint programTexture;
GLuint programSkybox;
GLuint programTextureNormal;
GLuint programEM;
GLuint programBRDF;

Core::Shader_Loader shaderLoader;

float timeF;
float timeFish;
float timeShark;

obj::Model shipModel;
obj::Model sharkModel;
obj::Model fishModel;
obj::Model plantModel;
obj::Model groundModel;
obj::Model sphereModel;
obj::Model chestModel;
obj::Model statueModel;

glm::vec3 sharkPos;
glm::vec3 sharkVecMin;
glm::vec3 sharkVecMax;

float cameraAngle = 0;
float cameraAngle1 = 0;
glm::vec3 cameraPos = glm::vec3(0, -100, 5);
glm::vec3 cameraDir; // Wektor "do przodu" kamery
glm::vec3 cameraSide; // Wektor "w bok" kamery
glm::vec3 cameraVer; // Wektor "w góre" kamery
GLuint sharkTex;
GLuint fishTex;
GLuint plantTex;
GLuint asteroidTex;
GLuint groundTex;
GLuint groundTexN;
GLuint skyTex;
GLuint chestTex;
GLuint chestTexN;
GLuint statueTex;
GLuint statueTexR;
GLuint statueTexM;
GLuint goldTex;
GLuint goldTexM;
GLuint goldTexR;

GLuint skybox;
std::vector<float> tangent(31956);

glm::mat4 cameraMatrix, perspectiveMatrix;

glm::vec3 lightDir = glm::normalize(glm::vec3(1.0f, -0.9f, -1.0f));
glm::vec3 lightDir1 = glm::normalize(glm::vec3(-1.0f, 0.9f, 1.0f));
glm::vec3 lightDir2 = glm::normalize(glm::vec3(-1.0f, -0.9f, -1.0f));
glm::vec3 lightDir3 = glm::normalize(glm::vec3(1.0f, -0.9f, 1.0f));

const float cubeVertices[] = {
	30.5f, 30.5f, 30.5f, 1.0f,
	30.5f, -30.5f, 30.5f, 1.0f,
	-30.5f, 30.5f, 30.5f, 1.0f,

	30.5f, -30.5f, 30.5f, 1.0f,
	-30.5f, -30.5f, 30.5f, 1.0f,
	-30.5f, 30.5f, 30.5f, 1.0f,

	30.5f, 30.5f, -30.5f, 1.0f,
	-30.5f, 30.5f, -30.5f, 1.0f,
	30.5f, -30.5f, -30.5f, 1.0f,

	30.5f, -30.5f, -30.5f, 1.0f,
	-30.5f, 30.5f, -30.5f, 1.0f,
	-30.5f, -30.5f, -30.5f, 1.0f,

	-30.5f, 30.5f, 30.5f, 1.0f,
	-30.5f, -30.5f, 30.5f, 1.0f,
	-30.5f, -30.5f, -30.5f, 1.0f,

	-30.5f, 30.5f, 30.5f, 1.0f,
	-30.5f, -30.5f, -30.5f, 1.0f,
	-30.5f, 30.5f, -30.5f, 1.0f,

	30.5f, 30.5f, 30.5f, 1.0f,
	30.5f, -30.5f, -30.5f, 1.0f,
	30.5f, -30.5f, 30.5f, 1.0f,

	30.5f, 30.5f, 30.5f, 1.0f,
	30.5f, 30.5f, -30.5f, 1.0f,
	30.5f, -30.5f, -30.5f, 1.0f,

	30.5f, 30.5f, -30.5f, 1.0f,
	30.5f, 30.5f, 30.5f, 1.0f,
	-30.5f, 30.5f, 30.5f, 1.0f,

	30.5f, 30.5f, -30.5f, 1.0f,
	-30.5f, 30.5f, 30.5f, 1.0f,
	-30.5f, 30.5f, -30.5f, 1.0f,

	30.5f, -30.5f, -30.5f, 1.0f,
	-30.5f, -30.5f, 30.5f, 1.0f,
	30.5f, -30.5f, 30.5f, 1.0f,

	30.5f, -30.5f, -30.5f, 1.0f,
	-30.5f, -30.5f, -30.5f, 1.0f,
	-30.5f, -30.5f, 30.5f, 1.0f,
};

void calcSharkPos() {
	sharkPos = cameraPos + cameraDir * 0.5f;
	glm::vec3 dir = glm::vec3(1, 0, 1);
	sharkVecMin = sharkPos + dir * 0.5f + glm::rotate(dir, glm::radians(90.0f), glm::vec3(0, 1, 0)) * 0.5f + glm::vec3(0, 0.5f, 0);
	sharkVecMax = sharkPos - dir * 0.5f + glm::rotate(dir, glm::radians(-90.0f), glm::vec3(0, 1, 0)) * 0.5f - glm::vec3(0, 0.5f, 0);
	//std::cout << glm::to_string(cameraDir) << std::endl;
}

glm::mat4 createCameraMatrix()
{
	cameraDir = glm::vec3(cosf(cameraAngle - glm::radians(90.0f)), 0.0f, sinf(cameraAngle - glm::radians(90.0f)));
	glm::vec3 up = glm::vec3(0, 1, 0);
	cameraVer = glm::vec3(0.0f, sinf(cameraAngle1 - glm::radians(90.0f)), 0.0f);
	cameraSide = glm::cross(cameraDir, up);

	return Core::createViewMatrix(cameraPos, cameraDir, -cameraVer);
}

void drawObjectColor(obj::Model * model, glm::mat4 modelMatrix, glm::vec3 color, float alpha = 1.0f)
{
	GLuint program = programColor;

	glUseProgram(program);

	glUniform3f(glGetUniformLocation(program, "objectColor"), color.x, color.y, color.z);
	glUniform3f(glGetUniformLocation(program, "lightDir"), lightDir.x, lightDir.y, lightDir.z);

	glUniform1f(glGetUniformLocation(program, "alpha"), alpha);

	glm::mat4 transformation = perspectiveMatrix * cameraMatrix * modelMatrix;
	glUniformMatrix4fv(glGetUniformLocation(program, "modelViewProjectionMatrix"), 1, GL_FALSE, (float*)&transformation);
	glUniformMatrix4fv(glGetUniformLocation(program, "modelMatrix"), 1, GL_FALSE, (float*)&modelMatrix);

	Core::DrawModel(model);

	glUseProgram(0);
}

void drawObjectProceduralTexture(obj::Model * model, glm::mat4 modelMatrix)
{

	GLuint program = programTexture;

	glUseProgram(program);
	glUniform3f(glGetUniformLocation(program, "lightDir"), lightDir.x, lightDir.y, lightDir.z);

	glm::mat4 transformation = perspectiveMatrix * cameraMatrix * modelMatrix;
	glUniformMatrix4fv(glGetUniformLocation(program, "modelViewProjectionMatrix"), 1, GL_FALSE, (float*)&transformation);
	glUniformMatrix4fv(glGetUniformLocation(program, "modelMatrix"), 1, GL_FALSE, (float*)&modelMatrix);

	Core::DrawModel(model);

	glUseProgram(0);





}


void drawObjectTexture(obj::Model * model, glm::mat4 modelMatrix, GLuint textureId)
{
	GLuint program = programTexture;

	glUseProgram(program);

	glUniform3f(glGetUniformLocation(program, "lightDir"), lightDir.x, lightDir.y, lightDir.z);
	glUniform3f(glGetUniformLocation(program, "lightDir1"), lightDir1.x, lightDir1.y, lightDir1.z);
	glUniform3f(glGetUniformLocation(program, "lightDir2"), lightDir2.x, lightDir2.y, lightDir2.z);

	Core::SetActiveTexture(textureId, "textureSampler", program, 0);

	glm::mat4 transformation = perspectiveMatrix * cameraMatrix * modelMatrix;
	glUniformMatrix4fv(glGetUniformLocation(program, "modelViewProjectionMatrix"), 1, GL_FALSE, (float*)&transformation);
	glUniformMatrix4fv(glGetUniformLocation(program, "modelMatrix"), 1, GL_FALSE, (float*)&modelMatrix);

	Core::DrawModel(model);

	glUseProgram(0);
}

void drawObjectTextureNormal(obj::Model * model, glm::mat4 modelMatrix, GLuint texture, GLuint normalMap)
{
	GLuint program = programTextureNormal;

	glUseProgram(program);

	Core::SetActiveTexture(texture, "colorTexture", program, 0);
	Core::SetActiveTexture(normalMap, "normalMap", program, 1);

	glUniform3f(glGetUniformLocation(program, "lightDir"), lightDir.x, lightDir.y, lightDir.z);
	glUniform3f(glGetUniformLocation(program, "cameraPos"), cameraPos.x, cameraPos.y, cameraPos.z);

	glm::mat4 transformation = perspectiveMatrix * cameraMatrix * modelMatrix;
	glm::mat4 ModelViewMatrix = cameraMatrix * modelMatrix;
	glUniformMatrix4fv(glGetUniformLocation(program, "modelViewProjectionMatrix"), 1, GL_FALSE, (float*)&transformation);
	glUniformMatrix4fv(glGetUniformLocation(program, "modelViewMatrix"), 1, GL_FALSE, (float*)&ModelViewMatrix);
	glUniformMatrix4fv(glGetUniformLocation(program, "modelMatrix"), 1, GL_FALSE, (float*)&modelMatrix);

	Core::DrawModelT(model, &tangent[0]);

	glUseProgram(0);
}

void drawObjectTextureEM(obj::Model * model, glm::mat4 modelMatrix, GLuint textureId)
{
	GLuint program = programEM;

	glUseProgram(program);

	glUniform3f(glGetUniformLocation(program, "lightDir"), lightDir.x, lightDir.y, lightDir.z);
	Core::SetActiveTexture(textureId, "textureSampler", program, 0);

	glm::mat4 transformation = perspectiveMatrix * cameraMatrix * modelMatrix;
	glUniformMatrix4fv(glGetUniformLocation(program, "modelViewProjectionMatrix"), 1, GL_FALSE, (float*)&transformation);
	glUniformMatrix4fv(glGetUniformLocation(program, "modelMatrix"), 1, GL_FALSE, (float*)&modelMatrix);


	//reflection
	glUniformMatrix3fv(glGetUniformLocation(program, "cameraPos"), 1, GL_FALSE, (float*)&cameraPos);

	glUniform1i(glGetUniformLocation(program, "cubeSampler"), skybox);
	glActiveTexture(GL_TEXTURE0 + skybox);
	glBindTexture(GL_TEXTURE_CUBE_MAP, skybox);


	Core::DrawModel(model);

	glUseProgram(0);
}

void drawObjectTextureBRDF(obj::Model * model, glm::mat4 modelMatrix, GLuint textureId, GLuint metallic, GLuint roughness)
{
	GLuint program = programBRDF;

	glUseProgram(program);

	glUniform3f(glGetUniformLocation(program, "lightDir"), lightDir.x, lightDir.y, lightDir.z);
	glUniform3f(glGetUniformLocation(program, "cameraPos"), cameraPos.x, cameraPos.y, cameraPos.z);
	
	Core::SetActiveTexture(textureId, "textureRustedIron", program, 0);
	Core::SetActiveTexture(metallic, "textureRustedIronMetallic", program, 1);
	Core::SetActiveTexture(roughness, "textureRustedIronRoughness", program, 2);

	glm::mat4 transformation = perspectiveMatrix * cameraMatrix * modelMatrix;
	glUniformMatrix4fv(glGetUniformLocation(program, "modelViewProjectionMatrix"), 1, GL_FALSE, (float*)&transformation);
	glUniformMatrix4fv(glGetUniformLocation(program, "modelMatrix"), 1, GL_FALSE, (float*)&modelMatrix);

	Core::DrawModel(model);

	glUseProgram(0);
}

void drawSkybox(glm::mat4 modelMatrix, GLuint skyboxId)
{
	//todo
	GLuint program = programSkybox;

	glUseProgram(program);

	glm::mat4 transformation = perspectiveMatrix * cameraMatrix * modelMatrix;
	glUniformMatrix4fv(glGetUniformLocation(program, "modelViewProjectionMatrix"), 1, GL_FALSE, (float*)&transformation);
	glUniformMatrix4fv(glGetUniformLocation(program, "modelMatrix"), 1, GL_FALSE, (float*)&modelMatrix);

	Core::VertexData vertexData;
	vertexData.NumActiveAttribs = 1;
	vertexData.Attribs[0].Pointer = cubeVertices;
	vertexData.Attribs[0].Size = 4;
	vertexData.NumVertices = 36;
	//not sure about these values...

	//binding skybox texture
	glUniform1i(glGetUniformLocation(program, "cubeSampler"), skyboxId);
	glActiveTexture(GL_TEXTURE0 + skyboxId);
	glBindTexture(GL_TEXTURE_CUBE_MAP, skyboxId);

	Core::DrawVertexArray(vertexData);
	glUseProgram(0);
}

class Ryba
{
public:
	Ryba();
	Ryba(obj::Model *model, GLuint *tex);
	void Render();

private:
	obj::Model *model;
	GLuint *tex;
	glm::vec3 position;
	glm::vec3 direction;
	GLfloat random;
	void Move();
	bool trigger;
	GLfloat moveSpeed;
	GLfloat slowSpeed;
	GLfloat scale;
	glm::vec3 temp;
	//float angle;
};

Ryba::Ryba()
{
	position = glm::ballRand(115.0f);
	direction = glm::ballRand(1.0f);
	//std::cout << "xD " + glm::to_string(this->direction) << std::endl;
	random = (std::rand() % 200) * 1.0f + glutGet(GLUT_ELAPSED_TIME) / 1000.0f;
	//this->moveSpeed = 1.0f;
	scale = 1.0f;
}

Ryba::Ryba(obj::Model *mdl, GLuint *txt)
{
	model = mdl;
	tex = txt;
	position = glm::sphericalRand(106.0f);
	random = (std::rand() % 200) * 1.0f + glutGet(GLUT_ELAPSED_TIME) / 1000.0f;
	//glm::vec3 lookat = glm::sphericalRand(1.0f);
	//float dot = position.x*lookat.x + position.z*lookat.z;
	//float det = position.x*lookat.z - position.z*lookat.x;
	//this->angle = atan2(dot, det);
	direction = glm::vec3(cosf(random), 0.0f, sinf(random));
	trigger = false;
	moveSpeed = 1.0f;
	slowSpeed = 0.05f;
	scale = (int)random % 75 / 100.0f + 0.25f;
}

void Ryba::Move()
{
	if (trigger == true) {
		position += direction * moveSpeed;
	}
	else
	{
		position += direction * slowSpeed;
	}
	temp = glm::vec3(position);
	temp.y -= 1.2*sinf(glutGet(GLUT_ELAPSED_TIME) / 500.0f - random);
	if (trigger == false && sqrt((cameraPos.x - temp.x)*(cameraPos.x - temp.x) + (cameraPos.y - temp.y)*(cameraPos.y - temp.y) + (cameraPos.z - temp.z)*(cameraPos.z - temp.z)) < 20.0f)
	{
		//std::cout << "BLISKO" << std::endl;
		direction = glm::normalize(cameraDir + direction);
		//random = acosf(direction.x);
		trigger = true;
	}
	
}

void Ryba::Render()
{
	Move();
	glm::mat4 ModelMatrix = glm::translate(temp + direction * 0.5f + glm::vec3(0, -0.25f, 0)) * glm::rotate(-random + glm::radians(90.0f), glm::vec3(0, 1, 0)) * glm::scale(glm::vec3(scale));
	drawObjectTexture(model, ModelMatrix, *tex);
	if (abs(sqrt((0 - position.x)*(0 - position.x)+ (0 - position.z)*(0 - position.z))) > 120.0f) *this = Ryba(this->model, this->tex);
}

class Wodorost
{
public:
	Wodorost();
	void Render();

private:
	glm::vec3 position;
	GLfloat random;
	glm::vec3 direction;
};

Wodorost::Wodorost()
{
	random = (std::rand() % 200) * 1.0f + glutGet(GLUT_ELAPSED_TIME) / 1000.0f;
	direction = glm::vec3(cosf(random), 0.0f, sinf(random));
	position = glm::ballRand(150.0f);
	position.y = -115;
}

void Wodorost::Render()
{
	drawObjectTexture(&plantModel, glm::translate(position + direction * 0.5f), plantTex);
}

class Bubble
{
public:
	Bubble();
	void Render();

private:
	void Move();
	glm::vec3 position;
	GLfloat speed;
};

Bubble::Bubble()
{
	position = glm::ballRand(150.0f);
	position.y = -116;
	speed = (std::rand() % 50) * 0.01f;
}

void Bubble::Move()
{
	if (position.y > -30) {
		position.y = -116;
	}
	else
	{
		position.y += speed;
	}
}

void Bubble::Render()
{
	Move();
	drawObjectTextureEM(&sphereModel, glm::translate(position) * glm::scale(glm::vec3(speed * 2.0f)), asteroidTex);
	//drawObjectColor(&sphereModel, glm::translate(position) * glm::scale(glm::vec3(0.4f * speed * 2.0f)), glm::vec3(1.0f, 1.0f, 1.0f), 0.3f);
}


class Statue
{
public:
	Statue();
	Statue(glm::vec3 pos, bool g);
	void Render();
	glm::vec3 vecMin;
	glm::vec3 vecMax;

private:
	glm::vec3 position;
	glm::vec3 direction;
	bool gold;
};

Statue::Statue() {
	;
}

Statue::Statue(glm::vec3 pos, bool g)
{
	direction = glm::vec3(1, 0, 1);
	position = pos;
	vecMin = pos + direction * 5 + glm::rotate(direction, glm::radians(-90.0f), glm::vec3(0, 1, 0)) * 8;
	vecMax = pos - direction * 5 + glm::rotate(direction, glm::radians(90.0f), glm::vec3(0, 1, 0)) * 8 + glm::vec3(0, 18, 0);
	gold = g;
}

void Statue::Render()
{
	/*drawObjectColor(&sphereModel, glm::translate(vecMin) * glm::scale(glm::vec3(1)), glm::vec3(1, 0, 0));
	int i = 0;
	glm::vec3 pos = vecMin + glm::vec3(0.1*i, 0, 0);
	while(pos.x < vecMax.x) {
		pos = vecMin + glm::vec3(0.1 * i, 0, 0);
		drawObjectColor(&sphereModel, glm::translate(pos) * glm::scale(glm::vec3(1)), glm::vec3(1, 0, 0));
		i++;
		//std::cout << i << std::endl;
	}
	drawObjectColor(&sphereModel, glm::translate(vecMax) * glm::scale(glm::vec3(1)), glm::vec3(1, 0, 0));
	i = 0;
	pos = vecMax - glm::vec3(0.1*i, 0, 0);
	while (pos.x > vecMin.x) {
		pos = vecMax - glm::vec3(0.1 * i, 0, 0);
		drawObjectColor(&sphereModel, glm::translate(pos) * glm::scale(glm::vec3(1)), glm::vec3(1, 0, 0));
		i++;
		//std::cout << i << std::endl;
	}*/
	if (gold) {
		drawObjectTextureBRDF(&statueModel, glm::translate(position) * glm::rotate(glm::radians(90.0f), glm::vec3(-1, 0, 0)) * glm::scale(glm::vec3(0.1f)), goldTex, goldTexM, goldTexR);
	}
	else {
		drawObjectTexture(&statueModel, glm::translate(position) * glm::rotate(glm::radians(90.0f), glm::vec3(-1, 0, 0)) * glm::scale(glm::vec3(0.1f)), statueTex);
	}
	
}

Ryba fish[ILOSCRYB];
Wodorost plant[ILOSCWODOROSTOW];
Bubble bubble[ILOSCBUBBLE];
Statue statue[ILOSCSTATUL];

void renderScene()
{
	//debug pozycji
	//std::cout << glm::to_string(cameraPos) << std::endl;

	cameraMatrix = createCameraMatrix();
	perspectiveMatrix = Core::createPerspectiveMatrix(1.0f, 10000.0f);

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glClearColor(0.005f, 0.4f, 0.6f, 1.0f);

	//drawSkybox(glm::translate(glm::vec3(0.0f, 0.0f, 0.0f)), skybox);
	drawSkybox(glm::translate(cameraPos) * glm::scale(glm::vec3(50.0f)), skybox);

	//glm::mat4 filtrMatrix = glm::translate(cameraPos + cameraDir * 0.3f) * glm::scale(glm::vec3(3.0f));

	glm::mat4 shipInitialTransformation = glm::translate(glm::vec3(0, -2.25f, -5)) * glm::rotate(glm::radians(180.0f), glm::vec3(0, 1, -0.1)) * glm::scale(glm::vec3(0.25f));
	glm::mat4 shipModelMatrix = glm::translate(cameraPos + cameraDir * 0.5f) * glm::rotate(-cameraAngle, glm::vec3(0, 1, 0)) * shipInitialTransformation;
	//drawObjectColor(&sphereModel, glm::translate(sharkVecMin) * glm::scale(glm::vec3(1)), glm::vec3(1, 0, 0));
	//drawObjectColor(&sphereModel, glm::translate(sharkVecMax) * glm::scale(glm::vec3(1)), glm::vec3(1, 0, 0));
	//glm::mat4 shipModelMatrix = glm::translate(cameraPos + cameraDir * 0.5f + glm::vec3(0,-0.25f,0)) * glm::rotate(-cameraAngle + glm::radians(90.0f), glm::vec3(0,1,0)) * glm::scale(glm::vec3(0.25f));
	//drawObjectColor(&shipModel, shipModelMatrix, glm::vec3(0.6f));
	drawObjectTexture(&sharkModel, shipModelMatrix, sharkTex);
	//drawObjectTexture(&groundModel, glm::translate(glm::vec3(0, -115, 0)), groundTex);
	
	
	//ground texture with normal map
	drawObjectTextureNormal(&groundModel, glm::translate(glm::vec3(0, -115, 0)), groundTex, groundTexN);
	
	
	//drawObjectTexture(&sphereModel, glm::translate(glm::vec3(0.0f, -130, 0.0f)) * glm::scale(glm::vec3(95.0f)), skyTex);

	//drawObjectColor(&sphereModel, glm::translate(glm::vec3(0.0f, -130, 0.0f)) * glm::scale(glm::vec3(40.0f)), glm::vec3(1.0f, 1.0f, 1.0f), 0.3f);
	//drawObjectTextureEM(&sphereModel, glm::translate(glm::vec3(0.0f, -130, 0.0f)) * glm::scale(glm::vec3(40.0f)), asteroidTex);

	drawObjectTextureNormal(&chestModel, glm::translate(glm::vec3(0, -113.5f, 0)), chestTex, chestTexN);

	//drawObjectTextureBRDF(&statueModel, glm::translate(glm::vec3(10, -115, -10)) * glm::rotate(glm::radians(90.0f), glm::vec3(-1, 0, 0)) * glm::scale(glm::vec3(0.1f)), goldTex, goldTexM, goldTexR);
	//drawObjectTexture(&statueModel, glm::translate(glm::vec3(-20, -115, -20)) * glm::rotate(glm::radians(90.0f), glm::vec3(-1, 0, 0)) * glm::scale(glm::vec3(0.1f)), statueTex);

	timeF = glutGet(GLUT_ELAPSED_TIME) / 1000.0f;

	for (int i = 0; i < ILOSCRYB; i++)
	{
		fish[i].Render();
	}

	for (int i = 0; i < ILOSCWODOROSTOW; i++)
	{
		plant[i].Render();
	}

	for (int i = 0; i < ILOSCBUBBLE; i++)
	{
		bubble[i].Render();
	}

	for (int i = 0; i < ILOSCSTATUL; i++)
	{
		statue[i].Render();
	}
	//drawObjectColor(&sphereModel, filtrMatrix, glm::vec3(0.23f, 0.74f, 0.94f), 0.3f);

	glutSwapBuffers();
}

void init()
{
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	for (int i = 0; i < ILOSCRYB; i++)
	{
		Ryba* temporary = new Ryba(&fishModel, &fishTex);
		fish[i] = *temporary;
	}

	for (int i = 0; i < ILOSCWODOROSTOW; i++)
	{
		Wodorost* temporary = new Wodorost();
		plant[i] = *temporary;
	}

	for (int i = 0; i < ILOSCBUBBLE; i++)
	{
		Bubble* temporary = new Bubble();
		bubble[i] = *temporary;
	}

	Statue* stat1 = new Statue(glm::vec3(10, -115, -10), true);
	statue[0] = *stat1;

	Statue* stat2 = new Statue(glm::vec3(-20, -115, -20), false);
	statue[1] = *stat2;


	programColor = shaderLoader.CreateProgram("shaders/shader_color.vert", "shaders/shader_color.frag");
	programTexture = shaderLoader.CreateProgram("shaders/shader_tex.vert", "shaders/shader_tex.frag");
	programTextureNormal = shaderLoader.CreateProgram("shaders/shader_texN.vert", "shaders/shader_texN.frag");
	programSkybox = shaderLoader.CreateProgram("shaders/shader_skybox.vert", "shaders/shader_skybox.frag");
	programEM = shaderLoader.CreateProgram("shaders/shader_texEM.vert", "shaders/shader_texEM.frag");
	programBRDF = shaderLoader.CreateProgram("shaders/shader_tex3.vert", "shaders/shader_tex3.frag");

	groundModel = obj::loadModelFromFile("models/Rockwall.obj");
	shipModel = obj::loadModelFromFile("models/spaceship.obj");
	sharkModel = obj::loadModelFromFile("models/shark.obj");
	sphereModel = obj::loadModelFromFile("models/sphere.obj");
	sharkTex = Core::LoadTexture("textures/shark.png");
	fishModel = obj::loadModelFromFile("models/fish.obj");
	fishTex = Core::LoadTexture("textures/fish.png");
	plantModel = obj::loadModelFromFile("models/plant.obj");
	plantTex = Core::LoadTexture("textures/plant.png");
	asteroidTex = Core::LoadTexture("textures/asteroid.png");
	skyTex = Core::LoadTexture("textures/Skybox.png");
	
	statueTex = Core::LoadTexture("textures/statue.png");
	statueTexR = Core::LoadTexture("textures/statue_roughness.png");
	statueTexM = Core::LoadTexture("textures/statue_metallic.png");
	statueModel = obj::loadModelFromFile("models/statue.obj");

	goldTex = Core::LoadTexture("textures/gold/albedo.png");
	goldTexM = Core::LoadTexture("textures/gold/metallic.png");
	goldTexR = Core::LoadTexture("textures/gold/roughness.png");

	chestModel = obj::loadModelFromFile("models/chest.obj");
	chestTex = Core::LoadTexture("textures/chest.png");
	chestTexN = Core::LoadTexture("textures/chest_normal.png");

	skybox = Core::setupCubeMap("textures/xpos.png", "textures/xneg.png", "textures/ypos.png", "textures/yneg.png", "textures/zpos.png", "textures/zneg.png");

	groundTex = Core::LoadTexture("textures/sand.png");
	groundTexN = Core::LoadTexture("textures/sand_normal.png");


	glm::vec3 uY = { 0.0, -1.0, 0.0 };
	glm::vec3 uX = { -1.0, 0.0, 0.0 };

	glm::vec3 tang[31956];
	
	int size = groundModel.normal.size();
	std::cout << std::to_string(size) << std::endl;
	for (int i = 0; i < size; i += 3)
	{

		glm::vec3 normal = { groundModel.normal[i + 0], groundModel.normal[i + 1], groundModel.normal[i + 2] };

		if (groundModel.normal[i + 1] < 0.99 && groundModel.normal[i + 1] > -0.99) tang[i] = glm::normalize(glm::cross(normal, uY));
		else tang[i] = glm::normalize(glm::cross(normal, uX));



		tangent[i + 0] = tang[i].x;
		tangent[i + 1] = tang[i].y;
		tangent[i + 2] = tang[i].z;

		//std::cout << i << std::endl;

	}
	calcSharkPos();
}

void shutdown()
{
	shaderLoader.DeleteProgram(programColor);
	shaderLoader.DeleteProgram(programTexture);
}

void idle()
{
	glutPostRedisplay();
}

bool pointAABB(glm::vec3 pos, glm::vec3 b1, glm::vec3 b2) {
	/*std::cout << "=======" << std::endl;
	std::cout << glm::to_string(a1) << std::endl;
	std::cout << glm::to_string(a2) << std::endl;
	
	if (a2.x < b1.x || a1.x > b2.x) return false;
	if (a2.y < b1.y || a1.y > b2.y) return false;
	if (a2.z < b1.z || a1.z > b2.z) return false;

	return true;*/
	std::cout << glm::to_string(b1) << std::endl;
	std::cout << glm::to_string(b2) << std::endl;
	if (pos.x > b1.x && pos.x < b2.x && pos.y >b1.y && pos.y < b2.y && pos.z < b1.z && pos.z > b2.z) {
		return true;
	}
	return false;
	/*return(a2.x > b1.x &&
		a1.x < b2.x &&
		a2.y > b1.y &&
		a1.y < b2.y &&
		a2.z > b1.z &&
		a1.z < b2.z);*/
}

bool isCollision() {
	for (int i = 0; i < ILOSCSTATUL; i++) {
		if (pointAABB(sharkPos, statue[i].vecMin, statue[i].vecMax)) {
			std::cout << "kolizja" << std::endl;
			return true;
		}
	}
	return false;
}

void keyboard(unsigned char key, int x, int y)
{
	float angleSpeed = 0.1f;
	float angleSpeed1 = 0.5f;
	float moveSpeed = 0.7f;
	switch (key)
	{
	case 'z':
		cameraAngle -= angleSpeed;
		calcSharkPos();
		if (isCollision()) {
			cameraAngle += angleSpeed;
		}
		break;
	case 'x':
		cameraAngle += angleSpeed;
		calcSharkPos();
		if (isCollision()) {
			cameraAngle -= angleSpeed;
		}
		break;
	case 'q':
		cameraPos -= cameraVer * moveSpeed;
		calcSharkPos();
		if (isCollision()) {
			cameraPos += angleSpeed;
		}
		break;
	case 'e':
		if (cameraPos.y > -111) {
			cameraPos += cameraVer * moveSpeed;
			calcSharkPos();
			if (isCollision()) {
				cameraPos -= cameraVer * moveSpeed;
			}
		}
		break;
	case 'w':
		cameraPos += cameraDir * moveSpeed;
		calcSharkPos();
		if (isCollision()) {
			cameraPos -= cameraDir * moveSpeed;
		}
		break;
	case 's':
		cameraPos -= cameraDir * moveSpeed;
		calcSharkPos();
		if (isCollision()) {
			cameraPos += cameraDir * moveSpeed;
		}
		break;
	case 'd':
		cameraPos += cameraSide * moveSpeed;
		calcSharkPos();
		if (isCollision()) {
			cameraPos -= cameraSide * moveSpeed;
		}
		break;
	case 'a':
		cameraPos -= cameraSide * moveSpeed;
		calcSharkPos();
		if (isCollision()) {
			cameraPos += cameraSide * moveSpeed;
		}
		break;
	}
	calcSharkPos();
}

int main(int argc, char ** argv)
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGBA);
	glutInitWindowPosition(200, 200);
	glutInitWindowSize(1024, 800);
	glutCreateWindow("CGP");
	glewInit();

	init();
	glutKeyboardFunc(keyboard);
	glutDisplayFunc(renderScene);
	glutIdleFunc(idle);

	glutMainLoop();

	shutdown();

	return 0;
}