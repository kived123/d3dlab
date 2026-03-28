#include <d3dcompiler.h>
#include <DirectXMath.h>
#include <climits>
#include <directxtk\WICTextureLoader.h>
#include <vector>
#include "Render.h"
#include "d3dwrap.h"
#include "LabMesh.h"

const float CLIENT_ASPECT = 4.0f/3.0f;

struct ConstBuffer
{
	DirectX::XMMATRIX MVP;
};

Render::Render() : VertexShader_(nullptr),
                    PixelShader_(nullptr),
                    Layout_(nullptr),
                    VertexBuffer_(nullptr),
                    ConstBuffer_(nullptr),
                    TextureWall_(nullptr),
                    TextureFloor_(nullptr),
                    Sampler_(nullptr),
                    VerticesCount_(0),
                    PosX_(0.0f),
	                PosY_(0.0f),
	                AtX_(1.0f),
	                AtY_(0.0f)



{    
}
  
Render::~Render()
{
   GContext()->ClearState();

   if (Sampler_) Sampler_->Release();
   if (TextureWall_) TextureWall_->Release();   
   if (TextureFloor_) TextureFloor_->Release();   
   if (ConstBuffer_) ConstBuffer_->Release();
   if (VertexBuffer_) VertexBuffer_->Release();
   if (PixelShader_) PixelShader_->Release();
   if (Layout_) Layout_->Release();
   if (VertexShader_) VertexShader_->Release();
      
}   

HRESULT Render::Init(Map &m)
{
   

    MeshBuilder mb;

    BuildLabirintMesh(mb,m,0.0f,0.0f);

    
    if (mb.vertices.size() > UINT_MAX) {
        return E_FAIL;
    }

    size_t BufferSize = mb.vertices.size() * sizeof(float);
    if (BufferSize > UINT_MAX) {
        return E_FAIL;
    }

    
    VerticesCount_ = static_cast<UINT>(mb.vertices.size()) / 5;

    ID3DBlob * VSBlob;
    HRESULT hr = D3DCompileFromFile(L"Data\\TransTex.hlsl",nullptr,nullptr,"VSMain","vs_4_0",0,0,&VSBlob,nullptr);
    if (FAILED(hr)) {
        return hr;
    }

    hr = GDevice()->CreateVertexShader(VSBlob->GetBufferPointer(),VSBlob->GetBufferSize(),nullptr,&VertexShader_);
    if (FAILED(hr)) {
        return hr;
    }

    D3D11_INPUT_ELEMENT_DESC layouts[] {
		 { "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		 { "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 12,  D3D11_INPUT_PER_VERTEX_DATA, 0 } 
    };

    UINT size = ARRAYSIZE(layouts);

    hr = GDevice()->CreateInputLayout(layouts,size,VSBlob->GetBufferPointer(), VSBlob->GetBufferSize(),&Layout_);
    VSBlob->Release();
    if (FAILED(hr)) {
        return hr;
    }


    ID3DBlob * PSBlob;
    hr = D3DCompileFromFile(L"Data\\TransTex.hlsl",nullptr,nullptr,"PSMain","ps_4_0",0,0,&PSBlob,nullptr);
    if (FAILED(hr)) {
        return hr;
    }

    hr = GDevice()->CreatePixelShader(PSBlob->GetBufferPointer(), PSBlob->GetBufferSize(), nullptr, &PixelShader_);
    if (FAILED(hr)) {
        return hr;
    }    





    D3D11_BUFFER_DESC desc{};
	desc.Usage = D3D11_USAGE_DEFAULT;
	desc.ByteWidth = static_cast<UINT>(BufferSize);
	desc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	D3D11_SUBRESOURCE_DATA  InitData{};
	InitData.pSysMem = &mb.vertices[0];

    hr =  GDevice()->CreateBuffer(&desc,&InitData,&VertexBuffer_);
    if (FAILED(hr)) {
        return hr;
    }    


    desc.ByteWidth = sizeof(ConstBuffer);
	desc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
    hr = GDevice()->CreateBuffer(&desc,nullptr,&ConstBuffer_);
    if (FAILED(hr)) {
        return hr;
    }    


    hr = DirectX::CreateWICTextureFromFile( GDevice(), GContext(),   L"Data\\wall.png",     nullptr,&TextureWall_);
    if (FAILED(hr)) {
        return hr;
    }

    hr = DirectX::CreateWICTextureFromFile( GDevice(), GContext(),   L"Data\\floor.png",     nullptr,&TextureFloor_);
    if (FAILED(hr)) {
        return hr;
    }

    
	D3D11_SAMPLER_DESC sampDesc={};
	sampDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
    sampDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
    sampDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
    sampDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
    sampDesc.ComparisonFunc = D3D11_COMPARISON_NEVER;
    sampDesc.MinLOD = 0;
    sampDesc.MaxLOD = D3D11_FLOAT32_MAX;
	hr = GDevice()->CreateSamplerState(&sampDesc, &Sampler_);


    GContext()->IASetInputLayout(Layout_);
    UINT Stride = 5*sizeof(float);
	UINT Offset = 0;

    GContext()->IASetVertexBuffers(0,1,&VertexBuffer_,&Stride,&Offset);
    GContext()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

    
 
    return S_OK;
}


void Render::Frame()
{


	DirectX::XMVECTOR Eye = DirectX::XMVectorSet(-PosX_,0.0f,PosY_,0.0f);
	DirectX::XMVECTOR At = DirectX::XMVectorSet(-AtX_,0.0f,AtY_,0.0f);
	DirectX::XMVECTOR Up = DirectX::XMVectorSet(0.0f,1.0f,0.0f,0.0f);

	DirectX::XMMATRIX View = DirectX::XMMatrixLookAtLH(Eye,At,Up);

	DirectX::XMMATRIX Proj = DirectX::XMMatrixPerspectiveFovLH(DirectX::XM_PIDIV4,CLIENT_ASPECT,1.0f,100.0f);

	DirectX::XMMATRIX MVP =  View * Proj;

    ConstBuffer CB;
	CB.MVP = XMMatrixTranspose(MVP);

    GContext()->UpdateSubresource(ConstBuffer_,0,nullptr,&CB,0,0);

    const float Color[] {0.0f, 0.0f, 0.1f, 1.0f};   

    D3DWrap::GetInstance()->Clear(Color);
    GContext()->VSSetShader(VertexShader_,nullptr,0);
    GContext()->VSSetConstantBuffers(0,1,&ConstBuffer_);
    GContext()->PSSetShader(PixelShader_,nullptr,0);
    GContext()->PSSetShaderResources(0, 1, &TextureWall_);
    GContext()->PSSetSamplers(0,1,&Sampler_);
    GContext()->Draw(VerticesCount_-6,0);


    GContext()->PSSetShaderResources(0, 1, &TextureFloor_);
    GContext()->Draw(6, VerticesCount_-6);

    D3DWrap::GetInstance()->GetSwapChain()->Present(0,0);

}