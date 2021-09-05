#ifndef SHADER_H
#define SHADER_H

#include "glfw.h"

struct _shader_t {
    GLuint program;
};

typedef struct _shader_t shader_t;

void load_shader(shader_t *shader, const char *vertex_shader_path, const char *fragment_shader_path);

#endif  // SHADER_H
