﻿#include "Car.h"
#include <WICTextureLoader.h>

SimpleVerCar m_vertices[] = {


		{ XMFLOAT3(-1,1,-1),XMFLOAT2(0.0f, 0.0f)},
		{ XMFLOAT3(1,1,-1),XMFLOAT2(1.0f, 0.0f)},
		{ XMFLOAT3(-1,1,1),XMFLOAT2(0.0f, 1.0f)},
		{ XMFLOAT3(1,1,1),XMFLOAT2(1.0f, 1.0f)},

		{ XMFLOAT3(-1,-1,1),XMFLOAT2(0.0f, 0.0f)},
		{ XMFLOAT3(1,-1,1),XMFLOAT2(1.0f, 0.0f)},
		{ XMFLOAT3(-1,-1,-1),XMFLOAT2(0.0f, 1.0f)},
		{ XMFLOAT3(1,-1,-1),XMFLOAT2(1.0f, 1.0f)},

		{ XMFLOAT3(-1,1,-1),XMFLOAT2(0.0f, 0.0f)},
		{ XMFLOAT3(-1,1,1),XMFLOAT2(1.0f, 0.0f)},
		{ XMFLOAT3(-1,-1,-1),XMFLOAT2(0.0f, 1.0f)},
		{ XMFLOAT3(-1,-1,1),XMFLOAT2(1.0f, 1.0f)},

		{ XMFLOAT3(1,1,1),XMFLOAT2(0.0f, 0.0f)},
		{ XMFLOAT3(1,1,-1),XMFLOAT2(1.0f, 0.0f)},
		{ XMFLOAT3(1,-1,1),XMFLOAT2(0.0f, 1.0f)},
		{ XMFLOAT3(1,-1,-1),XMFLOAT2(1.0f, 1.0f)},

		{ XMFLOAT3(-1,1,1),XMFLOAT2(0.0f, 0.0f)},
		{ XMFLOAT3(1,1,1),XMFLOAT2(1.0f, 0.0f)},
		{ XMFLOAT3(-1,-1,1),XMFLOAT2(0.0f, 1.0f)},
		{ XMFLOAT3(1,-1,1),XMFLOAT2(1.0f, 1.0f)},

		{ XMFLOAT3(1,1,-1),XMFLOAT2(0.0f, 0.0f)},
		{ XMFLOAT3(-1,1,-1),XMFLOAT2(1.0f, 0.0f)},
		{ XMFLOAT3(1,-1,-1),XMFLOAT2(0.0f, 1.0f)},
		{ XMFLOAT3(-1,-1,-1),XMFLOAT2(1.0f, 1.0f)}
};


Car::Car()
	:bodycolorMap_(nullptr),
	tirefrontcolorMap_(nullptr),
	tiresidecolorMap_(nullptr),
	bodyvertexBuffer_(nullptr),
	tirevertexBuffer_(nullptr),
	bodyindexBuffer_(nullptr),
	tirefrontindexBuffer_(nullptr),
	tiresideindexBuffer_(nullptr),
	wholerotation_(0.0f),
	tirerotation_(0.0f),
	tiretexturerotation_(0.0f)
{
	center_.x = center_.z = 0.0f;
	center_.y = 2.0f;
	for (auto &it : wheelturn_)
		it = 0;


	
}

Car::~Car() {
	if (bodycolorMap_) bodycolorMap_->Release();
	if (tirefrontcolorMap_) tirefrontcolorMap_->Release();
	if (tiresidecolorMap_) tiresidecolorMap_->Release();
	if (bodyvertexBuffer_) bodyvertexBuffer_->Release();
	if (tirevertexBuffer_) tirevertexBuffer_->Release();
	if (bodyindexBuffer_) bodyindexBuffer_->Release();
	if (tirefrontindexBuffer_) tirefrontindexBuffer_->Release();
	if (tiresideindexBuffer_) tiresideindexBuffer_->Release();
	bodycolorMap_ = nullptr;
	tirefrontcolorMap_ = nullptr;
	tiresidecolorMap_ = nullptr;
	bodyvertexBuffer_ = nullptr;
	tirevertexBuffer_ = nullptr;
	bodyindexBuffer_ = nullptr;
	tirefrontindexBuffer_ = nullptr;
	tiresideindexBuffer_ = nullptr;
}

