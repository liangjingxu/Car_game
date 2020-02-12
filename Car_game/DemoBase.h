﻿#pragma once

#include <d3d11.h>
#include <d3dcompiler.h>
#include <dinput.h>
#include "Mouse.h"
#include "Keyboard.h"


#include "GameTimer.h"
#include <assert.h>
#include <wrl/client.h>
#include <d2d1.h>
#include <dwrite.h>
#include<windows.h>


// 使用模板别名(C++11)简化类型名
template <class T>
using ComPtr = Microsoft::WRL::ComPtr<T>;

class DemoBase 
{
	
public:

	GameTimer m_Timer;
	DemoBase(HINSTANCE hInstance);    // 在构造函数的初始化列表应当设置好初始参数
	virtual ~DemoBase();

	HINSTANCE AppInst()const;       // 获取应用实例的句柄
	HWND      MainWnd()const;       // 获取主窗口句柄
	float     AspectRatio()const;   // 获取屏幕宽高比

	int Run();                      // 运行程序，执行消息事件的循环

	// 框架方法。客户派生类需要重载这些方法以实现特定的应用需求
	virtual bool Init();                      // 该父类方法需要初始化窗口、Direct2D和Direct3D部分
	virtual void UpdateScene(float dt) = 0;   // 子类需要实现该方法，完成每一帧的更新
	virtual void Render() = 0;             // 子类需要实现该方法，完成每一帧的绘制


	virtual LRESULT MsgProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);
	// 窗口的消息回调函数
protected:
	bool InitMainWindow();      // 窗口初始化
	bool InitDirect3D(HINSTANCE hInstance, HWND hwnd);        // Direct3D初始化

	bool CompileShaderFromFile(LPCTSTR filePath, const char* entry, const char* shaderModel, ID3DBlob** buffer);
	void CalculateFrameStats(); // 计算每秒帧数并在窗口显示

protected:

	HINSTANCE m_hAppInst;        // 应用实例句柄
	HWND      m_hMainWnd;        // 主窗口句柄
	bool      m_AppPaused;       // 应用是否暂停
	bool      m_Minimized;       // 应用是否最小化
	bool      m_Maximized;       // 应用是否最大化
	bool      m_Resizing;        // 窗口大小是否变化
	bool	  m_Enable4xMsaa;	 // 是否开启4倍多重采样
	UINT      m_4xMsaaQuality;   // MSAA支持的质量等级

	D3D_DRIVER_TYPE m_pd3ddriver;




	
	
	// Direct3D 11
	D3D_FEATURE_LEVEL m_pd3dFeature;
	ComPtr<ID3D11Device> m_pd3dDevice;							// D3D11设备
	ComPtr<ID3D11DeviceContext> m_pd3dImmediateContext;			// D3D11设备上下文
	ComPtr<IDXGISwapChain> m_pSwapChain;						// D3D11交换链
	// 常用资源
	ComPtr<ID3D11Texture2D> m_pDepthStencilBuffer;				// 深度模板缓冲区
	ComPtr<ID3D11RenderTargetView> m_pRenderTargetView;			// 渲染目标视图
	ComPtr<ID3D11DepthStencilView> m_pDepthStencilView;			// 深度模板视图

	// 键鼠输入
	std::unique_ptr<DirectX::Mouse> m_pMouse;					// 鼠标
	DirectX::Mouse::ButtonStateTracker m_MouseTracker;			// 鼠标状态追踪器
	std::unique_ptr<DirectX::Keyboard> m_pKeyboard;				// 键盘
	DirectX::Keyboard::KeyboardStateTracker m_KeyboardTracker;	// 键盘状态追踪器
	// 派生类应该在构造函数设置好这些自定义的初始参数
	//std::wstring m_MainWndCaption;                               // 主窗口标题
	int m_ClientWidth;                                           // 视口宽度
	int m_ClientHeight;                                          // 视口高度
};


