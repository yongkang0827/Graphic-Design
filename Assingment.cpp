#include <Windows.h> 
#include <gl/GL.h>
#include <gl/GLU.h>
#include <stdio.h>
#include <stdlib.h>
#include <Math.h>

#define WINDOW_TITLE "Robot"

int qNo = 2;

//Arm
char arm = 's';
bool elbowRight = false, elbowLeft = false;
bool shoulderTurnRight = false, shoulderTurnLeft = false;
float rotatspeed = 0.3, actRotat;

//finger
bool fingerRight = false, fingerLeft = false;
float rotatThumb = 0.3, rotatfinger = 0.3;
float actRotatFingLeft, actRotatFing2Left, actRotatFing3Left;
float actRotatFingRight, actRotatFing2Right, actRotatFing3Right;
float actRotatThumbRight, actRotatThumbLeft;
float actRotateElbowRight, actRotateElbowLeft;
float actShoulderRight, actShoulderLeft;

// Weapon
bool gunShow = false, gunBig = false, gunShootMove = false;
float gunSp = 0.005, gunTransUp, gunTransFront, gunTransNear, gunScale, bulletMove, bulletSp = 0.02;
bool weaponShow = false, weaponBig = false, weaponAttackMove = false, electricShow = false, weaponAttackDown = false;
float weaponSp = 0.005, weaponTransUp, weaponTransFront, weaponScale;
int elect = 0, electSp = 1;

//Leg
bool walkRight, walkLeft;
float walkRightKnee, walkUpRightKnee, walkLeftKnee, walkUpLeftKnee;
float walkSpeed = 0.2, walkKneeSpeed = 0.0016;
float walking, walkingSp = 0.0007;

//Body
float angle = 0;
float x1 = 0, y3 = 0, x2 = 0, y2 = 0;

//head
float headRotatex = 0;
float headRotatey = 0;

//move
float sliding = 0;
float slideMove = 0;
float flyingX = 0;
float flyingY = 0;

//thruster
bool isTurbo = false;

//Projection
bool isOrtho = true;

float tSpeed = 0.1;
float tx = 0, ty = 0, tz = 0;
float Ry = 0, Rx = -11, rSpeed = 1;
float orthoview = 1;                 //yong 

//Texture
BITMAP BMP;             //bitmap structure
HBITMAP hBMP = NULL;    //bitmap handle
LPCSTR bodyText[6] = { "black steel.bmp", "white.bmp" , "metal porcealin.bmp"};
LPCSTR jointText[3] = { "white.bmp","Rusty Black Steel.bmp" };
int changeBody1 = 0, changeBody2 = 1, changeBody3 = 2;
int changeJoint1 = 0;

LRESULT WINAPI WindowProcedure(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	switch (msg)
	{
	case WM_DESTROY:              //WM = window message, when click cross button window
		PostQuitMessage(0);
		break;

	case WM_KEYDOWN:                                     //press long key
		if (wParam == VK_ESCAPE)                         //Param = parameter, VK = virtual key
			PostQuitMessage(0);
		else if (wParam == 0x31)
			qNo = 1;
		else if (wParam == 0x32)
			qNo = 2;
		else if (wParam == 0x37)           //change color
		{
			changeBody1 = 1;
			changeBody2 = 0;
			changeJoint1 = 1;
		}

		else if (wParam == 0x30)                    //0 = gun
		{
			gunShow = true;
		}
		else if (wParam == 0x50)                    //p = gun shoot
			gunShootMove = true;

		else if (wParam == 0x39)                    //9 = weapon
			weaponShow = true;
		else if (wParam == 0x4F) {              //o = weapon attack
			weaponAttackMove = true;
		}
		else if (wParam == 0x4D)                  //m = walking
			walkRight = true;

		else if (wParam == 0x46) {             //f = finger wrap
			if (fingerRight == false) {
				fingerRight = true;
				fingerLeft = true;
			}
			else if (fingerRight == true) {
				fingerRight = false;
				fingerLeft = false;
			}
		}
		else if (wParam == 0x54) {             //t = left elbow up
			if (elbowLeft == false)
				elbowLeft = true;
			else if (elbowLeft == true)
				elbowLeft = false;
		}
		else if (wParam == 0x59) {              //y = left shoulder up
			if (shoulderTurnLeft == false)
				shoulderTurnLeft = true;
			else if (shoulderTurnLeft == true)
				shoulderTurnLeft = false;
		}
		else if (wParam == 0x47) {             //g = right elbow up
			if (elbowRight == false)
				elbowRight = true;
			else if (elbowRight == true)
				elbowRight = false;
		}
		else if (wParam == 0x48) {            //h = right shoulder up
			if (shoulderTurnRight == false)
				shoulderTurnRight = true;
			else if (shoulderTurnRight == true)
				shoulderTurnRight = false;
		}

		//////////////////////////////////////////////////////projection
		else if (wParam == 0x4B)    //key k = Ortho
		{
			isOrtho = true;
			tz = 0;
		}
		else if (wParam == 0x4C)    //key l = Perspective
		{
			isOrtho = false;
			tz = 0;
		}
		else if (wParam == 0x41)    //key A = view rotate
			Ry -= rSpeed;

		else if (wParam == 0x44)    //key D = view rotate
			Ry += rSpeed;

		else if (wParam == 0x57)    //key W = view rotate
			Rx -= rSpeed;

		else if (wParam == 0x53)    //key S = view rotate
			Rx += rSpeed;

		else if (wParam == VK_UP) {
			isTurbo = true;
			flyingY += 0.01;
		}
		else if (wParam == VK_DOWN) {
			if (flyingY <= 0) {
				isTurbo = false;
			}
			else {
				isTurbo = true;
				flyingY -= 0.01;
			}
		}
		else if (wParam == VK_LEFT)         //tx
			flyingX -= 0.01;
		else if (wParam == VK_RIGHT)
			flyingX += 0.01;

		else if (wParam == VK_SPACE)        //space to reset
		{
			walkRight = false;
			walkLeft = false;
			fingerLeft = false;
			elbowLeft = false;
			shoulderTurnLeft = false;
			fingerRight = false;
			elbowRight = false;
			shoulderTurnRight = false;
			headRotatex = 0;
			headRotatey = 0;
			weaponBig = false;
			gunBig = false;
		}

		else if (wParam == 0x5A)      //key z = object z 
		{
			isTurbo = true;
			sliding = 30;
			slideMove += 0.3;

			//if (isOrtho) {
			//	if (tz < 1.0) {
			//		tz += tSpeed;
			//	}
			//}
			//else {
			//	if (tz < 1.0) {
			//		tz += tSpeed;
			//	}
			//}
		}
		else if (wParam == 0x58)      //key x
		{
			isTurbo = true;
			sliding = -30;
			slideMove -= 0.3;
			//if (isOrtho) {
				//	if (tz > -1.0) {
				//		tz -= tSpeed;
				//	}
				//}
				//else {
				//	if (tz > -1.0) {
				//		tz -= tSpeed;
				//	}
				//}
		}
		///////////////////////////////yong 
		else if (wParam == 0xBD) {		// key -
			if (isOrtho) {
				if (orthoview < 5)
					orthoview += 0.5;
			}
		}
		else if (wParam == 0xBB) {		// key +
			if (isOrtho) {
				if (orthoview > 1)
					orthoview -= 0.5;
			}
		}
		//head rotate
		else if (wParam == 'C') {
			if (headRotatex > -20)
				headRotatex -= 0.5;
		}
		else if (wParam == 'V') {
			if (headRotatex < 20)
				headRotatex += 0.5;
		}
		else if (wParam == 'B') {
			if (headRotatey < 5)
				headRotatey += 0.5;
		}
		else if (wParam == 'N') {
			if (headRotatey > -3)
				headRotatey -= 0.5;
		}

		break;
	default:
		break;
	}

	return DefWindowProc(hWnd, msg, wParam, lParam);
}
//--------------------------------------------------------------------

bool initPixelFormat(HDC hdc)          //ini = initalize
{
	PIXELFORMATDESCRIPTOR pfd;
	ZeroMemory(&pfd, sizeof(PIXELFORMATDESCRIPTOR));

	pfd.cAlphaBits = 8;
	pfd.cColorBits = 32;
	pfd.cDepthBits = 24;
	pfd.cStencilBits = 0;

	pfd.dwFlags = PFD_DOUBLEBUFFER | PFD_SUPPORT_OPENGL | PFD_DRAW_TO_WINDOW;

	pfd.iLayerType = PFD_MAIN_PLANE;
	pfd.iPixelType = PFD_TYPE_RGBA;
	pfd.nSize = sizeof(PIXELFORMATDESCRIPTOR);
	pfd.nVersion = 1;

	// choose pixel format returns the number most similar pixel format available
	int n = ChoosePixelFormat(hdc, &pfd);

	// set pixel format returns whether it sucessfully set the pixel format
	if (SetPixelFormat(hdc, n, &pfd))
	{
		return true;
	}
	else
	{
		return false;
	}
}
//--------------------------------------------------------------------

//////////////////////////////////Texture
GLuint loadTexture(LPCSTR filename) {

	//Get from Step 1
	GLuint texture = 0;     //texture name

	//Step 3: Initialize texture info
	glPixelStorei(GL_UNPACK_ALIGNMENT, 4);
	HBITMAP hBMP = (HBITMAP)LoadImage(GetModuleHandle(NULL), filename, IMAGE_BITMAP, 0, 0, LR_CREATEDIBSECTION | LR_LOADFROMFILE);
	GetObject(hBMP, sizeof(BMP), &BMP);

	//Step 4: Assign texture to polygon
	glEnable(GL_TEXTURE_2D);
	glGenTextures(1, &texture);
	glBindTexture(GL_TEXTURE_2D, texture);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, BMP.bmWidth, BMP.bmHeight, 0, GL_BGR_EXT, GL_UNSIGNED_BYTE, BMP.bmBits);

	//From Step 5
	DeleteObject(hBMP);
	return texture;
}

//////////////////////////////////////Usually Use
void drawCube(float length, float width, float height) {
	glBegin(GL_QUADS);
	//Face 1? front
	glVertex3f(0.0f, 0.0f, height);
	glVertex3f(length, 0.0f, height);
	glVertex3f(length, 0.0f, 0.0f);
	glVertex3f(0.0f, 0.0f, 0.0f);
	//Face 2? left
	glVertex3f(0.0f, 0.0f, 0.0f);
	glVertex3f(0.0f, 0.0f, height);
	glVertex3f(0.0f, width, height);
	glVertex3f(0.0f, width, 0.0f);
	//Face 3? top
	glVertex3f(0.0f, 0.0f, height);
	glVertex3f(0.0f, width, height);
	glVertex3f(length, width, height);
	glVertex3f(length, 0.0f, height);
	//Face 4? right
	glVertex3f(length, 0.0f, height);
	glVertex3f(length, 0.0f, 0.0f);
	glVertex3f(length, width, 0.0f);
	glVertex3f(length, width, height);
	//Face 5? behind
	glVertex3f(length, width, height);
	glVertex3f(0.0f, width, height);
	glVertex3f(0.0f, width, 0.0f);
	glVertex3f(length, width, 0.0f);
	//Face 6? bottom
	glVertex3f(length, width, 0.0f);
	glVertex3f(length, 0.0f, 0.0f);
	glVertex3f(0.0f, 0.0f, 0.0f);
	glVertex3f(0.0f, width, 0.0f);
	glEnd();
}

void drawCubeTexture(float length, float width, float height, LPCSTR filename) {
	GLuint texture;
	texture = loadTexture(filename);
	glBegin(GL_QUADS);
	//Face size : Bottom
	glTexCoord2f(0.0f, 1.0f);
	glVertex3f(0.0f, 0.0f, height);
	glTexCoord2f(1.0f, 1.0f);
	glVertex3f(length, 0.0f, height);
	glTexCoord2f(1.0f, 0.0f);
	glVertex3f(length, 0.0f, 0.0f);
	glTexCoord2f(0.0f, 0.0f);
	glVertex3f(0.0f, 0.0f, 0.0f);

	//Face 2 : Left
	glTexCoord2f(0.0f, 1.0f);
	glVertex3f(0.0f, 0.0f, 0.0f);
	glTexCoord2f(1.0f, 1.0f);
	glVertex3f(0.0f, 0.0f, height);
	glTexCoord2f(1.0f, 0.0f);
	glVertex3f(0.0f, width, height);
	glTexCoord2f(0.0f, 0.0f);
	glVertex3f(0.0f, width, 0.0f);

	//Face 3 : Front
	glTexCoord2f(0.0f, 1.0f);
	glVertex3f(0.0f, 0.0f, height);
	glTexCoord2f(1.0f, 1.0f);
	glVertex3f(0.0f, width, height);
	glTexCoord2f(1.0f, 0.0f);
	glVertex3f(length, width, height);
	glTexCoord2f(0.0f, 0.0f);
	glVertex3f(length, 0.0f, height);

	//Face 4 : Right
	glTexCoord2f(0.0f, 1.0f);
	glVertex3f(length, 0.0f, height);
	glTexCoord2f(1.0f, 1.0f);
	glVertex3f(length, 0.0f, 0.0f);
	glTexCoord2f(1.0f, 0.0f);
	glVertex3f(length, width, 0.0f);
	glTexCoord2f(0.0f, 0.0f);
	glVertex3f(length, width, height);

	//Face 5 : Top
	glTexCoord2f(0.0f, 1.0f);
	glVertex3f(length, width, height);
	glTexCoord2f(1.0f, 1.0f);
	glVertex3f(0.0f, width, height);
	glTexCoord2f(1.0f, 0.0f);
	glVertex3f(0.0f, width, 0.0f);
	glTexCoord2f(0.0f, 0.0f);
	glVertex3f(length, width, 0.0f);

	//Face 6 : Back
	glTexCoord2f(1.0f, 1.0f);
	glVertex3f(length, width, 0.0f);
	glTexCoord2f(0.0f, 1.0f);
	glVertex3f(length, 0.0f, 0.0f);
	glTexCoord2f(0.0f, 0.0f);
	glVertex3f(0.0f, 0.0f, 0.0f);
	glTexCoord2f(1.0f, 0.0f);
	glVertex3f(0.0f, width, 0.0f);
	glEnd();
	glDeleteTextures(1, &texture);
}

void sphere(float radius) {
	GLUquadricObj* sphere = NULL;
	sphere = gluNewQuadric();

	gluQuadricDrawStyle(sphere, GLU_FILL);
	gluSphere(sphere, radius, 30, 30);

	gluDeleteQuadric(sphere);
}

void sphereTexture(float radius, LPCSTR filename) {
	GLuint texture;
	texture = loadTexture(filename);

	GLUquadricObj* sphere = NULL;
	sphere = gluNewQuadric();

	gluQuadricTexture(sphere, TRUE);
	gluQuadricNormals(sphere, GLU_SMOOTH);

	gluQuadricDrawStyle(sphere, GLU_FILL);
	gluSphere(sphere, radius, 30, 30);

	gluDeleteQuadric(sphere);
	glDeleteTextures(1, &texture);
}

void sphereTexture1(float radius, LPCSTR filename) {
	GLuint texture;
	texture = loadTexture(filename);

	GLUquadricObj* sphere = NULL;
	sphere = gluNewQuadric();

	gluQuadricTexture(sphere, TRUE);
	gluQuadricNormals(sphere, GLU_SMOOTH);

	gluQuadricDrawStyle(sphere, GLU_FILL);
	gluSphere(sphere, radius, 30, 30);

	gluDeleteQuadric(sphere);
	glDeleteTextures(1, &texture);
}

void cylinder(float baseRadius, float topRadius, float height) {
	GLUquadric* cylinDownkAim = NULL;
	cylinDownkAim = gluNewQuadric();

	gluQuadricDrawStyle(cylinDownkAim, GLU_FILL);
	gluCylinder(cylinDownkAim, baseRadius, topRadius, height, 30, 30);

	gluDeleteQuadric(cylinDownkAim);
}

void cylinderTexture(float baseRadius, float topRadius, float height, LPCSTR filename) {
	GLuint texture;
	texture = loadTexture(filename);

	GLUquadric* cylinder = NULL;
	cylinder = gluNewQuadric();

	gluQuadricTexture(cylinder, TRUE);
	gluQuadricNormals(cylinder, GLU_SMOOTH);

	gluQuadricDrawStyle(cylinder, GLU_FILL);
	gluCylinder(cylinder, baseRadius, topRadius, height, 30, 30);

	gluDeleteQuadric(cylinder);
	glDeleteTextures(1, &texture);
}

