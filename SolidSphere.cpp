#include "SolidSphere.h"
#include <math.h>

SolidSphere::SolidSphere(float r, int sl, int st) {
	center.setPos(0, 0, 0);
	velocity.setPos(0, 0, 0);

	mtl.setEmission(0.1, 0.1, 0.1, 1);
	mtl.setAmbient(0.3, 0.3, 0.3, 1);
	mtl.setDiffuse(0.5, 0.5, 0.5, 1);
	mtl.setSpecular(1.0, 1.0, 1.0, 1);
	mtl.setShininess(10);
	setDelete(false);
	setDropCheck(false);
	setDrop(true);
	setRadius(r);
	setSlice(sl);
	setStack(st);
	shooted = false;
}

void SolidSphere::setRadius(float r) {
	radius= r;
}
float SolidSphere::getRadius() const {
	return radius;
};
void SolidSphere::setSlice(int sl) {
	slice = sl;
};
int SolidSphere::getSlice() const {
	return slice;
};
void SolidSphere::setStack(int st) {
	stack = st;
};
int SolidSphere::getStack() const {
	return stack;
};

void SolidSphere::setCenter(const Vector3f& c) {
	center.setPos(c.getX(),c.getY(),c.getZ());
};

void SolidSphere::setCenter_p(float a, float b, float c) {
	center.setPos(a, b, c);
}
Vector3f SolidSphere::getCenter() const {
	return center;
};
void SolidSphere::setVelocity(const Vector3f& v) {
	velocity.setPos(v.getX(), v.getY(), v.getZ());
};
Vector3f SolidSphere::getVelocity() const {
	return velocity;
};
void SolidSphere::move() {
	setCenter(center + velocity);
};

void SolidSphere::setChecked(bool b) {
	checkedSphere1 = b;
}

bool SolidSphere::getChecked() const{
	return checkedSphere1;
}


void SolidSphere::setColorNum(int a) {
	colorNum = a;
}
int SolidSphere::getColorNum() const {
	return colorNum;
}

void SolidSphere::setDropCheck(bool b) {
	checkedSphere2 = b;
};
bool SolidSphere::getDropCheck() const {
	return checkedSphere2;
};

void  SolidSphere::setDelete(bool s) {
	deleteSphere = s;
};
bool  SolidSphere::getDelete() const {
	return deleteSphere;
};

void SolidSphere::setDrop(bool a) {
	dropSphere = a;
}

bool SolidSphere::getDrop() const {
	return dropSphere;
}

bool SolidSphere::getShooted() const {
	return shooted;
};
void SolidSphere::setShooted() {
	shooted = true;
};

void SolidSphere::setMTL(const Material& m) {
	mtl.setAmbient(m.getAmbient()[0], m.getAmbient()[1], m.getAmbient()[2], m.getAmbient()[3]) ;
	mtl.setDiffuse(m.getDiffuse()[0], m.getDiffuse()[1], m.getDiffuse()[2], m.getDiffuse()[3]) ;
	mtl.setEmission(m.getEmission()[0], m.getEmission()[1], m.getEmission()[2], m.getEmission()[3]) ;
    mtl.setSpecular(m.getSpecular()[0], m.getSpecular()[1], m.getSpecular()[2], m.getSpecular()[3]);
    mtl.setShininess(m.getShininess()[0]) ;
};

bool SolidSphere::collisionDetection(const SolidSphere& sph) {
	return (dotProduct(sph.getCenter() - center, sph.getCenter() - center) <= (sph.radius * 2) * (sph.radius * 2));
};
void SolidSphere::collisionHandling(SolidSphere& sph) {
	if (collisionDetection(sph)) {
		Vector3f k(0, 0, 0);
		setVelocity(k);
		if (getCenter()[0] >= sph.getCenter()[0] && getCenter()[1] < sph.getCenter()[1]-sph.getRadius()/2) {
			Vector3f n(sph.getCenter()[0] + sph.getRadius(), sph.getCenter()[1] - sph.getRadius() * sqrt(3), sph.getCenter()[2]);
			setCenter(n);
		}
		else if (getCenter()[0] < sph.getCenter()[0] && getCenter()[1] < sph.getCenter()[1] - sph.getRadius() / 2) {
			Vector3f n(sph.getCenter()[0] - sph.getRadius(), sph.getCenter()[1] - sph.getRadius() * sqrt(3), sph.getCenter()[2]);
			setCenter(n);
		}
		else if (getCenter()[0] > sph.getCenter()[0] && getCenter()[1] > sph.getCenter()[1] - sph.getRadius() / 2 && getCenter()[1] < sph.getCenter()[1] + sph.getRadius() / 2) {
			Vector3f n(sph.getCenter()[0] + 2 * sph.getRadius(), sph.getCenter()[1] , sph.getCenter()[2]);
			setCenter(n);
		}
		else if (getCenter()[0] < sph.getCenter()[0] && getCenter()[1] > sph.getCenter()[1] - sph.getRadius() / 2 && getCenter()[1] < sph.getCenter()[1] + sph.getRadius() / 2) {
			Vector3f n(sph.getCenter()[0] - 2 * sph.getRadius(), sph.getCenter()[1], sph.getCenter()[2]);
			setCenter(n);
		}
		else if (getCenter()[0] > sph.getCenter()[0] && getCenter()[1] > sph.getCenter()[1] + sph.getRadius() / 2) {
			Vector3f n(sph.getCenter()[0] + sph.getRadius(), sph.getCenter()[1] + sph.getRadius() * sqrt(3), sph.getCenter()[2]);
			setCenter(n);
		}
		else if (getCenter()[0] < sph.getCenter()[0] && getCenter()[1] > sph.getCenter()[1] + sph.getRadius() / 2) {
			Vector3f n(sph.getCenter()[0] - sph.getRadius(), sph.getCenter()[1] + sph.getRadius() * sqrt(3), sph.getCenter()[2]);
			setCenter(n);
		}
	}
};

Material SolidSphere::getMtl() const {
	return mtl;
}

void SolidSphere::draw() const {
	glPushMatrix();
	float mat_specular[] = {mtl.getSpecular()[0],mtl.getSpecular()[1],mtl.getSpecular()[2],mtl.getSpecular()[3]};
	float mat_shininess[] = {mtl.getShininess()[0]};
	float mat_emission[] = { mtl.getEmission()[0],mtl.getEmission()[1] ,mtl.getEmission()[2] ,mtl.getEmission()[3] };
	float mat_diffuse[] = { mtl.getDiffuse()[0],mtl.getDiffuse()[1] ,mtl.getDiffuse()[2] ,mtl.getDiffuse()[3] };
	float mat_ambient[] = { mtl.getAmbient()[0],mtl.getAmbient()[1] ,mtl.getAmbient()[2] ,mtl.getAmbient()[3] };
	glShadeModel(GL_SMOOTH);
	glMaterialfv(GL_FRONT, GL_EMISSION, mat_emission);
	glMaterialfv(GL_FRONT, GL_AMBIENT, mat_ambient);
	glMaterialfv(GL_FRONT, GL_DIFFUSE, mat_diffuse);
	glMaterialfv(GL_FRONT, GL_SPECULAR, mat_specular);
	glMaterialfv(GL_FRONT, GL_SHININESS, mat_shininess);
	glTranslatef(center[0], center[1], center[2]);
	glutSolidSphere(getRadius(),getSlice(),getStack());
	glPopMatrix();
};

