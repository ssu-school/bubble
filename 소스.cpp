#define _CRTDBG_MAP_ALLOC
#include <stdlib.h>
#include <crtdbg.h>
#include <cstdlib>
#include "SolidSphere.h"
#include <vector>
#include "Light.h"
#include <fmod.h> //file에 setup 한 놈은 <>로.
#include "Background.h"
#include <FreeImage.h>
#include <cmath>
#include <chrono>
#include <ctime>
#include <time.h>

using namespace std;

#define WIDTH 400
#define HEIGHT 600
#define PI 3.141592
#define RADIUS 25
#define boundaryX WIDTH/2
#define boundaryY HEIGHT/2
#define DONE 1
#define NOTYET 0
#define UNDERBAR 80
vector<SolidSphere> spheres;       /*next 구체와 현재의 구체*/
vector<SolidSphere> boardSpheres;  /*보드판에 그려지는 구체들*/
vector<Material> materials;
vector<SolidSphere> deleteSpheres;
vector<SolidSphere> fallSpheres;
vector<SolidSphere> obstacle;
vector<SolidSphere> tem;

Light light(boundaryX, boundaryY, boundaryX / 2, GL_LIGHT0);
Background b1(0);

enum DRAWMODE { FIRST, SECOND, THIRD, ALL };

FMOD_SYSTEM* g_FMODSystem; 
FMOD_SOUND* pFMOD_SOUND;
FMOD_SOUND* effectSound;
FMOD_SOUND* level3_SOUND;
FMOD_CHANNEL* channel;
FMOD_CHANNEL* channel2;
FMOD_CHANNEL* channel3;

random_device rd;
mt19937 gen(rd());

GLubyte* textureData, *textureData2, *textureData3, *textureData4, *textureData5, *textureData6,* textureData7;
static GLuint textureID,textureID4,textureID3,textureID2, textureID5, textureID6, textureID7;
int imageWidth, imageHeight,imageWidth2,imageHeight2,imageWidth3, imageHeight3,imageWidth4,imageHeight4,imageWidth5,imageHeight5;
bool shoot1 = false;
bool shoot2 = false;
bool arrive1 = false;
bool arrive2 = false;
bool gameOver = FALSE;
bool newgame = false;
bool programEnd = false;
clock_t start_fps = clock();
clock_t end_fps;
 /*texture*/
FIBITMAP* createBitMap(char const* filename) {
	FREE_IMAGE_FORMAT format = FreeImage_GetFileType(filename, 0);

	if (format == -1) {
		cout << "Could not find image: " << filename << " - Aborting." << endl;
		exit(-1);
	}

	if (format == FIF_UNKNOWN) {
		cout << "Couldn't determine file format - attempting to get from file extension..." << endl;
		format = FreeImage_GetFIFFromFilename(filename);

		if (!FreeImage_FIFSupportsReading(format)) {
			cout << "Detected image format cannot be read!" << endl;
			exit(-1);
		}
	}

	FIBITMAP* bitmap = FreeImage_Load(format, filename);

	int bitsPerPixel = FreeImage_GetBPP(bitmap);

	FIBITMAP* bitmap32;
	if (bitsPerPixel == 32) {
		cout << "Source image has " << bitsPerPixel << " bits per pixel. Skipping conversion." << endl;
		bitmap32 = bitmap;
	}
	else {
		cout << "Source image has " << bitsPerPixel << " bits per pixel. Converting to 32-bit colour." << endl;
		bitmap32 = FreeImage_ConvertTo32Bits(bitmap);
	}

	return bitmap32;
}

void generateTexture() {
	glGenTextures(1, &textureID);
	glBindTexture(GL_TEXTURE_2D, textureID);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, imageWidth, imageHeight, 0, GL_BGRA_EXT, GL_UNSIGNED_BYTE, textureData);
	glGenTextures(1, &textureID3);
	glBindTexture(GL_TEXTURE_2D, textureID3);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, imageWidth3, imageHeight3, 0, GL_BGRA_EXT, GL_UNSIGNED_BYTE, textureData3);
	glGenTextures(1, &textureID4);
	glBindTexture(GL_TEXTURE_2D, textureID4);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, imageWidth4, imageHeight4, 0, GL_BGRA_EXT, GL_UNSIGNED_BYTE, textureData4);
	glBindTexture(GL_TEXTURE_2D, textureID5);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, imageWidth5, imageHeight5, 0, GL_BGRA_EXT, GL_UNSIGNED_BYTE, textureData5);
}