///////////////////////////////////body
void drawsphere() {
	GLUquadricObj* sphere = NULL;
	sphere = gluNewQuadric();

	gluQuadricDrawStyle(sphere, GLU_FILL);
	gluSphere(sphere, 0.2, 20, 20);
	gluDeleteQuadric(sphere);
}

void drawCone(float topRadius, float height, int slices, int stacks, LPCSTR filename) {
	GLuint texture;
	texture = loadTexture(filename);
	GLUquadricObj* cylinder = NULL;
	cylinder = gluNewQuadric();
	gluQuadricTexture(cylinder, TRUE);
	gluQuadricNormals(cylinder, GLU_SMOOTH);

	gluQuadricDrawStyle(cylinder, GLU_POINT);
	gluCylinder(cylinder, 0.01, topRadius, height, slices, stacks);
	gluDeleteQuadric(cylinder);
	glDeleteTextures(1, &texture);
};

void drawCircle(float radius) {
	glBegin(GL_TRIANGLE_FAN);
	glVertex2f(x1, y3);

	for (angle = 0; angle <= 360; angle += 0.2) {
		x2 = x1 + radius * (cos(angle));
		y2 = y3 + radius * (sin(angle));
		glVertex2f(x2, y2);
	}
	glEnd();
}

void drawCylinder(float baseRadius, float topRadius, float height, int slices, int stacks, LPCSTR filename) {
	GLuint texture;
	texture = loadTexture(filename);

	drawCircle(baseRadius);
	GLUquadricObj* cylinder = NULL;
	cylinder = gluNewQuadric();

	gluQuadricTexture(cylinder, TRUE);
	gluQuadricNormals(cylinder, GLU_SMOOTH);

	gluQuadricDrawStyle(cylinder, GLU_FILL);
	gluCylinder(cylinder, baseRadius, topRadius, height, slices, stacks);
	gluDeleteQuadric(cylinder);

	glPushMatrix();
	glTranslatef(0, 0, height);
	drawCircle(topRadius);
	glPopMatrix();

	glDeleteTextures(1, &texture);
};

void drawCylinder2(float baseRadius, float topRadius, float height, int slices, int stacks, LPCSTR filename) {//use for glu point
	
	GLuint texture;
	texture = loadTexture(filename);	
	GLUquadricObj* cylinder = NULL;
	cylinder = gluNewQuadric();

	gluQuadricTexture(cylinder, TRUE);
	gluQuadricNormals(cylinder, GLU_SMOOTH);


	gluQuadricDrawStyle(cylinder, GLU_POINT);
	gluCylinder(cylinder, baseRadius, topRadius, height, slices, stacks);
	gluDeleteQuadric(cylinder);
	glDeleteTextures(1, &texture);
};

void drawCylinderLine(float baseRadius, float topRadius, float height, int slices, int stacks, LPCSTR filename) {
	GLuint texture;
	texture = loadTexture(filename);
	GLUquadricObj* cylinder = NULL;
	cylinder = gluNewQuadric();
	gluQuadricTexture(cylinder, TRUE);
	gluQuadricNormals(cylinder, GLU_SMOOTH);

	gluQuadricDrawStyle(cylinder, GLU_LINE);
	gluCylinder(cylinder, baseRadius, topRadius, height, slices, stacks);
	gluDeleteQuadric(cylinder);
	glDeleteTextures(1, &texture);
};

void turbo(LPCSTR filename) {
	//glColor3f(1, 1, 0);
	drawCircle(0.03);
	cylinderTexture(0.031, 0.051, 0.02, filename);
	cylinderTexture(0.03, 0.05, 0.02, filename);
	//glColor3f(0, 0, 0);
	drawCylinderLine(0.029, 0.049, 0.02, 10, 1, "weaponHandle.bmp");
	if (isTurbo) {
		//glColor3f(0, 0, 1);
		drawCone(0.05, 0.05, 50, 50, "fire.bmp");
		glPushMatrix();
		glTranslatef(0, 0, 0.05);
		//	glRotatef(180, 1, 0, 0);
		drawCylinder2(0.048, 0.02, 0.03, 30, 30,"fire.bmp");  
		glPopMatrix();
	}
}

void thruster(LPCSTR filename1, LPCSTR filename2) {
	glPushMatrix();
	glTranslatef(-0.2, 0.25, 0.35);
	glRotatef(180, 1, 0, 0);
	//glColor3f(1, 0, 0);
	drawCubeTexture(0.4, 0.5, 0.1, filename1);
	glPopMatrix();

	//glColor3f(0, 0, 0);
	glPushMatrix();
	glTranslatef(0.09, 0.15, 0.36);
	turbo(filename2);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(-0.09, 0.15, 0.36);
	turbo(filename2);
	glPopMatrix();


	glPushMatrix();
	glTranslatef(0.09, 0, 0.36);
	turbo(filename2);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(-0.09, 0, 0.36);
	turbo(filename2);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(0.09, -0.15, 0.36);
	turbo(filename2);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(-0.09, -0.15, 0.36);
	turbo(filename2);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(-0.09, -0.15, 0.36);
	turbo(filename2);
	glPopMatrix();

	//	below
	glPushMatrix();
	glRotatef(90, 1, 0, 0);

	glPushMatrix();
	glTranslatef(-0.09, 0.3, 0.251);
	turbo(filename2);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(0.09, 0.3, 0.251);
	turbo(filename2);
	glPopMatrix();

	glPopMatrix();

}

void bodyUp(float width, LPCSTR filename) {
	GLuint texture;
	texture = loadTexture(filename);

	glBegin(GL_QUADS);
//	glColor3f(1, 0, 0);
	//Face 1£º front
	glTexCoord2f(0, 1);
	glVertex3f(-0.05, 0.3f, -0.05);
	glTexCoord2f(1, 1);
	glVertex3f(-0.45f, 0.35f, 0);
	glTexCoord2f(1, 0);
	glVertex3f(-0.45, 0.2f, 0.0f);
	glTexCoord2f(0, 0);
	glVertex3f(0.0f, 0.0f, -0.1);
	//Face 2£º right
//	glColor3f(1, 0, 1);
	glTexCoord2f(0, 1);
	glVertex3f(0.0f, 0.0f, -0.1);
	glTexCoord2f(1, 1);
	glVertex3f(0.0f, 0.0f, width + 0.1);
	glTexCoord2f(1, 0);
	glVertex3f(-0.05, 0.3f, width + 0.05);
	glTexCoord2f(0, 0);
	glVertex3f(-0.05, 0.3f, -0.05);

	//Face 3£º top
//	glColor3f(1, 1, 0);
	glTexCoord2f(0, 1);
	glVertex3f(-0.05, 0.3f, -0.05);
	glTexCoord2f(1, 1);
	glVertex3f(-0.05, 0.3f, width + 0.05);
	glTexCoord2f(1, 0);
	glVertex3f(-0.45f, 0.35f, width);
	glTexCoord2f(0, 0);
	glVertex3f(-0.45f, 0.35f, 0);
	//Face 4£º left
//	glColor3f(0, 0, 1);
	glTexCoord2f(0, 1);
	glVertex3f(-0.45f, 0.35f, 0);
	glTexCoord2f(1, 1);
	glVertex3f(-0.45, 0.2f, 0.0f);
	glTexCoord2f(1, 0);
	glVertex3f(-0.45, 0.2f, width);
	glTexCoord2f(0, 0);
	glVertex3f(-0.45f, 0.35f, width);

	//Face 5£º behind
//	glColor3f(0, 1, 0);
	glTexCoord2f(0, 1);
	glVertex3f(-0.45f, 0.35f, width);
	glTexCoord2f(1, 1);
	glVertex3f(-0.05, 0.3f, width + 0.05);
	glTexCoord2f(1, 0);
	glVertex3f(0.0f, 0.0f, width + 0.1);
	glTexCoord2f(0, 0);
	glVertex3f(-0.45, 0.2f, width);

	//Face 6£º bottom
//	glColor3f(0, 0, 0);
	glTexCoord2f(0, 1);
	glVertex3f(-0.45, 0.2f, width);
	glTexCoord2f(1, 1);
	glVertex3f(-0.45, 0.2f, 0);
	glTexCoord2f(1, 0);
	glVertex3f(0.0f, 0.0f, -0.1);
	glTexCoord2f(0, 0);
	glVertex3f(0, 0, width + 0.1);

	glEnd();
	glDeleteTextures(1, &texture);
	glDisable(GL_TEXTURE_2D);
}

void bodyUpMid(float width, LPCSTR filename) {
	GLuint texture;
	texture = loadTexture(filename);

	glBegin(GL_TRIANGLES);
//	glColor3f(0, 0, 1);
	//Face 1£º front
	glTexCoord2f(1, 1);
	glVertex3f(-0.02, 0.1f, -0.075);
	glTexCoord2f(1, 0);
	glVertex3f(0, 0.075f, -0.075);
	glTexCoord2f(0, 0);
	glVertex3f(0.0f, 0.0f, -0.1);
	glEnd();

	//Face 5£º behind
	glBegin(GL_TRIANGLES);
//	glColor3f(0, 0, 1);
	glVertex3f(-0.02, 0.1f, width + 0.075);
	glTexCoord2f(1, 0);
	glVertex3f(0, 0.075f, width + 0.075);
	glTexCoord2f(0, 0);
	glVertex3f(0.0f, 0.0f, width + 0.1);
	glEnd();

	glBegin(GL_QUADS);
//	glColor3f(1, 1, 1);
	//Face 1£º front
	glTexCoord2f(0, 1);
	glVertex3f(-0.02, 0.1f, -0.075);
	glTexCoord2f(1, 1);
	glVertex3f(0.0f, 0.075f, -0.075);
	glTexCoord2f(1, 0);
	glVertex3f(0.0f, 0.175f, -0.06);
	glTexCoord2f(0, 0);
	glVertex3f(-0.035, 0.2, -0.06);


	//Face 5£º behind
	glTexCoord2f(0, 1);
	glVertex3f(-0.02, 0.1f, width + 0.075);
	glTexCoord2f(1, 1);
	glVertex3f(0.0f, 0.075f, width + 0.075);
	glTexCoord2f(1, 0);
	glVertex3f(0.0f, 0.175f, width + 0.06);
	glTexCoord2f(0, 0);
	glVertex3f(-0.035, 0.2, width + 0.06);
	glEnd();

	glBegin(GL_QUADS);
//	glColor3f(0, 0, 0);
	//Face 1£º front

	glTexCoord2f(0, 1);
	glVertex3f(0.0f, 0.175f, -0.06);
	glTexCoord2f(1, 1);
	glVertex3f(-0.035, 0.2, -0.06);
	glTexCoord2f(1, 0);
	glVertex3f(-0.05, 0.3, -0.05);
	glTexCoord2f(0, 0);
	glVertex3f(0, 0.29, -0.05);

	//Face 5£º behind
	glTexCoord2f(0, 1);
	glVertex3f(0.0f, 0.175f, width + 0.06);
	glTexCoord2f(1, 1);
	glVertex3f(-0.035, 0.2, width + 0.06);
	glTexCoord2f(1, 0);
	glVertex3f(-0.05, 0.3, width + 0.05);
	glTexCoord2f(0, 0);
	glVertex3f(0, 0.29, width + 0.05);
	glEnd();
	glDeleteTextures(1, &texture);
	glDisable(GL_TEXTURE_2D);
}

void tri(LPCSTR filename) {
	GLuint texture;
	texture = loadTexture(filename);

	glBegin(GL_TRIANGLES);
//	glColor3f(1, 0, 0);
	//front
	glTexCoord2f(1, 1);
	glVertex3f(0.0, 0, -0.1);
	glTexCoord2f(1, 0);
	glVertex3f(0.15, 0, -0.05);
	glTexCoord2f(0, 0);
	glVertex3f(0.15, 0.065, -0.05);
	//back
//	glColor3f(1, 1, 0);
	glTexCoord2f(1, 1);
	glVertex3f(0.15, 0, 0.05);
	glTexCoord2f(1, 0);
	glVertex3f(0.0, 0, 0.1);
	glTexCoord2f(0, 0);
	glVertex3f(0.15, 0.065, 0.05);
	glEnd();
	//right
	glBegin(GL_QUADS);
//	glColor3f(1, 0, 1);
	glTexCoord2f(0, 1);
	glVertex3f(0.15, 0.065, -0.05);
	glTexCoord2f(1, 1);
	glVertex3f(0.15, 0.065, 0.05);
	glTexCoord2f(1, 0);
	glVertex3f(0.15, 0, 0.05);
	glTexCoord2f(0, 0);
	glVertex3f(0.15, 0, -0.05);
	//bottom
//	glColor3f(0, 0, 1);
	glTexCoord2f(0, 1);
	glVertex3f(0, 0, 0.1);
	glTexCoord2f(1, 1);
	glVertex3f(0.0, 0, -0.1);
	glTexCoord2f(1, 0);
	glVertex3f(0.15, 0, -0.05);
	glTexCoord2f(0, 0);
	glVertex3f(0.15, 0, 0.05);
	//top
//	glColor3f(0, 1, 0);
	glTexCoord2f(0, 1);
	glVertex3f(0, 0, 0.1);
	glTexCoord2f(1, 1);
	glVertex3f(0, 0, -0.1);
	glTexCoord2f(1, 0);
	glVertex3f(0.15, 0.065, -0.05);
	glTexCoord2f(0, 0);
	glVertex3f(0.15, 0.065, 0.05);

	glEnd();

	//glBegin(GL_QUADS);
	//glTexCoord2f(0, 1);
	//glVertex3f(0.15, 0.065, -0.05);
	//glTexCoord2f(1, 1);
	//glVertex3f(0.15, 0, -0.05);
	//glTexCoord2f(1, 0);
	//glVertex3f(0.15, 0.065, 0.1);
	//glTexCoord2f(0, 0);
	//glVertex3f(0.15, 0, 0.1);
	//glEnd();


	//glBegin(GL_TRIANGLES);
	//glColor3f(1, 0, 1);
	//glTexCoord2f(1, 1);
	//glVertex3f(0.0, 0, -0.1);
	//glTexCoord2f(1, 0);
	//glVertex3f(-0.15, 0, -0.05);
	//glTexCoord2f(0, 0);
	//glVertex3f(-0.15, 0.065, -0.05);
	//glEnd();
	////bottom
	//glBegin(GL_TRIANGLES);
	//glTexCoord2f(1, 1);
	//glVertex3f(0.0, 0, -0.1);
	//glTexCoord2f(1, 0);
	//glVertex3f(-0.15, 0, -0.05);
	//glTexCoord2f(0, 0);
	//glVertex3f(-0.15, 0.0, 0);
	//glEnd();


	//glBegin(GL_TRIANGLES);
	//glColor3f(1, 1, 1);
	//glTexCoord2f(1, 1);
	//glVertex3f(0.0, 0, -0.1);
	//glTexCoord2f(1, 0);
	//glVertex3f(-0.15, 0, 0);
	//glTexCoord2f(0, 0);
	//glVertex3f(0, 0, 0);
	////bottom
	//glTexCoord2f(1, 1);
	//glVertex3f(0.0, 0, -0.1);
	//glTexCoord2f(1, 0);
	//glVertex3f(0.15, 0, 0);
	//glTexCoord2f(0, 0);
	//glVertex3f(0, 0, 0);
	//glEnd();

	//glBegin(GL_QUADS);
	//glVertex3f(-0.15, 0.065, -0.05);
	//glVertex3f(-0.15, 0, -0.05);
	//glVertex3f(-0.15, 0.065, 0.1);
	//glVertex3f(-0.15, 0, 0.1);
	//glEnd();
	glDeleteTextures(1, &texture);
	glDisable(GL_TEXTURE_2D);

}

void bodyMidTri(float width, LPCSTR filename) {
	tri(filename);
	//glPushMatrix();
	//glTranslatef(0, 0, width);
	//tri();
	//glPopMatrix();

	glPushMatrix();
	glRotatef(180, 0, 1, 0);
	//	glTranslatef(0, 0, 0.1);
	tri(filename);
	//glPushMatrix();
	//glTranslatef(0, 0, width);
	//tri();
	//glPopMatrix();
	glPopMatrix();
}

