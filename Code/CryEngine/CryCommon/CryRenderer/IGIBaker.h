#pragma once

#include "../CryMath/Cry_Vector3.h"
#include "../Cry3DEngine/IIndexedMesh.h"
#include <vector>

struct SMeshParam
{
	Matrix44A m_worldTM;
	Matrix44A m_rotationTM;
	Vec4 m_lightMapScaleAndBias;
};



struct IGIBaker
{
public:
	virtual void AddDirectionalLight() = 0;
	virtual void AddPointLight() = 0;
	virtual void AddMesh(IStatObj* inputStatObj,SMeshParam meshParam) = 0;
	virtual void Bake() = 0;
};