void initTexture() {
	FIBITMAP* bitmap32 = createBitMap("./forest_black.JPG");
	FIBITMAP* bitmap32_3 = createBitMap("./forest_black_2.JPG");
	FIBITMAP* bitmap32_4 = createBitMap("./gameover.JPG");
	FIBITMAP* bitmap32_5 = createBitMap("./heart.PNG");

	imageWidth = FreeImage_GetWidth(bitmap32);
	imageHeight = FreeImage_GetHeight(bitmap32);
	textureData = FreeImage_GetBits(bitmap32);
	imageWidth3 = FreeImage_GetWidth(bitmap32_3);
	imageHeight3 = FreeImage_GetHeight(bitmap32_3);
	textureData3 = FreeImage_GetBits(bitmap32_3);
	imageWidth4 = FreeImage_GetWidth(bitmap32_4);
	imageHeight4 = FreeImage_GetHeight(bitmap32_4);
	textureData4 = FreeImage_GetBits(bitmap32_4);
	imageWidth5 = FreeImage_GetWidth(bitmap32_5);
	imageHeight5 = FreeImage_GetHeight(bitmap32_5);
	textureData5 = FreeImage_GetBits(bitmap32_5);
	generateTexture();
}
void draw_characters(void* font, const char* c, float x, float y) {
	glColor3f(1.0, 1.0, 1.0);
	glRasterPos2f(x, y);
	for (int i = 0; i < strlen(c);i++)
		glutBitmapCharacter(font, c[i]);
}

void generateTexture2() {
	glGenTextures(1, &textureID2);
	glBindTexture(GL_TEXTURE_2D, textureID2);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, imageWidth2, imageHeight2, 0, GL_BGRA_EXT, GL_UNSIGNED_BYTE, textureData2);
}


void initTexture2() {
	FIBITMAP* bitmap32 = createBitMap("./background.JPG");
	imageWidth2 = FreeImage_GetWidth(bitmap32);
	imageHeight2 = FreeImage_GetHeight(bitmap32);
	textureData2 = FreeImage_GetBits(bitmap32);
	generateTexture2();
}