void bodyMid(float width, LPCSTR filename1, LPCSTR filename2) {
	bodyMidTri(width, filename2);

	float i, j;
	glPushMatrix();
	glScalef(0.5, 0.6, 0.7);
	glTranslatef(0, 0, 0.1);

	glPushMatrix();
	glRotatef(90, 1, 0, 0);
//	glColor3f(0, 0, 0);
	drawCylinder(0.2, 0.2, 0.6, 30, 30, filename1);
	glPopMatrix();
	for (j = 0.1; j < 0.7; j += 0.1) {
		for (i = 0; i < 360; i += 30) {
			glPushMatrix();
			glRotatef(-i, 0, 1, 0);
			glTranslatef(0.2, -j, 0);
//			glColor3f(1, 0, 0);
			drawCubeTexture(0.07, 0.07, 0.07, filename2);
			glPopMatrix();
		}
	}
	glPopMatrix();

	//glPushMatrix();
	//glTranslatef(-0.15, -0.1, 0);
	//drawCubeTexture(0.3, 0.1, width + 0.05, filename1);
	//glPopMatrix();

	//glPushMatrix();
	//glTranslatef(-0.15, -0.2, 0);
	//glColor3f(0, 0, 1);
	//drawCubeTexture(0.3, 0.1, width + 0.05, filename1);
	//glPopMatrix();

	//glPushMatrix();
	//glTranslatef(-0.15, -0.3, 0);
	//glColor3f(0, 0, 1);
	//drawCubeTexture(0.3, 0.1, width + 0.05, filename1);
	//glPopMatrix();
}

void bodySide(float width, LPCSTR filename) {
	GLuint texture;
	texture = loadTexture(filename);

	glBegin(GL_QUADS);
//	glColor3f(1, 1, 0);
	//Face 1£º front
	glTexCoord2f(0, 1);
	glVertex3f(0.15, 0.07f, -0.05);
	glTexCoord2f(1, 1);
	glVertex3f(0.15, -0.07, 0);
	glTexCoord2f(1, 0);
	glVertex3f(0.45f, 0.05f, 0.03);
	glTexCoord2f(0, 0);
	glVertex3f(0.45, 0.2, 0.025);

	//Face 2£º right
//	glColor3f(1, 0, 1);
	glTexCoord2f(0, 1);
	glVertex3f(0.45, 0.2, 0.025);
	glTexCoord2f(1, 1);
	glVertex3f(0.45f, 0.05f, 0.03);
	glTexCoord2f(1, 0);
	glVertex3f(0.45f, 0.05f, width - 0.03);
	glTexCoord2f(0, 0);
	glVertex3f(0.45, 0.2, width - 0.025);

	//Face 3£º top
//	glColor3f(1, 1, 0);
	glTexCoord2f(0, 1);
	glVertex3f(0.45, 0.2, width - 0.025);
	glTexCoord2f(1, 1);
	glVertex3f(0.45, 0.2, 0.025);
	glTexCoord2f(1, 0);
	glVertex3f(0.15, 0.07f, -0.05);
	glTexCoord2f(0, 0);
	glVertex3f(0.15, 0.07f, width + 0.05);
	//Face 4£º left
//	glColor3f(0, 0, 1);
	glTexCoord2f(0, 1);
	glVertex3f(0.15, 0.07f, width + 0.05);
	glTexCoord2f(1, 1);
	glVertex3f(0.15, 0.07f, -0.05);
	glTexCoord2f(1, 0);
	glVertex3f(0.15, -0.07, 0);
	glTexCoord2f(0, 0);
	glVertex3f(0.15, -0.07, width);

	//Face 5£º behind
//	glColor3f(0, 1, 0);
	glTexCoord2f(0, 1);
	glVertex3f(0.15, -0.07, width);
	glTexCoord2f(1, 1);
	glVertex3f(0.15, 0.07f, width + 0.05);
	glTexCoord2f(1, 0);
	glVertex3f(0.45, 0.2, width - 0.025);
	glTexCoord2f(0, 0);
	glVertex3f(0.45, 0.05, width - 0.03);

	//Face 6£º bottom
//	glColor3f(0, 0, 0);
	glTexCoord2f(0, 1);
	glVertex3f(0.45, 0.05, width - 0.03);
	glTexCoord2f(1, 1);
	glVertex3f(0.45, 0.05, 0.03);
	glTexCoord2f(1, 0);
	glVertex3f(0.15, -0.07, 0);
	glTexCoord2f(0, 0);
	glVertex3f(0.15, -0.07, width);

	glEnd();

	glPushMatrix();
	glTranslatef(-0.43, 0.13, 0.01);
	drawCylinder(0.01, 0.01, 0.17, 50, 50, "WHITE.BMP");
	glPopMatrix();

	glPushMatrix();
	glTranslatef(-0.4, 0.1, 0.01);
	drawCylinder(0.01, 0.01, 0.18, 50, 50, "WHITE.BMP");
	glPopMatrix();

	glPushMatrix();
	glTranslatef(-0.37, 0.08, 0.01);
	drawCylinder(0.01, 0.01, 0.19, 50, 50, "WHITE.BMP");
	glPopMatrix();
	glDeleteTextures(1, &texture);
	glDisable(GL_TEXTURE_2D);
}

void bodyRight(float width, LPCSTR filename) {
	bodySide(width, filename);
	glPushMatrix();
	glScalef(0.9, 0.95, 1);
	glTranslatef(0, -0.13, 0);
	bodySide(width, filename);
	glPopMatrix();
	glPushMatrix();
	glTranslatef(0, -0.26, 0);
	glScalef(0.8, 0.9, 1);
	bodySide(width, filename);
	glPopMatrix();
}

void lowerBody(float width, LPCSTR filename1, LPCSTR filename2) {
	GLuint textures[2];
	textures[0] = loadTexture(filename1);

	glBegin(GL_QUADS);
//	glColor3f(1, 1, 0);
	//Face 1£º front
	glTexCoord2f(0, 1);
	glVertex3f(0, -0.27, -0.1);
	glTexCoord2f(1, 1);
	glVertex3f(-0.3, -0.4, 0);
	glTexCoord2f(1, 0);
	glVertex3f(-0.3, -0.5f, 0);
	glTexCoord2f(0, 0);
	glVertex3f(0, -0.6, -0.1);

	//Face 2£º right
//	glColor3f(1, 0, 1);
	glTexCoord2f(0, 1);
	glVertex3f(0, -0.6, -0.1);
	glTexCoord2f(1, 1);
	glVertex3f(0, -0.6, width + 0.1);
	glTexCoord2f(1, 0);
	glVertex3f(0, -0.27, width + 0.1);
	glTexCoord2f(0, 0);
	glVertex3f(0, -0.27, -0.1);

	//Face 3£º top
//	glColor3f(0, 0, 0);
	glTexCoord2f(0, 1);
	glVertex3f(-0.3, -0.4, 0);
	glTexCoord2f(1, 1);
	glVertex3f(0, -0.27, -0.1);
	glTexCoord2f(1, 0);
	glVertex3f(0, -0.27, width + 0.1);
	glTexCoord2f(0, 0);
	glVertex3f(-0.3, -0.4, width);
	//Face 4£º left
//	glColor3f(0, 0, 1);
	glTexCoord2f(0, 1);
	glVertex3f(-0.3, -0.4, width);
	glTexCoord2f(1, 1);
	glVertex3f(-0.3, -0.4, 0);
	glTexCoord2f(1, 0);
	glVertex3f(-0.3, -0.5f, 0);
	glTexCoord2f(0, 0);
	glVertex3f(-0.3, -0.5f, width);


	//Face 5£º behind
//	glColor3f(0, 1, 0);
	glTexCoord2f(0, 1);
	glVertex3f(-0.3, -0.5f, width);
	glTexCoord2f(1, 1);
	glVertex3f(-0.3, -0.4, width);
	glTexCoord2f(1, 0);
	glVertex3f(0, -0.27, width + 0.1);
	glTexCoord2f(0, 0);
	glVertex3f(0, -0.6, width + 0.1);

	//Face 6£º bottom
//	glColor3f(0, 0, 0);
	glTexCoord2f(0, 1);
	glVertex3f(0, -0.6, width + 0.1);
	glTexCoord2f(1, 1);
	glVertex3f(0, -0.6, -0.1);
	glTexCoord2f(1, 0);
	glVertex3f(-0.3, -0.5f, 0);
	glTexCoord2f(0, 0);
	glVertex3f(-0.3, -0.5f, width);

	glEnd();
	glDeleteTextures(1, &textures[0]);
	glDisable(GL_TEXTURE_2D);

	glPushMatrix();
//	glColor3f(1, 0, 1);
	glTranslatef(-0.2, -0.3, -0.1);
	drawCylinder(0.07, 0.07, 0.4, 30, 30, filename2);
	glPopMatrix();

}

void neck(float width, LPCSTR filename) {
	GLuint texture;
	texture = loadTexture(filename);

	glBegin(GL_QUADS);
//	glColor3f(0, 0, 0);
	glTexCoord2f(0, 1);
	glVertex3f(0, 0.29, (width / 2) - 0.15);
	glTexCoord2f(1, 1);
	glVertex3f(-0.15, 0.3, width / 2);
	glTexCoord2f(1, 0);
	glVertex3f(-0.15, 0.36, width / 2);
	glTexCoord2f(0, 0);
	glVertex3f(0, 0.35, (width / 2) - 0.15);

	glTexCoord2f(0, 1);
	glVertex3f(0, 0.29, (width / 2) + 0.15);
	glTexCoord2f(1, 1);
	glVertex3f(-0.15, 0.3, width / 2);
	glTexCoord2f(1, 0);
	glVertex3f(-0.15, 0.36, width / 2);
	glTexCoord2f(0, 0);
	glVertex3f(0, 0.35, (width / 2) + 0.15);
	glEnd();

	glBegin(GL_TRIANGLES);
	glVertex3f(-0.15, 0.36, width / 2);
	glVertex3f(0, 0.35, (width / 2) + 0.15);
	glVertex3f(0, 0.35, (width / 2) - 0.15);
	glEnd();
	glDeleteTextures(1, &texture);
	glDisable(GL_TEXTURE_2D);
}

void body(float width, LPCSTR filename1, LPCSTR filename2) {
	bodyUp(width, filename1);
	bodyUpMid(width, filename2);

	glPushMatrix();
	glTranslatef(-0.245, 0.2, -0.045);
//	glColor3f(1, 1, 0);
	drawCircle(0.035);
//	glColor3f(0, 0, 0);
	glTranslatef(0, 0, -0.001);
	drawCircle(0.03);
	glPopMatrix();
	//////////////////////////////////////////thruster
	glPushMatrix();
	glTranslatef(-0.25, 0.2, -0.06);
//	glColor3f(1, 1, 0);
	cylinder(0.05, 0.02, 0.05);
//	glColor3f(0, 0, 0);
	drawCylinderLine(0.049, 0.019, 0.049, 7, 2,"weaponHandle.bmp");
	glPopMatrix();

	glPushMatrix();
	glTranslatef(0.245, 0.2, -0.06);
//	glColor3f(1, 1, 0);
	drawCircle(0.035);
//	glColor3f(0, 0, 0);
	glTranslatef(0, 0, -0.001);
	drawCircle(0.03);
	glPopMatrix();
	////////////////////////////////////////
	glPushMatrix();
	glTranslatef(0.25, 0.2, -0.075);
//	glColor3f(1, 1, 0);
	cylinder(0.05, 0.02, 0.05);
//	glColor3f(0, 0, 0);
	drawCylinderLine(0.049, 0.019, 0.049, 7, 2, "weaponHandle.bmp");
	glPopMatrix();

	glPushMatrix();
	glRotatef(180, 0, 1, 0);
	glTranslatef(0, 0, -width);
	bodyUp(width, filename1);
	bodyUpMid(width, filename2);
	glPopMatrix();

	bodyMid(width, filename1, filename2);
	bodyRight(width, filename1);

	glPushMatrix();
	glRotatef(180, 0, 1, 0);
	glTranslatef(0, 0, -width);
	bodyRight(width, filename1);
	glPopMatrix();
	//lowerbody
	glPushMatrix();
	glTranslatef(0, -0.08, 0);
	lowerBody(width, filename1, filename2);

	glPushMatrix();
	glRotatef(180, 0, 1, 0);
	glTranslatef(0, 0, -width);
	lowerBody(width, filename1, filename2);
	glPopMatrix();
	glPopMatrix();

	glPushMatrix();
	glRotatef(90, 0, 1, 0);
	glTranslatef(-0.2, -0.61, 0.3);
	drawCubeTexture(0.2, 0.15, 0.05, filename2);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(-0.34, -0.61, 0.2);
	glRotatef(90, 0, 1, 0);
	drawCubeTexture(0.2, 0.15, 0.05, filename2);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(-0.05, -0.58, 0.3);
	glRotatef(90, 0, 1, 0);
	drawCubeTexture(0.4, 0.1, 0.1, filename2);
	glPopMatrix();

	//
	//glPushMatrix();
	//glRotatef(180, -1, 0, 0);
	//glTranslatef(0, 0.3, -width);
	//bodyMidTri(width, filename2);
	//glPopMatrix();

	neck(width, filename2);

	glPushMatrix();
	glRotatef(180, 0, 1, 0);
	glTranslatef(0, 0, -0.2);
	neck(width, filename2);
	glPopMatrix();

	thruster(filename1, filename2);

}

void face(float width, LPCSTR filename) {
	GLuint texture;
	texture = loadTexture(filename);

	glBegin(GL_QUADS);
//	glColor3f(0, 0, 1);
	//Face 1£º front
	glTexCoord2f(0, 1);
	glVertex3f(0, -0.27, 0);
	glTexCoord2f(1, 1);
	glVertex3f(-0.1, -0.35, 0);
	glTexCoord2f(1, 0);
	glVertex3f(-0.05, -0.5f, 0);
	glTexCoord2f(0, 0);
	glVertex3f(0, -0.55, 0);

	//Face 2£º right
//	glColor3f(1, 0, 1);
	glTexCoord2f(0, 1);
	glVertex3f(0, -0.55, 0);
	glTexCoord2f(1, 1);
	glVertex3f(0, -0.55, width);
	glTexCoord2f(1, 0);
	glVertex3f(0, -0.27, width);
	glTexCoord2f(0, 0);
	glVertex3f(0, -0.27, 0);


	//Face 3£º top
//	glColor3f(0, 0, 0);
	glTexCoord2f(0, 1);
	glVertex3f(-0.1, -0.35, 0);
	glTexCoord2f(1, 1);
	glVertex3f(0, -0.27, 0);
	glTexCoord2f(1, 0);
	glVertex3f(0, -0.27, width);
	glTexCoord2f(0, 0);
	glVertex3f(-0.1, -0.35, width);
	//Face 4£º left
//	glColor3f(0, 0, 1);
	glTexCoord2f(0, 1);
	glVertex3f(-0.1, -0.35, width);
	glTexCoord2f(1, 1);
	glVertex3f(-0.1, -0.35, 0);
	glTexCoord2f(1, 0);
	glVertex3f(-0.05, -0.5f, 0);
	glTexCoord2f(0, 0);
	glVertex3f(-0.05, -0.5f, width);


	//Face 5£º behind
//	glColor3f(0, 1, 0);
	glTexCoord2f(0, 1);
	glVertex3f(-0.05, -0.5f, width);
	glTexCoord2f(1, 1);
	glVertex3f(-0.1, -0.35, width);
	glTexCoord2f(1, 0);
	glVertex3f(0, -0.27, width);
	glTexCoord2f(0, 0);
	glVertex3f(0, -0.55, width);

	//Face 6£º bottom
//	glColor3f(0, 0, 0);
	glTexCoord2f(0, 1);
	glVertex3f(0, -0.55, width);
	glTexCoord2f(1, 1);
	glVertex3f(0, -0.55, 0);
	glTexCoord2f(1, 0);
	glVertex3f(-0.05, -0.5f, 0);
	glTexCoord2f(0, 0);
	glVertex3f(-0.05, -0.5f, width);

	glEnd();

	glDeleteTextures(1, &texture);
	glDisable(GL_TEXTURE_2D);
}

