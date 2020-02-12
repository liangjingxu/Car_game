#include "GameDemo.h"
#include <string>

using std::string;
using namespace DirectX;

const string texture = "\\Texture\\";
enum CAMERA { FIRST, THIRD };
const float radian = XM_PI / 180;

GameDemo::GameDemo(HINSTANCE hInstance)
	:DemoBase(hInstance)
{
	m_status = THIRD;
	m_colorMapSampler = nullptr;
	m_camera.emplace_back(new FirstCamera());
	m_camera.emplace_back(new ThirdCamera());

}

GameDemo::~GameDemo()
{
	if (m_colorMapSampler) m_colorMapSampler->Release();
	m_colorMapSampler = nullptr;
}

bool GameDemo::Init()
{
	if (!DemoBase::Init())
		return false;

	if (!InitEffect())
		return false;

	return true;
}

void GameDemo::OnResize()
{
	/*//assert(m_pd2dFactory);
	assert(m_pdwriteFactory);
	// 释放D2D的相关资源
	m_pColorBrush.Reset();
	m_pd2dRenderTarget.Reset();
	HRESULT hr;
	DemoBase::OnResize();

	// 为D2D创建DXGI表面渲染目标
	ComPtr<IDXGISurface> surface;
	hr = m_pSwapChain->GetBuffer(0, __uuidof(IDXGISurface), reinterpret_cast<void**>(surface.GetAddressOf()));
	if (FAILED(hr))
		return;
	D2D1_RENDER_TARGET_PROPERTIES props = D2D1::RenderTargetProperties(
		D2D1_RENDER_TARGET_TYPE_DEFAULT,
		D2D1::PixelFormat(DXGI_FORMAT_UNKNOWN, D2D1_ALPHA_MODE_PREMULTIPLIED));
	//hr = m_pd2dFactory->CreateDxgiSurfaceRenderTarget(surface.Get(), &props, m_pd2dRenderTarget.GetAddressOf());
	surface.Reset();*/
	/*
	if (hr == E_NOINTERFACE)
	{
		OutputDebugStringW(L"\n警告：Direct2D与Direct3D互操作性功能受限，你将无法看到文本信息。现提供下述可选方法：\n"
			L"1. 对于Win7系统，需要更新至Win7 SP1，并安装KB2670838补丁以支持Direct2D显示。\n"
			L"2. 自行完成Direct3D 10.1与Direct2D的交互。详情参阅："
			L"https://docs.microsoft.com/zh-cn/windows/desktop/Direct2D/direct2d-and-direct3d-interoperation-overview""\n"
			L"3. 使用别的字体库，比如FreeType。\n\n");
	}
	else if (hr == S_OK)
	{
		// 创建固定颜色刷和文本格式
		HR(m_pd2dRenderTarget->CreateSolidColorBrush(
			D2D1::ColorF(D2D1::ColorF::White),
			m_pColorBrush.GetAddressOf()));
		HR(m_pdwriteFactory->CreateTextFormat(L"宋体", nullptr, DWRITE_FONT_WEIGHT_NORMAL,
			DWRITE_FONT_STYLE_NORMAL, DWRITE_FONT_STRETCH_NORMAL, 15, L"zh-cn",
			m_pTextFormat.GetAddressOf()));
	}
	else
	{
		// 报告异常问题
		assert(m_pd2dRenderTarget);
	}*/

	// 摄像机变更显示
	/*if (m_camera.empty())
	{
		m_camera[FIRST]->SetFrustum(XM_PI / 3, AspectRatio(), 0.5f, 1000.0f);
		m_camera[FIRST]->SetViewPort(0.0f, 0.0f, (float)m_ClientWidth, (float)m_ClientHeight);
		m_CBOnResize.proj = XMMatrixTranspose(m_camera[FIRST]->GetProjXM());

		D3D11_MAPPED_SUBRESOURCE mappedData;
		hr = m_pd3dImmediateContext->Map(m_pConstantBuffers[2].Get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedData);
		if (FAILED(hr))
			return;
		memcpy_s(mappedData.pData, sizeof(CBChangesOnResize), &m_CBOnResize, sizeof(CBChangesOnResize));
		m_pd3dImmediateContext->Unmap(m_pConstantBuffers[2].Get(), 0);
	}
}*/
}




