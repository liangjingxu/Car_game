#pragma once

#include "Camera.h"
#include <DirectXMath.h>


using DirectX::XMFLOAT3;
using DirectX::XMMATRIX;


class FirstCamera :public Camera {
public:
	FirstCamera();

	void SetPosition(XMFLOAT3 pos, XMFLOAT3 target);
	XMFLOAT3 GetPosition();
	XMMATRIX GetViewMatrix();
	void ApplyRotation(float yawDelta, float pitchDelta) {}

private:
	XMFLOAT3 m_position;
	XMFLOAT3 m_target;
	XMFLOAT3 m_up;
};