void mouth(LPCSTR filename) {
	GLuint texture;
	texture = loadTexture(filename);

	glBegin(GL_QUADS);
//	glColor3f(0, 0, 0);
	glTexCoord2f(0, 1);
	glVertex3f(-0.015, -0.45, -0.025);
	glTexCoord2f(1, 1);
	glVertex3f(-0.05, -0.45, 0.02);
	glTexCoord2f(1, 0);
	glVertex3f(-0.05, -0.47, 0.02);
	glTexCoord2f(0, 0);
	glVertex3f(0.01, -0.47, -0.025);
	glEnd();

	glPushMatrix();
	glRotatef(180, 0, 1, 0);
	glBegin(GL_QUADS);
//	glColor3f(0, 0, 0);
	glTexCoord2f(0, 1);
	glVertex3f(-0.015, -0.45, 0.025);
	glTexCoord2f(1, 1);
	glVertex3f(-0.05, -0.45, -0.04);
	glTexCoord2f(1, 0);
	glVertex3f(-0.05, -0.47, -0.04);
	glTexCoord2f(0, 0);
	glVertex3f(0.01, -0.47, 0.025);
	glEnd();
	glPopMatrix();

	//LEFT
	glBegin(GL_QUADS);
//	glColor3f(0, 0, 0);
	glTexCoord2f(0, 1);
	glVertex3f(0, -0.5, -0.02);
	glTexCoord2f(1, 1);
	glVertex3f(-0.05, -0.5, 0.03);
	glTexCoord2f(1, 0);
	glVertex3f(-0.05, -0.475, 0.02);
	glTexCoord2f(0, 0);
	glVertex3f(0, -0.475, -0.015);
	glEnd();

	glBegin(GL_QUADS);
//	glColor3f(0, 0, 0);
	glTexCoord2f(0, 1);
	glVertex3f(0, -0.505, -0.01);
	glTexCoord2f(1, 1);
	glVertex3f(-0.05, -0.505, 0.06);
	glTexCoord2f(1, 0);
	glVertex3f(-0.05, -0.525, 0.05);
	glTexCoord2f(0, 0);
	glVertex3f(0, -0.525, -0.01);
	glEnd();

	//RIGHT
	glPushMatrix();
	glRotatef(180, 0, 1, 0);
	glBegin(GL_QUADS);
//	glColor3f(0, 0, 0);
	glTexCoord2f(0, 1);
	glVertex3f(0, -0.5, 0.02);
	glTexCoord2f(1, 1);
	glVertex3f(-0.05, -0.5, -0.06);
	glTexCoord2f(1, 0);
	glVertex3f(-0.05, -0.475, -0.05);
	glTexCoord2f(0, 0);
	glVertex3f(0, -0.475, 0.015);
	glEnd();


	glBegin(GL_QUADS);
	glBegin(GL_QUADS);
//	glColor3f(0, 0, 0);
	glTexCoord2f(0, 1);
	glVertex3f(0, -0.505, 0.01);
	glTexCoord2f(1, 1);
	glVertex3f(-0.05, -0.505, -0.07);
	glTexCoord2f(1, 0);
	glVertex3f(-0.05, -0.525, -0.06);
	glTexCoord2f(0, 0);
	glVertex3f(0, -0.525, 0.01);
	glEnd();
	glPopMatrix();

	glDeleteTextures(1, &texture);
	glDisable(GL_TEXTURE_2D);
}

void head(float width, LPCSTR filename1, LPCSTR filename2) {
	GLuint textures[2];
	textures[0] = loadTexture(filename1);

	//MASK
	glPushMatrix();
	glTranslatef(0, 0, -0.02);
	glBegin(GL_QUADS);
	//	glColor3f(1, 0, 0);
	glTexCoord2f(0, 1);
	glVertex3f(-0.06, -0.525, 0.025);
	glTexCoord2f(1, 1);
	glVertex3f(-0.095, -0.4, 0.02);
	glTexCoord2f(1, 0);
	glVertex3f(0.03, -0.45, -0.1);
	glTexCoord2f(0, 0);
	glVertex3f(0.03, -0.6, -0.05);
	glEnd();
	glPopMatrix();

	glPushMatrix();
	glTranslatef(0, 0, -0.02);
	glBegin(GL_QUADS);
	//	glColor3f(1, 0, 1);
	glTexCoord2f(0, 1);
	glVertex3f(0.12, -0.525, 0.025);
	glTexCoord2f(1, 1);
	glVertex3f(0.16, -0.4, 0.025);
	glTexCoord2f(1, 0);
	glVertex3f(0.03, -0.45, -0.1);
	glTexCoord2f(0, 0);
	glVertex3f(0.03, -0.6, -0.05);
	glEnd();
	glPopMatrix();

	glBegin(GL_TRIANGLES);
	//	glColor3f(1, 0, 0);
		//Face 1£º top
	glTexCoord2f(0, 0.5);
	glVertex3f(-0.1, -0.349, 0);
	glTexCoord2f(1, 1);
	glVertex3f(0.03, -0.23, 0);
	glTexCoord2f(1, 0);
	glVertex3f(0.03, -0.4, -0.172);


	glEnd();

	glBegin(GL_TRIANGLES);
	//	glColor3f(1, 0, 0);
		//Face 1£º top
	glTexCoord2f(1, 1);
	glVertex3f(0.03, -0.23, 0);
	glTexCoord2f(1, 0);
	glVertex3f(0.03, -0.4, -0.172);
	glTexCoord2f(0, 0.5);
	glVertex3f(0.16, -0.349, 0);

	glEnd();

	//head
	glPushMatrix();
	glTranslatef(0, 0.1, 0);
	glScalef(1.2, 1.2, 1);

	glPushMatrix();
	glTranslatef(0, -0.55, 0);
	drawCube(0.05, 0.28, width);
	glPopMatrix();

	face(width, filename1);
	glPushMatrix();
	glRotatef(180, 0, 1, 0);
	glTranslatef(-0.05, 0, -width);
	face(width, filename1);
	glPopMatrix();


	//glBegin(GL_QUADS);
	//glColor3f(1, 0, 0);
	////Face 1£º top
	//glVertex3f(0.025, -0.249, 0.15);
	//glVertex3f(-0.1, -0.349, 0.1);
	//glVertex3f(-0.1, -0.349, -0.05);
	//glVertex3f(0.025, -0.249, -0.1);

	//glVertex3f(0.025, -0.249, 0.15);
	//glVertex3f(0.15, -0.349, 0.1);
	//glVertex3f(0.15, -0.349, -0.05);
	//glVertex3f(0.025, -0.249, -0.1);

	//glEnd();


	glDeleteTextures(1, &textures[0]);
	glDisable(GL_TEXTURE_2D);
	textures[1] = loadTexture(filename2);

	//eye
	glPushMatrix();
//	glColor3f(1, 1, 0);
	glTranslatef(0.03, -0.07, 0.02);
	glBegin(GL_QUADS);
	glTexCoord2f(0, 1);
	glVertex3f(0, -0.33, -0.2);
	glTexCoord2f(1, 1);
	glVertex3f(-0.13, -0.28, -0.02);
	glTexCoord2f(1, 0);
	glVertex3f(-0.12, -0.35, -0.02);
	glTexCoord2f(0, 0);
	glVertex3f(0, -0.39, -0.15);
	glEnd();

	glPushMatrix();
	glRotatef(180, 0, 1, 0);
	glBegin(GL_QUADS);
	glTexCoord2f(0, 1);
	glVertex3f(0, -0.33, 0.2);
	glTexCoord2f(1, 1);
	glVertex3f(-0.13, -0.28, 0.02);
	glTexCoord2f(1, 0);
	glVertex3f(-0.12, -0.35, 0.02);
	glTexCoord2f(0, 0);
	glVertex3f(0, -0.39, 0.15);
	glEnd();
	glPopMatrix();
	glPopMatrix();


	//ear
	glPushMatrix();
	glRotatef(-20, 0, 0, 1);
	glTranslatef(0.26, -0.35, 0);
//	glColor3f(0, 0, 0);
	drawCube(0.02, 0.15, 0.05);
	glPopMatrix();

	glPushMatrix();
	glRotatef(20, 0, 0, 1);
	glTranslatef(-0.23, -0.37, 0);
	drawCube(0.02, 0.15, 0.05);
	glPopMatrix();


	//mouth
	glPushMatrix();
	glTranslatef(0.03, -0.01, -0.095);
	mouth(filename2);
	glPopMatrix();

	//glPushMatrix();
	//glRotatef(180, 0, 1, 0);
	//glTranslatef(0, 0, 0.02);
	//mouth();
	//glPopMatrix();
	glPopMatrix();
	

	glDeleteTextures(1, &textures[1]);
	glDisable(GL_TEXTURE_2D);

}

void headAndBody(LPCSTR filename1, LPCSTR filename2) {
	glPushMatrix();
	glRotatef(180, 0, 1, 0);
	glTranslatef(0, 0, -0.2);

	glPushMatrix();
	glTranslatef(0, 0.4, 0);
	body(0.2, filename1, filename2);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(-0.02, 1.3, 0.05);
	glRotatef(headRotatex, 0, 1, 0);
	glRotatef(headRotatey, 1, 0, 0);
	head(0.1, filename1, filename2);
	glPopMatrix();
	glPopMatrix();
}

//////////////////////////////////////Weapon
void electricDown() {
	GLuint texture;
	texture = loadTexture("electric.bmp");
	glLineWidth(3);
	glBegin(GL_LINE_STRIP);
	glVertex3f(0.35, -0.2, 0);
	glVertex3f(0.4, -0.25, 0);
	glVertex3f(0.35, -0.3, 0);
	glVertex3f(0.4, -0.35, 0);
	glVertex3f(0.35, -0.4, 0);
	glVertex3f(0.4, -0.45, 0);
	glVertex3f(0.35, -0.5, 0);
	glVertex3f(0.4, -0.55, 0);
	glEnd();
	glDeleteTextures(1, &texture);
	glDisable(GL_TEXTURE_2D);
}

void electricUp() {
	GLuint texture;
	texture = loadTexture("electric.bmp");
	glBegin(GL_LINE_LOOP);
	glVertex3f(0.35, -0.5, -0.1);
	glVertex3f(0.3, -0.5, -0.05);
	glVertex3f(0.25, -0.5, -0.1);
	glVertex3f(0.2, -0.5, -0.05);
	glVertex3f(0.15, -0.5, -0.1);
	glVertex3f(0.1, -0.5, 0);
	glVertex3f(0.05, -0.5, 0.05);
	glVertex3f(0, -0.5, 0.1);
	glVertex3f(-0.05, -0.5, 0.05);
	glVertex3f(-0.1, -0.5, 0);
	glVertex3f(-0.15, -0.5, -0.05);
	glEnd();
	glDeleteTextures(1, &texture);
	glDisable(GL_TEXTURE_2D);
}

void electricFlash() {
	electricDown();
	glPushMatrix();
	glTranslatef(0, -0.1, 0.1);
	glRotatef(45, 1, 0, 0);
	electricDown();

	glPushMatrix();
	glTranslatef(0, 0, 0.25);
	glRotatef(45, 1, 0, 0);
	electricDown();

	glPushMatrix();
	glTranslatef(0, -0.2, 0);
	glRotatef(45, 1, 0, 0);
	electricDown();

	glPushMatrix();
	glRotatef(45, 1, 0, 0);
	electricDown();

	glPushMatrix();
	glRotatef(45, 1, 0, 0);
	electricDown();

	glPushMatrix();
	glTranslatef(0, 0, 0.2);
	glRotatef(45, 1, 0, 0);
	electricDown();

	glPushMatrix();
	glRotatef(15, 1, 0, 0);
	electricDown();

	glPushMatrix();
	glRotatef(45, 1, 0, 0);
	electricDown();

	glPopMatrix();
	glPopMatrix();
	glPopMatrix();
	glPopMatrix();
	glPopMatrix();
	glPopMatrix();
	glPopMatrix();
	glPopMatrix();
	/////////////////////////////////////////////////////
	electricUp();
	glPushMatrix();
	glTranslatef(0, 0.1, 0.1);
	glRotatef(45, 1, 0, 0);
	electricUp();

	glPushMatrix();
	glTranslatef(0, 0, 0.3);
	glRotatef(45, 1, 0, 0);
	electricUp();

	glPushMatrix();
	glTranslatef(0, 0, 0.5);
	glRotatef(90, 1, 0, 0);
	electricUp();

	glPushMatrix();
	glTranslatef(0, -0.3, 0.5);
	glRotatef(90, 1, 0, 0);
	electricUp();

	glPushMatrix();
	glTranslatef(0, 0, 0.5);
	glRotatef(90, 1, 0, 0);
	electricUp();

	glPushMatrix();
	glTranslatef(0, -0.2, 0.9);
	glRotatef(90, 1, 0, 0);
	electricUp();

	glPushMatrix();
	glTranslatef(0, -0.5, 0.5);
	glRotatef(90, 1, 0, 0);
	electricUp();

	glPushMatrix();
	glTranslatef(0, -0.1, 0.5);
	glRotatef(90, 1, 0, 0);
	electricUp();

	glPushMatrix();
	glTranslatef(0, -0.1, 0.5);
	glRotatef(90, 1, 0, 0);
	electricUp();

	glPopMatrix();
	glPopMatrix();
	glPopMatrix();
	glPopMatrix();
	glPopMatrix();
	glPopMatrix();
	glPopMatrix();
	glPopMatrix();
	glPopMatrix();
}

void weaponAttack() {
	elect += electSp;
	if (elect % 2 == 0) {
		electricFlash();
	}
	if (elect > 100) {
		elect = 0;
	}
	elbowLeft = true;
	shoulderTurnLeft = true;
	if (weaponAttackDown == true) {
		shoulderTurnLeft = false;
		elbowLeft = false;
	}
}

void weapon()
{
	glPushMatrix();
	if (weaponBig == true) {
		if (weaponScale < 1) {
			weaponScale += weaponSp;

		}
		glTranslatef(0.6, -1, 0.05);
		glScalef(weaponScale, weaponScale, weaponScale);

		if (weaponAttackMove == true) {
			weaponAttack();
		}
	}
	if (weaponBig == false) {
		glTranslatef(0.4, 0.05, 0.07);
		glScalef(0.2, 0.1, 0.2);
	}
	/////////////////////////////////////handle
	glPushMatrix();
	glRotatef(270, 1, 0, 0);
	cylinderTexture(0.05, 0.05, 1.5, "weaponHandle.bmp");
	glPopMatrix();

	////////////////////////////////////////////////sphere fill handle at back
	glPushMatrix();
	glTranslatef(0, 1.47, 0);
	sphereTexture(0.05, "weaponHandle.bmp");
	glPopMatrix();

	//////////////////////////////////////in front handle
	glPushMatrix();
	glTranslatef(-0.3, -0.2, 0);
	glRotatef(90, 0, 1, 0);
	cylinderTexture(0.2, 0.2, 0.6, "weaponLogo.bmp");
	glPopMatrix();

	//////////////////////////////////////top left in front handle, red color one
	glPushMatrix();
	glTranslatef(-0.3, -0.2, 0);
	glRotatef(90, 0, 1, 0);
	cylinderTexture(0.205, 0.205, 0.15, "weaponLogo.bmp");
	glPopMatrix();

	//////////////////////////////////////top right in front handle
	glPushMatrix();
	glTranslatef(0.15, -0.2, 0);
	glRotatef(90, 0, 1, 0);
	cylinderTexture(0.205, 0.205, 0.15, "weaponLogo.bmp");
	glPopMatrix();

	////////////////////////////////////////////////right side sphere to fill cylinder 
	glPushMatrix();
	glTranslatef(0.3, -0.2, 0);
	glScalef(0.1, 1, 1);
	sphereTexture(0.2, "weapon.bmp");
	glPopMatrix();

	////////////////////////////////////////////////left side sphere to fill cylinder 
	glPushMatrix();
	glTranslatef(-0.3, -0.2, 0);
	glScalef(0.1, 1, 1);
	sphereTexture(0.2, "weapon.bmp");
	glPopMatrix();

	////////////////////////////////////////////////////////connect handle and hammer
	glPushMatrix();
	glRotatef(270, 1, 0, 0);
	cylinderTexture(0.07, 0.052, 0.1, "weaponFront.bmp");
	glPopMatrix();
	glPopMatrix();
}

