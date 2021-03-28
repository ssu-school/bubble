#include "Background.h"
#include <chrono>
Background::Background(int a) {
	angle = a;
	//startTime = std::chrono::system_clock::now();
	//setPlayTime("0", "0");
	timer = 120;
	shoot = false;
	score = 0;
	life = 3;
}

void Background::reduceAng() {
	angle-=10;
}
void Background::increaseAng() {
	angle += 10;
}

float Background::getAngle() const {
	return angle;
}
//void Background::setStartTime(std::chrono::system_clock::time_point t) {
//	startTime = t;
//}
void Background::setScore() {
	score = 0;
}
//const string Background::getPlayTime1()const {
//	return playTime1;
//}
//const string Background::getPlayTime2()const {
//	return playTime2;
//}
//std::chrono::system_clock::time_point Background::getStartTime() {
//	return startTime;
//};
void Background::setTimer(float t) {
	timer = t;
} 
int Background::getLife() const {
	return life;
};
void Background::minusLife() {
	life--;
};
void Background::setLife() {
	life = 3;
}

void Background::drawObstacle() {
	glPushMatrix();
	glBegin(GL_QUADS);
	glColor3f(0, 0, 0);
	glVertex2f(30, 30);
	glVertex2f(30, -30);
	glVertex2f(-30, -30);
	glVertex2f(-30, 30);
	glEnd();
	glPopMatrix();
};


void Background::drawArrow(int height) {
	glPushMatrix();
	glTranslatef(0, height, 0);
	glRotatef(angle, 0,0,1);// rotate이 미리 되어야 한다. 
	glBegin(GL_TRIANGLES);
	glColor3f(1, 1,0.7 );
	glVertex2f(0, 50);
	glVertex2f(-8,30 );
	glVertex2f(8,30 );//
	glVertex2f(-4, 30);
	glVertex2f( 4, 30);
	glVertex2f(-4, -20);//
	glVertex2f(-4, -20);
	glVertex2f(4, 30);
	glVertex2f(4, -20);
	glEnd();
	glPopMatrix();
};
//void Background::setPlayTime(string t1, string t2) {
//	playTime1 = t1; playTime2 = t2;
//}

void Background::addScore(int a) {
	score += a;
};


int Background::getIntScore() const {
	return  score;
};

void Background::drawTimer(int a, int b) {
	glPushMatrix();
	glTranslatef(a, b, 0);
	glBegin(GL_QUADS);
	glColor3f(1, 0, 0);
	glVertex2f(timer,13);
	glVertex2f(timer,0);
	glVertex2f(0,0);
	glVertex2f(0,13);
	glEnd();
	glPopMatrix();
}

void Background::drawBar(int a, int b,int c) {
	glPushMatrix();
	glBegin(GL_QUADS);
	glColor3f(1, 1, 1);
	glVertex2f(b, 10+c);
	glVertex2f(b, 0+c);
	glVertex2f(a, 0+c);
	glVertex2f(a, 10+c);
	glEnd();
	glPopMatrix();
}


