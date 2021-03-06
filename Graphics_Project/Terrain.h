#pragma once

#ifndef __TERRAIN_H__
#define __TERRAIN_H__


#include "stdafx.h"
#include "BMPLoader.h"
#include "camera.h"


/** 地形类 */
class CTerrain
{
public:

	/** 构造函数 */
	CTerrain();

	~CTerrain();

	/** 初始化地形 */
	bool init();

	/** 渲染地形 */
	void render();

	/** 设置是否使用细节纹理标志 */
	void setDetail(bool _bDetail) { m_bDetail = _bDetail; }

	/** 获得地面高度 */
	float getAveHeight(float x, float z);

	/** 得到当前Terrain指针 */
	static CTerrain* GetTerrainPointer() { return m_pTerrain; }


private:

	/** 设置地形的大小 */
	void setSize(unsigned  int width, unsigned  int cell);

	/** 载入'.raw'高度图 */
	bool loadRawFile(const char* fileName);

	/** 装载纹理 */
	bool loadTexture();

	/** 获得点(x,y)的高度信息 */
	int getHeight(int x, int y);

	/** 设置纹理坐标 */
	void setTexCoord(float x, float z);

	/** 设置雾效 */
	void initFog();



public:
	static CTerrain*  m_pTerrain;        /**< 当前Terrain指针 */
	unsigned  int     m_nWidth;          /**< 地形网格数 */
	unsigned  int     m_nCellWidth;      /**< 每一格宽度 */
	BYTE*             m_pHeightMap;      /**< 存放高度信息 */
	CBMPLoader        m_texture[2];      /**< 地面纹理和细节纹理 */
	bool              m_bDetail;         /**< 是否使用细节纹理标志 */
	int               m_DetailScale;     /**< 缩放比例 */

};

#endif //__TERRAIN_H__
