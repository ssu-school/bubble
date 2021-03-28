#pragma once
#include <iostream>
class Vector3f
{
public:
	Vector3f();
	Vector3f(float x, float y, float z);
	Vector3f(const Vector3f& v) {
		for (int i = 0; i < 3; i++) pos[i] = v.pos[i];
		setPos(v.pos[0], v.pos[1], v.pos[2]);
	}
	//~Vector3f();
	void setPos(float x, float y, float z);
	/*Vector3f& operator= (const Vector3f& s) {
		for (int i = 0; i < 3; i++) pos[i] = s.pos[i];
		return *this;
	}*/
	float getX() const;
	float getY() const;
	float getZ() const;

	float& operator[](const int i);
	float operator[](const int i) const;
	
private:
	float pos[3];
};

Vector3f operator+(const Vector3f& v1, const Vector3f& v2);
Vector3f operator- (const Vector3f& v1, const Vector3f& v2);
Vector3f operator*(float k, const Vector3f& v1);
float dotProduct(const Vector3f& v1, const Vector3f& v2);