void weaponTrans() {
	glPushMatrix();
	if (weaponTransUp <= 1.1) {
		weaponTransUp += weaponSp;
	}
	glTranslatef(0, -weaponTransUp, 0);

	glPushMatrix();
	if (weaponTransUp > 1.1 && weaponTransFront <= 0.15) {
		weaponTransFront += gunSp;
	}
	glTranslatef(weaponTransFront, 0, 0);

	if (weaponTransFront > 0.15) {
		weaponBig = true;
		weaponShow = false;
	}
	weapon();
	glPopMatrix();
	glPopMatrix();
}

//////////////////////////////////////Gun
void gunShoot() {
	glPushMatrix();
	if (bulletMove < 3) {
		bulletMove += bulletSp;
		glTranslatef(bulletMove, 0, 0);
	}

	glPushMatrix();
	glTranslatef(0.8, 0.035, 0.05);
	glRotatef(-90, 0, 1, 0);
	cylinderTexture(0.001, 0.023, 0.05, "bullet.bmp");

	glPushMatrix();
	glTranslatef(0, 0, 0.05);
	cylinderTexture(0.023, 0.023, 0.1, "bullet.bmp");
	glPopMatrix();
	glPopMatrix();
	glPopMatrix();
}

void gun() {
	glPushMatrix();
	if (gunBig == true) {
		if (gunScale < 1) {
			gunScale += gunSp;

		}
		glTranslatef(1, 0.35, 0);
		glScalef(gunScale, gunScale, gunScale);

		if (gunShootMove == true) {
			gunShoot();
		}
	}
	if (gunBig == false) {
		glTranslatef(0.3, 0.1, 0.08);
		glScalef(0.2, 0.2, 0.2);
	}

	//////////////////////////////////////////////////front part
	drawCubeTexture(0.35, 0.2, 0.1, "gunFront.bmp");

	//////////////////////////////////////////////////////hand to hold
	glPushMatrix();
	glRotatef(5, 0, 0, 1);
	glTranslatef(0.4, 0.01, 0.05);
	glScalef(1.3, 0.5, 0.35);
	sphereTexture(0.15, "gunFrFrHandle.bmp");
	glPopMatrix();

	/////////////////////////////////////////////////////down
	glPushMatrix();
	glTranslatef(0.2, -0.03, -0.02);
	drawCubeTexture(0.2, 0.13, 0.13, "gunFrFrHandle.bmp");
	glPopMatrix();

	/////////////////////////////////////////////////////front
	glPushMatrix();
	glTranslatef(0.25, 0.06, -0.005);
	drawCubeTexture(0.3, 0.15, 0.11, "gunFrontUp.bmp");
	glPopMatrix();

	/////////////////////////////////////////////front up
	glPushMatrix();
	glTranslatef(0.55, 0.05, -0.005);
	drawCubeTexture(0.2, 0.15, 0.11, "gunFrontUp.bmp");
	glPopMatrix();

	///////////////////////////////////////front down
	glPushMatrix();
	glTranslatef(0.55, 0.05, 0.05);
	glRotatef(90, 0, 0, -1);
	glRotatef(270, 1, 0, 0);
	cylinderTexture(0.05, 0.05, 0.17, "gunFrontUp.bmp");
	glPopMatrix();

	/////////////////////////////////////////place back and to shoot
	glPushMatrix();
	glTranslatef(0.65, 0.035, 0.05);
	glRotatef(90, 0, 0, -1);
	glRotatef(270, 1, 0, 0);
	cylinderTexture(0.025, 0.025, 0.25, "gunAim.bmp");
	glPopMatrix();

	glPushMatrix();
	glTranslatef(0.9, 0.035, 0.05);
	glRotatef(90, 0, 0, -1);
	glRotatef(270, 1, 0, 0);
	cylinderTexture(0.035, 0.035, 0.1, "gunShoot.bmp");
	glPopMatrix();

	/////////////////////////////////////////place to aim
	glPushMatrix();
	glTranslatef(0.75, 0.15, 0.05);
	glRotatef(90, 0, 0, -1);
	glRotatef(270, 1, 0, 0);
	cylinderTexture(0.025, 0.025, 0.05, "gunAim.bmp");
	glPopMatrix();

	glPushMatrix();
	glTranslatef(0.7, 0.24, 0.05);
	glRotatef(90, 0, 0, -1);
	glRotatef(270, 1, 0, 0);
	cylinderTexture(0.04, 0.04, 0.03, "gunAim.bmp");
	glPopMatrix();

	///////////////////////////////////body
	glPushMatrix();
	glTranslatef(-0.5, -0.05, 0);
	drawCubeTexture(0.5, 0.2, 0.1, "gunBody.bmp");
	glPopMatrix();

	//////////////////////////////////body down
	glPushMatrix();
	glTranslatef(-0.17, -0.15, 0);
	drawCubeTexture(0.15, 0.1, 0.1, "gunFrHandle.bmp");

	glPushMatrix();
	glTranslatef(0.02, -0.15, 0);
	drawCubeTexture(0.11, 0.15, 0.1, "gunFrHandle.bmp");
	glPopMatrix();
	glPopMatrix();

	////////////////////////////////body up
	glPushMatrix();
	glTranslatef(-0.34, 0.23, 0.05);
	glRotatef(90, 0, 0, -1);
	glRotatef(270, 1, 0, 0);
	cylinderTexture(0.02, 0.02, 0.3, "gunBodyUp.bmp");
	glPopMatrix();

	glPushMatrix();
	glTranslatef(0.01, 0.135, 0.05);
	glRotatef(30, 0, 0, 1);
	glRotatef(270, 1, 0, 0);
	cylinderTexture(0.02, 0.02, 0.12, "gunBodyUp.bmp");
	glPopMatrix();

	glPushMatrix();
	glTranslatef(-0.44, 0.135, 0.05);
	glRotatef(50, 0, 0, -1);
	glRotatef(270, 1, 0, 0);
	cylinderTexture(0.02, 0.02, 0.15, "gunBodyUp.bmp");
	glPopMatrix();

	///////////////////////////////handle
	glPushMatrix();
	glTranslatef(-0.4, -0.3, 0.05);
	glRotatef(270, 1, 0, 0);
	cylinderTexture(0.035, 0.035, 0.3, "gunHandle.bmp");
	glPopMatrix();

	///////////////////////////////////////back
	glPushMatrix();                     //up
	glTranslatef(-0.9, 0.1, 0.05);
	glRotatef(90, 0, 0, -1);
	glRotatef(270, 1, 0, 0);
	cylinderTexture(0.02, 0.02, 0.4, "gunBodyBack.bmp");
	glPopMatrix();

	glPushMatrix();                     //down
	glTranslatef(-0.7, 0, 0.05);
	glRotatef(90, 0, 0, -1);
	glRotatef(270, 1, 0, 0);
	cylinderTexture(0.02, 0.02, 0.2, "gunBodyBack.bmp");
	glPopMatrix();

	glPushMatrix();
	glTranslatef(-0.8, -0.06, 0.05);
	glRotatef(60, 0, 0, -1);
	glRotatef(270, 1, 0, 0);
	cylinderTexture(0.02, 0.02, 0.13, "gunBodyBack.bmp");
	glPopMatrix();

	glPushMatrix();
	glTranslatef(-0.89, -0.06, 0.05);
	glRotatef(90, 0, 0, -1);
	glRotatef(270, 1, 0, 0);
	cylinderTexture(0.02, 0.02, 0.1, "gunBodyBack.bmp");
	glPopMatrix();

	glPushMatrix();      //back
	glTranslatef(-0.93, -0.08, 0.02);
	drawCubeTexture(0.05, 0.2, 0.05, "gunBodyBackBk.bmp");
	glPopMatrix();
	glPopMatrix();
}

void gunTrans() {
	glPushMatrix();
	if (gunTransUp <= 0.3) {
		gunTransUp += gunSp;
	}
	glTranslatef(0, gunTransUp, 0);

	glPushMatrix();
	if (gunTransUp > 0.3 && gunTransFront <= 0.7) {
		gunTransFront += gunSp;
	}
	glTranslatef(gunTransFront, 0, 0);

	glPushMatrix();
	if (gunTransFront > 0.6 && gunTransNear <= 0.1) {
		gunTransNear += gunSp;
	}
	glTranslatef(0, 0, -gunTransNear);

	if (gunTransNear > 0.1) {
		gunBig = true;
		gunShow = false;
	}
	gun();
	glPopMatrix();
	glPopMatrix();
	glPopMatrix();
}

//////////////////////////////////////hand
void shoulder(float sizex, float sizey, float sizez) {
	GLuint texture;
	texture = loadTexture("black steel.bmp");

	glBegin(GL_QUADS);
	//Face size : Bottom
	glTexCoord2f(0.0f, 0.0f);
	glVertex3f(0.2, -0.1, 0.5);
	glTexCoord2f(0.0f, 1.0f);
	glVertex3f(0.2, -0.1, 0.05);
	glTexCoord2f(1.0f, 1.0f);
	glVertex3f(sizex, 0.1, 0.05);
	glTexCoord2f(1.0f, 0.0f);
	glVertex3f(sizex, 0.1, 0.5);

	//Face 2 : Left
	glTexCoord2f(1.0f, 0.0f);
	glVertex3f(0.2, -0.1, 0.05);
	glTexCoord2f(0.0f, 0.0f);
	glVertex3f(0, sizey, 0);
	glTexCoord2f(0.0f, 1.0f);
	glVertex3f(0, sizey, sizez);
	glTexCoord2f(1.0f, 1.0f);
	glVertex3f(0.2, -0.1, 0.5);

	//Top
	glTexCoord2f(0.0f, 0.0f);
	glVertex3f(0, sizey, sizez);
	glTexCoord2f(1.0f, 0.0f);
	glVertex3f(0, sizey, 0);
	glTexCoord2f(0.0f, 1.0f);
	glVertex3f(0, 0.75, 0);
	glTexCoord2f(1.0f, 1.0f);
	glVertex3f(0, 0.75, sizez);

	glTexCoord2f(1.0f, 1.0f);
	glVertex3f(sizex, 0.7, sizez);
	glTexCoord2f(1.0f, 1.0f);
	glVertex3f(sizex, 0.7, 0);
	glTexCoord2f(0.0f, 0.0f);
	glVertex3f(0.4, 0.7, 0);
	glTexCoord2f(0.0f, 1.0f);
	glVertex3f(0.4, 0.7, sizez);

	glTexCoord2f(0.0f, 0.0f);
	glVertex3f(0, 0.75, 0);
	glTexCoord2f(0.0f, 1.0f);
	glVertex3f(0, 0.75, sizez);
	glTexCoord2f(1.0f, 1.0f);
	glVertex3f(0.4, 0.7, sizez);
	glTexCoord2f(1.0f, 0.0f);
	glVertex3f(0.4, 0.7, 0);

	//Front and Back
	glTexCoord2f(0.0f, 0.0f);
	glVertex3f(0.2, -0.1, 0.5);
	glTexCoord2f(0.0f, 1.0f);
	glVertex3f(0.35, sizey, 0.75);
	glTexCoord2f(1.0f, 1.0f);
	glVertex3f(sizex, sizey, 0.75);
	glTexCoord2f(1.0f, 0.0f);
	glVertex3f(sizex, 0.1, 0.5);

	glTexCoord2f(0.0f, 0.0f);
	glVertex3f(0.35, sizey, -0.25);
	glTexCoord2f(0.0f, 1.0f);
	glVertex3f(0.2, -0.1, 0.05);
	glTexCoord2f(1.0f, 1.0f);
	glVertex3f(sizex, 0.1, 0.05);
	glTexCoord2f(1.0f, 0.0f);
	glVertex3f(sizex, sizey, -0.25);

	//////////////////////////////////////
	glTexCoord2f(0.0f, 0.0f);
	glVertex3f(0, sizey, sizez);
	glTexCoord2f(0.0f, 1.0f);
	glVertex3f(0.35, sizey, 0.75);
	glTexCoord2f(1.0f, 1.0f);
	glVertex3f(0.4, 0.7, sizez);
	glTexCoord2f(1.0f, 0.0f);
	glVertex3f(0, 0.75, sizez);

	glTexCoord2f(0.0f, 0.0f);
	glVertex3f(0.35, sizey, 0.75);
	glTexCoord2f(0.0f, 1.0f);
	glVertex3f(0.4, 0.7, sizez);
	glTexCoord2f(1.0f, 1.0f);
	glVertex3f(sizex, 0.7, sizez);
	glTexCoord2f(1.0f, 0.0f);
	glVertex3f(sizex, sizey, 0.75);

	///////////////////////////////////////
	glTexCoord2f(0.0f, 0.0f);
	glVertex3f(0, sizey, 0);
	glTexCoord2f(0.0f, 1.0f);
	glVertex3f(0.35, sizey, -0.25);
	glTexCoord2f(1.0f, 1.0f);
	glVertex3f(0.4, 0.7, 0);
	glTexCoord2f(1.0f, 0.0f);
	glVertex3f(0, 0.75, 0);

	glTexCoord2f(0.0f, 0.0f);
	glVertex3f(0.4, 0.7, 0);
	glTexCoord2f(0.0f, 1.0f);
	glVertex3f(0.35, sizey, -0.25);
	glTexCoord2f(1.0f, 1.0f);
	glVertex3f(sizex, sizey, -0.25);
	glTexCoord2f(1.0f, 0.0f);
	glVertex3f(sizex, 0.7, 0);
	glEnd();

	glBegin(GL_TRIANGLES);
	//Face 3 : Front
	glTexCoord2f(0.5f, 1.0f);
	glVertex3f(0.2, -0.1, 0.5);
	glTexCoord2f(0.0f, 0.0f);
	glVertex3f(0, sizey, sizez);
	glTexCoord2f(1.0f, 0.0f);
	glVertex3f(0.35, sizey, 0.75);

	//Face 6 : Back
	glTexCoord2f(1.0f, 0.0f);
	glVertex3f(0.35, sizey, -0.25);
	glTexCoord2f(0.5f, 1.0f);
	glVertex3f(0.2, -0.1, 0.05);
	glTexCoord2f(0.0f, 0.0f);
	glVertex3f(0, sizey, 0);
	glEnd();

	glBegin(GL_POLYGON);
	//Face 4 : Right
	glTexCoord2f(0.02f, 1.0f);
	glVertex3f(sizex, 0.7, sizez);
	glTexCoord2f(0.0f, 0.7f);
	glVertex3f(sizex, 0.7, 0);
	glTexCoord2f(0.2f, 0.0f);
	glVertex3f(sizex, sizey, -0.25);
	glTexCoord2f(0.8f, 0.0f);
	glVertex3f(sizex, 0.1, 0.05);
	glTexCoord2f(1.0f, 0.7f);
	glVertex3f(sizex, 0.1, 0.5);
	glTexCoord2f(0.8f, 1.0f);
	glVertex3f(sizex, sizey, 0.75);
	glEnd();
	glDeleteTextures(1, &texture);
	glDisable(GL_TEXTURE_2D);
}

