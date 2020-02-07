#include "GameDemo.h"
#include <string>

using std::string;
using namespace DirectX;

const string texture = "\\Texture\\";
enum CAMERA { FIRST, THIRD };
const float radian = XM_PI / 180;

GameDemo::GameDemo()
{
	m_status = FIRST;
	m_colorMapSampler = nullptr;
	m_camera.emplace_back(new FirstCamera());
	m_camera.emplace_back(new ThirdCamera());
}

GameDemo::~GameDemo()
{}

bool GameDemo::LoadContent()
{
	HRESULT result;
	//编译并创建顶点着色器
	ID3DBlob* pVSBlob = 0;
	HRESULT compileResult = CompileShaderFromFile(L"Fx\\solid.fx", "VS_Main", "vs_5_0", &pVSBlob);
	if (FAILED(compileResult))
		return false;
	result = m_d3dDevice->CreateVertexShader(pVSBlob->GetBufferPointer(), pVSBlob->GetBufferSize(), 0, &m_solidColorVS);
	if (FAILED(result))
	{
		if (pVSBlob)
			pVSBlob->Release();
		return false;
	}
	compileResult = CompileShaderFromFile(L"Fx\\tiresolid.fx", "VS_Main", "vs_5_0", &pVSBlob);
	if (FAILED(compileResult))
		return false;
	result = m_d3dDevice->CreateVertexShader(pVSBlob->GetBufferPointer(), pVSBlob->GetBufferSize(), 0, &m_tiresolidColorVS);
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
	result = m_d3dDevice->CreateInputLayout( layout, numElements, pVSBlob->GetBufferPointer(),
												pVSBlob->GetBufferSize(), &m_inputLayout);
	pVSBlob->Release();
	if (FAILED(result))
		return false;

	//编译并创建像素着色器
	ID3DBlob* psBuffer = 0;
	compileResult = CompileShaderFromFile(L"Fx\\solid.fx", "PS_Main", "ps_5_0", &psBuffer);
	if (FAILED(compileResult))
		return false;
	result = m_d3dDevice->CreatePixelShader(psBuffer->GetBufferPointer(), psBuffer->GetBufferSize(), 0, &m_solidColorPS);
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
	result = m_d3dDevice->CreateSamplerState(&sampDesc, &m_colorMapSampler);
	if (FAILED(result))
		return false;


	//创建变换矩阵和常量缓存
	D3D11_BUFFER_DESC bd;
	ZeroMemory(&bd, sizeof(bd));
	bd.Usage = D3D11_USAGE_DEFAULT;
	bd.ByteWidth = sizeof(XMMATRIX);
	bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	bd.CPUAccessFlags = 0;


	m_projMatrix = XMMatrixPerspectiveFovLH(XM_PIDIV2,
		800.0f / 600.0f, 1.0f, 20000.0f);
	m_projMatrix = XMMatrixTranspose(m_projMatrix);


	result = m_d3dDevice->CreateBuffer(&bd, 0, &m_worldCB);
	if (FAILED(result))
		return false;
	result = m_d3dDevice->CreateBuffer(&bd, 0, &m_viewCB);
	if (FAILED(result))
		return false;
	result = m_d3dDevice->CreateBuffer(&bd, 0, &m_projCB);
	if (FAILED(result))
		return false;
	result = m_d3dDevice->CreateBuffer(&bd, 0, &m_textureCB);
	if (FAILED(result))
		return false;

	//初始化
	if (!m_skybox.Init_Resource(m_d3dDevice)) return false;
	if (!m_brick.Init_Resource(m_d3dDevice)) return false;
	if (!m_car.Init_Resource(m_d3dDevice)) return false;
	return true;
}

void GameDemo::UnloadContent()
{
	if (m_colorMapSampler) m_colorMapSampler->Release();
	m_colorMapSampler = nullptr;
}

void GameDemo::Update()
{
	m_keyboardDevice->Acquire();
	m_mouseDevice->Acquire();
	m_keyboardDevice->GetDeviceState(sizeof(m_keyboardKeys), (LPVOID)&m_keyboardKeys);
	m_mouseDevice->GetDeviceState(sizeof(m_diMouseState), (LPVOID)&m_diMouseState);
	if (KEYDOWN(m_prevKeyboardKeys, DIK_ESCAPE) && !KEYDOWN(m_keyboardKeys, DIK_ESCAPE))
		PostQuitMessage(0);
	if (KEYDOWN(m_prevKeyboardKeys, DIK_V) && !KEYDOWN(m_keyboardKeys, DIK_V))
		Switch();
	if (!KEYDOWN(m_prevKeyboardKeys, DIK_W) && KEYDOWN(m_keyboardKeys, DIK_W))
		m_motion.setDirection(1);
	else if (KEYDOWN(m_prevKeyboardKeys, DIK_W) && !KEYDOWN(m_keyboardKeys, DIK_W))
		m_motion.setDirection(0);
	if (!KEYDOWN(m_prevKeyboardKeys, DIK_S) && KEYDOWN(m_keyboardKeys, DIK_S))
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
		m_motion.setTurn(0);
	m_motion.Move();
	memcpy(m_prevKeyboardKeys, m_keyboardKeys, sizeof(m_keyboardKeys));
	m_camera[FIRST]->SetPosition(m_motion.getPos(), m_motion.getFocus());
	m_camera[THIRD]->SetPosition(m_motion.getPos(), m_motion.getPos());
	m_car.setCenter(m_motion.getPos());
	m_car.setWholerotation(m_motion.getWholerotation());
	m_car.settiretexturerotaion(m_motion.getDistance());
	m_car.setwheelTurn(m_motion.getDirection(), m_motion.getTurn());
	if (m_status)
		m_camera[THIRD]->ApplyRotation(m_diMouseState.lY*radian, m_diMouseState.lX*radian);
}

void GameDemo::Render()
{
	if (m_d3dContext == 0)
		return;
	float clearColor[4] = { 0.0f, 0.0f, 0.25f, 1.0f };
	m_d3dContext->ClearRenderTargetView(m_backBufferTarget, clearColor);
	m_d3dContext->ClearDepthStencilView(m_depthStencilView, D3D11_CLEAR_DEPTH, 1.0f, 0);
	m_d3dContext->IASetInputLayout(m_inputLayout);
	m_d3dContext->VSSetShader(m_solidColorVS, 0, 0);
	m_d3dContext->PSSetShader(m_solidColorPS, 0, 0);
	m_d3dContext->PSSetSamplers(0, 1, &m_colorMapSampler);
	XMMATRIX viewMatrix_ = m_camera[m_status]->GetViewMatrix();
	viewMatrix_ = XMMatrixTranspose(viewMatrix_);
	m_d3dContext->UpdateSubresource(m_viewCB, 0, 0, &viewMatrix_, 0, 0);
	m_d3dContext->UpdateSubresource(m_projCB, 0, 0, &m_projMatrix, 0, 0);
	m_d3dContext->VSSetConstantBuffers(0, 1, &m_worldCB);
	m_d3dContext->VSSetConstantBuffers(1, 1, &m_viewCB);
	m_d3dContext->VSSetConstantBuffers(2, 1, &m_projCB);
	m_d3dContext->VSSetConstantBuffers(3, 1, &m_textureCB);
	m_skybox.Render(m_d3dContext, m_worldCB, m_viewCB);
	m_brick.Render(m_d3dContext, m_worldCB, m_viewCB);
	m_car.Render(m_d3dContext, m_worldCB, m_viewCB, m_tiresolidColorVS, m_textureCB);
	m_swapChain->Present(0, 0);
}

void GameDemo::Switch() {
	m_status ^= 1;
}

