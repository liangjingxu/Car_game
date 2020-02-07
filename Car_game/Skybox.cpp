#include "SkyBox.h"
#include  <WICTextureLoader.h>
#include "DemoBase.h"



using namespace DirectX;



SkyBox::SkyBox()
{
	
	m_vertexs = {
		array<int,4>{4,5,6,7},
		{2,3,0,1},
		{4,6,0,2},
		{7,5,3,1},
		{6,7,2,3},
		{5,4,1,0}
	};
	m_pos = {
		
		XMFLOAT3(-1.0f ,-1.0f ,-1.0f),
		XMFLOAT3(1.0f ,-1.0f ,-1.0f),
		XMFLOAT3(-1.0f ,-1.0f ,1.0f),
		XMFLOAT3(1.0f ,-1.0f ,1.0f),
		XMFLOAT3(-1.0f ,1.0f ,-1.0f),
		XMFLOAT3(1.0f ,1.0f ,-1.0f),
		XMFLOAT3(-1.0f ,1.0f ,1.0f),
		XMFLOAT3(1.0f ,1.0f ,1.0f)
	};

	for (auto &it : m_colorMap)
		it = nullptr;

	m_vertexBuffer = nullptr;
	m_indexBuffer = nullptr;

	
}

SkyBox::~SkyBox() {
	if (m_vertexBuffer)
	{
		m_vertexBuffer->Release();
	}
	m_vertexBuffer = nullptr;

	if (m_indexBuffer)
	{
		m_indexBuffer->Release();
	}
	m_indexBuffer = nullptr;

	for (auto &it : m_colorMap) {
		if (it) it->Release();
		it = nullptr;
	}
}

bool SkyBox::Init_Resource(ID3D11Device* d3dDevice_) {
	HRESULT d3dResult;
	//创建顶点缓冲区

	D3D11_BUFFER_DESC vertexDesc;
	ZeroMemory(&vertexDesc, sizeof(vertexDesc));
	vertexDesc.Usage = D3D11_USAGE_DEFAULT;
	vertexDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	vertexDesc.ByteWidth = sizeof(VertexPosSkyBox) * 4;
	d3dResult = d3dDevice_->CreateBuffer(&vertexDesc, nullptr, &m_vertexBuffer);
	if (FAILED(d3dResult))
		return false;


	//创建索引缓存
	WORD indices[] =
	{
		0,1,2,1,3,2
	};
	D3D11_BUFFER_DESC indexDesc;
	ZeroMemory(&indexDesc, sizeof(indexDesc));
	indexDesc.Usage = D3D11_USAGE_DEFAULT;
	indexDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
	indexDesc.ByteWidth = sizeof(WORD) * 6;
	indexDesc.CPUAccessFlags = 0;
	D3D11_SUBRESOURCE_DATA resourceData;
	ZeroMemory(&resourceData, sizeof(resourceData));
	resourceData.pSysMem = indices;
	d3dResult = d3dDevice_->CreateBuffer(&indexDesc, &resourceData, &m_indexBuffer);
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
	return true;
}

void SkyBox::Render(ID3D11DeviceContext* d3dContext_, ID3D11Buffer* worldCB_, ID3D11Buffer* viewCB_) {
	unsigned int stride = sizeof(VertexPosSkyBox);
	unsigned int offset = 0;
	d3dContext_->IASetVertexBuffers(0, 1, &m_vertexBuffer, &stride, &offset);
	d3dContext_->IASetIndexBuffer(m_indexBuffer, DXGI_FORMAT_R16_UINT, 0);
	d3dContext_->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	XMMATRIX rotationMat = XMMatrixRotationRollPitchYaw(0.0f, 0.0f, 0.0f);
	XMMATRIX translationMat = XMMatrixTranslation(0.0f, 0.0f, 0.0f);
	XMMATRIX scaling = XMMatrixScaling(10000.0f, 10000.0f, 10000.0f);
	XMMATRIX worldMat = rotationMat * scaling * translationMat;
	worldMat = XMMatrixTranspose(worldMat);
	d3dContext_->UpdateSubresource(worldCB_, 0, 0, &worldMat, 0, 0);
	int num = m_colorMap.size();
	for (int i = 0; i < num; ++i) {
		array<int, 4> index = m_vertexs[i];
		VertexPosSkyBox vertex[] = {
			{m_pos[index[0]],XMFLOAT2(0,0)},
			{m_pos[index[1]],XMFLOAT2(1,0)},
			{m_pos[index[2]],XMFLOAT2(0,1)},
			{m_pos[index[3]],XMFLOAT2(1,1)}
		};
		d3dContext_->UpdateSubresource(m_vertexBuffer, 0, 0, &vertex, 0, 0);
		d3dContext_->PSSetShaderResources(0, 1, &m_colorMap[i]);
		d3dContext_->DrawIndexed(6, 0, 0);
	}
}