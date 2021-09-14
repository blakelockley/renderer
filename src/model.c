#include "model.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "linmath.h"

#define LARGE (float)10e+32

static const vec3 min = {+LARGE, +LARGE, +LARGE};
static const vec3 max = {-LARGE, -LARGE, -LARGE};

size_t indices_n;

uint32_t *indices, *bb_indices;
float* bb_vertices;

size_t bb_vertices_n, bb_indices_n, prev_indices_n;

submodel_t** finalise_submodel(submodel_t* submodel) {
    submodel->count = indices_n - prev_indices_n;
    prev_indices_n = indices_n;

    bb_vertices = realloc(bb_vertices, sizeof(float) * 3 * (bb_vertices_n + 8));
    bb_indices = realloc(bb_indices, sizeof(uint32_t) * (bb_indices_n + 24));

    float* min = submodel->bbox_min;
    float* max = submodel->bbox_max;

    float points[8][3] = {
        {min[0], min[1], max[2]},
        {max[0], min[1], max[2]},
        {max[0], max[1], max[2]},
        {min[0], max[1], max[2]},

        {min[0], min[1], min[2]},
        {max[0], min[1], min[2]},
        {max[0], max[1], min[2]},
        {min[0], max[1], min[2]},
    };

    int n = bb_vertices_n;

    // clang-format off
    u_int32_t lines[12][2] = {
        {n + 0, n + 1}, {n + 1, n + 2}, {n + 2, n + 3}, {n + 3, n + 0},
        {n + 4, n + 5}, {n + 5, n + 6}, {n + 6, n + 7}, {n + 7, n + 4}, 
        {n + 0, n + 4}, {n + 1, n + 5}, {n + 2, n + 6}, {n + 3, n + 7},
    };
    // clang-format on

    memcpy(bb_vertices + bb_vertices_n * 3, points, sizeof(points));
    memcpy(bb_indices + bb_indices_n, lines, sizeof(lines));

    bb_vertices_n += 8;
    bb_indices_n += 24;

    vec3_add(submodel->bbox_mid, submodel->bbox_min, submodel->bbox_max);
    vec3_scale(submodel->bbox_mid, submodel->bbox_mid, 0.5f);

    bb_vertices = realloc(bb_vertices, sizeof(float) * 3 * (bb_vertices_n + 1));
    bb_indices = realloc(bb_indices, sizeof(uint32_t) * (bb_indices_n + 1));

    memcpy(bb_vertices + bb_vertices_n * 3, submodel->bbox_mid, sizeof(submodel->bbox_mid));
    bb_indices[bb_indices_n++] = bb_vertices_n++;

    return &(submodel->child);
}