bool GameDemo::InitEffect()
{
	HRESULT result;
	//编译并创建顶点着色器
	ID3DBlob* pVSBlob = 0;
	HRESULT compileResult = CompileShaderFromFile(L"Fx\\solid.fx", "VS_Main", "vs_5_0", &pVSBlob);
	if (FAILED(compileResult))
		return false;
	result = m_pd3dDevice->CreateVertexShader(pVSBlob->GetBufferPointer(), pVSBlob->GetBufferSize(), 0, &m_pVertexShader3D);
	if (FAILED(result))
	{
		if (pVSBlob)
			pVSBlob->Release();
		return false;
	}
	compileResult = CompileShaderFromFile(L"Fx\\tiresolid.fx", "VS_Main", "vs_5_0", &pVSBlob);
	if (FAILED(compileResult))
		return false;
	result = m_pd3dDevice->CreateVertexShader(pVSBlob->GetBufferPointer(), pVSBlob->GetBufferSize(), 0, &m_pTiresolidColorVS);
	if (FAILED(result))
	{
		if (pVSBlob)
			pVSBlob->Release();
		return false;
	}
	//创建顶点输入布局
	D3D11_INPUT_ELEMENT_DESC layout[] =
	{
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	};
	UINT numElements = ARRAYSIZE(layout);
	result = m_pd3dDevice->CreateInputLayout( layout, numElements, pVSBlob->GetBufferPointer(),
												pVSBlob->GetBufferSize(), &m_pVertexLayout3D);
	pVSBlob->Release();
	if (FAILED(result))
		return false;

	//编译并创建像素着色器
	ID3DBlob* psBuffer = 0;
	compileResult = CompileShaderFromFile(L"Fx\\solid.fx", "PS_Main", "ps_5_0", &psBuffer);
	if (FAILED(compileResult))
		return false;
	result = m_pd3dDevice->CreatePixelShader(psBuffer->GetBufferPointer(), psBuffer->GetBufferSize(), 0, &m_psolidColorPS);
	psBuffer->Release();
	if (FAILED(result))
		return false;

	

	//创建采样状态对象
	D3D11_SAMPLER_DESC sampDesc;
	ZeroMemory(&sampDesc, sizeof(sampDesc));
	sampDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
	sampDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
	sampDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
	sampDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
	sampDesc.ComparisonFunc = D3D11_COMPARISON_NEVER;
	sampDesc.MinLOD = 0;
	sampDesc.MaxLOD = D3D11_FLOAT32_MAX;
	result = m_pd3dDevice->CreateSamplerState(&sampDesc, &m_colorMapSampler);
	if (FAILED(result))
		return false;
	projMatrix_ = XMMatrixPerspectiveFovLH(XM_PIDIV2,
		800.0f / 600.0f, 1.0f, 20000.0f);
	projMatrix_ = XMMatrixTranspose(projMatrix_);


	//创建变换矩阵和常量缓存
	D3D11_BUFFER_DESC bd;
	ZeroMemory(&bd, sizeof(bd));
	bd.Usage = D3D11_USAGE_DEFAULT;
	bd.ByteWidth = sizeof(XMMATRIX);
	bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	bd.CPUAccessFlags = 0;

	result = m_pd3dDevice->CreateBuffer(&bd, 0, &m_pworldCB);
	if (FAILED(result))
		return false;
	result = m_pd3dDevice->CreateBuffer(&bd, 0, &m_pviewCB);
	if (FAILED(result))
		return false;
	result = m_pd3dDevice->CreateBuffer(&bd, 0, &m_pprojCB);
	if (FAILED(result))
		return false;
	result = m_pd3dDevice->CreateBuffer(&bd, 0, &m_ptextureCB);
	if (FAILED(result))
		return false;

	if (!m_skybox.Init_Resource(m_pd3dDevice.Get())) return false;
	if (!m_brick.Init_Resource(m_pd3dDevice.Get())) return false;
	if (!m_car.Init_Resource(m_pd3dDevice.Get())) return false;
	return true;
}



