#include "model.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "linmath.h"

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

    uint32_t* indicies = NULL;
    size_t indicies_n = 0;

    model->root = NULL;
    submodel_t* prev_submodel = NULL;
    submodel_t** submodel = &model->root;

    vec3 buffer;
    while (getline(&line, &len, file) != EOF) {
        if (strncmp(line, "v ", 2) == 0) {
            sscanf(line, "v %f %f %f", &buffer[0], &buffer[1], &buffer[2]);

            positions = realloc(positions, sizeof(vec3) * (positions_n + 1));
            memcpy(positions + positions_n * 3, buffer, sizeof(float) * 3);
            positions_n += 1;

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
            char name[256];
            sscanf(line, "o %s", name);
            printf("Object: %s\n", name);

            if (prev_submodel != NULL)
                prev_submodel->count = indicies_n - prev_submodel->offset;

            *submodel = malloc(sizeof(submodel_t));
            (*submodel)->offset = indicies_n;
            (*submodel)->child = NULL;

            prev_submodel = *submodel;
            submodel = &(*submodel)->child;

        } else if (strncmp(line, "f ", 2) == 0) {
            uint32_t ap, at, an;  // a = vertex, p = position, t = texture, n = normal
            uint32_t bp, bt, bn;
            uint32_t cp, ct, cn;

            sscanf(line, "f %u/%u/%u %u/%u/%u %u/%u/%u", &ap, &at, &an, &bp, &bt, &bn, &cp, &ct, &cn);

            vertices = realloc(vertices, sizeof(float) * 8 * (vertices_n + 3));
            indicies = realloc(indicies, sizeof(uint32_t) * (indicies_n + 3));

            // clang-format off
            memcpy(vertices + (vertices_n + 0) * 8 + 0, positions   + (ap - 1) * 3, sizeof(float) * 3);
            memcpy(vertices + (vertices_n + 0) * 8 + 3, normals     + (an - 1) * 3, sizeof(float) * 3);
            memcpy(vertices + (vertices_n + 0) * 8 + 6, uvs         + (at - 1) * 2, sizeof(float) * 2);
            indicies[indicies_n + 0] = vertices_n + 0;

            memcpy(vertices + (vertices_n + 1) * 8 + 0, positions   + (bp - 1) * 3, sizeof(float) * 3);
            memcpy(vertices + (vertices_n + 1) * 8 + 3, normals     + (bn - 1) * 3, sizeof(float) * 3);
            memcpy(vertices + (vertices_n + 1) * 8 + 6, uvs         + (bt - 1) * 2, sizeof(float) * 2);
            indicies[indicies_n + 1] = vertices_n + 1;

            memcpy(vertices + (vertices_n + 2) * 8 + 0, positions   + (cp - 1) * 3, sizeof(float) * 3);
            memcpy(vertices + (vertices_n + 2) * 8 + 3, normals     + (cn - 1) * 3, sizeof(float) * 3);
            memcpy(vertices + (vertices_n + 2) * 8 + 6, uvs         + (ct - 1) * 2, sizeof(float) * 2);
            indicies[indicies_n + 2] = vertices_n + 2;
            // clang-format on

            vertices_n += 3;
            indicies_n += 3;
        }
    }

    if (prev_submodel != NULL)
        prev_submodel->count = indicies_n - prev_submodel->offset;

    free(line);
    free(positions);
    free(normals);
    free(uvs);

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
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int) * indicies_n, indicies, GL_STATIC_DRAW);

    free(vertices);
    free(indicies);

    model->count = indicies_n;
}

void draw_model(model_t* model) {
    glBindVertexArray(model->vao);
    glDrawElements(GL_TRIANGLES, model->count, GL_UNSIGNED_INT, (void*)0);
}

void free_model(model_t* model) {
    glDeleteVertexArrays(1, &model->vao);
    glDeleteBuffers(1, &model->ebo);
    glDeleteBuffers(1, &model->vbo);

    submodel_t *submodel = model->root, *next;
    while (submodel != NULL) {
        next = submodel->child;

        free(submodel);
        submodel = next;
    }
}
