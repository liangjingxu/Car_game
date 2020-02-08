#pragma once

#include <wrl/client.h>
#include <vector>
#include <DirectXmath.h>
#include <d3d11.h>
#include <DDSTextureLoader.h>

using std::vector;
using DirectX::XMFLOAT3;
using DirectX::XMFLOAT2;
using namespace DirectX;




struct SimpleVertex
{
	XMFLOAT3 Pos;
	XMFLOAT2 Tex;
};


class MySkyBox {
public:
	template<class T>
	using ComPtr = Microsoft::WRL::ComPtr<T>;

	MySkyBox();
	virtual ~MySkyBox();

		// 需要提供完整的天空盒贴图 或者 已经创建好的天空盒纹理.dds文件
	bool Init_Resource1(ID3D11Device* d3dDevice_, ID3D11DeviceContext* d3dContext_, bool generateMips = false);
	
	void Render(ID3D11DeviceContext* d3dContext_, ID3D11Buffer* worldCB_, ID3D11Buffer* viewCB_);

private:
	bool Init_Resource2(ID3D11Device* d3dDevice_);

private:
	vector<SimpleVertex> m_vertices;
	ComPtr<ID3D11Buffer> m_pVertexBuffer;
	ComPtr<ID3D11Buffer> m_pIndexBuffer;
	ComPtr<ID3D11ShaderResourceView> m_pTextureCubeSRV;
};