void GameDemo::UpdateScene(float dt)
{
	// 更新鼠标事件，获取相对偏移量
	Mouse::State mouseState = m_pMouse->GetState();
	Mouse::State lastMouseState = m_MouseTracker.GetLastState();
	m_MouseTracker.Update(mouseState);

	Keyboard::State keyState = m_pKeyboard->GetState();
	Keyboard::State lastkeyState = m_KeyboardTracker.GetLastState();
	m_KeyboardTracker.Update(keyState);

	/*m_keyboardDevice->Acquire();
	m_mouseDevice->Acquire();
	m_keyboardDevice->GetDeviceState(sizeof(m_keyboardKeys), (LPVOID)&m_keyboardKeys);
	m_mouseDevice->GetDeviceState(sizeof(m_diMouseState), (LPVOID)&m_diMouseState);
	if (KEYDOWN(m_prevKeyboardKeys, DIK_ESCAPE) && !KEYDOWN(m_keyboardKeys, DIK_ESCAPE))
		PostQuitMessage(0);
	if (KEYDOWN(m_prevKeyboardKeys, DIK_V) && !KEYDOWN(m_keyboardKeys, DIK_V))
		Switch();*/

	
	/*if (!KEYDOWN(m_prevKeyboardKeys, DIK_S) && KEYDOWN(m_keyboardKeys, DIK_S))
		m_motion.setDirection(-1);
	else if (KEYDOWN(m_prevKeyboardKeys, DIK_S) && !KEYDOWN(m_keyboardKeys, DIK_S))
		m_motion.setDirection(0);
	if (!KEYDOWN(m_prevKeyboardKeys, DIK_A) && KEYDOWN(m_keyboardKeys, DIK_A))
		m_motion.setTurn(-1);
	else if (KEYDOWN(m_prevKeyboardKeys, DIK_A) && !KEYDOWN(m_keyboardKeys, DIK_A))
		m_motion.setTurn(0);
	if (!KEYDOWN(m_prevKeyboardKeys, DIK_D) && KEYDOWN(m_keyboardKeys, DIK_D))
		m_motion.setTurn(1);
	else if (KEYDOWN(m_prevKeyboardKeys, DIK_D) && !KEYDOWN(m_keyboardKeys, DIK_D))
		m_motion.setTurn(0);*/


		// 退出程序，这里应向窗口发送销毁信息
	if (keyState.IsKeyDown(Keyboard::Escape))
		SendMessage(MainWnd(), WM_DESTROY, 0, 0);
	if (keyState.IsKeyDown(Keyboard::V))
		Switch();
	if (keyState.IsKeyDown(Keyboard::W))
		m_motion.setDirection(1);
	else if (keyState.IsKeyUp(Keyboard::W))
		m_motion.setDirection(0);
	m_motion.Move();
	//memcpy(m_prevKeyboardKeys, m_keyboardKeys, sizeof(m_keyboardKeys));
	m_camera[FIRST]->SetPosition(m_motion.getPos(), m_motion.getFocus());
	m_camera[THIRD]->SetPosition(m_motion.getPos(), m_motion.getPos());
	m_car.setCenter(m_motion.getPos());
	m_car.setWholerotation(m_motion.getWholerotation());
	m_car.settiretexturerotaion(m_motion.getDistance());
	m_car.setwheelTurn(m_motion.getDirection(), m_motion.getTurn());
	if (m_status)
		m_camera[THIRD]->ApplyRotation(mouseState.y*radian, mouseState.x*radian);
}

void GameDemo::Render()
{
	if (m_pd3dImmediateContext == 0)
		return;
	float clearColor[4] = { 0.0f, 0.0f, 0.25f, 1.0f };
	m_pd3dImmediateContext->ClearRenderTargetView(m_pRenderTargetView.Get(), clearColor);
	m_pd3dImmediateContext->ClearDepthStencilView(m_pDepthStencilView.Get(), D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);

	m_pd3dImmediateContext->IASetInputLayout(m_pVertexLayout3D.Get());

	// 默认绑定3D着色器
	m_pd3dImmediateContext->VSSetShader(m_pVertexShader3D.Get(), nullptr, 0);
	m_pd3dImmediateContext->PSSetShader(m_pPixelShader3D.Get(), nullptr, 0);
	m_pd3dImmediateContext->PSSetSamplers(0, 1, m_colorMapSampler.GetAddressOf());
	XMMATRIX viewMatrix_ = m_camera[m_status]->GetViewMatrix();
	viewMatrix_ = XMMatrixTranspose(viewMatrix_);
	m_pd3dImmediateContext->UpdateSubresource(m_pviewCB.Get(), 0, 0, &viewMatrix_, 0, 0);
	m_pd3dImmediateContext->UpdateSubresource(m_pprojCB.Get(), 0, 0, &projMatrix_, 0, 0);
	m_pd3dImmediateContext->VSSetConstantBuffers(0, 1, &m_pworldCB);
	m_pd3dImmediateContext->VSSetConstantBuffers(1, 1, &m_pviewCB);
	m_pd3dImmediateContext->VSSetConstantBuffers(2, 1, &m_pprojCB);
	m_pd3dImmediateContext->VSSetConstantBuffers(3, 1, &m_ptextureCB);
	m_skybox.Render(m_pd3dImmediateContext.Get(), m_pworldCB.Get(), m_pviewCB.Get());
	m_brick.Render(m_pd3dImmediateContext.Get(), m_pworldCB.Get(), m_pviewCB.Get());
	m_car.Render(m_pd3dImmediateContext.Get(), m_pworldCB.Get(), m_pviewCB.Get(), m_pTiresolidColorVS.Get(), m_ptextureCB.Get());
	m_pSwapChain->Present(0, 0);
}

void GameDemo::Switch() {
	m_status ^= 1;
}