bool Car::Init_Resource(ID3D11Device* d3dDevice_) {
	HRESULT d3dResult;

	//创建顶点缓冲区
	D3D11_BUFFER_DESC vertexDesc;
	ZeroMemory(&vertexDesc, sizeof(vertexDesc));
	vertexDesc.Usage = D3D11_USAGE_DEFAULT;
	vertexDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	vertexDesc.ByteWidth = sizeof(SimpleVerCar) * 8;
	d3dResult = d3dDevice_->CreateBuffer(&vertexDesc, nullptr, &bodyvertexBuffer_);
	if (FAILED(d3dResult))
		return false;


	SimpleVerCar tirevertices[1442] =
	{
		{XMFLOAT3(-0.5f,0.0f,0.0f),XMFLOAT2(0.0f,0.0f)},
		{XMFLOAT3(0.5f,0.0f,0.0f),XMFLOAT2(0.0f,0.0f)}
	};
	tirevertices[2] = { XMFLOAT3(-0.5,0.5,0), XMFLOAT2(0,0.5) };
	tirevertices[3] = { XMFLOAT3(-0.5,0.5,0),XMFLOAT2(0,0) };
	tirevertices[4] = { XMFLOAT3(0.5,0.5,0),XMFLOAT2(0,0.5) };
	tirevertices[5] = { XMFLOAT3(0.5,0.5,0),XMFLOAT2(1,0) };
	XMMATRIX rotationMat1 = XMMatrixRotationRollPitchYaw(-XM_PI / 90.0f, 0.0f, 0.0f);
	XMMATRIX rotationMat2 = XMMatrixRotationRollPitchYaw(0.0f, 0.0f, XM_PI / 90.0f);
	XMVECTOR vec1 = XMVectorSet(-0.5f, 0.5f, 0.0f, 1.0f);
	XMVECTOR vec2 = XMVectorSet(0.5f, 0.5f, 0.0f, 1.0f);
	XMVECTOR vec3 = XMVectorSet(0.0f, 0.5f, 0.0f, 1.0f);
	vec1 = XMVector4Transform(vec1, rotationMat1);
	vec2 = XMVector4Transform(vec2, rotationMat1);
	vec3 = XMVector4Transform(vec3, rotationMat2);
	XMFLOAT3 next1, next2;
	XMFLOAT2 next3;
	XMStoreFloat3(&next1, vec1);
	XMStoreFloat3(&next2, vec2);
	XMStoreFloat2(&next3, vec3);
	tirevertices[6] = { next1,next3 };
	tirevertices[7] = { next1,XMFLOAT2(0,1) };
	tirevertices[8] = { next2,next3 };
	tirevertices[9] = { next2,XMFLOAT2(1,1) };

	//创建索引缓存
	WORD bodyindices[] =
	{
		2,1,0,2,3,1
	};
	WORD tirefrontindices[] = {
		3,5,7,5,9,7
	};
	WORD tiresideindices[] = {
		0,2,6,1,8,4
	};

	D3D11_BUFFER_DESC indexDesc;
	ZeroMemory(&indexDesc, sizeof(indexDesc));
	indexDesc.Usage = D3D11_USAGE_DEFAULT;
	indexDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
	indexDesc.ByteWidth = sizeof(WORD) * 6;
	indexDesc.CPUAccessFlags = 0;

	D3D11_SUBRESOURCE_DATA resourceData;
	ZeroMemory(&resourceData, sizeof(resourceData));
	resourceData.pSysMem = tirevertices;
	vertexDesc.ByteWidth = sizeof(SimpleVerCar) * 10;
	d3dResult = d3dDevice_->CreateBuffer(&vertexDesc, &resourceData, &tirevertexBuffer_);
	if (FAILED(d3dResult))
		return false;
	resourceData.pSysMem = bodyindices;
	d3dResult = d3dDevice_->CreateBuffer(&indexDesc, &resourceData, &bodyindexBuffer_);
	if (FAILED(d3dResult))
		return false;
	indexDesc.ByteWidth = sizeof(WORD) * 6;
	resourceData.pSysMem = tirefrontindices;
	d3dResult = d3dDevice_->CreateBuffer(&indexDesc, &resourceData, &tirefrontindexBuffer_);
	if (FAILED(d3dResult))
		return false;
	resourceData.pSysMem = tiresideindices;
	d3dResult = d3dDevice_->CreateBuffer(&indexDesc, &resourceData, &tiresideindexBuffer_);
	if (FAILED(d3dResult))
		return false;



	//加载贴图获得资源视图对象
	array<const wchar_t*, 6> path = { L"top.BMP",L"bottom.BMP",L"left.BMP",L"right.BMP",L"front.BMP",L"back.BMP" };
	wchar_t texture[15] = L"Texture\\";
	int num = path.size();
	for (int i = 0; i < num; ++i) {
		wchar_t target[30];
		wcscpy_s(target, texture);
		wcscat_s(target, path[i]);
		d3dResult = CreateWICTextureFromFile(d3dDevice_, target, nullptr, &m_colorMap[i]);
		if (FAILED(d3dResult))
			return false;
	}

	d3dResult = CreateWICTextureFromFile(d3dDevice_, L"Texture\\tirefront.jpg", nullptr, &tirefrontcolorMap_);
	if (FAILED(d3dResult))
		return false;
	d3dResult = CreateWICTextureFromFile(d3dDevice_, L"Texture\\tireside.jpg", nullptr, &tiresidecolorMap_);
	if (FAILED(d3dResult))
		return false;
	return true;
}

