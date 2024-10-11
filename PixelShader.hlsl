struct PS_INPUT
{
    float4 position : SV_POSITION;
    float3 color    : COLOR;
    float3 color1   : COLOR1;
    float2 uv       : TEXCOORD0; // Add texture coordinates to calculate distance from the center
};

cbuffer constant : register(b0)
{
    row_major float4x4 m_world;
    row_major float4x4 m_view;
    row_major float4x4 m_proj;

    unsigned int m_time;
}

float4 psmain(PS_INPUT input) : SV_TARGET
{
    // Calculate the distance from the center (0.5, 0.5 in UV space)
    float dist = length(input.uv - float2(0.5f, 0.5f));

    // Discard fragments that are outside the circle (radius 0.5)
    if (dist > 0.15f)
        discard;

    // Calculate color blending based on time
    float3 blendedColor = lerp(input.color, input.color1, (sin(m_time / 500.0f) + 1.0f) / 2.0f);

    return float4(blendedColor, 1.0f);
}