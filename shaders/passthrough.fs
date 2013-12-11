// Fake Gaussian Blur
// To use for a glow:
// 1. Render glow sources
// 2. Blur filter the resulting image
// 2.5 Can separate horizontal and vertical blur filter into separate passes; just set blend function to GL_ONE, GL_ONE
// 3. Save result to a temporary texture object
// 4. Blend result of glow with non-glow image for final result

uniform sampler2D tex;

// TODO allow setting size of Gaussian blur pixel neighborhood
// TODO allow setting of amplitude and width of Gaussian function
// TODO add attenuation factor for color
// TODO separate into separate vertical/horizontal passes

void main()
{		
	gl_FragData[0] = texture2D(tex, vec2(gl_TexCoord[0].x, gl_TexCoord[0].y));
}
