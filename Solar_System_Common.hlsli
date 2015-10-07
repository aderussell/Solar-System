//**************************************************************************//
// Common structures for the Thing3D vertex and pixel shaders.				// 
//																			//
// This  code is copyright of Dr Nigel Barlow, lecturer in	computing,		//
// University of Plymouth, UK.  email: nigel@soc.plymouth.ac.uk.			//
//																			//
// You may use, modify and distribute this (rather cack-handed in places)	//
// code subject to the following conditions:								//
//																			//
//	1:	You may not use it, or sell it, or use it in any adapted form for	//
//		financial gain, without my written premission.						//
//																			//
//	2:	You must not remove the copyright messages.							//
//																			//
//	3:	You should correct at least 10% of the typig abd spekking errirs.   //
//																			//
// This isn't M$ sample code; I done it all myself!							//
// ©Nigel Barlow nigel@soc.plymouth.ac.uk.									//
//**************************************************************************//


//**************************************************************************//
// Vertex shader output structure.	This is also the input to the pixel		//
// shader.																	//
//**************************************************************************//


struct Light
{
	float3 dir;
	float3 pos;
	float  range;
	float3 att;
	float4 ambient;
	float4 diffuse;
};


//**************************************************************************//
// Constant Buffer Variables.												//
//**************************************************************************//

cbuffer cbChangesEveryFrame : register( b0 )
{
    matrix matWorld;
	matrix matWorldViewProjection;
};


//**************************************************************************//
// Textures and Texture Samplers.  These variables can't (it seems) go in	//
// constant buffers; I think the size of a CB is too restricted.			//
//**************************************************************************//
Texture2D    txDiffuse  : register( t0 );
Texture2D    txDiffuse2 : register( t1 );
Texture2D    txSpecular : register( t2 );
SamplerState samLinear  : register( s0 );


struct VS_INPUT
{
	float4 Position  : POSITION;
	float2 TexUV     : TEXCOORD0;
	float3 Normal    : NORMAL;
    float3 Tangent   : TANGENT;
	float3 Binormal  : BINORMAL;
};

struct VS_OUTPUT
{
	float4 Position  : SV_POSITION;
	float2 TexUV     : TEXCOORD0;
	float3 Normal    : NORMAL;
    float3 Tangent   : TANGENT;
	float3 Binormal  : BINORMAL;
};

