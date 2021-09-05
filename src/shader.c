#include "shader.h"

#include <stdio.h>
#include <stdlib.h>

char *load_file(const char *filename);

void load_shader(shader_t *shader, const char *vertex_shader_path, const char *fragment_shader_path) {
    const char *const vertex_shader_source = load_file(vertex_shader_path);
    const char *const fragment_shader_source = load_file(fragment_shader_path);

    GLuint vertex_shader, fragment_shader;
    GLint result = GL_FALSE;
    GLint info_log_length;
    GLchar *info_buffer;

    vertex_shader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertex_shader, 1, &vertex_shader_source, NULL);
    glCompileShader(vertex_shader);

    fragment_shader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragment_shader, 1, &fragment_shader_source, NULL);
    glCompileShader(fragment_shader);

    shader->program = glCreateProgram();
    glAttachShader(shader->program, vertex_shader);
    glAttachShader(shader->program, fragment_shader);
    glLinkProgram(shader->program);

    glGetShaderiv(vertex_shader, GL_COMPILE_STATUS, &result);
    glGetShaderiv(vertex_shader, GL_INFO_LOG_LENGTH, &info_log_length);

    if (info_log_length > 0) {
        info_buffer = (char *)malloc(info_log_length + 1);
        glGetShaderInfoLog(vertex_shader, info_log_length, NULL, info_buffer);

        fprintf(stderr, "[Vertex Shader Error]\n%s\n", info_buffer);
        free(info_buffer);
    }

    glGetShaderiv(fragment_shader, GL_COMPILE_STATUS, &result);
    glGetShaderiv(fragment_shader, GL_INFO_LOG_LENGTH, &info_log_length);

    if (info_log_length > 0) {
        info_buffer = (char *)malloc(info_log_length + 1);
        glGetShaderInfoLog(fragment_shader, info_log_length, NULL, info_buffer);

        fprintf(stderr, "[Fragment Shader Error]\n%s\n", info_buffer);
        free(info_buffer);
    }

    glGetProgramiv(shader->program, GL_LINK_STATUS, &result);
    glGetProgramiv(shader->program, GL_INFO_LOG_LENGTH, &info_log_length);

    if (info_log_length > 0) {
        info_buffer = (char *)malloc(info_log_length + 1);
        glGetProgramInfoLog(shader->program, info_log_length, NULL, info_buffer);

        fprintf(stderr, "%s\n", info_buffer);
        free(info_buffer);
    }

    free((void *)vertex_shader_source);
    free((void *)fragment_shader_source);
}

char *load_file(const char *filename) {
    long int len;
    char *buffer = NULL;

    FILE *fp;
    fp = fopen(filename, "r");

    if (!fp)
        return NULL;

    fseek(fp, 0, SEEK_END);
    len = ftell(fp);
    buffer = (char *)malloc(len + 1);

    if (buffer) {
        fseek(fp, 0, SEEK_SET);
        fread(buffer, sizeof(char), len, fp);
        buffer[len] = '\0';
    }

    return buffer;
}
