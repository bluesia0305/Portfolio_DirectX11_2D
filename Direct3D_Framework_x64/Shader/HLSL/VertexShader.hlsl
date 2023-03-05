#include "Layout.hlsli"

namespace Shader
{
    cbuffer Transform : register(B0) { matrix World; };
    cbuffer Transform : register(B1) { matrix ViewByProj; };
    
	Layout::Pixel Vertex(const Layout::Vertex Input)
	{
		Layout::Pixel Output =
		{
			Input.Position,
			Input.Texcoord
		};
        
        Output.Position = mul(Output.Position, World);
        Output.Position = mul(Output.Position, ViewByProj);
        
		return Output;
	}
}