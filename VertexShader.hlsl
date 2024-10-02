struct VS_INPUT
{
    float4 position  : POSITION;
    float4 position1 : POSITION1;
    float3 color     : COLOR;
    float3 color1    : COLOR1;
    
    float4 xvelocity : XVELOCITY;
    float4 yvelocity : YVELOCITY;
};
    
struct VS_OUTPUT
{
    float4 position : SV_POSITION; 
    float3 color    : COLOR;
    float3 color1   : COLOR1;
};

cbuffer constant : register(b0)
{
    unsigned int m_time;
}

VS_OUTPUT vsmain( VS_INPUT input) 
{
    VS_OUTPUT output = (VS_OUTPUT)0;
    output.position = input.position;
    
    output.position = lerp(input.position, input.position1, (sin(m_time / 1000.0f) + 1.0f) / 2.0f);
    output.color = input.color;
    output.color1 = input.color1;
    
    output.position.x += (sin(input.xvelocity.x * (m_time / 1000.0f))) / 6.0f;
    output.position.y += input.yvelocity.y * (m_time / 1000.0f); 
    
    //output.position.x += (sin(m_time / 1000.0f)) / 6.0f;
    //output.position.y += m_time / 6500.0f;
    
    
    return output;
}

