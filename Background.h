#pragma once
#include <string>
#include <GL/glut.h>
#include <chrono>
#include "Vector3f.h"


using namespace std;
class Background{
public: 
	Background(int a);
	//~Background();
	void reduceAng();
	void increaseAng();
	float  getAngle() const;
	void drawArrow(int height);
	//void setStartTime(std::chrono::system_clock::time_point t);
	void drawBar(int a, int b, int c);
	void drawTimer(int a, int b);
	//std::chrono::system_clock::time_point getStartTime();
	void setTimer(float t);
	//void setPlayTime(string t1,string t2);
	//const string getPlayTime1() const ;
	void addScore(int a);
	//string getScore() const;
	int getIntScore() const;
	int getLife() const;
	void minusLife();
	void drawObstacle();
	//const string getPlayTime2() const;
	void setScore();
	void setLife();
private:
	float angle;
	//std::chrono::system_clock::time_point startTime;
	float timer;
	//string playTime1;
	//string playTime2;
	int score;
	bool shoot;
	int life;
};

