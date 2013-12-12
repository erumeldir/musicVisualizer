uniform vec3 baseColor;
uniform float time;
uniform vec2 freqs;
uniform float tempo;
uniform vec2 resolution;

const float di = 0.707107;

void main( void ) {
/*
   vec2 p = gl_FragCoord.xy / resolution.xy;

   float t = time/12.0;
   float st = sin(t);
   float ct = cos(t);

   p -= vec2(0.33, 0.66);
   p *= vec2(0.5);

   float pinwheel;
   pinwheel = -atan(p.y*st,p.x*st)/6.28+t;
   pinwheel = abs(fract(pinwheel*6.0)*3.0-1.0);

   vec3 color;
   color = vec3(0.0, clamp(pinwheel+st, 0.0, 0.3), clamp(pinwheel-ct, 0.0, 0.3));
   gl_FragColor = vec4(color, 1.0);*/

  /*vec2 uv = gl_FragCoord.xy / resolution.xy;
  vec2 pos = -1.0 + 2.0 * uv;
  //vec2 pos = gl_FragCoord.xy / resolution;
  
  float colorAmount = 10.0;
  float wave = 0.0;
  vec3 color = vec3(0.0);
  const float waveSpacing = 0.3;

  for(float i=0.0; i<5.0;i++){
    wave = sin(2.0*pos.x + i*waveSpacing + time)+ pos.x;
    colorAmount = 1.0/abs(wave-pos.y)*0.03; 
  
    color += colorAmount*baseColor;
  }
  color = clamp(color, 0.0, 0.4);

  gl_FragColor = vec4(color, 1.0);*/


  vec2 p = gl_FragCoord.xy / resolution.xy;

  gl_FragColor = vec4(clamp(sin(p.y - 0.1), 0.0, 0.6), baseColor.gb, 1.0);
  gl_FragColor.r *= .6;
  gl_FragColor.r += sin(time/3.0)*.015;
  gl_FragColor.r += tempo*.2;
}  
