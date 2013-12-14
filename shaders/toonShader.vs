varying vec3 normal, lightDir, eyeVec;
varying float height;
void main()
{	
	normal = gl_NormalMatrix * gl_Normal;

	vec3 vVertex = vec3(gl_ModelViewMatrix * gl_Vertex);

	lightDir = vec3(gl_LightSource[0].position.xyz - vVertex);
	eyeVec = -vVertex;
	gl_FrontColor = vec4(0.0, 1.0, 0.0, 1.0);

	height = gl_Vertex.y/150.0;

	gl_Position = ftransform();
}
