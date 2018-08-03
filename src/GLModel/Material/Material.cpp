//
// Created by vlado on 01-Aug-18.
//

#include "Material.h"
#include "../../Program/Program.h"
#include "../../util/uniforms.h"

void Material::use() const {
    const Program &program = getProgram();
    program.setVec3(UNI_MATERIAL_AMBIENT, ambient);
    program.setVec3(UNI_MATERIAL_DIFFUSE, diffuse);
    program.setVec3(UNI_MATERIAL_SPECULAR, specular);
    program.setFloat(UNI_MATERIAL_SHININESS, shininess);
}

void Material::cleanGL() const {}

void Material::printData() const {
    printf("Printing data for material (%x):\n", this);
    printf("\tambient\t= (%.2f, %.2f %.2f)\n", ambient.r, ambient.g, ambient.b);
    printf("\tdiffuse\t= (%.2f, %.2f %.2f)\n", diffuse.r, diffuse.g, diffuse.b);
    printf("\tspecular\t= (%.2f, %.2f %.2f)\n", specular.r, specular.g, specular.b);
    printf("\tshininess\t= %.2f\n", shininess);
}
