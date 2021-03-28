#pragma once

#include "Material.h"
#include "Vector3f.h"

class SolidSphere
{
public:
	SolidSphere(float r, int sl, int st);
	~SolidSphere() {
		//delete [;
		//std::cout << "solidSphere destructor" << std::endl;
	}
	//SolidSphere(const SolidSphere& sph) {
	//	center=Vector3f();
	//	velocity = Vector3f();
	//	center.setPos(sph.center.getX(), sph.center.getY(), sph.center.getZ());
	//	velocity.setPos(sph.velocity.getX(), sph.velocity.getY(), sph.velocity.getZ());
	//	mtl.setEmission(sph.getMtl().getEmission()[0], sph.getMtl().getEmission()[1],
	//		sph.getMtl().getEmission()[2], sph.getMtl().getEmission()[3]);
	//	mtl.setAmbient(sph.getMtl().getAmbient()[0], sph.getMtl().getAmbient()[1],
	//		sph.getMtl().getAmbient()[2], sph.getMtl().getAmbient()[3]);
	//	mtl.setDiffuse(0.5, 0.5, 0.5, 1);
	//	mtl.setSpecular(1.0, 1.0, 1.0, 1);
	//	mtl.setShininess(10);
	//	setColorNum(sph.getColorNum());
	//	setDelete(sph.getDelete());
	//	setDropCheck(sph.getDropCheck());
	//	setDrop(sph.getDrop());
	//	setRadius(sph.getRadius());
	//	setSlice(sph.getSlice());
	//	setStack(sph.getStack());
	//	setChecked(sph.getChecked());
	//	shooted = sph.getShooted();
	//}

	void setRadius(float r);//
	float getRadius() const;
	void setSlice(int sl);//
	void setStack(int st);//
	bool collisionDetection(const SolidSphere& sph);//
	void collisionHandling(SolidSphere& sph);//

	void setColorNum(int a);//
	void setDelete(bool s);//
	void setChecked(bool b);
	void setDropCheck(bool b);
	void setDrop(bool a);
	void setShooted();

	void draw() const;

	bool getDelete() const ;
	bool getChecked() const;
	int getStack() const;
	int getSlice() const;
	bool getDropCheck() const;
	int getColorNum() const;
	bool getDrop() const ;
	bool getShooted() const;



	void setCenter_p(float a, float b, float c);




	Material getMtl() const;





	void setCenter(const Vector3f& c);//
	void setVelocity(const Vector3f& v);//
	Vector3f getVelocity() const;
	Vector3f getCenter() const;
	void setMTL(const Material& m);//
	void move();

private:
	float radius;
	int slice;
	int stack;
	bool checkedSphere1;
	bool checkedSphere2;
	bool dropSphere;
	bool deleteSphere;
	bool shooted;
	int colorNum;
	Vector3f center;
	Vector3f velocity;
	Material mtl;	
};

