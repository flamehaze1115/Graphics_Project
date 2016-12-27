#define _CRT_SECURE_NO_WARNINGS
// glutEx1.cpp : 定义控制台应用程序的入口点。
#include <glut.h>
#include"BMPLoader.h"
#include"Camera.h"
#include"SkyBox.h"
#include"stdafx.h"
#include"Vector.h"
#include"Control.h"
#include"Terrain.h"
#include"Snow.h"

bool    m_RenderMode;		          /**< 绘制模式 */
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

extern int wHeight;//窗口的高度和宽度
extern int wWidth;
extern int wPositionX;//窗口设置的原点
extern int wPositionY;

bool init() {
	/** 用户自定义的初始化过程 */
	glClearColor(0.0f, 0.0f, 0.0f, 0.5f);
	glClearDepth(1.0f);
	glDepthFunc(GL_LEQUAL);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);
	glShadeModel(GL_SMOOTH);
	glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);

	/** 初始化天空 */
	if (!m_SkyBox.init())
	{
		MessageBox(NULL, L"初始化天空失败!", L"错误", MB_OK);
		exit(0);
	}
	/** 初始化地形 */
	if (!m_Terrain.init())
	{
		MessageBox(NULL, L"初始化地形失败!", L"错误", MB_OK);
		exit(0);
	}

	/** 初始化雪花实例 */
	if (!m_Snow.Init(5000))
	{
		MessageBox(NULL, L"雪花系统初始化失败!", L"错误", MB_OK);
		exit(-1);
	}

	/** 设置摄像机 */
	m_Camera.setCamera(500, 35, 400, 501, 35, 400, 0, 1, 0);

	return true;                                        /**< 成功返回 */
}



void updateView(int width, int height)
{

	glViewport(0, 0, width, height);						// Reset The Current Viewport

	glMatrixMode(GL_PROJECTION);						// Select The Projection Matrix
	glLoadIdentity();									// Reset The Projection Matrix

	float whRatio = (float)width / (height ? height : 1);

	//gluPerspective(45, 1, 1, 100);
	gluPerspective(45.0f, whRatio, 1.0f, 5000.0f); //设置不合理的话 不能看看见全部的skybox

	glMatrixMode(GL_MODELVIEW);							// Select The Modelview Matrix
	glLoadIdentity(); //这里必须有
}

void reshape(int width, int height)
{
	if (height == 0)						// Prevent A Divide By Zero By
	{
		height = 1;					// Making Height Equal One
	}

	wHeight = height;
	wWidth = width;

	glViewport(0, 0, width, height);		/**< 重新设置视口 */
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(45.0f, (GLfloat)width / (GLfloat)height, 1.0f, 4000.0f);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity(); //这个不能缺少
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

	/** 根据地形高度更新摄像机 */
	Vector3 vPos = m_Camera.getPosition();                  /**< 得到当前摄像机位置 */
	Vector3 vNewPos = vPos;


	/** 设置摄像机高度为 地形高度 + 10 */
	vNewPos.y = (float)m_Terrain.getAveHeight(vPos.x, vPos.z) + 10;

	/** 得到高度差值 */
	float temp = vNewPos.y - vPos.y;

	/** 更新摄像机方向 */
	Vector3 vView = m_Camera.getView();
	vView.y += temp;

	/** 设置摄像机 */
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
	glMatrixMode(GL_PROJECTION);// 选择投影矩阵
	glPushMatrix();// 保存原矩阵
	glLoadIdentity();// 装入单位矩阵
	glOrtho(0, 480, 0, 480, -1, 1);// 位置正投影
	glMatrixMode(GL_MODELVIEW);// 选择Modelview矩阵
	glPushMatrix();// 保存原矩阵
	glLoadIdentity();// 装入单位矩阵*/
	glRasterPos2f(10, 10);
	for (c = buffer; *c != '\0'; c++) {
		glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, *c);
	}
	glMatrixMode(GL_PROJECTION);// 选择投影矩阵
	glPopMatrix();// 重置为原保存矩阵
	glMatrixMode(GL_MODELVIEW);// 选择Modelview矩阵
	glPopMatrix();// 重置为原保存矩阵
	glEnable(GL_DEPTH_TEST);
}

void drawSnow() {
	//绘制四个面的雪花

	//绘制雪花
	/** 设置混合因子获得半透明效果 */
	glBlendFunc(GL_SRC_ALPHA, GL_ONE);
	glEnable(GL_BLEND);				     /**< 启用混和 */

	//正面
	glPushMatrix();
	//glTranslatef(0.0f, 0.0f, 4.0f);
	//注意地面的法线位置是y轴方向
	glTranslatef(m_Camera.getPosition().x, m_Camera.getPosition().y, m_Camera.getPosition().z);
	glColor4f(1.0f, 1.0f, 1.0f, 0.5f);
	m_Snow.Render();
	glPopMatrix();

	//反面
	glPushMatrix();
	glTranslatef(m_Camera.getPosition().x, m_Camera.getPosition().y, m_Camera.getPosition().z);
	glColor4f(1.0f, 1.0f, 1.0f, 0.5f);
	glRotatef(180, 0.0, 1.0, 0.0);
	m_Snow.Render();
	glPopMatrix();

	//左边
	glPushMatrix();
	glTranslatef(m_Camera.getPosition().x, m_Camera.getPosition().y, m_Camera.getPosition().z);
	glColor4f(1.0f, 1.0f, 1.0f, 0.5f);
	glRotatef(90, 0.0, 1.0, 0.0);
	m_Snow.Render();
	glPopMatrix();

	//右边
	glPushMatrix();
	glTranslatef(m_Camera.getPosition().x, m_Camera.getPosition().y, m_Camera.getPosition().z);
	glColor4f(1.0f, 1.0f, 1.0f, 0.5f);
	glRotatef(270, 0.0, 1.0, 0.0);
	m_Snow.Render();
	glPopMatrix();

	glDisable(GL_BLEND);   //关闭混合
}

void redraw()
{
	/** 设置鼠标位置在窗口中心 */
	glutWarpPointer(wWidth / 2, wHeight / 2);  //这个函数必须放在这里，不能放在mousefunc（）里面，否则会
											   //使得绘制暂停

	glMatrixMode(GL_MODELVIEW);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glLoadIdentity();									// Reset The Current Modelview Matrix

	m_Camera.setLook();			// 场景（0，0，0）的视点中心 (0,5,50)，Y轴向上


	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);


	glPushMatrix();
	glutSolidCube(1);
	glPopMatrix();

	//绘制skyBox

	m_SkyBox.render();

	//绘制地面

	m_Terrain.render();

	//绘制雪墙
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
	}//将窗口设置在屏幕中心
	glutInitWindowSize(wWidth, wHeight);

	int windowHandle = glutCreateWindow("Simple GLUT App");
	//glutFullScreen();
	init();
	glutDisplayFunc(redraw);
	glutReshapeFunc(reshape);
	glutKeyboardFunc(key);
	glutPassiveMotionFunc(&MouseMove);//鼠标移动
	glutIdleFunc(idle);


	glutMainLoop();
	return 0;
}