void drawSquareWithTexture() {
	glPushMatrix();
	glEnable(GL_TEXTURE_2D);
	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);
	glBindTexture(GL_TEXTURE_2D, textureID);
	glBegin(GL_QUADS);
	glTexCoord2f(0, 0);glVertex3f(28, -boundaryY, 0.0);
	glTexCoord2f(0, 1);glVertex3f(28, -boundaryY+ UNDERBAR-5, 0.0);
	glTexCoord2f(1, 1);glVertex3f(95, -boundaryY+ UNDERBAR-5, 0.0);
	glTexCoord2f(1, 0);glVertex3f(95, -boundaryY, 0.0);
	glEnd();
	glDisable(GL_TEXTURE_2D);
	glPopMatrix();
	glPushMatrix();
	glEnable(GL_TEXTURE_2D);
	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);
	glBindTexture(GL_TEXTURE_2D, textureID3);
	glBegin(GL_QUADS);
	glTexCoord2f(0, 0);glVertex3f(-95, -boundaryY, 0.0);
	glTexCoord2f(0, 1);glVertex3f(-95, -boundaryY + UNDERBAR - 5, 0.0);
	glTexCoord2f(1, 1);glVertex3f(-28, -boundaryY + UNDERBAR - 5, 0.0);
	glTexCoord2f(1, 0);glVertex3f(-28, -boundaryY, 0.0);
	glEnd();
	glDisable(GL_TEXTURE_2D);
	glPopMatrix();
	
	glPushMatrix();
	if (b1.getLife()>0) {
	glEnable(GL_TEXTURE_2D);
	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);
	glBindTexture(GL_TEXTURE_2D, textureID5);
		glBegin(GL_QUADS);
		glTexCoord2f(1,1);glVertex3f(boundaryX - 100, boundaryY -10, 0.0);
		glTexCoord2f(1,0);glVertex3f(boundaryX - 100, boundaryY -30, 0.0);
		glTexCoord2f(0,0);glVertex3f(boundaryX - 80, boundaryY -30, 0.0);
		glTexCoord2f(0,1);glVertex3f(boundaryX - 80, boundaryY -10, 0.0);
		glEnd();
		glDisable(GL_TEXTURE_2D);
		glPopMatrix();
	}
	if (b1.getLife() > 1) {
		glEnable(GL_TEXTURE_2D);
		glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);
		glBindTexture(GL_TEXTURE_2D, textureID5);
		glBegin(GL_QUADS);
		glTexCoord2f(1,1);glVertex3f(boundaryX - 75, boundaryY -10, 0.0);
		glTexCoord2f(1,0);glVertex3f(boundaryX - 75, boundaryY -30, 0.0);
		glTexCoord2f(0,0);glVertex3f(boundaryX - 55, boundaryY -30, 0.0);
		glTexCoord2f(0,1);glVertex3f(boundaryX - 55, boundaryY -10, 0.0);
		glEnd();
		glDisable(GL_TEXTURE_2D);
		glPopMatrix();
	}
	if (b1.getLife() > 2) {
		glEnable(GL_TEXTURE_2D);
		glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);
		glBindTexture(GL_TEXTURE_2D, textureID5);
		glBegin(GL_QUADS);
		glTexCoord2f(1, 1);glVertex3f(boundaryX - 50, boundaryY -10, 0.0);
		glTexCoord2f(1, 0);glVertex3f(boundaryX - 50, boundaryY -30, 0.0);
		glTexCoord2f(0, 0);glVertex3f(boundaryX - 30, boundaryY -30, 0.0);
		glTexCoord2f(0, 1);glVertex3f(boundaryX - 30, boundaryY -10, 0.0);
		glEnd();												  
		glDisable(GL_TEXTURE_2D);
		glPopMatrix();
	}
}
void drawSquareWithTexture2() {
	glPushMatrix();
	glEnable(GL_TEXTURE_2D);
	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);
	glBindTexture(GL_TEXTURE_2D, textureID2);
	glBegin(GL_QUADS);
	glTexCoord2f(1, 1);glVertex3f(boundaryX, +boundaryY - 70, 0.0);
	glTexCoord2f(1, 0);glVertex3f(boundaryX, -boundaryY + UNDERBAR, 0.0);
	glTexCoord2f(0, 0);glVertex3f(-boundaryX, -boundaryY + UNDERBAR, 0.0);
	glTexCoord2f(0, 1);glVertex3f(-boundaryX, +boundaryY - 70, 0.0);
	glEnd();
	glDisable(GL_TEXTURE_2D);
	glPopMatrix();
	
}

void drawGameOver() {
	if (b1.getLife() <= 0) {
		glPushMatrix();
		glEnable(GL_TEXTURE_2D);
		glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);
		glBindTexture(GL_TEXTURE_2D, textureID4);
		glBegin(GL_QUADS);
		glTexCoord2f(1, 0);glVertex3f(boundaryX, -boundaryY, 0.0);
		glTexCoord2f(1, 1);glVertex3f(boundaryX, boundaryY, 0.0);
		glTexCoord2f(0, 1);glVertex3f(-boundaryX, boundaryY, 0.0);
		glTexCoord2f(0, 0);glVertex3f(-boundaryX, -boundaryY, 0.0);
		glEnd();
		glDisable(GL_TEXTURE_2D);
		glPopMatrix();
		boardSpheres.clear();
		b1.setScore();
	}
}

class sphereFun {
public:
	sphereFun() {};
	bool operator()(const SolidSphere& k){
		if (k.getDelete() == true) return true;
		else return false;
	}
};

class sphereFun2 {
public:
	sphereFun2() {};
	bool operator()(const SolidSphere& k) {
		if (k.getDrop() == true) return true;
		else return false;
	}
};

	vector<SolidSphere> temp;
void spherDelete(){
	temp.clear();
	if (deleteSpheres.size() >= 2) {
		boardSpheres.back().setDelete(TRUE);
		deleteSpheres.push_back(boardSpheres.back());
		b1.addScore(deleteSpheres.size());
		for(auto &i : boardSpheres){
			if (i.getDelete() != true) temp.push_back(i);
		}
		temp.swap(boardSpheres);
		temp.clear();
	}
		deleteSpheres.clear();
}

