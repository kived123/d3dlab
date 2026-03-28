#pragma once

#include <windows.h>
#include <d3d11.h>
#include <dxgi.h>

class D3DWrap
{
public:  
  ~D3DWrap();

  static HRESULT Create(HWND hWnd);
  static D3DWrap * GetInstance() {
    return D3DWrap_;
  }

  
  ID3D11Device * GetDevice() {
    return Device_;
  }

  ID3D11DeviceContext * GetContext() {
    return Context_;
  }

  IDXGISwapChain * GetSwapChain() {
    return SwapChain_;
  }

  void Clear(const float color[]);
  

private:
  HRESULT Init(HWND hWnd);
  D3DWrap();
  static D3DWrap * D3DWrap_;
  ID3D11Device * Device_;
  ID3D11DeviceContext * Context_;
  IDXGISwapChain * SwapChain_;
  ID3D11RenderTargetView * RTV_;
  ID3D11RasterizerState * RasterizerState_;
  ID3D11Texture2D * DepthStencil_;
  ID3D11DepthStencilView * DSV_;
};


inline ID3D11Device * GDevice() 
{
    return D3DWrap::GetInstance()->GetDevice();
}

inline ID3D11DeviceContext * GContext()
{
    return D3DWrap::GetInstance()->GetContext();
}