void UpHand(float sizex, float sizey, float sizez) {

	glPushMatrix();
	glTranslatef(0, 0.025, 0.025);

	glPushMatrix();                         //sphere 1 connect
	glTranslatef(0.1, 0.07, 0.07);
	sphereTexture(0.07, "black steel.bmp");
	glPopMatrix();

	glPushMatrix();
	glTranslatef(-0.7, 0, 0);

	glPushMatrix();
	glTranslatef(0.7, 0.15, 0);
	glRotatef(180, 0, 0, 1);

	GLuint texture;
	texture = loadTexture("black steel.bmp");

	glBegin(GL_TRIANGLES);    //Pyramid connection
	glTexCoord2f(0.0f, 0.0f);
	glVertex3f(0, 0, 0);
	glTexCoord2f(1.0f, 0.0f);
	glVertex3f(0, 0.15, 0);
	glTexCoord2f(0.5f, 1.0f);
	glVertex3f(-0.1, 0.075, 0.075);

	glTexCoord2f(0.5f, 1.0f);
	glVertex3f(-0.1, 0.075, 0.075);
	glTexCoord2f(0.0f, 0.0f);
	glVertex3f(0, 0.15, 0);
	glTexCoord2f(1.0f, 0.0f);
	glVertex3f(0, 0.15, 0.15);

	glTexCoord2f(0.0f, 0.0f);
	glVertex3f(0, 0.15, 0.15);
	glTexCoord2f(0.5f, 1.0f);
	glVertex3f(-0.1, 0.075, 0.075);
	glTexCoord2f(0.0f, 1.0f);
	glVertex3f(0, 0, 0.15);

	glTexCoord2f(0.0f, 0.0f);
	glVertex3f(0, 0, 0.15);
	glTexCoord2f(0.5f, 1.0f);
	glVertex3f(-0.1, 0.075, 0.075);
	glTexCoord2f(0.0f, 1.0f);
	glVertex3f(0, 0, 0);
	glEnd();

	glDeleteTextures(1, &texture);
	glDisable(GL_TEXTURE_2D);
	glPopMatrix();

	glPushMatrix();              //Big Quads Arm
	glTranslatef(0.2, 0, 0);
	drawCubeTexture(sizex, sizey, sizez, "black steel.bmp");
	glDisable(GL_TEXTURE_2D);
	glPopMatrix();
	glPopMatrix();
	glPopMatrix();
}

void robotArmDecor1(float point, float point2) {
	glBegin(GL_QUADS);
	glTexCoord2f(0.0f, 0.0f);
	glVertex3f(0, 0, 0);
	glTexCoord2f(1.0f, 1.0f);
	glVertex3f(0.5, 0.03, point);
	glTexCoord2f(0.0f, 1.0f);
	glVertex3f(0, 0.1, 0);
	glTexCoord2f(1.0f, 0.0f);
	glVertex3f(0, 0, point2);

	glTexCoord2f(0.0f, 0.0f);
	glVertex3f(0, 0.2, 0);
	glTexCoord2f(1.0f, 1.0f);
	glVertex3f(0.5, 0.17, point);
	glTexCoord2f(0.0f, 1.0f);
	glVertex3f(0, 0.1, 0);
	glTexCoord2f(1.0f, 0.0f);
	glVertex3f(0, 0.2, point2);
	glEnd();

	glBegin(GL_TRIANGLES);
	glTexCoord2f(0.0f, 0.0f);
	glVertex3f(0, 0, 0);
	glTexCoord2f(0.5f, 1.0f);
	glVertex3f(0, 0.1, 0);
	glTexCoord2f(0.0f, 0.0f);
	glVertex3f(0, 0, 0.1);

	glTexCoord2f(0.0f, 0.0f);
	glVertex3f(0, 0.2, 0);
	glTexCoord2f(1.0f, 0.0f);
	glVertex3f(0, 0.1, 0);
	glTexCoord2f(0.5f, 1.0f);
	glVertex3f(0, 0.2, 0.1);
	glEnd();
}

void robotArmDecor2() {
	glBegin(GL_QUADS);
	glTexCoord2f(0.0f, 0.0f);
	glVertex3f(0, 0, 0.2);
	glTexCoord2f(1.0f, 1.0f);
	glVertex3f(0.5, 0.03, 0.17);
	glTexCoord2f(1.0f, 0.0f);
	glVertex3f(0, 0.1, 0.2);
	glTexCoord2f(0.0f, 1.0f);
	glVertex3f(0, 0, 0.3);

	glVertex3f(0, 0.2, 0.2);
	glVertex3f(0.5, 0.17, 0.17);
	glVertex3f(0, 0.1, 0.2);
	glVertex3f(0, 0.2, 0.3);
	glEnd();

	glBegin(GL_TRIANGLES);
	glVertex3f(0, 0, 0.2);
	glVertex3f(0, 0.1, 0.2);
	glVertex3f(0, 0, 0.3);

	glVertex3f(0, 0.2, 0.2);
	glVertex3f(0, 0.1, 0.2);
	glVertex3f(0, 0.2, 0.3);
	glEnd();
}

void robotArm(float sizex, float sizey, float sizez)
{
	GLuint texture;
	texture = loadTexture("black steel.bmp");

	glBegin(GL_TRIANGLES);    //Pyramid on Arm
	glTexCoord2f(0.0f, 0.0f);
	glVertex3f(0, 0, 0);
	glTexCoord2f(1.0f, 0.0f);
	glVertex3f(0, 0.2, 0);
	glTexCoord2f(0.5f, 1.0f);
	glVertex3f(-0.1, 0.1, 0.1);

	glTexCoord2f(0.5f, 1.0f);
	glVertex3f(-0.1, 0.1, 0.1);
	glTexCoord2f(0.0f, 0.0f);
	glVertex3f(0, 0.2, 0);
	glTexCoord2f(1.0f, 0.0f);
	glVertex3f(0, 0.2, 0.2);

	glTexCoord2f(0.0f, 0.0f);
	glVertex3f(0, 0.2, 0.2);
	glTexCoord2f(0.5f, 1.0f);
	glVertex3f(-0.1, 0.1, 0.1);
	glTexCoord2f(1.0f, 0.0f);
	glVertex3f(0, 0, 0.2);

	glTexCoord2f(0.0f, 0.0f);
	glVertex3f(0, 0, 0.2);
	glTexCoord2f(0.5f, 1.0f);
	glVertex3f(-0.1, 0.1, 0.1);
	glTexCoord2f(1.0f, 0.0f);
	glVertex3f(0, 0, 0);
	glEnd();
	glDeleteTextures(1, &texture);
	glDisable(GL_TEXTURE_2D);

	glPushMatrix();         //Big Quads Arm
	texture = loadTexture("black steel.bmp");

	glBegin(GL_QUADS);
	//Face size : Bottom
	glTexCoord2f(0.0f, 1.0f);
	glVertex3f(0, 0, sizez);
	glTexCoord2f(1.0f, 1.0f);
	glVertex3f(sizex, 0.03, 0.17);
	glTexCoord2f(1.0f, 0.0f);
	glVertex3f(sizex, 0.03, 0.03);
	glTexCoord2f(0.0f, 0.0f);
	glVertex3f(0, 0, 0);

	//Face 2 : Left
	glTexCoord2f(0.0f, 0.0f);
	glVertex3f(0, 0, 0);
	glTexCoord2f(0.0f, 1.0f);
	glVertex3f(0, sizey, 0);
	glTexCoord2f(1.0f, 1.0f);
	glVertex3f(0, sizey, sizez);
	glTexCoord2f(1.0f, 0.0f);
	glVertex3f(0, 0, sizez);

	//Face 3 : Front
	glTexCoord2f(0.0f, 0.0f);
	glVertex3f(0, 0, sizez);
	glTexCoord2f(0.0f, 1.0f);
	glVertex3f(0, sizey, sizez);
	glTexCoord2f(1.0f, 1.0f);
	glVertex3f(sizex, 0.17, 0.17);
	glTexCoord2f(1.0f, 0.0f);
	glVertex3f(sizex, 0.03, 0.17);

	//Face 4 : Right
	glTexCoord2f(1.0f, 0.0f);
	glVertex3f(sizex, 0.03, 0.17);
	glTexCoord2f(0.0f, 0.0f);
	glVertex3f(sizex, 0.03, 0.03);
	glTexCoord2f(0.0f, 1.0f);
	glVertex3f(sizex, 0.17, 0.03);
	glTexCoord2f(1.0f, 1.0f);
	glVertex3f(sizex, 0.17, 0.17);

	//Face 5 : Top
	glTexCoord2f(1.0f, 1.0f);
	glVertex3f(sizex, 0.17, 0.17);
	glTexCoord2f(0.0f, 1.0f);
	glVertex3f(0, sizey, sizez);
	glTexCoord2f(0.0f, 0.0f);
	glVertex3f(0, sizey, 0);
	glTexCoord2f(1.0f, 0.0f);
	glVertex3f(sizex, 0.17, 0.03);

	//Face 6 : Back
	glTexCoord2f(1.0f, 1.0f);
	glVertex3f(sizex, 0.17, 0.03);
	glTexCoord2f(1.0f, 0.0f);
	glVertex3f(sizex, 0.03, 0.03);
	glTexCoord2f(0.0f, 0.0f);
	glVertex3f(0, 0, 0);
	glTexCoord2f(0.0f, 1.0f);
	glVertex3f(0, sizey, 0);
	glEnd();
	glDeleteTextures(1, &texture);
	glDisable(GL_TEXTURE_2D);
	glPopMatrix();

	/////////////////////////////////////decoration
	texture = loadTexture("white.bmp");

	glPushMatrix();              //first
	glTranslatef(0, 0, -0.0001);
	robotArmDecor1(0.03, -0.1);
	glPopMatrix();

	glPushMatrix();               //second
	glTranslatef(0, -0.0001, 0);
	glRotatef(90, 1, 0, 0);
	robotArmDecor1(-0.03, 0.1);
	glPopMatrix();

	glPushMatrix();              //third
	glTranslatef(0, 0, 0.0001);
	robotArmDecor2();
	glPopMatrix();

	glPushMatrix();               //fourth
	glTranslatef(0, 0.0001, 0.2);
	glRotatef(-90, 1, 0, 0);
	robotArmDecor2();
	glPopMatrix();
	glDeleteTextures(1, &texture);
	glDisable(GL_TEXTURE_2D);
}

void palm(float sizex, float sizey, float sizez) {
	glPushMatrix();
	glTranslatef(0.5, 0, 0.1);
	drawCubeTexture(sizex, sizey, sizez, "black steel.bmp");
	glDisable(GL_TEXTURE_2D);
	glPopMatrix();
}

void thumb(float sizex, float sizey, float sizez) {
	glPushMatrix();
	glTranslatef(0.5, 0.2, 0.1);
	drawCubeTexture(sizex, sizey, sizez, "white.bmp");
	glDisable(GL_TEXTURE_2D);
	glPopMatrix();
}

void robotFinger(float sizex, float sizey, float sizez)
{
	glPushMatrix();                     //finger connector
	glTranslatef(0.636, 0.025, 0.12);
	sphereTexture(0.02, "white.bmp");                       //first

	glPushMatrix();
	glTranslatef(0, 0.05, 0);
	sphereTexture(0.02, "white.bmp");

	glPushMatrix();
	glTranslatef(0, 0.05, 0);
	sphereTexture(0.02, "white.bmp");

	glPushMatrix();
	glTranslatef(0, 0.05, 0);
	sphereTexture(0.02, "white.bmp");

	glPopMatrix();
	glPopMatrix();
	glPopMatrix();
	glPopMatrix();

	glPushMatrix();                        //first
	glTranslatef(0.65, 0.05, 0.1);
	glRotatef(-90, 0, 0, 1);
	glScalef(0.1, 0.1, 0.1);
	drawCubeTexture(sizex, sizey, sizez, "white.bmp");

	glPushMatrix();	                       //second
	glTranslatef(-0.5, 0, 0);
	drawCubeTexture(sizex, sizey, sizez, "white.bmp");

	glPushMatrix();	                       //third
	glTranslatef(-0.5, 0, 0);
	drawCubeTexture(sizex, sizey, sizez, "white.bmp");

	glPushMatrix();	                       //fourth
	glTranslatef(-0.5, 0, 0);
	drawCubeTexture(sizex, sizey, sizez, "white.bmp");
	glPopMatrix();
	glPopMatrix();
	glPopMatrix();
	glPopMatrix();
}

void leftHand() {
	glPushMatrix();               //rotate shoulder up
	if (shoulderTurnLeft == true) {
		if (actShoulderLeft < 90) {
			actShoulderLeft += rotatfinger;
		}
		glTranslatef(-0.6, 0.13, 0);
		glRotatef(actShoulderLeft, 0, 0, -1);
		glTranslatef(0.6, -0.13, 0);
		if (actShoulderLeft > 90) {
			weaponAttackDown = true;
		}
	}
	else if (shoulderTurnLeft == false) {
		if (actShoulderLeft > 0) {
			actShoulderLeft -= rotatfinger;
		}
		glTranslatef(-0.6, 0.13, 0);
		glRotatef(actShoulderLeft, 0, 0, -1);
		glTranslatef(0.6, -0.13, 0);
	}

	glPushMatrix();
	glTranslatef(-0.6, 0.1, -0.05);
	glScalef(0.7, 0.7, 1);
	sphereTexture(0.1, "Rusty black steel.bmp");
	glPopMatrix();

	glPushMatrix();
	glTranslatef(-0.75, -0.045, 0.32);//0.225);

	glRotatef(-90, 1, 0, 0);
	glScalef(0.5, 0.5, 0.5);

	shoulder(0.55, 0.55, 0.55);         //shoulder
	glPopMatrix();

	UpHand(0.5, 0.15, 0.15);      //UpHand

	glPushMatrix();               //rotate elbow up
	if (elbowLeft == true) {
		if (actRotateElbowLeft > -90) {
			actRotateElbowLeft -= rotatfinger;
		}

		glRotatef(actRotateElbowLeft, 0, 0, 1);
	}
	else if (elbowLeft == false) {
		if (actRotateElbowLeft < 0) {
			actRotateElbowLeft += rotatfinger;
		}

		glRotatef(actRotateElbowLeft, 0, 0, 1);
	}

	if (weaponShow == true)                              ///////Weapon
		weaponTrans();

	if (weaponBig == true) {
		weapon();
		fingerLeft = true;
	}

	robotArm(0.5, 0.2, 0.2);                   //arm

	glPushMatrix();
	glTranslatef(0, 0.02, 0);
	glScalef(1, 0.8, 1);
	palm(0.15, 0.2, 0.05);             //palm
	glPopMatrix();

	glPushMatrix();                        //left thumb connector
	glTranslatef(0.525, 0.01, 0.125);
	sphereTexture(0.02, "white.bmp");
	glPopMatrix();

	glPushMatrix();
	glTranslatef(0, 0.02, 0);
	glScalef(1, 0.8, 1);
	if (fingerLeft == true) {
		if (actRotatThumbLeft <= 90) {                     //rotate thumb
			actRotatThumbLeft += rotatThumb;
		}
		glTranslatef(0, 0, 0.1);
		glRotatef(actRotatThumbLeft, 1, 0, 0);
		glTranslatef(0, 0, -0.1);
	}
	else if (fingerLeft == false) {
		if (actRotatThumbLeft > 0) {                    //rotate thumb
			actRotatThumbLeft -= rotatThumb;
		}
		glTranslatef(0, 0, 0.1);
		glRotatef(actRotatThumbLeft, 1, 0, 0);
		glTranslatef(0, 0, -0.1);
	}

	glTranslatef(0, -0.25, 0);

	thumb(0.05, 0.06, 0.05);            //thumb
	glPopMatrix();
	/////////////////////////////////////////////////////////
	glPushMatrix();                     //first finger
	glTranslatef(0, 0.02, 0);
	glScalef(1, 0.8, 1);

	if (fingerLeft == true) {
		if (actRotatFingLeft <= 80) {           //rotate finger1
			actRotatFingLeft += rotatfinger;
		}

		glTranslatef(0.65, 0, 0.1);
		glRotatef(actRotatFingLeft, 0, 1, 0);
		glTranslatef(-0.65, 0, -0.1);
	}
	else if (fingerLeft == false) {
		if (actRotatFingLeft > 0) {           //rotate finger1
			actRotatFingLeft -= rotatfinger;
		}

		glTranslatef(0.65, 0, 0.1);
		glRotatef(actRotatFingLeft, 0, 1, 0);
		glTranslatef(-0.65, 0, -0.1);
	}
	robotFinger(0.5, 0.7, 0.5);

	/////////////////////////
	glPushMatrix();
	glTranslatef(0.05, 0, 0);

	if (fingerLeft == true) {
		if (actRotatFing2Left <= 45) {           //rotate finger2
			actRotatFing2Left += rotatfinger;
		}

		glTranslatef(0.65, 0, 0.09);
		glRotatef(actRotatFing2Left, 0, 1, 0);
		glTranslatef(-0.65, 0, -0.09);
	}
	else if (fingerLeft == false) {
		if (actRotatFing2Left > 0) {           //rotate finger2
			actRotatFing2Left -= rotatfinger;
		}

		glTranslatef(0.65, 0, 0.09);
		glRotatef(actRotatFing2Left, 0, 1, 0);
		glTranslatef(-0.65, 0, -0.09);
	}

	robotFinger(0.5, 0.7, 0.5);
	/////////////////////////////////////
	glPushMatrix();
	glTranslatef(0.05, 0, 0);

	if (fingerLeft == true) {
		if (actRotatFing3Left <= 60) {           //rotate finger3
			actRotatFing3Left += rotatfinger;
		}

		glTranslatef(0.65, 0, 0.09);
		glRotatef(actRotatFing3Left, 0, 1, 0);
		glTranslatef(-0.65, 0, -0.09);
	}
	else if (fingerLeft == false) {
		if (actRotatFing3Left > 0) {           //rotate finger3
			actRotatFing3Left -= rotatfinger;
		}

		glTranslatef(0.65, 0, 0.09);
		glRotatef(actRotatFing3Left, 0, 1, 0);
		glTranslatef(-0.65, 0, -0.09);
	}

	robotFinger(0.5, 0.7, 0.5);
	glPopMatrix();
	glPopMatrix();
	glPopMatrix();
	glPopMatrix();
	glPopMatrix();
}

