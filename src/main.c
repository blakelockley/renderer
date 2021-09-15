#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "glfw.h"
#include "linmath.h"

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

#define ENGINE_INCLUDES
#include "model.h"
#include "shader.h"

GLFWwindow *window;

void init();
void deinit();

int main() {
    init();

    shader_t shader, line_shader;
    load_shader(&shader, "shaders/vertex.glsl", "shaders/fragment.glsl");
    load_shader(&line_shader, "shaders/line-vertex.glsl", "shaders/line-fragment.glsl");

    model_t object;
    load_model(&object, "assets/bulb.obj");

    char title[16];

    double time_elapsed = 0, last_second = 0;
    int frames = 0;

    while (!glfwWindowShouldClose(window)) {
        double current_time = glfwGetTime();
        double delta = current_time - time_elapsed;
        time_elapsed = current_time;

        frames++;
        if (current_time - last_second > 1.0) {
            double fps = frames / (current_time - last_second);

            sprintf(title, "FPS: %.2f", fps);
            glfwSetWindowTitle(window, title);

            frames = 0;
            last_second = current_time;
        }

        int width, height;
        glfwGetFramebufferSize(window, &width, &height);

        // Render
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        mat4x4 model, view, projection;
        mat4x4_identity(model);
        mat4x4_look_at(view, (vec3){0, 0, 10}, (vec3){0, 2, 0}, (vec3){0, 1, 0});
        mat4x4_perspective(projection, 45.0f, (float)width / (float)height, 0.1f, 100.0f);

        mat4x4 normal, temp;
        mat4x4_transpose(temp, model);
        mat4x4_invert(normal, temp);

        int i = 0;
        submodel_t *submodel = object.root;
        while (submodel != NULL) {
            glUseProgram(shader.program);

            GLint model_loc = glGetUniformLocation(shader.program, "model");
            glUniformMatrix4fv(model_loc, 1, GL_FALSE, (float *)model);

            GLint view_loc = glGetUniformLocation(shader.program, "view");
            glUniformMatrix4fv(view_loc, 1, GL_FALSE, (float *)view);

            GLint projection_loc = glGetUniformLocation(shader.program, "projection");
            glUniformMatrix4fv(projection_loc, 1, GL_FALSE, (float *)projection);

            GLint normal_loc = glGetUniformLocation(shader.program, "normal");
            glUniformMatrix4fv(normal_loc, 1, GL_FALSE, (float *)normal);

            GLint color_loc = glGetUniformLocation(shader.program, "color");
            glUniform3f(color_loc, 1.0f, 1.0f, 1.0f);

            // Draw model

            // clang-format off
            float colors[] = { 0.5f, 0.0f, 0.0f, 
                               0.0f, 0.5f, 0.0f,
                               0.0f, 0.0f, 0.5f };
            // clang-format on

            glUniform3fv(color_loc, 1, colors + i++ * 3);

            mat4x4_identity(model);
            mat4x4_translate(model, model, 0, sin(time_elapsed * i) * 0.1f, 0);
            glUniformMatrix4fv(model_loc, 1, GL_FALSE, (float *)model);

            glBindVertexArray(object.vao);
            glDrawElements(GL_TRIANGLES, submodel->count, GL_UNSIGNED_INT, (void *)(sizeof(u_int32_t) * submodel->offset));

            glUseProgram(line_shader.program);

            model_loc = glGetUniformLocation(line_shader.program, "model");
            glUniformMatrix4fv(model_loc, 1, GL_FALSE, (float *)model);

            view_loc = glGetUniformLocation(line_shader.program, "view");
            glUniformMatrix4fv(view_loc, 1, GL_FALSE, (float *)view);

            projection_loc = glGetUniformLocation(line_shader.program, "projection");
            glUniformMatrix4fv(projection_loc, 1, GL_FALSE, (float *)projection);

            color_loc = glGetUniformLocation(line_shader.program, "color");
            glUniform3f(color_loc, 1.0f, 1.0f, 1.0f);

            glBindVertexArray(object.bb_vao);

            uint32_t offset = submodel->bb_index * 25;
            glDrawElements(GL_LINES, 24, GL_UNSIGNED_INT, (void *)(sizeof(u_int32_t) * offset));

            glDisable(GL_DEPTH_TEST);
            glDrawElements(GL_POINTS, 1, GL_UNSIGNED_INT, (void *)(sizeof(u_int32_t) * (offset + 24)));
            glEnable(GL_DEPTH_TEST);

            submodel = submodel->child;
        }

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    free_model(&object);
    glDeleteProgram(shader.program);

    deinit();
    return EXIT_SUCCESS;
}

void error_callback(int error, const char *description) {
    fprintf(stderr, "Error: %s\n", description);
}

void key_callback(GLFWwindow *window, int key, int scancode, int action, int mods) {
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
        glfwSetWindowShouldClose(window, GLFW_TRUE);
}

void init() {
    glfwSetErrorCallback(error_callback);
    if (!glfwInit())
        exit(EXIT_FAILURE);

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    window = glfwCreateWindow(800, 600, "GLFW Window", NULL, NULL);
    if (!window) {
        glfwTerminate();
        exit(EXIT_FAILURE);
    }

    glfwMakeContextCurrent(window);
    glfwSwapInterval(1);

    glfwSetKeyCallback(window, key_callback);

    // OpenGL setup

    glEnable(GL_CULL_FACE);
    glEnable(GL_DEPTH_TEST);

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    glPointSize(8.0f);
    glClearColor(0.5f, 0.5f, 0.5f, 1.0f);
}

void deinit() {
    glfwDestroyWindow(window);
    glfwTerminate();
}
