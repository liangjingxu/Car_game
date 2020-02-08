#pragma once

#include <d3d11.h>
#include <d3dcompiler.h>
#include <dinput.h>

#define KEYDOWN(name, key) ( name[key] & 0x80 )
#define BUTTONDOWN(name, key) ( name.rgbButtons[key] & 0x80 )

class DemoBase {
public:
	DemoBase();
	virtual ~DemoBase();

	bool InitResource(HINSTANCE hInstance, HWND hwnd);


	virtual bool LoadContent();
	virtual void UnloadContent();
	virtual void Update() = 0;
	virtual void Render() = 0;

protected:
	HINSTANCE m_hInstance;
	HWND m_hwnd;

	D3D_DRIVER_TYPE m_driverType;
	ID3D11Device* m_d3dDevice;

	D3D_FEATURE_LEVEL m_featureLevel;
	ID3D11DeviceContext* m_d3dContext;
	IDXGISwapChain* m_swapChain;
	ID3D11RenderTargetView* m_backBufferTarget;

	ID3D11Texture2D* m_depthTexture;
	ID3D11DepthStencilView* m_depthStencilView;

	LPDIRECTINPUT8 m_directInput;


	LPDIRECTINPUTDEVICE8 m_keyboardDevice;
	LPDIRECTINPUTDEVICE8 m_mouseDevice;
	char m_keyboardKeys[256];
	char m_prevKeyboardKeys[256];
	DIMOUSESTATE m_diMouseState;

	HRESULT CompileShaderFromFile(LPCTSTR filePath, const char* entry, const char* shaderModel, ID3DBlob** buffer);
};


