#include <stdlib.h>
#include <GL\glut.h>
#include "Material.h"
#include "Vector4.h"

/*
 * create a new material with default properties
 */
Material::Material()
{
	ambientColor.set(0,0,0,1);
	diffuseColor.set(1,1,1,1);
	specularColor.set(1,1,1,1);
	emissionColor.set(0,0,0,1);
	shininess[0] = 1.0f;
}

/*
 * create a new material given diffuse color
 */
Material::Material(Vector4 color)
{
	ambientColor.set(0,0,0,1);
	diffuseColor.set(color);
	specularColor.set(1,1,1,1);
	emissionColor.set(0,0,0,1);
	shininess[0] = 1.0f;
}

/*
 * create a new material with given ambient, diffuse and specular components
 */
Material::Material(Vector4 ambient, Vector4 diffuse, Vector4 specular)
{
	ambientColor.set(ambient);
	diffuseColor.set(diffuse);
	specularColor.set(specular);
	emissionColor.set(0,0,0,1);
	shininess[0] = 1.0f;
}

/*
 * create a new material with given ambient, diffuse and specular components and shininess
 */
Material::Material(Vector4 ambient, Vector4 diffuse, Vector4 specular, float shine)
{
	ambientColor.set(ambient);
	diffuseColor.set(diffuse);
	specularColor.set(specular);
	emissionColor.set(0,0,0,1);
	shininess[0] = shine;
}

/*
 * set the ambient color
 */
void Material::setAmbient(Vector4 ambient)
{
	ambientColor = ambient;
}

/*
 * set the diffuse color
 */
void Material::setDiffuse(Vector4 diffuse)
{
	diffuseColor = diffuse;
}

/*
 * set the specular color
 */
void Material::setSpecular(Vector4 specular)
{
	specularColor = specular;
}

/*
 * set the emission color
 */
void Material::setEmission(Vector4 emission)
{
	emissionColor = emission;
}

/*
 * set shininess
 */
void Material::setShininess(float shine)
{
	shininess[0] = shine;
}

/*
 * load the material properties into open gl (applies to all objects drawn after)
 */
void Material::loadMaterial()
{
	glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT,   ambientColor.getPtrf());
	glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE,   diffuseColor.getPtrf());
	glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR,  specularColor.getPtrf());
	glMaterialfv(GL_FRONT_AND_BACK, GL_EMISSION,  emissionColor.getPtrf());
	glMaterialfv(GL_FRONT_AND_BACK, GL_SHININESS, shininess);
}