void colorCheck(SolidSphere& sphere, const SolidSphere& sphere2) {
	sphere.setChecked(TRUE);
	for (auto& i : boardSpheres) {
		if (i.getChecked() != TRUE && i.getColorNum() == sphere2.getColorNum() && sphere.collisionDetection(i)) {
			i.setDelete(true);
			deleteSpheres.push_back(i);
			colorCheck(i,sphere2);
		}
		else if (i.getChecked() != TRUE && sphere.collisionDetection(i)) { 
			i.setChecked(TRUE);
		};
	}
	boardSpheres.back().setDelete(TRUE);
}

void fallCheck(SolidSphere& sphere) {
	if (sphere.getCenter()[1] == boundaryY - 70 - RADIUS && sphere.getDropCheck() != DONE) {
		sphere.setDrop(false);
	}
	sphere.setDropCheck(TRUE);
	for (auto& i : boardSpheres) {
		if (sphere.collisionDetection(i)){
			if (i.getDrop() == false) sphere.setDrop(false);
			if(i.getDropCheck()==NOTYET)fallCheck(i);
		}
		if (i.getDropCheck()==NOTYET) {
			fallCheck(i);
		}
	}
}

void sphereFall(){
	for (auto &i : boardSpheres) {
		if (i.getDrop() == TRUE) {
			fallSpheres.push_back(i);
		}
		else {
			tem.push_back(i);
		}
	}

	b1.addScore(fallSpheres.size());
	tem.swap(boardSpheres);
	tem.clear();
	for (int i = 0; i < fallSpheres.size();i++) fallSpheres[i].setVelocity(Vector3f(1+0.2*i, -9-0.1*i, 0));
}

void shoot() {
	if ((boardSpheres.size() == 0 || (boardSpheres.size() != 0 && boardSpheres.back().getVelocity()[1] == 0))&&b1.getLife()>=1) {
		int score = b1.getIntScore();
		boardSpheres.push_back(spheres[1]);
		boardSpheres.back().setVelocity(13 * Vector3f(-sin(b1.getAngle() / 180 * PI), cos(b1.getAngle() / 180 * PI), 0));
		int colorNum=0;
		if (score < 8) { 
			uniform_int_distribution<int> dis(0, 3);
			colorNum = dis(gen);
		}else if (score < 16 && score >= 8) {
			uniform_int_distribution<int> dis(0, 4);
			colorNum = dis(gen);
		}else if (score >= 16) {
			uniform_int_distribution<int> dis(0, 5);
			colorNum = dis(gen);
		}
		
		SolidSphere newSphere(RADIUS, 40, 40);
		newSphere.setCenter(Vector3f(-boundaryX + 50, -boundaryY + 45, 0));
		newSphere.setVelocity(Vector3f(0, 0, 0));
		newSphere.setMTL(materials[colorNum]);
		newSphere.setMTL(materials[colorNum]);
		newSphere.setColorNum(colorNum);
		SolidSphere k(spheres[0]);
		spheres.clear();
		spheres.push_back(newSphere);
		spheres.push_back(k);

		spheres[1].setCenter_p(0, -boundaryY + UNDERBAR - RADIUS, 0);
		b1.setTimer(120);
		shoot1 = true;
		shoot2 = true;
		glutPostRedisplay();
	}
}

int getRandomNumber(int min, int max) { 
	static const double fraction = 1.0 / (RAND_MAX + 1.0);
    return min + static_cast<int>((max - min + 1) * (std::rand() * fraction)); }

