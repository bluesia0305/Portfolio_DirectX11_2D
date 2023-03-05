#include "Layout.hlsli"

namespace Shader
{
    const Texture2D Resource : register(T0);
    cbuffer Transparency : register(B0) { float blendAmount; };
    
	Layout::Color Pixel(const Layout::Pixel Input) : SV_TARGET
	{
		Layout::Color Output =
		{
            Resource.Load(int3(Input.Texcoord.x, Input.Texcoord.y, 0))
        };
        
        if (blendAmount != 1 && Output.a > 0.0f)
            Output.a *= blendAmount;
        
		return Output;
	}
}