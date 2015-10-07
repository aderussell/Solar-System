//**************************************************************************//
// PIXEL Shader file for the descendent of AbstractThing3D.  It is very		//
// heavily based on the shader provided in Microsoft's "SimpleSample".		//
//																			//
// This (poggled) code is copyright of Dr Nigel Barlow, lecturer in			//
// computing, University of Plymouth, UK.  email: nigel@soc.plymouth.ac.uk.	//
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
// Poggeling ©Nigel Barlow nigel@soc.plymouth.ac.uk.						//
// "Simple Sample" ©Microsoft Corporation. All rights reserved.				//
//**************************************************************************//

#include "Solar_System_Common.hlsli"


cbuffer cbNeverChanges : register( b1 )
{
    float4 materialColour;
	float4 vecLight;
};




//**************************************************************************//
// Pixel Shader.	This one has no lighting.  You can have many pixel		//
// shaders in one of these files.											//
//**************************************************************************//
float4 PS_TexturesNoLighting( VS_OUTPUT input) : SV_Target
{
    return txDiffuse.Sample( samLinear, input.TexUV ) * materialColour;
}


//**************************************************************************//
// Pixel Shader.	This one has basic lighting, however the really			//
// important part is the rexture sampler.									//
//**************************************************************************//
float4 PS_TexturesWithLighting( VS_OUTPUT input) : SV_Target
{
	//float4 lightVec = float4(1.0, 0.0, 1.0, 1.0);
	//float3 vecNorm  = float3(1.0, 1.0, 0.0);
	//float4 light = saturate( dot( lightVec, vecNorm ) );
	float4 light = saturate( dot( vecLight, input.Normal ) );
    return txDiffuse.Sample( samLinear, input.TexUV ) * materialColour * light;
}


float4 PS_TexturesWithDarkTexture( VS_OUTPUT input) : SV_Target
{
	float4 light = saturate( dot( vecLight, input.Normal ) );
	light *= 1.3;
	float4 lightTex = txDiffuse.Sample( samLinear, input.TexUV );//;

	float4 dark = saturate( float4(0.5,0.5,0.5,0.0) - light );
	float4 darkTex = txDiffuse2.Sample( samLinear, input.TexUV );// * materialColour * dark;

	return saturate(   ( (lightTex * materialColour * light) + saturate(darkTex * dark) ) );// * materialColour * light*50 );
}



float4 PS_TextureWithBumpMap( VS_OUTPUT input ) : SV_Target
{

	float4 textureColor;
    float4 bumpMap;
    float3 bumpNormal;
    float3 lightDir;
    float lightIntensity;
    float4 color;


    // Sample the texture pixel at this location.
    textureColor = txDiffuse.Sample( samLinear, input.TexUV );
	
    // Sample the pixel in the bump map.
    bumpMap = txDiffuse2.Sample( samLinear, input.TexUV );

    // Expand the range of the normal value from (0, +1) to (-1, +1).
    bumpMap = (bumpMap * 2.0f) - 1.0f;

    // Calculate the normal from the data in the bump map.
    bumpNormal.rgb = (bumpMap.x * input.Tangent) + (bumpMap.y * input.Binormal) + (bumpMap.z * input.Normal);
	
    // Normalize the resulting bump normal.
    bumpNormal = normalize(bumpNormal);

    // Invert the light direction for calculations.
    lightDir = vecLight;

    // Calculate the amount of light on this pixel based on the bump map normal value.
    lightIntensity = saturate(dot(bumpNormal, lightDir));

    // Determine the final diffuse color based on the diffuse color and the amount of light intensity.
    color = materialColour * lightIntensity;

    // Combine the final bump light color with the texture color.
    return textureColor * color;
}


//***************************************************************************************************//


//**************************************************************************//
// Pixel Shader.	This one has no lighting.  You can have many pixel		//
// shaders in one of these files.											//
//**************************************************************************//
float4 PS_NoTexturesNoLighting( VS_OUTPUT input) : SV_Target
{
    return materialColour;
}


//**************************************************************************//
// Pixel Shader.	This one has basic lighting, however the really			//
// important part is the rexture sampler.									//
//**************************************************************************//
float4 PS_NoTexturesWithLighting(VS_OUTPUT input) : SV_Target
{
	float4 light = saturate( dot( vecLight, input.Normal ) );
    return materialColour * light;
}