//set light&mtl&sphere
void init() {
	srand(static_cast<unsigned int>(std::time(0)));
	FMOD_System_CreateSound(g_FMODSystem, "./level1.mp3", FMOD_LOOP_NORMAL, 0, &pFMOD_SOUND);
	FMOD_System_CreateSound(g_FMODSystem, "./level2.mp3", FMOD_LOOP_NORMAL, 0, &effectSound);
	FMOD_System_CreateSound(g_FMODSystem, "./level3.mp3", FMOD_LOOP_NORMAL, 0, &level3_SOUND);
	initTexture();
	initTexture2();

	light.setAmbient(0.5, 0.5, 0.5, 1.0);
	light.setDiffuse(0.3, 0.3, 0.3, 1.0);
	light.setSpecular(0.7, 0.7, 0.7, 1.0);

	Material mtl1;
	mtl1.setEmission(0.1, 0.1, 0.1, 1);
	mtl1.setAmbient(0.7, 0.7, 0.1, 1);
	mtl1.setDiffuse(0.5, 0.5, 0.5, 1);
	mtl1.setSpecular(1.0, 1.0, 1.0, 1);
	mtl1.setShininess(10);

	Material mtl2(mtl1),mtl3(mtl1),mtl4(mtl1),mtl5(mtl1),mtl6(mtl1);
	mtl2.setAmbient(0.7, 0.1, 0.1, 1); 
	mtl3.setAmbient(0.1, 0.7, 0.1, 1); 
	mtl4.setAmbient(0.1, 0.1, 0.7, 1); 
	mtl5.setAmbient(0.7, 0.1, 0.7, 1);
	mtl6.setAmbient(0.1, 0.7, 0.7, 1); 
	Material mtl7;
	mtl7.setAmbient(0, 0, 0, 1);
	mtl7.setDiffuse(0.0, 0.0, 0.0, 1);
	mtl7.setEmission(0,0,0,0);
	mtl7.setSpecular(0.0, 0.0, 0.0, 0.1);
	mtl7.setShininess(0.1);
	materials.push_back(mtl1);
	materials.push_back(mtl2);
	materials.push_back(mtl3);
	materials.push_back(mtl4);
	materials.push_back(mtl5);
	materials.push_back(mtl6);
	materials.push_back(mtl7);

	int colorNum1 = getRandomNumber(0, 3);
	int colorNum2 = getRandomNumber(0, 3);


    /*쏘아지려는 공 생성*/
	SolidSphere sphere1(RADIUS, 150, 150);
	sphere1.setCenter(Vector3f(0, -boundaryY+UNDERBAR-RADIUS, 0));
	sphere1.setVelocity(Vector3f(0, 0, 0));
	sphere1.setMTL(materials[colorNum1]);

	/*next 공 생성*/

	SolidSphere sphere2(RADIUS,150,150);
	sphere2.setCenter(Vector3f(-boundaryX + 50, -boundaryY + 40, 0));
	sphere2.setVelocity(Vector3f(0, 0, 0));
	sphere2.setMTL(materials[colorNum2]);
	sphere1.setColorNum(colorNum1);
	sphere2.setColorNum(colorNum2);

	spheres.push_back(sphere2);
	spheres.push_back(sphere1);

	SolidSphere sphere3(2.3*RADIUS, 150, 150);
	sphere3.setCenter(Vector3f(-20,  50, 0));
	sphere3.setVelocity(Vector3f(6, 4, 0));
	sphere3.setMTL(materials.back());
	SolidSphere sphere4(2.3*RADIUS, 150, 150);
	sphere4.setCenter(Vector3f(0,  -50, 0));
	sphere4.setVelocity(Vector3f(7, -3, 0));
	sphere4.setMTL(materials.back());
	SolidSphere sphere5(2.3*RADIUS, 150, 150);
	sphere5.setCenter(Vector3f(0,  + 50, 0));
	sphere5.setVelocity(Vector3f(3, 7, 0));
	sphere5.setMTL(materials.back());

	obstacle.push_back(sphere3);
	obstacle.push_back(sphere4);
	obstacle.push_back(sphere5);

}

//keyfunc
void proccesNormalKeys(unsigned char key, int x, int y) {
	if (key == 27 || key == 'q') {
		exit(0);
	}
	if (key == 32) {
		shoot();
	}
	if ((key == 'r' || key == 'R') && b1.getIntScore()==0) {
		b1.setScore();
		b1.setLife();
	}
}

