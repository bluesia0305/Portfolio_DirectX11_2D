namespace Layout
{
	struct Vertex
	{
		float4 Position : POSITION;
		float4 Texcoord : TEXCOORD;
	};
	
	struct Pixel
	{
		float4 Position : SV_POSITION;
		float4 Texcoord : TEXCOORD;
	};
	
	typedef float4 Color;
}