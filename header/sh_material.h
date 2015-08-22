#ifndef MATERIAL_H
#define MATERIAL_H

#include <Shar\headers\shar.h>

//material object properties, this includes reflective properites, (k)
typedef struct sh_material {
vec4 ambient;
vec4 diffuse;
vec4 specular;
vec4 emission;
float shininess;
} sh_material;
#endif 
