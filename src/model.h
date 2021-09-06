#ifndef MODEL_H
#define MODEL_H

#include "glfw.h"
#include "linmath.h"

struct _submodel_t {
    GLuint count;
    GLuint offset;

    vec3 bbox_min, bbox_max, bbox_mid;
    GLuint bb_index;

    struct _submodel_t* child;
};

struct _model_t {
    GLuint vao, vbo, ebo;
    GLuint count;

    GLuint bb_vao, bb_vbo, bb_ebo;

    struct _submodel_t* root;
};

typedef struct _model_t model_t;
typedef struct _submodel_t submodel_t;

void load_model(model_t* model, const char* path);
void draw_model(model_t* model);
void free_model(model_t* model);

#endif  // MODEL_H
