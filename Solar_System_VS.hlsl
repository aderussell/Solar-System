//**************************************************************************//
// VERTEX Shader file for the descendent of AbstractThing3D.  It is very	//
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


//**************************************************************************//
// Vertex Shader.															//
//**************************************************************************//
VS_OUTPUT VS_obj( VS_INPUT input )
{
	VS_OUTPUT output;

	// Change the position vector to be 4 units for proper matrix calculations.
    //input.Position.w = 1.0f;

    // Calculate the position of the vertex against the world, view, and projection matrices.
    output.Position = mul(input.Position, matWorldViewProjection);
    
    // Store the texture coordinates for the pixel shader.
    output.TexUV = input.TexUV;
    
    // Calculate the normal vector against the world matrix only and then normalize the final value.
    output.Normal = normalize(mul(input.Normal, (float3x3)matWorld)); 

	//Both the input tangent and binormal are calculated against the world matrix and then normalized the same as the input normal vector. 
    // Calculate the tangent vector against the world matrix only and then normalize the final value.
    output.Tangent = normalize(mul(input.Tangent, (float3x3)matWorld)); 

    // Calculate the binormal vector against the world matrix only and then normalize the final value.
    output.Binormal = normalize(mul(input.Binormal, (float3x3)matWorld)); 

    return output;
};