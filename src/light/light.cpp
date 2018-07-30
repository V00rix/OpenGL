//
// Created by vlado on 21-Jul-18.
//

#include "light.h"

static light::uni::u_base getBase(GLuint shader, std::string &u_name) {
    return {
            glGetUniformLocation(shader, (u_name + ".base.color").c_str()),
            glGetUniformLocation(shader, (u_name + ".base.ambient_intensity").c_str()),
            glGetUniformLocation(shader, (u_name + ".base.diffuse_intensity").c_str())
    };
}

static light::uni::u_point::Attenuation getAttenuation(GLuint shader, std::string &u_name) {
    return {
            glGetUniformLocation(shader, (u_name + ".attenuation.constant").c_str()),
            glGetUniformLocation(shader, (u_name + ".attenuation.linear").c_str()),
            glGetUniformLocation(shader, (u_name + ".attenuation.exponential").c_str())
    };
}

static void setBase(const light::uni::u_base &uniform, const light::Base &light) {
    glUniform3f(uniform.color, light.color.r, light.color.g, light.color.b);
    glUniform1f(uniform.ambient_intensity, light.ambientIntensity);
    glUniform1f(uniform.diffuse_intensity, light.diffuseIntensity);
}

light::uni::u_directional light::uni::getDirectional(GLuint shader, const char *uniform_name) {
    return light::uni::getDirectional(shader, std::string(uniform_name));
}

light::uni::u_directional light::uni::getDirectional(GLuint shader, std::string uniform_name) {
    std::cout << uniform_name << "\n";

    return {
            getBase(shader, uniform_name),
            glGetUniformLocation(shader, (uniform_name + ".direction").c_str())
    };
}

void light::uni::setDirectional(const u_directional &uniform, const light::Directional &light) {
    setBase(uniform.base, light);
    glUniform3f(uniform.u_direction, light.direction.x, light.direction.y, light.direction.z);
}

light::uni::u_point light::uni::getPoint(GLuint shader, const char *uniform_name) {
    std::string u_name = std::string(uniform_name);

    return {
            getBase(shader, u_name),
            getAttenuation(shader, u_name),
            glGetUniformLocation(shader, (u_name + ".position").c_str())
    };
}

void light::uni::setPoint(const light::uni::u_point &uniform, const light::Point &light) {
    setBase(uniform.base, light);
    glUniform3f(uniform.position, light.position.x, light.position.y, light.position.z);
    glUniform1f(uniform.attenuation.constant, light.attenuation.constant);
    glUniform1f(uniform.attenuation.linear, light.attenuation.linear);
    glUniform1f(uniform.attenuation.exponential, light.attenuation.exponential);
}

light::uni::u_spot light::uni::getSpot(GLuint shader, const char *uniform_name) {
    std::string u_name = std::string(uniform_name);

    return {
            getPoint(shader,  (u_name + ".point").c_str()),
            glGetUniformLocation(shader, (u_name + ".direction").c_str()),
            glGetUniformLocation(shader, (u_name + ".cutoff").c_str())
    };
}

void light::uni::setSpot(const light::uni::u_spot &uniform, const light::Spot &light) {
    setPoint(uniform, light);
    glUniform3f(uniform.direction, light.direction.x, light.direction.y, light.direction.z);
    glUniform1f(uniform.cutoff, light.cutoff);
}
