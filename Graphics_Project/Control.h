#pragma once
Camera m_Camera;
int wHeight = 0;
int wWidth = 0;

int wPositionX = 0;
int wPositionY = 0;
void MouseMove(int x, int y)   //无点击的鼠标移动
{
	POINT mousePos;									  /**< 保存当前鼠标位置 */
													  //int middleX = GetSystemMetrics(SM_CXSCREEN) >> 1; /**< 得到屏幕宽度的一半 */
													  //int middleY = GetSystemMetrics(SM_CYSCREEN) >> 1; /**< 得到屏幕高度的一半 */
	int middleX = wWidth>>1;
	int middleY = wHeight >> 1;
	float angleY = 0.0f;							  /**< 摄像机左右旋转角度 */
	float angleZ = 0.0f;		                      /**< 摄像机上下旋转角度 */
	static float currentRotX = 0.0f;

	mousePos.x = x;
	mousePos.y = y;
	/**< 得到鼠标移动方向 */
	angleY = (float)((middleX - mousePos.x)) / 1000.0f;
	angleZ = (float)((middleY - mousePos.y)) / 1000.0f;

	static float lastRotX = 0.0f;      /**< 用于保存旋转角度 */
	lastRotX = currentRotX;

	/** 跟踪摄像机上下旋转角度 */
	currentRotX += angleZ;

	/** 如果上下旋转弧度大于1.0,我们截取到1.0并旋转 */
	if (currentRotX > 1.0f)
	{
		currentRotX = 1.0f;

		/** 根据保存的角度旋转方向 */
		if (lastRotX != 1.0f)
		{
			/** 通过叉积找到与旋转方向垂直的向量 */
			Vector3 vAxis = m_Camera.getView() - m_Camera.getPosition();
			vAxis = vAxis.crossProduct(m_Camera.getUpVector());
			vAxis = vAxis.normalize();

			///旋转
			m_Camera.rotateView(1.0f - lastRotX, vAxis.x, vAxis.y, vAxis.z);
		}
	}
	/** 如果旋转弧度小于-1.0,则也截取到-1.0并旋转 */
	else if (currentRotX < -1.0f)
	{
		currentRotX = -1.0f;

		if (lastRotX != -1.0f)
		{

			/** 通过叉积找到与旋转方向垂直的向量 */
			Vector3 vAxis = m_Camera.getView() - m_Camera.getPosition();
			vAxis = vAxis.crossProduct(m_Camera.getUpVector());
			vAxis = vAxis.normalize();

			///旋转
			m_Camera.rotateView(-1.0f - lastRotX, vAxis.x, vAxis.y, vAxis.z);
		}
	}
	/** 否则就旋转angleZ度 */
	else
	{
		/** 找到与旋转方向垂直向量 */
		Vector3 vAxis = m_Camera.getView() - m_Camera.getPosition();
		vAxis = vAxis.crossProduct(m_Camera.getUpVector());
		vAxis = vAxis.normalize();

		///旋转
		m_Camera.rotateView(angleZ, vAxis.x, vAxis.y, vAxis.z);
	}

	/** 总是左右旋转摄像机 */
	m_Camera.rotateView(angleY, 0, 1, 0);

}