#pragma once

#ifndef  __SNOW_H__
#define  __SNOW_H__


#include "BMPLoader.h"
#include "Particle.h"


/** ѩ����Ⱦ�� */
class CSnow : public CParticle
{

public:
	CSnow() {};
	~CSnow() {};

	bool		Init(int num);  /** ��ʼ������ */
	void		Render();        /**< ��Ⱦ���� */
	void		Update();        /**< ���¹��� */

private:
	CBMPLoader  m_texture;       /**< ���ӵ����� */


};

#endif //__SNOW_H__
