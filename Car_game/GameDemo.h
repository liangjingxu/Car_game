#pragma once

#include <DirectXmath.h>
#include <memory>
#include "DemoBase.h"
#include "FirstCamera.h"
#include "ThirdCamera.h"
#include "SkyBox.h"
#include "Brick.h"
#include "Car.h"
#include "Motion.h"
#include <vector>


using DirectX::XMMATRIX;
using std::shared_ptr;
using std::vector;

class GameDemo:public DemoBase {
public:
	GameDemo();
	virtual ~GameDemo();
	bool LoadContent();
	void UnloadContent();
	void Update();
	void Render();

private:
	vector<shared_ptr<Camera>> m_camera;
	int m_status;

	SkyBox m_skybox;
	Brick m_brick;
	Car m_car;
	Motion m_motion;

	ID3D11SamplerState* m_colorMapSampler;

	ID3D11Buffer* m_viewCB;
	ID3D11Buffer* m_worldCB;
	ID3D11Buffer* m_projCB;
	ID3D11Buffer* m_textureCB;
	XMMATRIX m_projMatrix;

	ID3D11VertexShader* m_solidColorVS;
	ID3D11VertexShader* m_tiresolidColorVS;
	ID3D11PixelShader* m_solidColorPS;

	ID3D11InputLayout* m_inputLayout;

	void Switch();
};
