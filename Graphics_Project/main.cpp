// glutEx1.cpp : �������̨Ӧ�ó������ڵ㡣
//

#include <glut.h>
#include"BMPLoader.h"
#include"Camera.h"
#include"SkyBox.h"
#include"stdafx.h"
#include"Vector.h"
#include"Control.h"

bool    m_RenderMode;		          /**< ����ģʽ */
CSkyBox m_SkyBox;
Camera m_Camera;

float fTranslate;
float fRotate;
float fScale = 1.0f;	// set inital scale value to 1.0f
float fDistance = 0.2f;

bool bPersp = false;
bool bAnim = false;
bool bWire = false;

int wHeight = 0;
int wWidth = 0;

bool init() {
	/** �û��Զ���ĳ�ʼ������ */
	glClearColor(0.0f, 0.0f, 0.0f, 0.5f);
	glClearDepth(1.0f);
	glDepthFunc(GL_LEQUAL);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);
	glShadeModel(GL_SMOOTH);
	glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);

	/** ��ʼ����� */
	if (!m_SkyBox.init())
	{
		MessageBox(NULL, L"��ʼ�����ʧ��!", L"����", MB_OK);
		exit(0);
	}

	/** ��������� */
	m_Camera.setCamera(0, 0, 8, 0, 0, 0, 0, 1, 0);

	return true;                                        /**< �ɹ����� */
}



void updateView(int width, int height)
{

	glViewport(0, 0, width, height);						// Reset The Current Viewport

	glMatrixMode(GL_PROJECTION);						// Select The Projection Matrix
	glLoadIdentity();									// Reset The Projection Matrix

	float whRatio = (GLfloat)width / (GLfloat)height;

	gluPerspective(45, 1, 1, 100);
	

	glMatrixMode(GL_MODELVIEW);							// Select The Modelview Matrix
}

void reshape(int width, int height)
{
	if (height == 0)										// Prevent A Divide By Zero By
	{
		height = 1;										// Making Height Equal One
	}

	wHeight = height;
	wWidth = width;

	updateView(wHeight, wWidth);
}

void idle()
{
	glutPostRedisplay();
}




void getFPS()
{
	static int frame = 0, time, timebase = 0;
	static char buffer[256];

	char mode[64];
	strcpy_s(mode, "naive");

	frame++;
	time = glutGet(GLUT_ELAPSED_TIME);
	if (time - timebase > 1000) {
		sprintf_s(buffer, "FPS:%4.2f %s",
			frame*1000.0 / (time - timebase), mode);
		timebase = time;
		frame = 0;
	}

	//glutSetWindowTitle(buffer);
	char *c;
	glDisable(GL_DEPTH_TEST);
	glMatrixMode(GL_PROJECTION);// ѡ��ͶӰ����
	glPushMatrix();// ����ԭ����
	glLoadIdentity();// װ�뵥λ����
	glOrtho(0, 480, 0, 480, -1, 1);// λ����ͶӰ
	glMatrixMode(GL_MODELVIEW);// ѡ��Modelview����
	glPushMatrix();// ����ԭ����
	glLoadIdentity();// װ�뵥λ����*/
	glRasterPos2f(10, 10);
	for (c = buffer; *c != '\0'; c++) {
		glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, *c);
	}
	glMatrixMode(GL_PROJECTION);// ѡ��ͶӰ����
	glPopMatrix();// ����Ϊԭ�������
	glMatrixMode(GL_MODELVIEW);// ѡ��Modelview����
	glPopMatrix();// ����Ϊԭ�������
	glEnable(GL_DEPTH_TEST);
}

void redraw()
{

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glLoadIdentity();									// Reset The Current Modelview Matrix

	m_Camera.setLook();			// ������0��0��0�����ӵ����� (0,5,50)��Y������

	
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	



	//	glTranslatef(0.0f, 0.0f,-6.0f);			// Place the triangle at Center
	//glRotatef(fRotate, 0, 1.0f, 0);			// Rotate around Y axis
	glRotatef(-90, 1, 0, 0);
	glScalef(0.2, 0.2, 0.2);



										//	Gen3DObjectList();
	m_SkyBox.render();

	if (bAnim) fRotate += 0.5f;

	getFPS();

	glutSwapBuffers();
}
void mouse() {
	m_Camera.setViewByMouse();
}
int main(int argc, char *argv[])
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_RGBA | GLUT_DEPTH | GLUT_DOUBLE);
	glutInitWindowSize(480, 480);
	int windowHandle = glutCreateWindow("Simple GLUT App");

	init();
	glutDisplayFunc(redraw);
	glutReshapeFunc(reshape);
	glutKeyboardFunc(key);
	glutIdleFunc(idle);


	glutMainLoop();
	return 0;
}
