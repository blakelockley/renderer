#ifndef MODEL_H
#define MODEL_H

#include "glfw.h"

struct _model_t {
    GLuint vao, vbo, ebo;
    GLsizei count;
};

typedef struct _model_t model_t;

void load_model(model_t* model, const char* path);
void draw_model(model_t* model);
void free_model(model_t* model);

#endif  // MODEL_H
