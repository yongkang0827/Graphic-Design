#include <Windows.h>
#include <gl/GL.h>
#include <gl/GLU.h>
#include <math.h>

#pragma comment (lib, "OpenGL32.lib")

#define WINDOW_TITLE "OpenGL Window"

float angle = 0;
float x1 = 0, y3 = 0, x2 = 0, y2 = 0;



LRESULT WINAPI WindowProcedure(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	switch (msg)
	{
	case WM_DESTROY:
		PostQuitMessage(0);
		break;

	case WM_KEYDOWN:
		if (wParam == VK_ESCAPE) PostQuitMessage(0);
		break;

	default:
		break;
	}

	return DefWindowProc(hWnd, msg, wParam, lParam);
}
//--------------------------------------------------------------------

bool initPixelFormat(HDC hdc)
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

void drawsphere() {
	GLUquadricObj* sphere = NULL;
	sphere = gluNewQuadric();

	gluQuadricDrawStyle(sphere, GLU_FILL);
	gluSphere(sphere, 0.2, 20, 20);
	gluDeleteQuadric(sphere);
}

void drawsphere1() {
	GLUquadricObj* sphere = NULL;
	sphere = gluNewQuadric();

	gluQuadricDrawStyle(sphere, GLU_FILL);
	gluSphere(sphere, 0.04, 20, 20);
	gluDeleteQuadric(sphere);
}

void drawsphere2() {
	glColor3f(0.25, 0.1, 0.01);
	GLUquadricObj* sphere = NULL;
	sphere = gluNewQuadric();

	gluQuadricDrawStyle(sphere, GLU_FILL);
	gluSphere(sphere, 0.01, 20, 20);
	gluDeleteQuadric(sphere);
}

void drawCone1() {
	GLUquadricObj* cylinder = NULL;
	cylinder = gluNewQuadric();

	gluQuadricDrawStyle(cylinder, GLU_FILL);
	gluCylinder(cylinder, 0.01, 0.2, 0.8, 30, 30);
	gluDeleteQuadric(cylinder);
};

