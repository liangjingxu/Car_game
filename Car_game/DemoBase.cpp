#include"DemoBase.h"

DemoBase::DemoBase()
{
	m_driverType = D3D_DRIVER_TYPE_NULL;
	m_featureLevel = D3D_FEATURE_LEVEL_11_0;
	m_d3dDevice = nullptr;
	m_d3dContext = nullptr;
	m_swapChain = nullptr;
	m_backBufferTarget = nullptr;
	m_depthStencilView = nullptr;
	m_depthTexture = nullptr;
}

DemoBase::~DemoBase()
{

	if (m_backBufferTarget)
	{
		m_backBufferTarget->Release();
	}

	if (m_swapChain)
	{
		m_swapChain->Release();
	}

	if (m_d3dContext)
	{
		m_d3dContext->Release();
	}
	if (m_d3dDevice)
	{
		m_d3dDevice->Release();
	}
	if (m_depthStencilView)
	{
		m_depthStencilView->Release();
	}
	if (m_depthTexture)
	{
		m_depthTexture->Release();
	}
	if (m_keyboardDevice)
	{

		m_keyboardDevice->Release();
	}
	if (m_directInput)
	{
		m_directInput->Release();
	}
	m_backBufferTarget = nullptr;
	m_swapChain = nullptr;
	m_d3dContext = nullptr;
	m_d3dDevice = nullptr;
	m_depthStencilView = nullptr;
	m_depthTexture = nullptr;
	m_keyboardDevice = nullptr;
	m_directInput = nullptr;
}

bool DemoBase::LoadContent()
{
	// Override with demo specifics, if any...
	return true;
}

void DemoBase::UnloadContent()
{
	// Override with demo specifics, if any...
}


