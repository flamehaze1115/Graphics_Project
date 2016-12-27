#define _CRT_SECURE_NO_WARNINGS
// glutEx1.cpp : �������̨Ӧ�ó������ڵ㡣
#include <glut.h>
#include"BMPLoader.h"
#include"Camera.h"
#include"SkyBox.h"
#include"stdafx.h"
#include"Vector.h"
#include"Control.h"
#include"Terrain.h"
#include"Snow.h"

bool    m_RenderMode;		          /**< ����ģʽ */
CSkyBox m_SkyBox;
extern Camera m_Camera;
CTerrain m_Terrain;
CSnow m_Snow;

float fTranslate;
float fRotate;
float fScale = 1.0f;	// set inital scale value to 1.0f
float fDistance = 0.2f;

bool bPersp = false;
bool bAnim = false;
bool bWire = false;

extern int wHeight;//���ڵĸ߶ȺͿ��
extern int wWidth;
extern int wPositionX;//�������õ�ԭ��
extern int wPositionY;

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
	/** ��ʼ������ */
	if (!m_Terrain.init())
	{
		MessageBox(NULL, L"��ʼ������ʧ��!", L"����", MB_OK);
		exit(0);
	}

	/** ��ʼ��ѩ��ʵ�� */
	if (!m_Snow.Init(5000))
	{
		MessageBox(NULL, L"ѩ��ϵͳ��ʼ��ʧ��!", L"����", MB_OK);
		exit(-1);
	}

	/** ��������� */
	m_Camera.setCamera(500, 35, 400, 501, 35, 400, 0, 1, 0);

	return true;                                        /**< �ɹ����� */
}



void updateView(int width, int height)
{

	glViewport(0, 0, width, height);						// Reset The Current Viewport

	glMatrixMode(GL_PROJECTION);						// Select The Projection Matrix
	glLoadIdentity();									// Reset The Projection Matrix

	float whRatio = (float)width / (height ? height : 1);

	//gluPerspective(45, 1, 1, 100);
	gluPerspective(45.0f, whRatio, 1.0f, 5000.0f); //���ò�����Ļ� ���ܿ�����ȫ����skybox

	glMatrixMode(GL_MODELVIEW);							// Select The Modelview Matrix
	glLoadIdentity(); //���������
}

void reshape(int width, int height)
{
	if (height == 0)						// Prevent A Divide By Zero By
	{
		height = 1;					// Making Height Equal One
	}

	wHeight = height;
	wWidth = width;

	glViewport(0, 0, width, height);		/**< ���������ӿ� */
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(45.0f, (GLfloat)width / (GLfloat)height, 1.0f, 4000.0f);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity(); //�������ȱ��
}

void idle()
{
	//m_Camera.setViewByMouse();
	//Sleep(10);
	glutPostRedisplay();
}

void key(unsigned char key, int x, int y) {
	switch (key)
	{
	case 'p':m_Camera.setSpeed(1.0f); break;
	case 'w':m_Camera.moveCamera(m_Camera.getSpeed()); break;
	case 's':m_Camera.moveCamera(-m_Camera.getSpeed()); break;
	case 'a':m_Camera.yawCamera(-m_Camera.getSpeed()); break;
	case 'd':m_Camera.yawCamera(m_Camera.getSpeed()); break;
	default:
		if (key == 27)exit(0);
		break;
	}

	/** ���ݵ��θ߶ȸ�������� */
	Vector3 vPos = m_Camera.getPosition();                  /**< �õ���ǰ�����λ�� */
	Vector3 vNewPos = vPos;


	/** ����������߶�Ϊ ���θ߶� + 10 */
	vNewPos.y = (float)m_Terrain.getAveHeight(vPos.x, vPos.z) + 10;

	/** �õ��߶Ȳ�ֵ */
	float temp = vNewPos.y - vPos.y;

	/** ������������� */
	Vector3 vView = m_Camera.getView();
	vView.y += temp;

	/** ��������� */
	m_Camera.setCamera(vNewPos.x, vNewPos.y, vNewPos.z,
		vView.x, vView.y, vView.z,
		0, 1, 0);
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

void drawSnow() {
	//�����ĸ����ѩ��

	//����ѩ��
	/** ���û�����ӻ�ð�͸��Ч�� */
	glBlendFunc(GL_SRC_ALPHA, GL_ONE);
	glEnable(GL_BLEND);				     /**< ���û�� */

	//����
	glPushMatrix();
	//glTranslatef(0.0f, 0.0f, 4.0f);
	//ע�����ķ���λ����y�᷽��
	glTranslatef(m_Camera.getPosition().x, m_Camera.getPosition().y, m_Camera.getPosition().z);
	glColor4f(1.0f, 1.0f, 1.0f, 0.5f);
	m_Snow.Render();
	glPopMatrix();

	//����
	glPushMatrix();
	glTranslatef(m_Camera.getPosition().x, m_Camera.getPosition().y, m_Camera.getPosition().z);
	glColor4f(1.0f, 1.0f, 1.0f, 0.5f);
	glRotatef(180, 0.0, 1.0, 0.0);
	m_Snow.Render();
	glPopMatrix();

	//���
	glPushMatrix();
	glTranslatef(m_Camera.getPosition().x, m_Camera.getPosition().y, m_Camera.getPosition().z);
	glColor4f(1.0f, 1.0f, 1.0f, 0.5f);
	glRotatef(90, 0.0, 1.0, 0.0);
	m_Snow.Render();
	glPopMatrix();

	//�ұ�
	glPushMatrix();
	glTranslatef(m_Camera.getPosition().x, m_Camera.getPosition().y, m_Camera.getPosition().z);
	glColor4f(1.0f, 1.0f, 1.0f, 0.5f);
	glRotatef(270, 0.0, 1.0, 0.0);
	m_Snow.Render();
	glPopMatrix();

	glDisable(GL_BLEND);   //�رջ��
}

void redraw()
{
	/** �������λ���ڴ������� */
	glutWarpPointer(wWidth / 2, wHeight / 2);  //��������������������ܷ���mousefunc�������棬�����
											   //ʹ�û�����ͣ

	glMatrixMode(GL_MODELVIEW);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glLoadIdentity();									// Reset The Current Modelview Matrix

	m_Camera.setLook();			// ������0��0��0�����ӵ����� (0,5,50)��Y������


	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);


	glPushMatrix();
	glutSolidCube(1);
	glPopMatrix();

	//����skyBox

	m_SkyBox.render();

	//���Ƶ���

	m_Terrain.render();

	//����ѩǽ
	drawSnow();
	

	if (bAnim) fRotate += 0.5f;

	getFPS();

	glutSwapBuffers();
}

int main(int argc, char *argv[])
{
	wHeight = 720;
	wWidth = 1280;

	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_RGBA | GLUT_DEPTH | GLUT_DOUBLE);
	{
		int cx = glutGet(GLUT_SCREEN_WIDTH);
		int cy = glutGet(GLUT_SCREEN_HEIGHT);
		wPositionX = (cx - wWidth) >> 1;
		wPositionY = (cy - wHeight) >> 1;
		glutInitWindowPosition(wPositionX, wPositionY);
	}//��������������Ļ����
	glutInitWindowSize(wWidth, wHeight);

	int windowHandle = glutCreateWindow("Simple GLUT App");
	//glutFullScreen();
	init();
	glutDisplayFunc(redraw);
	glutReshapeFunc(reshape);
	glutKeyboardFunc(key);
	glutPassiveMotionFunc(&MouseMove);//����ƶ�
	glutIdleFunc(idle);


	glutMainLoop();
	return 0;
}
