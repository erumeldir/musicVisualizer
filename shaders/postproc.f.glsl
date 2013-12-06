uniform sampler2D fbo_texture;
varying vec2 f_texcoord;
float blurSize = 0.002;

void main()
{	
	vec4 color = vec4(0.0);
	vec2 coord = vec2(0.0);

	// Blur y
	coord.x = f_texcoord.x;
	coord.y = (f_texcoord.y - 4.0*blurSize);
	color += texture2D(fbo_texture, coord)*1.0/40.0;
	coord.y = (f_texcoord.y - 3.0*blurSize);
	color += texture2D(fbo_texture, coord)*2.0/40.0;
	coord.y = (f_texcoord.y - 2.0*blurSize);
	color += texture2D(fbo_texture, coord)*3.0/40.0;
	coord.y = (f_texcoord.y - blurSize);
	color += texture2D(fbo_texture, coord)*4.0/40.0;
	coord.y = (f_texcoord.y + blurSize);
	color += texture2D(fbo_texture, coord)*4.0/40.0;
	coord.y = (f_texcoord.y + 2.0*blurSize);
	color += texture2D(fbo_texture, coord)*3.0/40.0;
	coord.y = (f_texcoord.y + 3.0*blurSize);
	color += texture2D(fbo_texture, coord)*2.0/40.0;
	coord.y = (f_texcoord.y + 4.0*blurSize);
	color += texture2D(fbo_texture, coord)*1.0/40.0;


	// Blur x
	coord.y = f_texcoord.y;
	coord.x = (f_texcoord.x - 4.0*blurSize);
	color += texture2D(fbo_texture, coord)*1.0/40.0;
	coord.x = (f_texcoord.x - 3.0*blurSize);
	color += texture2D(fbo_texture, coord)*2.0/40.0;
	coord.x = (f_texcoord.x - 2.0*blurSize);
	color += texture2D(fbo_texture, coord)*3.0/40.0;
	coord.x = (f_texcoord.x - blurSize);
	color += texture2D(fbo_texture, coord)*4.0/40.0;
	coord.x = (f_texcoord.x + blurSize);
	color += texture2D(fbo_texture, coord)*4.0/40.0;
	coord.x = (f_texcoord.x + 2.0*blurSize);
	color += texture2D(fbo_texture, coord)*3.0/40.0;
	coord.x = (f_texcoord.x + 3.0*blurSize);
	color += texture2D(fbo_texture, coord)*2.0/40.0;
	coord.x = (f_texcoord.x + 4.0*blurSize);
	color += texture2D(fbo_texture, coord)*1.0/40.0;

	gl_FragColor = color;	
	//gl_FragColor = texture2D(fbo_texture, f_texcoord);
}
