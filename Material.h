#pragma once
#include <iostream>
#include <GL/glut.h>
#include <random>

class Material
{
public:
	Material() {}
	//Material(const Material& m) {
	//	emission = new float[4];
	//	ambient = new float[4];
	//	diffuse = new float[4];
	//	specular = new float[4];
	//	shininess = new float[1];
	//	for (int i = 0; i < 4;i++) {
	//		emission[i] = m.getEmission()[i];
	//		ambient[i] = m.getAmbient()[i];
	//		diffuse[i] = m.getDiffuse()[i];
	//		specular[i] = m.getSpecular()[i];
	//	}
	//	shininess[0] = m.getShininess()[0];
	//};
	//void delMaterial(){
	//delete[] emission;
	//delete[] ambient;
	//delete[] diffuse;
	//delete[] specular;
	//delete[] shininess;
	//}
	void setEmission(float r, float g, float b, float a);
	void setAmbient(float r, float g, float b, float a);
	void setDiffuse(float r, float g, float b, float a);
	void setSpecular(float r, float g, float b, float a);
	void setShininess(float sh);
	const float* getEmission() const;
	const float* getAmbient() const;
	const float* getDiffuse() const;
	const float* getSpecular() const;
	const float* getShininess() const;
	//float operator [](const int i) const;

private:
	float emission[4];
	float ambient[4];
	float diffuse[4];
	float specular[4];
	float shininess[1];

};

