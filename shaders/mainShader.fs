
uniform float glow;

varying vec3 normal, lightDir, eyeVec;
varying float height;

vec4 colorGradient(float h)
{
    vec4 color;
    vec3 col1 = vec3(0.1992,   0.0,  0.2578);
    vec3 col2 = vec3(0.5039, 0.0036, 0.2031);
    vec3 col3 = vec3(0.9726, 0.2773, 0.0351);
    vec3 col4 = vec3(0.9922, 0.6719, 0.1367);
    vec3 col5 = vec3(0.9648, 0.8984,    0.0);
    color = vec4(mix(col1, col2, h*4.0), 1.0)*(1.0-step(0.25, h)) +
        vec4(mix(col2, col3, ((h-0.25)*4.0)), 1.0)*(step(0.25, h))*(1.0-step(0.5, h)) + 
        vec4(mix(col3, col4, ((h-0.5)*4.0)), 1.0)*(step(0.5, h))*(1.0-step(0.75, h)) +
        vec4(mix(col4, col5, ((h-0.75)*4.0)), 1.0)*(step(0.75, h))*(1.0-step(1.0, h));
    return color;
}

void main()
{	

    vec4 materialDiffuse = colorGradient(height);
    vec4 materialAmbient = colorGradient(height);

	vec4 final_color = 
	(gl_FrontLightModelProduct.sceneColor * materialAmbient) + 
	(gl_LightSource[0].ambient * materialAmbient);

	vec3 N = normalize(normal);
	vec3 L = normalize(lightDir);
    
	float lambertTerm = dot(N,L);

	if(lambertTerm > 0.0)
	{
		final_color += gl_LightSource[0].diffuse * 
			       materialDiffuse * 
			       lambertTerm * 2.0;

		vec3 E = normalize(eyeVec);
		vec3 R = reflect(-L,N);
		float specular = pow( max(dot(R,E), 1.0),
				      gl_FrontMaterial.shininess );
		final_color += gl_LightSource[0].specular * gl_FrontMaterial.specular * specular;
	}
    gl_FragData[0] = final_color;
    //if (glow == 1.0)
    //    gl_FragData[0] = materialDiffuse;
    gl_FragData[1] = vec4(glow, 0.0, 0.0, 1.0);
}
