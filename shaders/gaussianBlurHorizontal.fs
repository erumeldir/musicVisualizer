uniform sampler2D tex;
uniform sampler2D mask;
uniform float blurSize;
uniform float sigma;
const float pi = 3.14159265;

void main()
{		
    vec3 incrementalGaussian;
    incrementalGaussian.x = 1.0 / (sqrt(2.0 * pi) * sigma);
    incrementalGaussian.y = exp(-0.5 / (sigma * sigma));
    incrementalGaussian.z = incrementalGaussian.y * incrementalGaussian.y;

    vec4 avgValue = vec4(0.0, 0.0, 0.0, 0.0);
    float coefficientSum = 0.0;

    avgValue += texture2D(tex, gl_TexCoord[0].xy) * incrementalGaussian.x * texture2D(mask,gl_TexCoord[0].xy).r;
    coefficientSum += incrementalGaussian.x;
    incrementalGaussian.xy *= incrementalGaussian.yz;

    for (float i = 1.0; i <= 4.0; i++) { 
        avgValue += texture2D(tex, gl_TexCoord[0].xy - i * blurSize * 
                          vec2(1.0, 0.0)) * incrementalGaussian.x * texture2D(mask,gl_TexCoord[0].xy + i * blurSize * vec2(1.0, 0.0)).r;         
        avgValue += texture2D(tex, gl_TexCoord[0].xy + i * blurSize * 
                          vec2(1.0, 0.0)) * incrementalGaussian.x * texture2D(mask,gl_TexCoord[0].xy + i * blurSize * vec2(1.0, 0.0)).r;         
        coefficientSum += 2.0 * incrementalGaussian.x;
        incrementalGaussian.xy *= incrementalGaussian.yz;
    }

    gl_FragData[0] = avgValue / coefficientSum;
}
