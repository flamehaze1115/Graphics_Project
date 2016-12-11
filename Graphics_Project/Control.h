#pragma once
Camera m_Camera;
int wHeight = 0;
int wWidth = 0;

int wPositionX = 0;
int wPositionY = 0;
void MouseMove(int x, int y)   //�޵��������ƶ�
{
	POINT mousePos;									  /**< ���浱ǰ���λ�� */
													  //int middleX = GetSystemMetrics(SM_CXSCREEN) >> 1; /**< �õ���Ļ��ȵ�һ�� */
													  //int middleY = GetSystemMetrics(SM_CYSCREEN) >> 1; /**< �õ���Ļ�߶ȵ�һ�� */
	int middleX = wWidth>>1;
	int middleY = wHeight >> 1;
	float angleY = 0.0f;							  /**< �����������ת�Ƕ� */
	float angleZ = 0.0f;		                      /**< �����������ת�Ƕ� */
	static float currentRotX = 0.0f;

	mousePos.x = x;
	mousePos.y = y;
	/**< �õ�����ƶ����� */
	angleY = (float)((middleX - mousePos.x)) / 1000.0f;
	angleZ = (float)((middleY - mousePos.y)) / 1000.0f;

	static float lastRotX = 0.0f;      /**< ���ڱ�����ת�Ƕ� */
	lastRotX = currentRotX;

	/** ���������������ת�Ƕ� */
	currentRotX += angleZ;

	/** ���������ת���ȴ���1.0,���ǽ�ȡ��1.0����ת */
	if (currentRotX > 1.0f)
	{
		currentRotX = 1.0f;

		/** ���ݱ���ĽǶ���ת���� */
		if (lastRotX != 1.0f)
		{
			/** ͨ������ҵ�����ת����ֱ������ */
			Vector3 vAxis = m_Camera.getView() - m_Camera.getPosition();
			vAxis = vAxis.crossProduct(m_Camera.getUpVector());
			vAxis = vAxis.normalize();

			///��ת
			m_Camera.rotateView(1.0f - lastRotX, vAxis.x, vAxis.y, vAxis.z);
		}
	}
	/** �����ת����С��-1.0,��Ҳ��ȡ��-1.0����ת */
	else if (currentRotX < -1.0f)
	{
		currentRotX = -1.0f;

		if (lastRotX != -1.0f)
		{

			/** ͨ������ҵ�����ת����ֱ������ */
			Vector3 vAxis = m_Camera.getView() - m_Camera.getPosition();
			vAxis = vAxis.crossProduct(m_Camera.getUpVector());
			vAxis = vAxis.normalize();

			///��ת
			m_Camera.rotateView(-1.0f - lastRotX, vAxis.x, vAxis.y, vAxis.z);
		}
	}
	/** �������תangleZ�� */
	else
	{
		/** �ҵ�����ת����ֱ���� */
		Vector3 vAxis = m_Camera.getView() - m_Camera.getPosition();
		vAxis = vAxis.crossProduct(m_Camera.getUpVector());
		vAxis = vAxis.normalize();

		///��ת
		m_Camera.rotateView(angleZ, vAxis.x, vAxis.y, vAxis.z);
	}

	/** ����������ת����� */
	m_Camera.rotateView(angleY, 0, 1, 0);

}