void proccesSpecialKeys(int key, int x, int y) {
	switch (key) {
	case GLUT_KEY_DOWN:
		break;
	case GLUT_KEY_LEFT:
		if (b1.getAngle() <= 70)
			b1.increaseAng();
		break;
	case GLUT_KEY_RIGHT:
		if (b1.getAngle() >= -70)
			b1.reduceAng();
		break;
	case GLUT_KEY_UP:
		shoot();
		break;
	}
			
}
void idle() {
	end_fps = clock();
	if (end_fps - start_fps > 1000 / 60) {
		/* implement: boundary collision handling */
		for (auto& k : boardSpheres) {
			if (k.getCenter().getX() >= boundaryX - RADIUS ||
				k.getCenter().getX() <= -boundaryX + RADIUS) {
				k.setVelocity(Vector3f(-k.getVelocity()[0], k.getVelocity()[1], k.getVelocity()[2]));
			}
			/*ceiling location reorganize*/
			if (k.getCenter().getY() >= boundaryY - 70 - RADIUS) {
				k.setShooted();
				k.setVelocity(Vector3f(0, 0, 0));
				for (int i = 0; i < WIDTH / (RADIUS * 2);i++) {
					if (k.getCenter().getX() >= -boundaryX + RADIUS * 2 * i && k.getCenter().getX() < -boundaryX + RADIUS * 2 * (i + 1)) k.setCenter(Vector3f(-boundaryX + RADIUS * (1 + 2 * i), boundaryY - 70 - RADIUS, k.getCenter()[2]));
				}
			}
			if (k.getCenter().getY() <= -boundaryY + UNDERBAR + RADIUS && k.getShooted() == TRUE) {
				gameOver = TRUE;
				b1.minusLife();
				cout << "game over" << endl;
				boardSpheres.clear();
			}
			k.move();
		}


		for (auto& k : fallSpheres) {
			k.move();
			if (k.getCenter()[1] <= -boundaryY + UNDERBAR) {
				fallSpheres.clear();
				break;
			}
		}
		int ceilingOn = false;
		if (boardSpheres.size() > 0) {

			if (boardSpheres.back().getCenter().getY() >= boundaryY - 70 - RADIUS) {
				ceilingOn = true;
			}
			for (int i = 0; i < boardSpheres.size() - 1; i++) {
				boardSpheres.back().collisionHandling(boardSpheres[i]);
				if (boardSpheres.back().collisionDetection(boardSpheres[i]) && shoot2) {
					boardSpheres.back().setShooted();
					arrive1 = TRUE; //cout << "arrive1 = true" << endl;
				}
			}
		}

		if (arrive1) {
			fallSpheres.clear();
			colorCheck(boardSpheres.back(), boardSpheres.back());
			spherDelete();
			if (boardSpheres.size() != 0)fallCheck(boardSpheres[0]);
			sphereFall();
			for (auto& i : boardSpheres) {
				i.setChecked(FALSE);
				i.setDelete(FALSE);
				i.setDrop(TRUE);
				i.setDropCheck(FALSE);
			}
			arrive1 = FALSE;
			shoot2 = FALSE;
		}
		glutPostRedisplay();
		int score = b1.getIntScore();
		static bool firstBackMusic = false;
		static bool secondBackMusic = false;
		static bool thirdBackMusic = false;
		if (b1.getLife() != 0 && score < 8 && !firstBackMusic) {
			FMOD_System_PlaySound(g_FMODSystem, pFMOD_SOUND, NULL, 0, &channel);
			firstBackMusic = true;


		}
		else if (score < 16 && score >= 8 && !secondBackMusic) {
			FMOD_System_PlaySound(g_FMODSystem, effectSound, NULL, 0, &channel2);
			secondBackMusic = true;
			FMOD_Channel_Stop(channel);
		}
		else if (score < 20 && score >= 16 && !thirdBackMusic) {
			FMOD_System_PlaySound(g_FMODSystem, level3_SOUND, NULL, 0, &channel3);
			thirdBackMusic = TRUE;
			FMOD_Channel_Stop(channel2);
		}
		else if (b1.getLife() == 0) {
			FMOD_Channel_Stop(channel);
			FMOD_Channel_Stop(channel2);
			FMOD_Channel_Stop(channel3);
			firstBackMusic = false;
			secondBackMusic = false;
			thirdBackMusic = false;

		}

		static float timeBar = 0;
		b1.setTimer(120 - timeBar);

		if (score < 8) {
			if (ceilingOn || !shoot2) timeBar += 0.3;

		}
		else if (score < 16 && score >= 8) {
			if (ceilingOn || !shoot2) timeBar += 0.6;

		}
		else if (score >= 16) {
			if (ceilingOn || !shoot2) timeBar += 0.9;
			for (auto& sph : obstacle) {
				if (sph.getCenter()[0] >= boundaryX - RADIUS ||
					sph.getCenter()[0] <= -boundaryX + RADIUS)
					sph.setVelocity(Vector3f(-sph.getVelocity()[0], sph.getVelocity()[1], sph.getVelocity()[2]));
				if (sph.getCenter()[1] + sph.getRadius() >= boundaryY - 70 - RADIUS ||
					sph.getCenter()[1] - sph.getRadius() <= -boundaryY + UNDERBAR + RADIUS)
					sph.setVelocity(Vector3f(sph.getVelocity()[0], -sph.getVelocity()[1], sph.getVelocity()[2]));
				sph.move();
			}

		}

		if (timeBar >= 120) {
			timeBar = 0, shoot1 = false;
			shoot();
		}
		if (shoot1) timeBar = 0, shoot1 = false;
		start_fps = end_fps;
}
glutPostRedisplay();
}

