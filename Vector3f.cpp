#include "Vector3f.h"

Vector3f::Vector3f() {
	//pos = new float[3];
	setPos(0, 0, 0);
}
Vector3f::Vector3f(float x, float y, float z) {
	//pos = new float[3];
	setPos(x, y, z);
};

void Vector3f::setPos(float x, float y, float z) {
	pos[0] = x;
	pos[1] = y;
	pos[2] = z;
};

float Vector3f::getX() const {
	return pos[0];
};
float Vector3f::getY() const {
	return pos[1];
};
float Vector3f::getZ() const {
	return pos[2];
};

float& Vector3f::operator[](const int i) {
	return pos[i];
};
float Vector3f::operator[](const int i) const {
	return pos[i];
};

Vector3f operator+(const Vector3f& v1, const Vector3f& v2) {
    Vector3f v3(v1.getX()+v2.getX(), v1.getY() + v2.getY(), v1.getZ() + v2.getZ());
	return v3;
};

float dotProduct(const Vector3f& v1, const Vector3f& v2) {
	return v1[0] * v2[0] + v1[1] * v2[1] + v1[2] * v2[2];
}
Vector3f operator-(const Vector3f& v1, const Vector3f& v2) {
	Vector3f v3(v1[0] - v2[0], v1[1] - v2[1], v1[2] - v2[2]);
	return v3;
};

Vector3f operator*(float k,const Vector3f& v1) {
	Vector3f v3(v1[0] * k, v1[1] * k, v1[2] * k);
	return v3;
};
		