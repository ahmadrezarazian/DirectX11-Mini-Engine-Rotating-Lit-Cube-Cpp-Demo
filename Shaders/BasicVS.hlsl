cbuffer ObjectCB : register(b0)
{
    float4x4 gWorld;
    float4x4 gView;
    float4x4 gProjection;
};

struct VSInput
{
    float3 position : POSITION;
    float3 normal   : NORMAL;
    float3 color    : COLOR;
};

struct VSOutput
{
    float4 position  : SV_POSITION;
    float3 worldPos  : TEXCOORD0;
    float3 normal    : TEXCOORD1;
    float3 color     : COLOR0;
};

VSOutput VSMain(VSInput input)
{
    VSOutput output;

    float4 worldPos = mul(float4(input.position, 1.0f), gWorld);
    float4 viewPos  = mul(worldPos, gView);
    output.position = mul(viewPos, gProjection);

    output.worldPos = worldPos.xyz;
    output.normal = normalize(mul(float4(input.normal, 0.0f), gWorld).xyz);
    output.color = input.color;

    return output;
}