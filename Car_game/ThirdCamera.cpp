#include "ThirdCamera.h"
#include <algorithm>

using std::max;
using std::min;

ThirdCamera::ThirdCamera()
	:m_target(0.0f, 2.0f, 0.0f),
	m_distance(5.0f),
	m_xRotation(0.0f),
	m_yRotation(0.0f),
	m_xlimit(XM_PIDIV2),
	m_ylimit(XM_PI)
{}

ThirdCamera::~ThirdCamera() {}

void ThirdCamera::SetRotation(float x, float y)
{
	m_xRotation = x;
	m_yRotation = y;
	m_xRotation = max(m_xRotation, -m_xlimit);
	m_xRotation = min(m_xRotation, m_xlimit);
	m_yRotation = max(m_yRotation, -m_ylimit);
	m_yRotation = min(m_yRotation, m_ylimit);
}

void ThirdCamera::SetPosition(XMFLOAT3 pos, XMFLOAT3 target)
{
	m_target = target;
}

void ThirdCamera::ApplyRotation(float yawDelta, float pitchDelta)
{
	m_xRotation += yawDelta;
	m_yRotation += pitchDelta;
	m_xRotation = max(m_xRotation, -m_xlimit);
	m_xRotation = min(m_xRotation, m_xlimit);
	m_yRotation = max(m_yRotation, -m_ylimit);
	m_yRotation = min(m_yRotation, m_ylimit);
}

XMMATRIX ThirdCamera::GetViewMatrix()
{
	XMVECTOR zoom = XMVectorSet(0.0f, 0.0f, -m_distance, 1.0f);
	XMMATRIX rotation = XMMatrixRotationRollPitchYaw(m_xRotation, m_yRotation, 0.0f);
	zoom = XMVector3Transform(zoom, rotation);
	XMVECTOR lookAt = XMLoadFloat3(&m_target);
	XMVECTOR pos = lookAt + zoom;
	XMVECTOR up = XMVectorSet(0.0f, 1.0f, 0.0f, 1.0f);
	up = XMVector3Transform(up, rotation);
	XMMATRIX viewMat = XMMatrixLookAtLH(pos, lookAt, up);
	return viewMat;
}