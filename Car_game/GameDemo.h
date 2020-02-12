#ifndef GAMEAPP_H
#define GAMEAPP_H

#include "DemoBase.h"
#include "FirstCamera.h"
#include "ThirdCamera.h"
#include <vector>
#include "Car.h"
#include "SkyBox.h"
#include "Brick.h"
#include "Motion.h"

class GameDemo : public DemoBase
{
public:


	
	


public:
	GameDemo(HINSTANCE hInstance);
	~GameDemo();

	bool Init();
	void OnResize();
	void UpdateScene(float dt);
	void Render();

private:
	bool InitEffect();
	void Switch();

private:
	int m_status;
	ComPtr<ID2D1SolidColorBrush> m_pColorBrush;				    // 单色笔刷
	ComPtr<IDWriteFont> m_pFont;								// 字体
	ComPtr<IDWriteTextFormat> m_pTextFormat;					// 文本格式

	
	ComPtr<ID3D11InputLayout> m_pVertexLayout3D;				// 用于3D的顶点输入布局
	ComPtr <ID3D11Buffer> m_pviewCB;
	ComPtr <ID3D11Buffer> m_pworldCB;
	ComPtr <ID3D11Buffer> m_pprojCB;
	ComPtr <ID3D11Buffer> m_ptextureCB;
	/*
	GameObject m_WoodCrate;									    // 木盒
	GameObject m_Floor;										    // 地板
	std::vector<GameObject> m_Walls;							// 墙壁
	*/

	DirectX::XMFLOAT4X4 m_WorldMatrix;				    // 世界矩阵
	ComPtr<ID3D11ShaderResourceView> m_pTexture;		// 纹理
	ComPtr<ID3D11Buffer> m_pVertexBuffer;				// 顶点缓冲区
	ComPtr<ID3D11Buffer> m_pIndexBuffer;				// 索引缓冲区
	UINT m_VertexStride;								// 顶点字节大小
	UINT m_IndexCount;								    // 索引数目	

	ComPtr<ID3D11PixelShader> m_psolidColorPS;
	ComPtr<ID3D11VertexShader> m_pTiresolidColorVS;
	ComPtr<ID3D11VertexShader> m_pVertexShader3D;				// 用于3D的顶点着色器
	ComPtr<ID3D11PixelShader> m_pPixelShader3D;				    // 用于3D的像素着色器
	ComPtr<ID3D11VertexShader> m_pVertexShader2D;				// 用于2D的顶点着色器
	ComPtr<ID3D11PixelShader> m_pPixelShader2D;				    // 用于2D的像素着色器

	XMMATRIX projMatrix_;


	ComPtr<ID3D11SamplerState> m_colorMapSampler;				    // 采样器状态

	std::vector<std::shared_ptr<Camera>> m_camera; 						    // 摄像机
	//CameraMode m_CameraMode;									// 摄像机模式
	Car m_car;
	SkyBox m_skybox;
	Brick m_brick;
	Motion m_motion;
};


#endif