void load_model(model_t* model, const char* path) {
    FILE* file = fopen(path, "r");

    if (file == NULL) {
        fprintf(stderr, "Failed to load model: %s\n", path);
        return;
    }

    char* line = NULL;
    size_t len = 0;

    float *positions = NULL, *normals = NULL, *uvs = NULL;
    size_t positions_n = 0, normals_n = 0, uvs_n = 0;

    float* vertices = NULL;
    size_t vertices_n = 0;

    indices = NULL;
    indices_n = 0;

    bb_vertices = NULL;
    bb_vertices_n = 0;

    bb_indices = NULL;
    bb_indices_n = 0;

    prev_indices_n = 0;

    model->root = NULL;
    submodel_t** submodel = &model->root;

    uint32_t submodel_n = 0;

    vec3 buffer;
    while (getline(&line, &len, file) != EOF) {
        if (strncmp(line, "v ", 2) == 0) {
            sscanf(line, "v %f %f %f", &buffer[0], &buffer[1], &buffer[2]);

            positions = realloc(positions, sizeof(vec3) * (positions_n + 1));
            memcpy(positions + positions_n * 3, buffer, sizeof(float) * 3);
            positions_n += 1;

            for (int i = 0; i < 3; i++) {
                if (buffer[i] < (*submodel)->bbox_min[i])
                    (*submodel)->bbox_min[i] = buffer[i];

                if (buffer[i] > (*submodel)->bbox_max[i])
                    (*submodel)->bbox_max[i] = buffer[i];
            }

        } else if (strncmp(line, "vn ", 3) == 0) {
            sscanf(line, "vn %f %f %f", &buffer[0], &buffer[1], &buffer[2]);

            normals = realloc(normals, sizeof(vec3) * (normals_n + 1));
            memcpy(normals + normals_n * 3, buffer, sizeof(float) * 3);
            normals_n += 1;

        } else if (strncmp(line, "vt ", 3) == 0) {
            sscanf(line, "vt %f %f %f", &buffer[0], &buffer[1], &buffer[2]);

            uvs = realloc(uvs, sizeof(vec2) * (uvs_n + 1));
            memcpy(uvs + uvs_n * 2, buffer, sizeof(float) * 2);
            uvs_n += 1;

        } else if (strncmp(line, "o ", 2) == 0) {
            if (*submodel != NULL)
                submodel = finalise_submodel(*submodel);

            *submodel = malloc(sizeof(submodel_t));
            (*submodel)->offset = indices_n;
            (*submodel)->child = NULL;

            (*submodel)->bb_index = submodel_n++;
            memcpy(&(*submodel)->bbox_min, min, sizeof(vec3));
            memcpy(&(*submodel)->bbox_max, max, sizeof(vec3));

        } else if (strncmp(line, "f ", 2) == 0) {
            uint32_t ap, at, an;  // a = vertex, p = position, t = texture, n = normal
            uint32_t bp, bt, bn;
            uint32_t cp, ct, cn;

            sscanf(line, "f %u/%u/%u %u/%u/%u %u/%u/%u", &ap, &at, &an, &bp, &bt, &bn, &cp, &ct, &cn);

            vertices = realloc(vertices, sizeof(float) * 8 * (vertices_n + 3));
            indices = realloc(indices, sizeof(uint32_t) * (indices_n + 3));

            // clang-format off
            memcpy(vertices + (vertices_n + 0) * 8 + 0, positions   + (ap - 1) * 3, sizeof(float) * 3);
            memcpy(vertices + (vertices_n + 0) * 8 + 3, normals     + (an - 1) * 3, sizeof(float) * 3);
            memcpy(vertices + (vertices_n + 0) * 8 + 6, uvs         + (at - 1) * 2, sizeof(float) * 2);
            indices[indices_n + 0] = vertices_n + 0;

            memcpy(vertices + (vertices_n + 1) * 8 + 0, positions   + (bp - 1) * 3, sizeof(float) * 3);
            memcpy(vertices + (vertices_n + 1) * 8 + 3, normals     + (bn - 1) * 3, sizeof(float) * 3);
            memcpy(vertices + (vertices_n + 1) * 8 + 6, uvs         + (bt - 1) * 2, sizeof(float) * 2);
            indices[indices_n + 1] = vertices_n + 1;

            memcpy(vertices + (vertices_n + 2) * 8 + 0, positions   + (cp - 1) * 3, sizeof(float) * 3);
            memcpy(vertices + (vertices_n + 2) * 8 + 3, normals     + (cn - 1) * 3, sizeof(float) * 3);
            memcpy(vertices + (vertices_n + 2) * 8 + 6, uvs         + (ct - 1) * 2, sizeof(float) * 2);
            indices[indices_n + 2] = vertices_n + 2;
            // clang-format on

            vertices_n += 3;
            indices_n += 3;
        }
    }

    if (*submodel != NULL)
        submodel = finalise_submodel(*submodel);

    free(line);
    free(positions);
    free(normals);
    free(uvs);

    // Model

    glGenVertexArrays(1, &model->vao);
    glBindVertexArray(model->vao);

    glGenBuffers(1, &model->vbo);
    glBindBuffer(GL_ARRAY_BUFFER, model->vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 8 * vertices_n, vertices, GL_STATIC_DRAW);

    // Positions
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 8, (void*)0);

    // Normals
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 8, (void*)(sizeof(float) * 3));

    // UVs
    glEnableVertexAttribArray(2);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 8, (void*)(sizeof(float) * 6));

    glGenBuffers(1, &model->ebo);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, model->ebo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int) * indices_n, indices, GL_STATIC_DRAW);

    free(vertices);
    free(indices);

    model->count = indices_n;

    // Bounding Box

    glGenVertexArrays(1, &model->bb_vao);
    glBindVertexArray(model->bb_vao);

    glGenBuffers(1, &model->bb_vbo);
    glBindBuffer(GL_ARRAY_BUFFER, model->bb_vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 3 * bb_vertices_n, bb_vertices, GL_STATIC_DRAW);

    // Positions
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 3, (void*)0);

    glGenBuffers(1, &model->bb_ebo);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, model->bb_ebo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int) * bb_indices_n, bb_indices, GL_STATIC_DRAW);

    free(bb_vertices);
    free(bb_indices);
}

void draw_model(model_t* model) {
    glBindVertexArray(model->vao);
    glDrawElements(GL_TRIANGLES, model->count, GL_UNSIGNED_INT, (void*)0);
}

void free_model(model_t* model) {
    glDeleteVertexArrays(1, &model->vao);
    glDeleteBuffers(1, &model->ebo);
    glDeleteBuffers(1, &model->vbo);

    glDeleteVertexArrays(1, &model->bb_vao);
    glDeleteBuffers(1, &model->bb_ebo);
    glDeleteBuffers(1, &model->bb_vbo);

    submodel_t *submodel = model->root, *next;
    while (submodel != NULL) {
        next = submodel->child;

        free(submodel);
        submodel = next;
    }
}
