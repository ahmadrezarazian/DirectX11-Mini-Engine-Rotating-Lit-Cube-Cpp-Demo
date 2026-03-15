struct DirectionalLight
{
    float3 direction;
    float intensity;
    float3 color;
    float padding0;
};

struct PointLight
{
    float3 position;
    float intensity;
    float3 color;
    float range;
};

cbuffer SceneCB : register(b1)
{
    float3 gCameraPosition;
    float gAmbientStrength;

    DirectionalLight gDirectionalLight;
    PointLight gPointLight;
};

struct PSInput
{
    float4 position  : SV_POSITION;
    float3 worldPos  : TEXCOORD0;
    float3 normal    : TEXCOORD1;
    float3 color     : COLOR0;
};

float4 PSMain(PSInput input) : SV_TARGET
{
    float3 N = normalize(input.normal);
    float3 baseColor = input.color;

    float3 ambient = baseColor * gAmbientStrength;

    // Directional light
    float3 Ld = normalize(-gDirectionalLight.direction);
    float NdotL_d = max(dot(N, Ld), 0.0f);
    float3 directional = baseColor * gDirectionalLight.color * (NdotL_d * gDirectionalLight.intensity);

    // Point light
    float3 toLight = gPointLight.position - input.worldPos;
    float dist = length(toLight);
    float3 Lp = normalize(toLight);
    float atten = saturate(1.0f - dist / gPointLight.range);
    atten *= atten;

    float NdotL_p = max(dot(N, Lp), 0.0f);
    float3 pointDiffuse = baseColor * gPointLight.color * (NdotL_p * gPointLight.intensity * atten);

    // Specular from point light
    float3 V = normalize(gCameraPosition - input.worldPos);
    float3 R = reflect(-Lp, N);
    float spec = pow(max(dot(V, R), 0.0f), 32.0f) * atten;
    float3 specular = gPointLight.color * spec * 0.5f;

    float3 finalColor = ambient + directional + pointDiffuse + specular;

    return float4(saturate(finalColor), 1.0f);
}