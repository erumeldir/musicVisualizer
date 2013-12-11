// Vertex shader that does nothing but what OpenGL normally does
//uniform float glow;

varying vec3 normal, lightDir, eyeVec;

void main()
{	
	normal = gl_NormalMatrix * gl_Normal;

	vec3 vVertex = vec3(gl_ModelViewMatrix * gl_Vertex);

	lightDir = vec3(gl_LightSource[0].position.xyz - vVertex);
	eyeVec = -vVertex;

	gl_Position = ftransform();
}
