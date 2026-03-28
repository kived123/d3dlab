#include "d3dwrap.h"

D3DWrap * D3DWrap::D3DWrap_ = nullptr;

HRESULT D3DWrap::Create(HWND hWnd)
{
    if (D3DWrap_ != nullptr) {
        return S_OK;
    }

    static D3DWrap D3DWrap;

    D3DWrap_ = &D3DWrap;

    return D3DWrap_->Init(hWnd);
}

D3DWrap::D3DWrap():Device_(nullptr),
                   Context_(nullptr),
                   SwapChain_(nullptr),
                   RTV_(nullptr),
                   RasterizerState_(nullptr),
                   DepthStencil_(nullptr),
                   DSV_(nullptr)
{
}

HRESULT D3DWrap::Init(HWND hWnd) 
{
    IDXGIFactory1 * factory;
    HRESULT hr = CreateDXGIFactory1(__uuidof(IDXGIFactory1), (void **)&factory);
    if (FAILED(hr)) {
        return hr;
    }

   
    IDXGIAdapter1 * adapter;
    factory->EnumAdapters1(0, &adapter);

    D3D_FEATURE_LEVEL levels[] {D3D_FEATURE_LEVEL_11_0};
    int num = ARRAYSIZE(levels);
    hr  = D3D11CreateDevice(adapter,D3D_DRIVER_TYPE_UNKNOWN,0,0,levels,num,D3D11_SDK_VERSION,&Device_,nullptr,&Context_);    
    adapter->Release();
    if (FAILED(hr)) {
        factory->Release();        
        return hr;
    }

    

    RECT rc;
    GetClientRect(hWnd, &rc);
    int Width = rc.right - rc.left;
    int Height = rc.bottom - rc.top;

    
    DXGI_SWAP_CHAIN_DESC sd{};

    sd.BufferCount = 1;
    sd.BufferDesc.Width = Width;
    sd.BufferDesc.Height = Height;
    sd.BufferDesc.Format = DXGI_FORMAT_B8G8R8A8_UNORM;
    sd.BufferDesc.RefreshRate.Numerator = 60;
    sd.BufferDesc.RefreshRate.Denominator = 1;
    sd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
    sd.OutputWindow = hWnd;
    sd.SampleDesc.Count = 1;
    sd.SampleDesc.Quality = 0;
    sd.Windowed = TRUE;

    hr = factory->CreateSwapChain(Device_,&sd,&SwapChain_);
    factory->Release();
    if (FAILED(hr)) {
        return hr;
    }
   
    ID3D11Texture2D * BackBuffer;
    hr = SwapChain_->GetBuffer(0,__uuidof(ID3D11Texture2D),(void**)&BackBuffer);

    if (FAILED(hr)) {
       return hr;
    }

    hr = Device_->CreateRenderTargetView(BackBuffer,nullptr,&RTV_);
    BackBuffer->Release();

    if (FAILED(hr)) {
       return hr;
    }


    D3D11_TEXTURE2D_DESC descDepth{};
	ZeroMemory( &descDepth, sizeof(descDepth) );
    descDepth.Width = Width;
    descDepth.Height = Height;
    descDepth.MipLevels = 1;
    descDepth.ArraySize = 1;
    descDepth.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
    descDepth.SampleDesc.Count = 1;
    descDepth.SampleDesc.Quality = 0;
    descDepth.Usage = D3D11_USAGE_DEFAULT;
    descDepth.BindFlags = D3D11_BIND_DEPTH_STENCIL;
    descDepth.CPUAccessFlags = 0;
    descDepth.MiscFlags = 0;

    hr = Device_->CreateTexture2D(&descDepth, nullptr,  &DepthStencil_);
    if (FAILED(hr)) {
        return hr;
    }

    D3D11_DEPTH_STENCIL_VIEW_DESC descDSV{};

    descDSV.Format = descDepth.Format;
    descDSV.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
    descDSV.Texture2D.MipSlice = 0;

    hr = Device_->CreateDepthStencilView(DepthStencil_,&descDSV,&DSV_);
    if (FAILED(hr)) {
        return hr;
    }


    D3D11_VIEWPORT vp;
    vp.Width = (float)Width;
    vp.Height = (float)Height;
    vp.MinDepth = 0.0f;
    vp.MaxDepth = 1.0f;
    vp.TopLeftX = 0;
    vp.TopLeftY = 0;

  
    D3D11_RASTERIZER_DESC rasterizerDesc={};
 


    rasterizerDesc.FillMode = D3D11_FILL_SOLID;
    rasterizerDesc.CullMode = D3D11_CULL_NONE;
    rasterizerDesc.FrontCounterClockwise = FALSE;
    rasterizerDesc.DepthBias = 0;
    rasterizerDesc.DepthBiasClamp = 0.0f;
    rasterizerDesc.SlopeScaledDepthBias = 0.0f;
    rasterizerDesc.DepthClipEnable = TRUE;
    rasterizerDesc.ScissorEnable = FALSE;
    rasterizerDesc.MultisampleEnable = FALSE;
    rasterizerDesc.AntialiasedLineEnable = FALSE;

    Device_->CreateRasterizerState(&rasterizerDesc,&RasterizerState_);

    Context_->OMSetRenderTargets(1,&RTV_,DSV_);
    Context_->RSSetViewports(1,&vp);
    Context_->RSSetState(RasterizerState_);
   


    return S_OK;
}

void D3DWrap::Clear(const float color[])
{
    Context_->ClearRenderTargetView(RTV_,color);
    Context_->ClearDepthStencilView(DSV_,D3D11_CLEAR_DEPTH, 1.0f, 0 );
}

D3DWrap::~D3DWrap()
{
    if (!RasterizerState_) RasterizerState_->Release();
    if (!DSV_) DSV_->Release();
    if (!DepthStencil_) DepthStencil_->Release();
    if (!RTV_) RTV_->Release();
    if (!SwapChain_) SwapChain_->Release();
    if (!Context_) Context_->Release();
    if (!Device_) Device_->Release();
}