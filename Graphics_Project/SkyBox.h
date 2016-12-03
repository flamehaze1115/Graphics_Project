#pragma once
#include "stdafx.h"
#include "BMPLoader.h"
#include "Vector.h"
#include "Camera.h"

#define GL_CLAMP_TO_EDGE	0x812F

/** ��պ��� */
class CSkyBox
{
public:
	/** ���캯�� */
	CSkyBox();
	~CSkyBox();

	/** ��ʼ�� */
	bool init();

	/** ��Ⱦ */
	void render();

private:

	CBMPLoader  m_texture[5];   /**< ��պ�����   */
	Vector3     m_CameraPos;    /**< ��ǰ�����λ�� */
	float       length;         /**< ���� */
	float       width;          /**< ���� */
	float       height;         /**< �߶� */
	float       yRot;           /**< ��Y����ת */

};