void rightHand() {
	glPushMatrix();               //rotate shoulder up
	if (shoulderTurnRight == true) {
		if (actShoulderRight > -90) {
			actShoulderRight -= rotatfinger;
		}
		glTranslatef(-0.6, 0.11, 0);
		glRotatef(actShoulderRight, 0, 0, -1);
		glTranslatef(0.6, -0.11, 0);
	}
	else if (shoulderTurnRight == false) {
		if (actShoulderRight < 0) {
			actShoulderRight += rotatfinger;
		}
		glTranslatef(-0.6, 0.11, 0);
		glRotatef(actShoulderRight, 0, 0, -1);
		glTranslatef(0.6, -0.11, 0);
	}

	glPushMatrix();                 //connect shoulder and body
	glTranslatef(-0.6, 0.1, -0.05);
	glScalef(0.7, 0.7, 1);
	sphereTexture(0.1, "Rusty black steel.bmp");
	glPopMatrix();

	glPushMatrix();
	glTranslatef(-0.75, -0.01, 0.32);
	glRotatef(-90, 1, 0, 0);

	glScalef(0.5, 0.5, 0.5);

	shoulder(0.55, 0.55, 0.55);        //shoulder
	glPopMatrix();

	UpHand(0.5, 0.15, 0.15);

	glPushMatrix();               //rotate elbow up
	if (elbowRight == true) {
		if (actRotateElbowRight < 90) {
			actRotateElbowRight += rotatfinger;
		}

		glTranslatef(0, 0.2, 0);
		glRotatef(actRotateElbowRight, 0, 0, 1);
		glTranslatef(0, -0.2, 0);
	}
	else if (elbowRight == false) {// && rightHandMove == true) {
		if (actRotateElbowRight > 0) {
			actRotateElbowRight -= rotatfinger;
		}

		glTranslatef(0, 0.2, 0);
		glRotatef(actRotateElbowRight, 0, 0, 1);
		glTranslatef(0, -0.2, 0);
	}

	if (gunShow == true)                                      //Gun Show
		gunTrans();

	if (gunBig == true) {
		gun();
		fingerRight = true;
	}

	robotArm(0.5, 0.2, 0.2);    //arm

	glPushMatrix();
	glTranslatef(0, 0.02, 0);
	glScalef(1, 0.8, 1);
	palm(0.15, 0.2, 0.05);       //palm
	glPopMatrix();

	glPushMatrix();                        //left thumb connector
	glTranslatef(0.525, 0.19, 0.125);
	sphereTexture(0.02, "white.bmp");
	glPopMatrix();

	glPushMatrix();
	if (fingerRight == true) {
		if (actRotatThumbRight >= -90) {           //rotate thumb
			actRotatThumbRight -= rotatThumb;
		}

		glTranslatef(0, 0.18, 0.1);
		glRotatef(actRotatThumbRight, 1, 0, 0);
		glTranslatef(0, -0.18, -0.1);
	}
	else if (fingerRight == false) {
		if (actRotatThumbRight < 0) {           //rotate thumb
			actRotatThumbRight += rotatThumb;
		}

		glTranslatef(0, 0.18, 0.1);
		glRotatef(actRotatThumbRight, 1, 0, 0);
		glTranslatef(0, -0.18, -0.1);
	}
	glTranslatef(0, -0.03, 0);

	thumb(0.05, 0.06, 0.05);            //thumb
	glPopMatrix();
	///////////////////////////////////////
	glPushMatrix();                     //first finger
	glTranslatef(0, 0.02, 0);
	glScalef(1, 0.8, 1);

	if (fingerRight == true) {
		if (actRotatFingRight <= 80) {           //rotate finger1
			actRotatFingRight += rotatfinger;
		}

		glTranslatef(0.65, 0, 0.1);
		glRotatef(actRotatFingRight, 0, 1, 0);
		glTranslatef(-0.65, 0, -0.1);
	}
	else if (fingerRight == false) {
		if (actRotatFingRight > 0) {           //rotate finger1
			actRotatFingRight -= rotatfinger;
		}

		glTranslatef(0.65, 0, 0.1);
		glRotatef(actRotatFingRight, 0, 1, 0);
		glTranslatef(-0.65, 0, -0.1);
	}

	robotFinger(0.5, 0.7, 0.5);

	/////////////////////////
	glPushMatrix();
	glTranslatef(0.05, 0, 0);

	if (fingerRight == true) {
		if (actRotatFing2Right <= 45) {           //rotate finger2
			actRotatFing2Right += rotatfinger;
		}

		glTranslatef(0.65, 0, 0.09);
		glRotatef(actRotatFing2Right, 0, 1, 0);
		glTranslatef(-0.65, 0, -0.09);
	}
	else if (fingerRight == false) {
		if (actRotatFing2Right > 0) {           //rotate finger2
			actRotatFing2Right -= rotatfinger;
		}

		glTranslatef(0.65, 0, 0.09);
		glRotatef(actRotatFing2Right, 0, 1, 0);
		glTranslatef(-0.65, 0, -0.09);
	}

	robotFinger(0.5, 0.7, 0.5);
	/////////////////////////////////////
	glPushMatrix();
	glTranslatef(0.05, 0, 0);

	if (fingerRight == true) {
		if (actRotatFing3Right <= 60) {           //rotate finger3
			actRotatFing3Right += rotatfinger;
		}

		glTranslatef(0.65, 0, 0.09);
		glRotatef(actRotatFing3Right, 0, 1, 0);
		glTranslatef(-0.65, 0, -0.09);
	}
	else if (fingerRight == false) {
		if (actRotatFing3Right > 0) {           //rotate finger3
			actRotatFing3Right -= rotatfinger;
		}

		glTranslatef(0.65, 0, 0.09);
		glRotatef(actRotatFing3Right, 0, 1, 0);
		glTranslatef(-0.65, 0, -0.09);
	}

	robotFinger(0.5, 0.7, 0.5);

	glPopMatrix();
	glPopMatrix();
	glPopMatrix();
	glPopMatrix();
	glPopMatrix();
}

void hand() {
	glPushMatrix();
	glTranslatef(0, 0.15, 0);
	glScalef(1, 0.8, 1);

	glPushMatrix();
	glTranslatef(0.5, 0, 0.22);  //can change
	glRotatef(-90, 0, 0, 1);  //cannot
	glRotatef(-90, 1, 0, 0);  //cannot

	leftHand();
	glPopMatrix();

	glPushMatrix();
	glTranslatef(-0.5, 0, -0.02);  //can change
	glRotatef(-90, 0, 0, 1);   //cannot
	glRotatef(90, 1, 0, 0);    //cannot

	rightHand();
	glPopMatrix();
	glPopMatrix();
}

//////////////////////////////////////Leg
void sharpCorner(LPCSTR textBody2) {
	GLuint texture;
	texture = loadTexture(textBody2);

	glBegin(GL_TRIANGLES);
	glTexCoord2f(0.0f, 0.0f);
	glVertex3f(0.03, 0.1, -0.002);
	glTexCoord2f(0.5f, 1.0f);
	glVertex3f(0.075, 0.3, -0.15);
	glTexCoord2f(1.0f, 0.0f);
	glVertex3f(0.03, 0.3, -0.002);

	glTexCoord2f(0.0f, 0.0f);
	glVertex3f(0.03, 0.3, -0.002);
	glTexCoord2f(0.5f, 1.0f);
	glVertex3f(0.075, 0.3, -0.15);
	glTexCoord2f(1.0f, 0.0f);
	glVertex3f(0.12, 0.3, -0.002);

	glTexCoord2f(0.0f, 0.0f);
	glVertex3f(0.12, 0.3, -0.002);
	glTexCoord2f(0.5f, 1.0f);
	glVertex3f(0.075, 0.3, -0.15);
	glTexCoord2f(1.0f, 0.0f);
	glVertex3f(0.12, 0.1, -0.002);

	glTexCoord2f(0.0f, 0.0f);
	glVertex3f(0.12, 0.1, -0.002);
	glTexCoord2f(0.5f, 1.0f);
	glVertex3f(0.075, 0.3, -0.15);
	glTexCoord2f(1.0f, 0.0f);
	glVertex3f(0.03, 0.1, -0.002);
	glEnd();
	glDeleteTextures(1, &texture);
	glDisable(GL_TEXTURE_2D);
}

void upLeg(LPCSTR textBody1, LPCSTR textBody2, LPCSTR textBody3) {
	glPushMatrix();
	glScalef(0.5, 0.6, 0.3);
	drawCubeTexture(0.5, 1, 0.5, textBody1);

	GLuint texture;
	texture = loadTexture(textBody2);

	glPushMatrix();
	glTranslatef(0.075, 0.4, 0.5);

	glBegin(GL_QUADS);                  //up leg back
	//Face 2 : Left
	glTexCoord2f(0.0f, 0.0f);
	glVertex3f(0, 0, 0);
	glTexCoord2f(1.0f, 0.0f);
	glVertex3f(0, 0.5, 0);
	glTexCoord2f(1.0f, 1.0f);
	glVertex3f(0, 0.4, 0.4);
	glTexCoord2f(0.0f, 1.0f);
	glVertex3f(0, 0.25, 0.4);

	//Face 3 : Front                 
	glTexCoord2f(0.0f, 0.0f);
	glVertex3f(0, 0.4, 0.4);
	glTexCoord2f(1.0f, 0.0f);
	glVertex3f(0, 0.25, 0.4);
	glTexCoord2f(1.0f, 1.0f);
	glVertex3f(0.35, 0.25, 0.4);
	glTexCoord2f(0.0f, 1.0f);
	glVertex3f(0.35, 0.4, 0.4);

	//Face 4 : Right
	glTexCoord2f(0.0f, 0.0f);
	glVertex3f(0.35, 0.25, 0.4);
	glTexCoord2f(1.0f, 0.0f);
	glVertex3f(0.35, 0, 0);
	glTexCoord2f(1.0f, 1.0f);
	glVertex3f(0.35, 0.5, 0);
	glTexCoord2f(0.0f, 1.0f);
	glVertex3f(0.35, 0.4, 0.4);

	//Face 5 : Top                     
	glTexCoord2f(0.0f, 0.0f);
	glVertex3f(0.35, 0.4, 0.4);
	glTexCoord2f(1.0f, 0.0f);
	glVertex3f(0, 0.4, 0.4);
	glTexCoord2f(1.0f, 1.0f);
	glVertex3f(0, 0.5, 0);
	glTexCoord2f(0.0f, 1.0f);
	glVertex3f(0.35, 0.5, 0);

	//Face 6 : Bottom
	glTexCoord2f(0.0f, 0.0f);
	glVertex3f(0, 0.25, 0.4);
	glTexCoord2f(1.0f, 0.0f);
	glVertex3f(0.35, 0.25, 0.4);
	glTexCoord2f(1.0f, 1.0f);
	glVertex3f(0.35, 0, 0);
	glTexCoord2f(0.0f, 1.0f);
	glVertex3f(0, 0, 0);
	glEnd();
	glPopMatrix();
	glDeleteTextures(1, &texture);
	glDisable(GL_TEXTURE_2D);

	texture = loadTexture(textBody2);

	glBegin(GL_QUADS);                //decoration
	glTexCoord2f(0.0f, 0.0f);
	glVertex3f(0.15, 0, -0.001);  //front
	glTexCoord2f(1.0f, 0.0f);
	glVertex3f(0.15, 1, -0.001);
	glTexCoord2f(1.0f, 1.0f);
	glVertex3f(0, 1, -0.001);
	glTexCoord2f(0.0f, 1.0f);
	glVertex3f(0, 0, -0.001);

	glTexCoord2f(0.0f, 0.0f);
	glVertex3f(0.35, 0, -0.001);
	glTexCoord2f(1.0f, 0.0f);
	glVertex3f(0.35, 1, -0.001);
	glTexCoord2f(1.0f, 1.0f);
	glVertex3f(0.5, 1, -0.001);
	glTexCoord2f(0.0f, 1.0f);
	glVertex3f(0.5, 0, -0.001);

	glTexCoord2f(0.0f, 0.0f);
	glVertex3f(0.501, 1, 0);       //right
	glTexCoord2f(1.0f, 0.0f);
	glVertex3f(0.501, 1, 0.15);
	glTexCoord2f(1.0f, 1.0f);
	glVertex3f(0.501, 0, 0.15);
	glTexCoord2f(0.0f, 1.0f);
	glVertex3f(0.501, 0, 0);

	glTexCoord2f(0.0f, 0.0f);
	glVertex3f(0.501, 1, 0.5);
	glTexCoord2f(1.0f, 0.0f);
	glVertex3f(0.501, 1, 0.35);
	glTexCoord2f(1.0f, 1.0f);
	glVertex3f(0.501, 0, 0.35);
	glTexCoord2f(0.0f, 1.0f);
	glVertex3f(0.501, 0, 0.5);

	glTexCoord2f(0.0f, 0.0f);
	glVertex3f(-0.001, 1, 0);       //left
	glTexCoord2f(1.0f, 0.0f);
	glVertex3f(-0.001, 1, 0.15);
	glTexCoord2f(1.0f, 1.0f);
	glVertex3f(-0.001, 0, 0.15);
	glTexCoord2f(0.0f, 1.0f);
	glVertex3f(-0.001, 0, 0);

	glTexCoord2f(0.0f, 0.0f);
	glVertex3f(-0.001, 1, 0.5);
	glTexCoord2f(1.0f, 0.0f);
	glVertex3f(-0.001, 1, 0.35);
	glTexCoord2f(1.0f, 1.0f);
	glVertex3f(-0.001, 0, 0.35);
	glTexCoord2f(0.0f, 1.0f);
	glVertex3f(-0.001, 0, 0.5);
	glEnd();
	glDeleteTextures(1, &texture);
	glDisable(GL_TEXTURE_2D);

	sharpCorner(textBody3);                         //right sharp corner

	glPushMatrix();
	glTranslatef(0, 0.3, 0);
	sharpCorner(textBody3);

	glPushMatrix();
	glTranslatef(0, 0.3, 0);
	sharpCorner(textBody3);

	glPopMatrix();
	glPopMatrix();

	glPushMatrix();           //left sharp corner
	glTranslatef(0.34, 0, 0);
	sharpCorner(textBody3);

	glPushMatrix();
	glTranslatef(0, 0.3, 0);
	sharpCorner(textBody3);

	glPushMatrix();
	glTranslatef(0, 0.3, 0);
	sharpCorner(textBody3);

	glPopMatrix();
	glPopMatrix();
	glPopMatrix();
	glPopMatrix();


	glPushMatrix();                  //up leg back Thruster
	glTranslatef(0.13, 0.36, 0.16);
	glRotatef(35, 1, 0, 0);

	//glColor3f(1, 0, 0);
	cylinder(0.01, 0.07, 0.1);

	//glColor3f(1, 1, 0);
	cylinder(0.01, 0.069, 0.1);

	/*glPushMatrix();
	glTranslatef(0, 0, 0.02);
	sphereTexture(0.069, "white.bmp");
	glPopMatrix();
	//turbo();
	GLUquadric* cylinder = NULL;
	cylinder = gluNewQuadric();

	glColor3f(0, 0, 0);
	gluQuadricDrawStyle(cylinder, GLU_LINE);
	gluCylinder(cylinder, 0.01, 0.068, 0.1, 10, 5);

	gluDeleteQuadric(cylinder);*/
	glPopMatrix();
}