void drawCone() {
	glColor3f(0, 0, 0);
	GLUquadricObj* cylinder = NULL;
	cylinder = gluNewQuadric();

	gluQuadricDrawStyle(cylinder, GLU_LINE);
	gluCylinder(cylinder, 0.01, 0.2, 0.8, 30, 30);
	gluDeleteQuadric(cylinder);
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
void drawCylinder(float baseRadius, float topRadius, float height, int slices, int stacks) {
	drawCircle(baseRadius);
	GLUquadricObj* cylinder = NULL;
	cylinder = gluNewQuadric();

	gluQuadricDrawStyle(cylinder, GLU_FILL);
	gluCylinder(cylinder, baseRadius, topRadius, height, slices, stacks);
	gluDeleteQuadric(cylinder);

	glPushMatrix();
	glTranslatef(0, 0, height);
	drawCircle(topRadius);
	glPopMatrix();

};

void drawCube(float length, float width, float height) {
	glBegin(GL_QUADS);
	//Face 1£º front
	glVertex3f(0.0f, 0.0f, height);
	glVertex3f(length, 0.0f, height);
	glVertex3f(length, 0.0f, 0.0f);
	glVertex3f(0.0f, 0.0f, 0.0f);
	//Face 2£º left
	glVertex3f(0.0f, 0.0f, 0.0f);
	glVertex3f(0.0f, 0.0f, height);
	glVertex3f(0.0f, width, height);
	glVertex3f(0.0f, width, 0.0f);
	//Face 3£º top
	glVertex3f(0.0f, 0.0f, height);
	glVertex3f(0.0f, width, height);
	glVertex3f(length, width, height);
	glVertex3f(length, 0.0f, height);
	//Face 4£º right
	glVertex3f(length, 0.0f, height);
	glVertex3f(length, 0.0f, 0.0f);
	glVertex3f(length, width, 0.0f);
	glVertex3f(length, width, height);
	//Face 5£º behind
	glVertex3f(length, width, height);
	glVertex3f(0.0f, width, height);
	glVertex3f(0.0f, width, 0.0f);
	glVertex3f(length, width, 0.0f);
	//Face 6£º bottom
	glVertex3f(length, width, 0.0f);
	glVertex3f(length, 0.0f, 0.0f);
	glVertex3f(0.0f, 0.0f, 0.0f);
	glVertex3f(0.0f, width, 0.0f);



	glEnd();

}

void bodyUp(float width) {
	glBegin(GL_QUADS);
	glColor3f(1, 0, 0);
	//Face 1£º front
	glVertex3f(-0.05, 0.3f, 0);
	glVertex3f(-0.45f, 0.35f, 0);
	glVertex3f(-0.45, 0.2f, 0.0f);
	glVertex3f(0.0f, 0.0f, 0.0f);
	//Face 2£º right
	glColor3f(1, 0, 1);
	glVertex3f(0.0f, 0.0f, 0.0f);
	glVertex3f(0.0f, 0.0f, width);
	glVertex3f(-0.05, 0.3f, width);
	glVertex3f(-0.05, 0.3f, 0);


	//Face 3£º top
	glColor3f(1, 1, 0);
	glVertex3f(-0.05, 0.3f, 0);
	glVertex3f(-0.05, 0.3f, width);
	glVertex3f(-0.45f, 0.35f, width);
	glVertex3f(-0.45f, 0.35f, 0);
	//Face 4£º left
	glColor3f(0, 0, 1);
	glVertex3f(-0.45f, 0.35f, 0);
	glVertex3f(-0.45, 0.2f, 0.0f);
	glVertex3f(-0.45, 0.2f, width);
	glVertex3f(-0.45f, 0.35f, width);


	//Face 5£º behind
	glColor3f(0, 1, 0);
	glVertex3f(-0.45f, 0.35f, width);
	glVertex3f(-0.05, 0.3f, width);
	glVertex3f(0.0f, 0.0f, width);
	glVertex3f(-0.45, 0.2f, width);

	//Face 6£º bottom
	glColor3f(0, 0, 0);
	glVertex3f(-0.45, 0.2f, width);
	glVertex3f(-0.45, 0.2f, 0);
	glVertex3f(0.0f, 0.0f, 0.0f);
	glVertex3f(0, 0, width);

	glEnd();
}

void bodyUpMid(float width) {
	glBegin(GL_TRIANGLES);
	glColor3f(0, 0, 1);
	//Face 1£º front
	glVertex3f(-0.02, 0.1f, 0);
	glVertex3f(0, 0.075f, 0.0f);
	glVertex3f(0.0f, 0.0f, 0.0f);
	glEnd();

	//Face 5£º behind
	glBegin(GL_TRIANGLES);
	glColor3f(0, 0, 1);
	glVertex3f(-0.02, 0.1f, width);
	glVertex3f(0, 0.075f, width);
	glVertex3f(0.0f, 0.0f, width);
	glEnd();

	glBegin(GL_QUADS);
	glColor3f(1, 0, 1);
	//Face 1£º front
	glVertex3f(-0.02, 0.1f, 0);
	glVertex3f(0.0f, 0.075f, 0.0f);
	glVertex3f(0.0f, 0.175f, 0.0f);
	glVertex3f(-0.035, 0.2, 0.0f);


	//Face 5£º behind
	glVertex3f(-0.02, 0.1f, width);
	glVertex3f(0.0f, 0.075f, width);
	glVertex3f(0.0f, 0.175f, width);
	glVertex3f(-0.035, 0.2, width);
	glEnd();

	glBegin(GL_QUADS);
	glColor3f(0, 0, 0);
	//Face 1£º front

	glVertex3f(0.0f, 0.175f, 0.0f);
	glVertex3f(-0.035, 0.2, 0.0f);
	glVertex3f(-0.05, 0.3, 0.0f);
	glVertex3f(0, 0.29, 0.0f);

	//Face 5£º behind
	glVertex3f(0.0f, 0.175f, width);
	glVertex3f(-0.035, 0.2, width);
	glVertex3f(-0.05, 0.3, width);
	glVertex3f(0, 0.29, width);
	glEnd();
}

void tri() {
	glBegin(GL_TRIANGLES);
	glColor3f(0, 0, 0);
	glVertex3f(0.0, 0, 0.0);
	glVertex3f(0.15, 0, 0.0f);
	glVertex3f(0.15, 0.065, 0.0f);
	glEnd();
}

void bodyMidTri(float width) {
	tri();
	glPushMatrix();
	glTranslatef(0, 0, width);
	tri();
	glPopMatrix();

	glPushMatrix();
	glRotatef(180, 0, 1, 0);
	glTranslatef(0, 0, -width);
	tri();
	glPushMatrix();
	glTranslatef(0, 0, width);
	tri();
	glPopMatrix();
	glPopMatrix();
}

void bodyMid(float width) {
	bodyMidTri(width);

	glPushMatrix();
	glTranslatef(-0.15, -0.1, 0);
	glColor3f(0, 0, 1);
	drawCube(0.3, 0.1, width);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(-0.15, -0.2, 0);
	glColor3f(0, 0, 1);
	drawCube(0.3, 0.1, width);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(-0.15, -0.3, 0);
	glColor3f(0, 0, 1);
	drawCube(0.3, 0.1, width);
	glPopMatrix();
}

void bodySide(float width) {
	glBegin(GL_QUADS);
	glColor3f(1, 1, 0);
	//Face 1£º front
	glVertex3f(0.15, 0.07f, 0.0f);
	glVertex3f(0.15, -0.07, 0.0f);
	glVertex3f(0.45f, 0.05f, 0);
	glVertex3f(0.45, 0.2, 0);



	//Face 2£º right
	glColor3f(1, 0, 1);
	glVertex3f(0.45, 0.2, 0);
	glVertex3f(0.45f, 0.05f, 0);
	glVertex3f(0.45f, 0.05f, width);
	glVertex3f(0.45, 0.2, width);


	//Face 3£º top
	glColor3f(1, 1, 0);
	glVertex3f(0.45, 0.2, width);
	glVertex3f(0.45, 0.2, 0);
	glVertex3f(0.15, 0.07f, 0.0f);
	glVertex3f(0.15, 0.07f, width);
	//Face 4£º left
	glColor3f(0, 0, 1);
	glVertex3f(0.15, 0.07f, width);
	glVertex3f(0.15, 0.07f, 0.0f);
	glVertex3f(0.15, -0.07, 0.0f);
	glVertex3f(0.15, -0.07, width);


	//Face 5£º behind
	glColor3f(0, 1, 0);
	glVertex3f(0.15, -0.07, width);
	glVertex3f(0.15, 0.07f, width);
	glVertex3f(0.45, 0.2, width);
	glVertex3f(0.45, 0.05, width);

	//Face 6£º bottom
	glColor3f(0, 0, 0);
	glVertex3f(0.45, 0.05, width);
	glVertex3f(0.45, 0.05, 0);
	glVertex3f(0.15, -0.07, 0);
	glVertex3f(0.15, -0.07, width);

	glEnd();
}

void bodyRight(float width) {
	bodySide(width);
	glPushMatrix();
	glTranslatef(0, -0.13, 0);
	bodySide(width);
	glPopMatrix();
	glPushMatrix();
	glTranslatef(0, -0.26, 0);
	bodySide(width);
	glPopMatrix();
}

void lowerBody(float width) {
	glBegin(GL_QUADS);
	glColor3f(1, 1, 0);
	//Face 1£º front
	glVertex3f(0, -0.27, 0.0f);
	glVertex3f(-0.3, -0.4, 0);
	glVertex3f(-0.3, -0.5f, 0);
	glVertex3f(0, -0.6, 0.0f);

	//Face 2£º right
	glColor3f(1, 0, 1);
	glVertex3f(0, -0.6, 0.0f);
	glVertex3f(0, -0.6, width);
	glVertex3f(0, -0.27, width);
	glVertex3f(0, -0.27, 0.0f);


	//Face 3£º top
	glColor3f(0, 0, 0);
	glVertex3f(-0.3, -0.4, 0);
	glVertex3f(0, -0.27, 0.0f);
	glVertex3f(0, -0.27, width);
	glVertex3f(-0.3, -0.4, width);
	//Face 4£º left
	glColor3f(0, 0, 1);
	glVertex3f(-0.3, -0.4, width);
	glVertex3f(-0.3, -0.4, 0);
	glVertex3f(-0.3, -0.5f, 0);
	glVertex3f(-0.3, -0.5f, width);


	//Face 5£º behind
	glColor3f(0, 1, 0);
	glVertex3f(-0.3, -0.5f, width);
	glVertex3f(-0.3, -0.4, width);
	glVertex3f(0, -0.27, width);
	glVertex3f(0, -0.6, width);

	//Face 6£º bottom
	glColor3f(0, 0, 0);
	glVertex3f(0, -0.6, width);
	glVertex3f(0, -0.6, 0);
	glVertex3f(-0.3, -0.5f, 0);
	glVertex3f(-0.3, -0.5f, width);

	glEnd();
}

void body(float width) {
	bodyUp(width);
	bodyUpMid(width);

	glPushMatrix();
	glRotatef(180, 0, 1, 0);
	glTranslatef(0, 0, -width);
	bodyUp(width);
	bodyUpMid(width);
	glPopMatrix();

	bodyMid(width);
	bodyRight(width);

	glPushMatrix();
	glRotatef(180, 0, 1, 0);
	glTranslatef(0, 0, -width);
	bodyRight(width);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(0, -0.03, 0);
	lowerBody(width);

	glPushMatrix();
	glRotatef(180, 0, 1, 0);
	glTranslatef(0, 0, -width);
	lowerBody(width);
	glPopMatrix();
	glPopMatrix();

	glPushMatrix();
	glRotatef(180, -1, 0, 0);
	glTranslatef(0, 0.3, -width);
	bodyMidTri(width);
	glPopMatrix();
}
void display()
{
	glEnable(GL_DEPTH_TEST);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glClearColor(1.0f, 1.0f, 1.0f, 0.0f);
	glRotatef(0.5, 0, 1, 0);
	
	body(0.2);
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

	HWND hWnd = CreateWindow(WINDOW_TITLE, WINDOW_TITLE, WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT, CW_USEDEFAULT, 700, 700,
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