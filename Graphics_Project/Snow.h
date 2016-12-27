#pragma once

#ifndef  __SNOW_H__
#define  __SNOW_H__


#include "BMPLoader.h"
#include "Particle.h"


/** 雪花渲染类 */
class CSnow : public CParticle
{

public:
	CSnow() {};
	~CSnow() {};

	bool		Init(int num);  /** 初始化过程 */
	void		Render();        /**< 渲染过程 */
	void		Update();        /**< 更新过程 */

private:
	CBMPLoader  m_texture;       /**< 粒子的纹理 */


};

#endif //__SNOW_H__
