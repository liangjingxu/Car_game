#pragma once

#include <wrl/client.h>
#include <array>
#include <DirectXmath.h>
#include <d3d11.h>
#include <DDSTextureLoader.h>


using std::array;
using namespace DirectX;


struct SimpleVerSky
{
	XMFLOAT3 m_pos;
	XMFLOAT2 m_tex0;
};



class SkyBox
{
public:
	template<class T>
	using ComPtr = Microsoft::WRL::ComPtr<T>;

	SkyBox();
	virtual ~SkyBox();

		// 需要提供完整的天空盒贴图 或者 已经创建好的天空盒纹理.dds文件
	bool Init_Resource(ID3D11Device* d3dDevice_);
	
	void Render(ID3D11DeviceContext* d3dContext_, ID3D11Buffer* worldCB_, ID3D11Buffer* viewCB_);



private:

	ID3D11Buffer* m_vertexBuffer;
	ID3D11Buffer* m_indexBuffer;
	ComPtr<ID3D11ShaderResourceView> m_pTextureCubeSRV;

	array<ID3D11ShaderResourceView*, 6> m_colorMap;

	
};