void renderScene() {
	int score = b1.getIntScore();
	glClearColor(0, 0, 0, 0);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(-boundaryX, boundaryX, -boundaryY, boundaryY, -100.0, 100.0);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	string scoreStr = to_string(b1.getIntScore());
	// Draw 2D
	draw_characters(GLUT_BITMAP_HELVETICA_18, scoreStr.c_str(), -boundaryX + 105, boundaryY - 50); 
	draw_characters(GLUT_BITMAP_HELVETICA_18, "Next", -boundaryX+20, -boundaryY+90);
	draw_characters(GLUT_BITMAP_HELVETICA_18, "SCORE : ", -boundaryX + 20, boundaryY - 50);
	draw_characters(GLUT_BITMAP_HELVETICA_18, "Time ", boundaryX - 180, boundaryY - 50);
	glPushMatrix();
	draw_characters(GLUT_BITMAP_HELVETICA_18, "LIFE ", boundaryX - 180, boundaryY - 26);
	if (score < 8) {
		draw_characters(GLUT_BITMAP_HELVETICA_18, "<STAGE 1> ", boundaryX - 95, -boundaryY + 30);

	}
	else if (score < 16 && score >= 8) {
		draw_characters(GLUT_BITMAP_HELVETICA_18, "<STAGE 2> ", boundaryX - 95, -boundaryY + 30);

	}
	else if (score >= 16) {
		draw_characters(GLUT_BITMAP_HELVETICA_18, "<STAGE 3> ", boundaryX - 95, -boundaryY + 30);
	}
	glPopMatrix();

	drawSquareWithTexture2();
	drawSquareWithTexture();

	//b1.setTime

	b1.drawTimer(boundaryX-134,boundaryY-50);
	b1.drawBar(-boundaryX,boundaryX,boundaryY-70);
	b1.drawBar(-boundaryX, boundaryX, -boundaryY +UNDERBAR);
	b1.drawArrow(-boundaryY+UNDERBAR-10);

	// Draw 3D
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_LIGHTING);
	glEnable(light.getID());
	light.draw();
	
	for (auto& sph : spheres) {
		glPushMatrix();
		sph.draw();
		glPopMatrix();
	}
	for (auto& sph : boardSpheres) {
		glPushMatrix();
		sph.draw();
		glPopMatrix();
	}
	for (auto& sph : fallSpheres) {
		glPushMatrix();
		sph.draw();
		glPopMatrix();
	}
	if (score >= 16) {
		for (auto& sph : obstacle) {
			glPushMatrix();
			sph.draw();
			glPopMatrix();
		}
	}
	
	/* implement: draw spheres */
	glDisable(GL_DEPTH_TEST);
	glDisable(GL_LIGHTING);
	glDisable(light.getID());
	drawGameOver();
	if(b1.getLife()==0)draw_characters(GLUT_BITMAP_HELVETICA_18, "If you want new game, press R", -140, -160);
	if (b1.getLife() == 0)draw_characters(GLUT_BITMAP_HELVETICA_18, "If you want end game, press q", -140, -180);

	
	glutSwapBuffers();
}

int main(int argc, char** argv) {

	//FMOD_System_Create(&g_FMODSystem);
	//FMOD_System_Init(g_FMODSystem, 10, FMOD_INIT_NORMAL, NULL);
		glutInit(&argc, argv);
		glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH);
		glutInitWindowPosition(600, 170);
		glutInitWindowSize(WIDTH, HEIGHT);
		glutCreateWindow("Draw bouncing spheres");
		init();
		//Popup Menu

		// register callbacks
		glutDisplayFunc(renderScene);
		glutKeyboardFunc(proccesNormalKeys);
		glutSpecialFunc(proccesSpecialKeys);
		glutIdleFunc(idle);

		// enter GLUT event processing cycle

		//_CrtSetBreakAlloc(234);
		glutMainLoop();
		FMOD_Sound_Release(pFMOD_SOUND);
		FMOD_Sound_Release(effectSound);
		FMOD_System_Close(g_FMODSystem);
		FMOD_System_Release(g_FMODSystem);
	
		return 0;
}
