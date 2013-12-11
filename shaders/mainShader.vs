varying vec3 normal, lightDir, eyeVec;

void main()
{	
	normal = gl_NormalMatrix * gl_Normal;

	vec3 view_vertex = vec3(gl_ModelViewMatrix * gl_Vertex);

	lightDir = gl_LightSource[0].position.xyz - view_vertex;
	eyeVec = -view_vertex;

	gl_Position = ftransform();
}