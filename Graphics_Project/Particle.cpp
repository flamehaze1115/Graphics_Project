#include "Particle.h"

/** ���캯�� */
CParticle::CParticle() :m_iNum(0), m_pList(NULL)
{
}

/** �������� */
CParticle::~CParticle()
{
	if (m_pList != NULL)
	{
		delete[]m_pList;
		m_pList = NULL;
	}

}

/** ���ӵĳ�ʼ�� */
bool CParticle::Init(int _num)
{
	m_iNum = _num;
	m_pList = new Particle[m_iNum];
	if (m_pList == NULL)
		return false;
	return true;
}

/** ���ӵ���Ⱦ */
void CParticle::Render()
{
}

/** ���ӵĸ��� */
void CParticle::Update()
{
}