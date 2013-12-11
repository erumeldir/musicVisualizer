varying vec3 normal, lightDir, eyeVec;

void main()
{
  
    // if using textures, can do vec4(.2,.15,.15,1.0)*tex_color

    vec4 final_color = gl_FrontMaterial.ambient * gl_LightSource[0].ambient + gl_LightModel.ambient * gl_FrontMaterial.ambient;
    vec3 N = normalize(normal);
    vec3 L = normalize(lightDir);

    float lambertTerm = dot(N,L);

    if (lambertTerm > 0.0)
    {
        final_color += gl_LightSource[0].diffuse * gl_FrontMaterial.diffuse * lambertTerm;

        vec3 E = normalize(eyeVec);
        vec3 R = reflect(-L, N);

        float specular = pow(max(dot(R, E), 0.0), gl_FrontMaterial.shininess);
        final_color += gl_LightSource[0].specular * gl_FrontMaterial.specular * specular;
    }

    final_color.a = 1.0;
    gl_FragData[0] = vec4(0.0, 1.0, 1.0, 1.0);
}