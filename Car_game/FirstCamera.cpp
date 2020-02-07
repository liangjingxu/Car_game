#include "FirstCamera.h"

using DirectX::XMMatrixLookAtLH;

FirstCamera::FirstCamera()
	: m_position(XMFLOAT3(0.0f, 2.0f, 0.0f)),
	m_target(XMFLOAT3(0.0f, 2.0f, 10.0f)),
	m_up(XMFLOAT3(0.0f, 1.0f, 0.0f))
{}

void FirstCamera::SetPosition(XMFLOAT3 pos, XMFLOAT3 target)
{
	m_position = pos;
	m_target = target;
}
XMMATRIX FirstCamera::GetViewMatrix()
{
	XMMATRIX viewMat = XMMatrixLookAtLH(XMLoadFloat3(&m_position), XMLoadFloat3(&m_target), XMLoadFloat3(&m_up));
	return viewMat;
}

XMFLOAT3 FirstCamera::GetPosition() {
	return m_position;
}