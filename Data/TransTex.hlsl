Texture2D txDiffuse : register(t0);
SamplerState samLinear : register( s0 );

matrix mMVP;

struct VS_INPUT
{
   float4 Pos : POSITION;
   float2 TexUV :TEXCOORD0;
};

struct PS_INPUT
{
   float4 Pos : SV_POSITION;
   float2 TexUV :TEXCOORD0;
};

PS_INPUT VSMain(VS_INPUT  Inp)
{
  PS_INPUT Out = (PS_INPUT)0;
  Out.Pos = mul(Inp.Pos, mMVP);
  Out.TexUV = Inp.TexUV;

  return Out;
}

float4 PSMain( PS_INPUT Inp) : SV_Target
{
    return txDiffuse.Sample( samLinear, Inp.TexUV );
}