void leftUpLeg(LPCSTR textBody1, LPCSTR textBody2, LPCSTR textBody3, LPCSTR textJoin1)
{
	glPushMatrix();
	glTranslatef(0.4, 0.73, 0.08);
	glScalef(0.7, 1, 0.7);
	sphereTexture(0.1, textJoin1);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(0.27, 0.1, 0);
	upLeg(textBody1, textBody2, textBody3);
	glPopMatrix();
}

void rightUpLeg(LPCSTR textBody1, LPCSTR textBody2, LPCSTR textBody3, LPCSTR textJoin1)
{
	glPushMatrix();
	glTranslatef(-0.4, 0.73, 0.08);
	glScalef(0.7, 1, 0.7);
	sphereTexture(0.1, textJoin1);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(-0.53, 0.1, 0);
	upLeg(textBody1, textBody2, textBody3);
	glPopMatrix();
}

void downLeg(LPCSTR textBody1, LPCSTR textBody2, LPCSTR textBody3, LPCSTR textJoin1) {
	glPushMatrix();
	glTranslatef(0, 0.3, -0.13);
	glRotatef(60, 1, 0, 0);
	cylinderTexture(0.001, 0.05, 0.2, textBody3);
	glPopMatrix();

	glPushMatrix();
	glScalef(0.1, 0.2, 0.1);    //become smaller

	glPushMatrix();            //knee
	glTranslatef(0, 0.55, 0.45);
	glScalef(1, 0.2, 1);
	sphereTexture(1.2, textBody3);
	glPopMatrix();

	GLuint texture;
	texture = loadTexture(textJoin1);

	glBegin(GL_QUADS);          //top
	glTexCoord2f(0.0f, 0.0f);        //back
	glVertex3f(0, 0.6, 0);
	glTexCoord2f(1.0f, 0.0f);
	glVertex3f(1, 1, 1);
	glTexCoord2f(1.0f, 1.0f);
	glVertex3f(0, 1.6, 1);
	glTexCoord2f(0.0f, 1.0f);
	glVertex3f(-1, 1, 1);

	glTexCoord2f(0.0f, 0.0f);
	glVertex3f(0, 0.6, 0);        //front
	glTexCoord2f(1.0f, 0.0f);
	glVertex3f(0.5, 1, 0.5);
	glTexCoord2f(1.0f, 1.0f);
	glVertex3f(0, 1.6, 1);
	glTexCoord2f(0.0f, 1.0f);
	glVertex3f(-0.5, 1, 0.5);

	glTexCoord2f(0.0f, 0.0f);
	glVertex3f(0, 1.6, 1);         //right
	glTexCoord2f(1.0f, 0.0f);
	glVertex3f(1, 1, 1);
	glTexCoord2f(1.0f, 1.0f);
	glVertex3f(0, 0.6, 0);
	glTexCoord2f(0.0f, 1.0f);
	glVertex3f(0.5, 1, 0.5);

	glTexCoord2f(0.0f, 0.0f);
	glVertex3f(0, 1.6, 1);         //left
	glTexCoord2f(1.0f, 0.0f);
	glVertex3f(-1, 1, 1);
	glTexCoord2f(1.0f, 1.0f);
	glVertex3f(0, 0.6, 0);
	glTexCoord2f(0.0f, 1.0f);
	glVertex3f(-0.5, 1, 0.5);
	glEnd();
	glDeleteTextures(1, &texture);
	glDisable(GL_TEXTURE_2D);

	texture = loadTexture(textBody1);
	glBegin(GL_QUADS);
	glTexCoord2f(0.0f, 0.0f);     //right
	glVertex3f(0, 0.6, 0);
	glTexCoord2f(1.0f, 0.0f);
	glVertex3f(1, 1, 1);
	glTexCoord2f(1.0f, 1.0f);
	glVertex3f(1, -1, 1);
	glTexCoord2f(0.0f, 1.0f);
	glVertex3f(0, -0.6, 0);

	glTexCoord2f(0.0f, 0.0f);     //left
	glVertex3f(0, 0.6, 0);
	glTexCoord2f(1.0f, 0.0f);
	glVertex3f(-1, 1, 1);
	glTexCoord2f(1.0f, 1.0f);
	glVertex3f(-1, -1, 1);
	glTexCoord2f(0.0f, 1.0f);
	glVertex3f(0, -0.6, 0);

	glTexCoord2f(0.0f, 0.0f);     //back
	glVertex3f(1, 1, 1);
	glTexCoord2f(1.0f, 0.0f);
	glVertex3f(1, -2, 1);
	glTexCoord2f(1.0f, 1.0f);
	glVertex3f(-1, -2, 1);
	glTexCoord2f(0.0f, 1.0f);
	glVertex3f(-1, 1, 1);
	glEnd();

	glBegin(GL_TRIANGLES);
	glTexCoord2f(0.5f, 1.0f);       //down
	glVertex3f(0, -0.6, 0);
	glTexCoord2f(0.0f, 0.0f);
	glVertex3f(1, -1, 1);
	glTexCoord2f(1.0f, 0.0f);
	glVertex3f(-1, -1, 1);
	glEnd();
	glDeleteTextures(1, &texture);
	glDisable(GL_TEXTURE_2D);

	texture = loadTexture(textBody1);
	glBegin(GL_QUADS);          //ankle
	glTexCoord2f(0.0f, 0.0f);     //front
	glVertex3f(-0.5, -0.8, 0.5);
	glTexCoord2f(1.0f, 0.0f);
	glVertex3f(0.5, -0.8, 0.5);
	glTexCoord2f(1.0f, 1.0f);
	glVertex3f(0.5, -1.5, 0.5);
	glTexCoord2f(0.0f, 1.0f);
	glVertex3f(-0.5, -1.5, 0.5);

	glTexCoord2f(0.0f, 0.0f);       //right
	glVertex3f(0.5, -0.8, 0.5);
	glTexCoord2f(1.0f, 0.0f);
	glVertex3f(1, -1, 1);
	glTexCoord2f(1.0f, 1.0f);
	glVertex3f(1, -1.5, 1);
	glTexCoord2f(0.0f, 1.0f);
	glVertex3f(0.5, -1.5, 0.5);

	glTexCoord2f(0.0f, 0.0f);       //left
	glVertex3f(-0.5, -0.8, 0.5);
	glTexCoord2f(1.0f, 0.0f);
	glVertex3f(-1, -1, 1);
	glTexCoord2f(1.0f, 1.0f);
	glVertex3f(-1, -1.5, 1);
	glTexCoord2f(0.0f, 1.0f);
	glVertex3f(-0.5, -1.5, 0.5);
	glEnd();
	glDeleteTextures(1, &texture);
	glDisable(GL_TEXTURE_2D);

	texture = loadTexture(textBody2);
	glBegin(GL_TRIANGLES);        //shoe      
	glTexCoord2f(0.5f, 1.0f);       //up
	glVertex3f(0, -2, -2);
	glTexCoord2f(1.0f, 0.0f);
	glVertex3f(-0.5, -1.5, 0.5);
	glTexCoord2f(0.0f, 0.0f);
	glVertex3f(0.5, -1.5, 0.5);

	glTexCoord2f(0.0f, 0.0f);         //down
	glVertex3f(1, -2, 1);
	glTexCoord2f(0.5f, 1.0f);
	glVertex3f(0, -2, -2);
	glTexCoord2f(1.0f, 0.0f);
	glVertex3f(-1, -2, 1);
	glEnd();

	glBegin(GL_QUADS);
	glTexCoord2f(0.0f, 0.0f);         //right 
	glVertex3f(1, -1.5, 1);
	glTexCoord2f(1.0f, 0.0f);
	glVertex3f(1, -2, 1);
	glTexCoord2f(1.0f, 1.0f);
	glVertex3f(0, -2, -2);
	glTexCoord2f(0.0f, 1.0f);
	glVertex3f(0.5, -1.5, 0.5);

	glTexCoord2f(0.0f, 0.0f);
	glVertex3f(-1, -1.5, 1);   //left
	glTexCoord2f(1.0f, 0.0f);
	glVertex3f(-1, -2, 1);
	glTexCoord2f(1.0f, 1.0f);
	glVertex3f(0, -2, -2);
	glTexCoord2f(0.0f, 1.0f);
	glVertex3f(-0.5, -1.5, 0.5);
	glEnd();
	glDeleteTextures(1, &texture);
	glDisable(GL_TEXTURE_2D);
	glPopMatrix();
}

void leftDownLeg(LPCSTR textBody1, LPCSTR textBody2, LPCSTR textBody3, LPCSTR textJoin1)
{
	glPushMatrix();
	glTranslatef(0.4, -0.05, 0);    //right leg translate to right
	downLeg(textBody1, textBody2, textBody3, textJoin1);
	glPopMatrix();
}

void rightDownLeg(LPCSTR textBody1, LPCSTR textBody2, LPCSTR textBody3, LPCSTR textJoin1)
{
	glPushMatrix();
	glTranslatef(-0.4, -0.05, 0);    //right leg translate to right
	downLeg(textBody1, textBody2, textBody3, textJoin1);
	glPopMatrix();
}

void leg(LPCSTR textBody1, LPCSTR textBody2, LPCSTR textBody3, LPCSTR textJoin1) {
	if (walkUpLeftKnee > 20 && walkLeftKnee > 0.18) {          //walking
		walkLeft = false;
		walkRight = true;
	}
	else if (walkUpRightKnee > 20 && walkRightKnee > 0.18) {
		walkRight = false;
		walkLeft = true;
	}

	glPushMatrix();
	glTranslatef(0, -0.9, 0.2);
	glRotatef(180, 0, 1, 0);
	glScalef(0.8, 1, 1);
	//////////////////////////////////////////////Left Leg
	glPushMatrix();
	glTranslatef(-0.2, 0, 0);

	glPushMatrix();
	if (walkLeft == true) {                 //move Left up leg
		if (walkUpLeftKnee < 30) {
			walkUpLeftKnee += walkSpeed;
		}
		glTranslatef(0, 0.6, 0);
		glRotatef(walkUpLeftKnee, 1, 0, 0);
		glTranslatef(0, -0.6, 0);
	}
	else if (walkLeft == false) {
		if (walkUpLeftKnee > 0) {
			walkUpLeftKnee -= walkSpeed;
		}
		glTranslatef(0, 0.6, 0);
		glRotatef(walkUpLeftKnee, 1, 0, 0);
		glTranslatef(0, -0.6, 0);
	}
	leftUpLeg(textBody1, textBody2, textBody3, textJoin1);
	glPopMatrix();

	glPushMatrix();
	if (walkLeft == true) {                   //move Left down leg
		if (walkLeftKnee < 0.3) {
			walkLeftKnee += walkKneeSpeed;
		}
		glTranslatef(0, walkLeftKnee / 8, -walkLeftKnee);
	}
	else if (walkLeft == false) {
		if (walkLeftKnee > 0) {
			walkLeftKnee -= walkKneeSpeed;
		}
		glTranslatef(0, walkLeftKnee / 8, -walkLeftKnee);
	}

	leftDownLeg(textBody1, textBody2, textBody3, textJoin1);
	glPopMatrix();
	glPopMatrix();
	////////////////////////////////////////////////////////////Right Leg
	glPushMatrix();
	glTranslatef(0.2, 0, 0);

	glPushMatrix();                  //move Right up leg
	if (walkRight == true) {
		if (walkUpRightKnee < 30) {
			walkUpRightKnee += walkSpeed;
		}
		glTranslatef(0, 0.6, 0);
		glRotatef(walkUpRightKnee, 1, 0, 0);
		glTranslatef(0, -0.6, 0);
	}
	else if (walkRight == false) {
		if (walkUpRightKnee > 0) {
			walkUpRightKnee -= walkSpeed;
		}
		glTranslatef(0, 0.6, 0);
		glRotatef(walkUpRightKnee, 1, 0, 0);
		glTranslatef(0, -0.6, 0);
	}

	rightUpLeg(textBody1, textBody2, textBody3, textJoin1);
	glPopMatrix();

	glPushMatrix();
	if (walkRight == true) {                   //move Right down leg
		if (walkRightKnee < 0.3) {
			walkRightKnee += walkKneeSpeed;
		}
	}
	else if (walkRight == false) {
		if (walkRightKnee > 0) {
			walkRightKnee -= walkKneeSpeed;
		}
	}
	glTranslatef(0, walkRightKnee / 8, -walkRightKnee);

	rightDownLeg(textBody1, textBody2, textBody3, textJoin1);
	glPopMatrix();
	glPopMatrix();
	glPopMatrix();
}

/////////////////////////////////////////Projection
void projection() {
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();

	glTranslatef(tx, ty, 0);

	glRotatef(Rx, 1, 0, 0);
	glRotatef(Ry, 0, 1, 0);

	if (isOrtho) {
		glOrtho(-orthoview, orthoview, -orthoview, orthoview, -orthoview, orthoview);
	}
	else {
		gluPerspective(16, 1, -1, 1);
		glFrustum(-1, 1, -1, 1, 1, 17);
	}
}

void display()
{
	switch (qNo)
	{
	case 1:
	{
		glEnable(GL_DEPTH_TEST);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		//glClearColor(1, 1, 1, 1);

		glTranslatef(0, 0, tz);

		projection();

		glMatrixMode(GL_MODELVIEW);
		glLoadIdentity();


		if (walkRight == true || walkLeft == true) {   //walking
			walking += walkingSp;
		}
		glTranslatef(0, 0, walking);
		glTranslatef(0, 0, walking);
		glTranslatef(flyingX, flyingY, 0);
		glTranslatef(0, 0, slideMove);
		glRotatef(sliding, 1, 0, 0);
		headAndBody(bodyText[changeBody1], jointText[changeBody1]);
		hand();
		//leg();
		glPopMatrix();
		if (sliding == 30) {
			isTurbo = false;
			for (int i = 0; i < sliding; sliding--)
				sliding -= 0.01;
		}
		else if (sliding == -30) {
			isTurbo = false;
			for (int i = 0; i > sliding; sliding++)
				sliding += 0.01;
		}
		break;
	}
	case 2:
	{
		glEnable(GL_DEPTH_TEST);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		projection();

		glMatrixMode(GL_MODELVIEW);
		glLoadIdentity();
		glPushMatrix();

		leg(bodyText[changeBody1], bodyText[changeBody2], bodyText[changeBody3], jointText[changeJoint1]);
		
		glDisable(GL_TEXTURE_2D);
		glPopMatrix();
		break;
	}

	default:
		break;
	};

}
//--------------------------------------------------------------------

int WINAPI WinMain(HINSTANCE hInst, HINSTANCE, LPSTR, int nCmdShow)
{
	WNDCLASSEX wc;
	ZeroMemory(&wc, sizeof(WNDCLASSEX));

	wc.cbSize = sizeof(WNDCLASSEX);
	wc.hInstance = GetModuleHandle(NULL);
	wc.lpfnWndProc = WindowProcedure;
	wc.lpszClassName = WINDOW_TITLE;
	wc.style = CS_HREDRAW | CS_VREDRAW;

	if (!RegisterClassEx(&wc)) return false;

	HWND hWnd = CreateWindow(WINDOW_TITLE, WINDOW_TITLE, WS_OVERLAPPEDWINDOW,        //WS = window style
		CW_USEDEFAULT, CW_USEDEFAULT, 600, 600,
		NULL, NULL, wc.hInstance, NULL);

	//--------------------------------
	//	Initialize window for OpenGL
	//--------------------------------

	HDC hdc = GetDC(hWnd);

	//	initialize pixel format for the window
	initPixelFormat(hdc);

	//	get an openGL context
	HGLRC hglrc = wglCreateContext(hdc);

	//	make context current
	if (!wglMakeCurrent(hdc, hglrc)) return false;

	//--------------------------------
	//	End initialization
	//--------------------------------

	ShowWindow(hWnd, nCmdShow);

	MSG msg;
	ZeroMemory(&msg, sizeof(msg));

	while (true)
	{
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			if (msg.message == WM_QUIT) break;

			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}

		display();

		SwapBuffers(hdc);
	}

	UnregisterClass(WINDOW_TITLE, wc.hInstance);

	return true;
}
//--------------------------------------------------------------------