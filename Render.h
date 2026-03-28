#pragma once

#include <d3d11.h>

#include "Map.h"

class Render
{
public:
  Render();
  ~Render();
  HRESULT Init(Map &m);

  
  void Pos(float posx, float posy)
	{
		PosX_=posx;
		PosY_=posy;
	}

	void LookAt(float atx, float aty)
	{
		AtX_=atx;
	  AtY_=aty;
	}

  void Frame();
private:
  ID3D11VertexShader * VertexShader_;
  ID3D11PixelShader * PixelShader_;
  ID3D11InputLayout * Layout_;
  ID3D11Buffer * VertexBuffer_;
  ID3D11Buffer * ConstBuffer_;
  ID3D11ShaderResourceView * TextureWall_;
  ID3D11ShaderResourceView * TextureFloor_;
  ID3D11SamplerState * Sampler_; 
  UINT VerticesCount_;
  float PosX_, PosY_;
	float AtX_, AtY_;


};