#version 330 core

out vec4 FragColor;

in vec2 v_TexCoord;

uniform float time;
uniform float timeSpeed;
uniform float heightScale;
uniform float decay;

const int MAX_RIPPLES = 10;
uniform int numRipples;
uniform vec2 rippleCenters[MAX_RIPPLES];
uniform float rippleStartTimes[MAX_RIPPLES];
uniform float frequency;
uniform float speed;

uniform sampler2D u_Texture;

void main()
{
    vec2 uv = v_TexCoord;
    vec2 displacedUV = uv;

    for (int i = 0; i < numRipples; ++i)
    {
        vec2 pos = uv - rippleCenters[i];
        float r = length(pos);
        float acTime = (time - rippleStartTimes[i]) * timeSpeed;

        if (acTime < 0.0) continue; // ignore future ripples

        vec2 dir = (r > 1e-4) ? normalize(pos) : vec2(0.0);
        float ripple = sin(frequency * r - speed * acTime)
                     * heightScale
                     * exp(-decay * (1.0 - r))
                     * exp(-0.4 * decay * acTime);

        displacedUV += dir * ripple;
    }

    vec4 baseColor = texture(u_Texture, displacedUV);
    FragColor = vec4(baseColor.rgb, 1.0);
}
