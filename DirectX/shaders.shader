
cbuffer cbPerObject
{
	float4x4 WVP;
};
struct VOut
{
    float4 position : SV_POSITION;
    float4 color : COLOR;
};

VOut VShader(float4 normal : NORMAL, float4 position : POSITION, float4 color : COLOR)
{
    VOut output;
    output.position = mul(position+normal, WVP);
    output.color = color;

    return output;
}


float4 PShader(float4 position : SV_POSITION, float4 color : COLOR) : SV_TARGET
{
    return color;
}