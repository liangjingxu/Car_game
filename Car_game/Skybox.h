#pragma once


#include <array>
#include <DirectXmath.h>
#include <d3d11.h>


using std::array;
using DirectX::XMFLOAT3;
using DirectX::XMFLOAT2;
using namespace DirectX;

struct VertexPosSkyBox
{
	XMFLOAT3 pos;
	XMFLOAT2 tex0;
};

struct VertexPosColor
{
	DirectX::XMFLOAT3 pos;
	DirectX::XMFLOAT4 color;
	static const D3D11_INPUT_ELEMENT_DESC inputLayout[2];
};

class SkyBox{
public:
	SkyBox();
	virtual ~SkyBox();

	//初始化资源
	bool Init_Resource(ID3D11Device* d3dDevice_);
	void Render(ID3D11DeviceContext* d3dContext_, ID3D11Buffer* worldCB_, ID3D11Buffer* viewCB_);

private:

	//8个顶点
	array<XMFLOAT3, 8> m_pos;
	//每个面对应的顶点
	array<array<int, 4>, 6> m_vertexs;

	ID3D11Buffer* m_vertexBuffer;
	ID3D11Buffer* m_indexBuffer;

	array<ID3D11ShaderResourceView*, 6> m_colorMap;
	VertexPosColor *m_vertices;
};