bool DemoBase::InitResource(HINSTANCE hInstance, HWND hwnd)
{
	m_hInstance = hInstance;
	m_hwnd = hwnd;
	

	RECT rc;
	HRESULT result = S_OK;
	GetClientRect(hwnd, &rc);
	UINT width = rc.right - rc.left;
	UINT height = rc.bottom - rc.top;

	//指明驱动设备类型和特征等级
	UINT creationFlags = 0;
#ifdef _DEBUG
	creationFlags |= D3D11_CREATE_DEVICE_DEBUG;
#endif
	D3D_DRIVER_TYPE driverTypes[] =
	{
	D3D_DRIVER_TYPE_HARDWARE, D3D_DRIVER_TYPE_WARP,D3D_DRIVER_TYPE_SOFTWARE
	};

	unsigned int totalDriverTypes = ARRAYSIZE(driverTypes);
	D3D_FEATURE_LEVEL featureLevels[] =
	{
	D3D_FEATURE_LEVEL_11_0,
	D3D_FEATURE_LEVEL_10_1,
	D3D_FEATURE_LEVEL_10_0
	};
	unsigned int numDriverTypes = ARRAYSIZE(featureLevels);



	//交换链的设置
	DXGI_SWAP_CHAIN_DESC sd;
	UINT driver = 0;
	ZeroMemory(&sd, sizeof(sd));
	sd.BufferCount = 1;
	sd.BufferDesc.Width = width;
	sd.BufferDesc.Height = height;
	sd.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	sd.BufferDesc.RefreshRate.Numerator = 60;
	sd.BufferDesc.RefreshRate.Denominator = 1;
	sd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	sd.OutputWindow = m_hwnd;
	sd.SampleDesc.Count = 1;
	sd.SampleDesc.Quality = 0;
	sd.Windowed = TRUE;

	for (UINT driverTypeIndex = 0; driverTypeIndex < numDriverTypes; driverTypeIndex++)
	{
		m_driverType = driverTypes[driverTypeIndex];
		result = D3D11CreateDeviceAndSwapChain(0, driverTypes[driver], 0, creationFlags,
			featureLevels, numDriverTypes,
			D3D11_SDK_VERSION, &sd, &m_swapChain,
			&m_d3dDevice, &m_featureLevel, &m_d3dContext);
		if (SUCCEEDED(result))
			break;
	}
	if (FAILED(result))
		return false;


	//创建目标渲染视图
	ID3D11Texture2D* pBackBuffer = NULL;
	result = m_swapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (LPVOID*)&pBackBuffer );
	if (FAILED(result))
	{
		MessageBox(NULL, TEXT("Failed to get the swap chain backbuffer!"), TEXT("Error"), MB_OK);
		return false;
	}
	result = m_d3dDevice->CreateRenderTargetView(pBackBuffer, 0, &m_backBufferTarget);
	if (pBackBuffer)
		pBackBuffer->Release();
	if (FAILED(result))
	{
		MessageBox(NULL, TEXT("Failed to create the render targetview!"), TEXT("Error"), MB_OK);
		return false;
	}


	//创建深度缓存目标视图
	D3D11_TEXTURE2D_DESC descDepth;
	ZeroMemory(&descDepth, sizeof(descDepth));
	descDepth.Width = width;
	descDepth.Height = height;
	descDepth.MipLevels = 1;
	descDepth.ArraySize = 1;
	descDepth.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
	descDepth.SampleDesc.Count = 1;
	descDepth.SampleDesc.Quality = 0;
	descDepth.Usage = D3D11_USAGE_DEFAULT;
	descDepth.BindFlags = D3D11_BIND_DEPTH_STENCIL;
	descDepth.CPUAccessFlags = 0;
	result = m_d3dDevice->CreateTexture2D(&descDepth, NULL, &m_depthTexture);
	if (FAILED(result))
	{
		MessageBox(NULL, TEXT("Failed to create the depth texture!"), TEXT("Error"), MB_OK);
		return false;
	}


	// 创建深度模具视图
	D3D11_DEPTH_STENCIL_VIEW_DESC descDSV;
	ZeroMemory(&descDSV, sizeof(descDSV));
	descDSV.Format = descDepth.Format;
	descDSV.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
	descDSV.Texture2D.MipSlice = 0;
	result = m_d3dDevice->CreateDepthStencilView(m_depthTexture, &descDSV, &m_depthStencilView);
	if (FAILED(result))
		return false;
	m_d3dContext->OMSetRenderTargets(1, &m_backBufferTarget, m_depthStencilView);


	//全屏视口的创建和设置
	D3D11_VIEWPORT viewport;
	viewport.Width = FLOAT(width);
	viewport.Height = FLOAT(height);
	viewport.MinDepth = 0.0f;
	viewport.MaxDepth = 1.0f;
	viewport.TopLeftX = 0.0f;
	viewport.TopLeftY = 0.0f;
	m_d3dContext->RSSetViewports(1, &viewport);


	

	//初始化键盘
	ZeroMemory(m_keyboardKeys, sizeof(m_keyboardKeys));
	ZeroMemory(m_prevKeyboardKeys, sizeof(m_prevKeyboardKeys));
	result = DirectInput8Create(m_hInstance, DIRECTINPUT_VERSION, IID_IDirectInput8, (void**)&m_directInput, 0);
	if (FAILED(result))
	{
		return false;
	}
	result = m_directInput->CreateDevice(GUID_SysKeyboard, &m_keyboardDevice, 0);
	if (FAILED(result))
	{
		return false;
	}
	result = m_keyboardDevice->SetDataFormat(&c_dfDIKeyboard);
	if (FAILED(result))
	{
		return false;
	}
	result = m_keyboardDevice->SetCooperativeLevel(m_hwnd, DISCL_FOREGROUND | DISCL_NONEXCLUSIVE);
	if (FAILED(result))
	{
		return false;
	}
	result = m_keyboardDevice->Acquire();
	if (FAILED(result))
	{
		return false;
	}


	//初始化鼠标
	result = m_directInput->CreateDevice(GUID_SysMouse, &m_mouseDevice, 0);
	if (FAILED(result))
	{
		return false;
	}
	result = m_mouseDevice->SetDataFormat(&c_dfDIMouse);
	if (FAILED(result))
	{
		return false;
	}
	result = m_mouseDevice->SetCooperativeLevel(m_hwnd, DISCL_FOREGROUND | DISCL_NONEXCLUSIVE);
	if (FAILED(result))
	{
		return false;
	}
	result = m_mouseDevice->Acquire();
	if (FAILED(result))
	{
		return false;
	}
	return LoadContent();
}

HRESULT DemoBase::CompileShaderFromFile(LPCTSTR szFileName, const char* szEntryPoint, const char* szShaderModel, ID3DBlob** ppBlobOut)
{
	HRESULT result = S_OK;

	DWORD dwShaderFlags = D3DCOMPILE_ENABLE_STRICTNESS;
#if defined( DEBUG ) || defined( _DEBUG )
	//将D3DCOMPILE_DEBUG标志设置为在着色器中嵌入调试信息。

	//设置此标志可改善着色器调试体验，但仍允许

	//要优化的明暗器，并以其运行方式运行

	//此程序的发布配置。
	dwShaderFlags |= D3DCOMPILE_DEBUG;
#endif

	ID3DBlob* pErrorBlob = 0;
	result = D3DCompileFromFile(szFileName, NULL, NULL, szEntryPoint, szShaderModel,
		dwShaderFlags, 0, ppBlobOut, &pErrorBlob);
	if (FAILED(result))
	{
		if (pErrorBlob != NULL)
			OutputDebugStringA((char*)pErrorBlob->GetBufferPointer());
		if (pErrorBlob) pErrorBlob->Release();
		return result;
	}
	if (pErrorBlob) pErrorBlob->Release();

	return S_OK;
	
}