void Car::setCenter(XMFLOAT3 center) {
	center_ = center;
}

void Car::setWholerotation(float wholerotation) {
	wholerotation_ = wholerotation;
}

void Car::settiretexturerotaion(float distance) {
	tiretexturerotation_ += distance * 2;
	if (tiretexturerotation_ >= XM_2PI)
		tiretexturerotation_ -= XM_2PI;
	if (tiretexturerotation_ < 0)
		tiretexturerotation_ += XM_2PI;
}
void Car::setwheelTurn(int direction, int turn) {
	switch (direction) {

	case -1:
	case 1:
		wheelturn_[1] = turn; 
		break;
	case 0:
		wheelturn_[0] = wheelturn_[1] = 0; 
		break;
	default:
		break;
	}
}

void Car::Renderbody(ID3D11DeviceContext* d3dContext, ID3D11Buffer* worldCB_, ID3D11Buffer* viewCB_) {
	
	unsigned int stride = sizeof(SimpleVerCar);
	unsigned int offset = 0;
	d3dContext->IASetVertexBuffers(0, 1, &bodyvertexBuffer_, &stride, &offset);
	d3dContext->IASetIndexBuffer(bodyindexBuffer_, DXGI_FORMAT_R16_UINT, 0);
	d3dContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	XMMATRIX rotationMat = XMMatrixRotationRollPitchYaw(0.0f, wholerotation_, 0.0f);
	XMMATRIX translationMat = XMMatrixTranslation(center_.x, 2.0f, center_.z);
	XMMATRIX scaling = XMMatrixScaling(1.5f, 1.0f, 1.5f);
	XMMATRIX worldMat = rotationMat * scaling * translationMat;
	worldMat = XMMatrixTranspose(worldMat);
	d3dContext->UpdateSubresource(worldCB_, 0, 0, &worldMat, 0, 0);
	int num = m_colorMap.size();
	SimpleVerCar vertex[4];
	for (int i = 0; i < num; ++i) {
		for (int j = 0; j < 4; ++j)
		{
			vertex[j] = m_vertices[4 * i + j];
		}
		d3dContext->UpdateSubresource(bodyvertexBuffer_, 0, 0, &vertex, 0, 0);
		d3dContext->PSSetShaderResources(0, 1, &m_colorMap[i]);
		d3dContext->DrawIndexed(7, 0, 0);
	}
}


