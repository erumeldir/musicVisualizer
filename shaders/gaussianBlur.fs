// Fake Gaussian Blur
// To use for a glow:
// 1. Render glow sources
// 2. Blur filter the resulting image
// 2.5 Can separate horizontal and vertical blur filter into separate passes; just set blend function to GL_ONE, GL_ONE
// 3. Save result to a temporary texture object
// 4. Blend result of glow with non-glow image for final result

uniform sampler2D tex;
float blurSize = .005;

// TODO allow setting size of Gaussian blur pixel neighborhood
// TODO allow setting of amplitude and width of Gaussian function
// TODO add attenuation factor for color
// TODO separate into separate vertical/horizontal passes

void main()
{		
	vec4 color = vec4(0.0);
	vec2 coord = vec2(0.0);

	// Blur y
	coord.x = gl_TexCoord[0].s;
	coord.y = (gl_TexCoord[0].t - 4.0*blurSize);
	color += texture2D(tex, coord)*1.0/40.0;
	coord.y = (gl_TexCoord[0].t - 3.0*blurSize);
	color += texture2D(tex, coord)*2.0/40.0;
	coord.y = (gl_TexCoord[0].t - 2.0*blurSize);
	color += texture2D(tex, coord)*3.0/40.0;
	coord.y = (gl_TexCoord[0].t - blurSize);
	color += texture2D(tex, coord)*4.0/40.0;
	coord.y = (gl_TexCoord[0].t + blurSize);
	color += texture2D(tex, coord)*4.0/40.0;
	coord.y = (gl_TexCoord[0].t + 2.0*blurSize);
	color += texture2D(tex, coord)*3.0/40.0;
	coord.y = (gl_TexCoord[0].t + 3.0*blurSize);
	color += texture2D(tex, coord)*2.0/40.0;
	coord.y = (gl_TexCoord[0].t + 4.0*blurSize);
	color += texture2D(tex, coord)*1.0/40.0;


	// Blur x
	coord.y = gl_TexCoord[0].t;
	coord.x = (gl_TexCoord[0].s - 4.0*blurSize);
	color += texture2D(tex, coord)*1.0/40.0;
	coord.x = (gl_TexCoord[0].s - 3.0*blurSize);
	color += texture2D(tex, coord)*2.0/40.0;
	coord.x = (gl_TexCoord[0].s - 2.0*blurSize);
	color += texture2D(tex, coord)*3.0/40.0;
	coord.x = (gl_TexCoord[0].s - blurSize);
	color += texture2D(tex, coord)*4.0/40.0;
	coord.x = (gl_TexCoord[0].s + blurSize);
	color += texture2D(tex, coord)*4.0/40.0;
	coord.x = (gl_TexCoord[0].s + 2.0*blurSize);
	color += texture2D(tex, coord)*3.0/40.0;
	coord.x = (gl_TexCoord[0].s + 3.0*blurSize);
	color += texture2D(tex, coord)*2.0/40.0;
	coord.x = (gl_TexCoord[0].s + 4.0*blurSize);
	color += texture2D(tex, coord)*1.0/40.0;

	gl_FragColor = color;	
}
