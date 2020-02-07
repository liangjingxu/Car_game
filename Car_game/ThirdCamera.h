#pragma once

#include "Camera.h"


class ThirdCamera :public Camera{
public:
	ThirdCamera();
	virtual ~ThirdCamera();
	void SetPosition(XMFLOAT3 pos, XMFLOAT3 target); 
	void SetRotation(float x, float y);
	XMMATRIX GetViewMatrix();
	void ApplyRotation(float yawDelta, float pitchDelta);


private:
	XMFLOAT3 m_target;
	float m_distance;
	float m_xRotation, m_yRotation, m_xlimit, m_ylimit;
};