void Car::Rendertire(ID3D11DeviceContext* d3dContext_, ID3D11Buffer* worldCB_, ID3D11Buffer* viewCB_, ID3D11VertexShader* tiresolidColorVS_, ID3D11Buffer* textureCB_) {
	unsigned int stride = sizeof(SimpleVerCar);
	unsigned int offset = 0;
	d3dContext_->IASetVertexBuffers(0, 1, &tirevertexBuffer_, &stride, &offset);
	d3dContext_->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	const float radian = -XM_PI / 90.0f;
	XMVECTOR tireoffset[4] = {
		XMVectorSet(-1.0f,0.5f,-1.0f,1.0f),
		XMVectorSet(1.0f,0.5f,-1.0f,1.0f),
		XMVectorSet(-1.0f,0.5f,1.0f,1.0f),
		XMVectorSet(1.0f,0.5f,1.0f,1.0f),
	};
	XMMATRIX translationMat[4];
	for (int i = 0; i < 4; ++i) {
		tireoffset[i] = XMVector4Transform(tireoffset[i], XMMatrixRotationRollPitchYaw(0.0f, wholerotation_, 0.0f));
		tireoffset[i] += XMVectorSet(center_.x, 0.0f, center_.z, 1.0f);
		translationMat[i] = XMMatrixTranslationFromVector(tireoffset[i]);
	}
	for (int i = 0; i < 4; ++i) {
		d3dContext_->IASetIndexBuffer(tirefrontindexBuffer_, DXGI_FORMAT_R16_UINT, 0);
		d3dContext_->PSSetShaderResources(0, 1, &tirefrontcolorMap_);
		for (int j = 0; j < 180; ++j) {
			XMMATRIX rotationMat = XMMatrixRotationRollPitchYaw(radian*j + tiretexturerotation_, wholerotation_ + wheelturn_[i / 2] * XM_PIDIV4, 0.0f);
			XMMATRIX worldMat = rotationMat * translationMat[i];
			worldMat = XMMatrixTranspose(worldMat);
			d3dContext_->UpdateSubresource(worldCB_, 0, 0, &worldMat, 0, 0);
			d3dContext_->DrawIndexed(6, 0, 0);
		}
	}
	for (int i = 0; i < 4; ++i) {
		d3dContext_->VSSetShader(tiresolidColorVS_, 0, 0);
		d3dContext_->IASetIndexBuffer(tiresideindexBuffer_ , DXGI_FORMAT_R16_UINT, 0);
		d3dContext_->PSSetShaderResources(0, 1, &tiresidecolorMap_);
		for (int j = 0; j < 180; ++j) {
			XMMATRIX rotationMat = XMMatrixRotationRollPitchYaw(radian*j  + tiretexturerotation_, wholerotation_ + wheelturn_[i >> 1] * XM_PIDIV4, 0.0f);
			XMMATRIX worldMat = rotationMat * translationMat[i];
			worldMat = XMMatrixTranspose(worldMat);
			d3dContext_->UpdateSubresource(worldCB_, 0, 0, &worldMat, 0, 0);
			XMMATRIX textureMat = XMMatrixRotationRollPitchYaw(0.0f, 0.0f, radian*j);
			textureMat *= XMMatrixTranslation(0.5f, 0.5f, 0.0f);
			d3dContext_->UpdateSubresource(textureCB_, 0, 0, &textureMat, 0, 0);
			d3dContext_->DrawIndexed(6, 0, 0);
		}
	}
}
void Car::Render(ID3D11DeviceContext* d3dContext_, ID3D11Buffer* worldCB_, ID3D11Buffer* viewCB_, ID3D11VertexShader* tiresolidColorVS_, ID3D11Buffer* textureCB_) {
	Renderbody(d3dContext_, worldCB_, viewCB_);
	Rendertire(d3dContext_, worldCB_, viewCB_, tiresolidColorVS_, textureCB_);
}