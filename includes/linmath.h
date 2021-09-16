#ifndef LINMATH_H
#define LINMATH_H

#include <math.h>
#include <string.h>

typedef float vec2[2];
typedef float vec3[3];
typedef float vec4[4];

typedef float mat4x4[4][4];

// vec2

static inline void vec2_zero(vec2 r) {
    r[0] = 0.0f;
    r[1] = 0.0f;
}

static inline void vec2_set(vec2 r, float x, float y) {
    r[0] = x;
    r[1] = y;
}

static inline void vec2_copy(vec2 r, const vec2 v) {
    r[0] = v[0];
    r[1] = v[1];
}

static inline void vec2_scale(vec2 r, const vec2 a, float s) {
    r[0] = a[0] * s;
    r[1] = a[1] * s;
}

static inline void vec2_add(vec2 r, const vec2 a, const vec2 b) {
    r[0] = a[0] + b[0];
    r[1] = a[1] + b[1];
}

static inline void vec2_sub(vec2 r, const vec2 a, const vec2 b) {
    r[0] = a[0] - b[0];
    r[1] = a[1] - b[1];
}

static inline void vec2_mul(vec2 r, const vec2 a, const vec2 b) {
    r[0] = a[0] * b[0];
    r[1] = a[1] * b[1];
}

static inline void vec2_div(vec2 r, const vec2 a, const vec2 b) {
    r[0] = a[0] / b[0];
    r[1] = a[1] / b[1];
}

static inline float vec2_dot(const vec2 a, const vec2 b) {
    float r = 0.0f;
    r += a[0] * b[0];
    r += a[1] * b[1];
    return r;
}

static inline float vec2_len(const vec2 v) {
    return sqrtf(vec2_dot(v, v));
}

static inline void vec2_normalize(vec2 r, const vec2 v) {
    return vec2_scale(r, v, 1.0f / vec2_len(v));
}

static inline void vec2_reflect(vec2 r, const vec2 v, const vec2 n) {
    float p = 2.0f * vec2_dot(v, n);
    r[0] = v[0] - p * n[0];
    r[1] = v[1] - p * n[1];
}

// vec3

static inline void vec3_zero(vec3 r) {
    r[0] = 0.0f;
    r[1] = 0.0f;
    r[2] = 0.0f;
}

static inline void vec3_set(vec3 r, float x, float y, float z) {
    r[0] = x;
    r[1] = y;
    r[2] = z;
}

static inline void vec3_copy(vec3 r, const vec3 v) {
    r[0] = v[0];
    r[1] = v[1];
    r[2] = v[2];
}

static inline void vec3_from_vec4(vec3 r, const vec4 v) {
    r[0] = v[0] / v[3];
    r[1] = v[1] / v[3];
    r[2] = v[2] / v[3];
}

static inline void vec3_scale(vec3 r, const vec3 a, float s) {
    r[0] = a[0] * s;
    r[1] = a[1] * s;
    r[2] = a[2] * s;
}

static inline void vec3_add(vec3 r, const vec3 a, const vec3 b) {
    r[0] = a[0] + b[0];
    r[1] = a[1] + b[1];
    r[2] = a[2] + b[2];
}

static inline void vec3_sub(vec3 r, const vec3 a, const vec3 b) {
    r[0] = a[0] - b[0];
    r[1] = a[1] - b[1];
    r[2] = a[2] - b[2];
}

static inline void vec3_mul(vec3 r, const vec3 a, const vec3 b) {
    r[0] = a[0] * b[0];
    r[1] = a[1] * b[1];
    r[2] = a[2] * b[2];
}

static inline void vec3_div(vec3 r, const vec3 a, const vec3 b) {
    r[0] = a[0] / b[0];
    r[1] = a[1] / b[1];
    r[2] = a[2] / b[2];
}

static inline void vec3_cross(vec3 r, const vec3 a, const vec3 b) {
    r[0] = a[1] * b[2] - a[2] * b[1];
    r[1] = a[2] * b[0] - a[0] * b[2];
    r[2] = a[0] * b[1] - a[1] * b[0];
}

static inline float vec3_dot(const vec3 a, const vec3 b) {
    float r = 0.0f;
    r += a[0] * b[0];
    r += a[1] * b[1];
    r += a[2] * b[2];
    return r;
}

static inline float vec3_len(const vec3 v) {
    return sqrtf(vec3_dot(v, v));
}

static inline void vec3_normalize(vec3 r, const vec3 v) {
    return vec3_scale(r, v, 1.0f / vec3_len(v));
}

static inline void vec3_reflect(vec3 r, const vec3 v, const vec3 n) {
    float p = 2.0f * vec3_dot(v, n);
    r[0] = v[0] - p * n[0];
    r[1] = v[1] - p * n[1];
    r[2] = v[2] - p * n[2];
}

// vec4

static inline void vec4_zero(vec4 r) {
    r[0] = 0.0f;
    r[1] = 0.0f;
    r[2] = 0.0f;
    r[3] = 0.0f;
}

static inline void vec4_set(vec4 r, float x, float y, float z, float w) {
    r[0] = x;
    r[1] = y;
    r[2] = z;
    r[3] = w;
}

static inline void vec4_copy(vec4 r, const vec4 v) {
    r[0] = v[0];
    r[1] = v[1];
    r[2] = v[2];
    r[3] = v[3];
}

static inline void vec4_from_vec3(vec4 r, const vec3 v, float w) {
    r[0] = v[0];
    r[1] = v[1];
    r[2] = v[2];
    r[3] = w;
}

static inline void vec4_scale(vec4 r, const vec4 a, float s) {
    r[0] = a[0] * s;
    r[1] = a[1] * s;
    r[2] = a[2] * s;
    r[3] = a[3] * s;
}

static inline void vec4_add(vec4 r, const vec4 a, const vec4 b) {
    r[0] = a[0] + b[0];
    r[1] = a[1] + b[1];
    r[2] = a[2] + b[2];
    r[3] = a[3] + b[3];
}

static inline void vec4_sub(vec4 r, const vec4 a, const vec4 b) {
    r[0] = a[0] - b[0];
    r[1] = a[1] - b[1];
    r[2] = a[2] - b[2];
    r[3] = a[3] - b[3];
}

static inline void vec4_mul(vec4 r, const vec4 a, const vec4 b) {
    r[0] = a[0] * b[0];
    r[1] = a[1] * b[1];
    r[2] = a[2] * b[2];
}

static inline void vec4_div(vec4 r, const vec4 a, const vec4 b) {
    r[0] = a[0] / b[0];
    r[1] = a[1] / b[1];
    r[2] = a[2] / b[2];
}

static inline float vec4_dot(const vec4 a, const vec4 b) {
    float r = 0.0f;
    r += a[0] * b[0];
    r += a[1] * b[1];
    r += a[2] * b[2];
    r += a[3] * b[3];
    return r;
}

// mat4x4

static inline void mat4x4_zero(mat4x4 M) {
    for (int i = 0; i < 4; ++i)
        for (int j = 0; j < 4; ++j)
            M[i][j] = 0.0f;
}

static inline void mat4x4_identity(mat4x4 M) {
    for (int i = 0; i < 4; ++i)
        for (int j = 0; j < 4; ++j)
            M[i][j] = i == j ? 1.0f : 0.0f;
}

static inline void mat4x4_copy(mat4x4 M, const mat4x4 m) {
    memcpy(M, m, sizeof(mat4x4));
}

static inline void mat4x4_col(vec4 r, const mat4x4 m, int i) {
    for (int j = 0; j < 4; ++j)
        r[j] = m[i][j];
}

static inline void mat4x4_row(vec4 r, const mat4x4 m, int j) {
    for (int i = 0; i < 4; ++i)
        r[i] = m[i][j];
}

static inline void mat4x4_transpose(mat4x4 M, const mat4x4 m) {
    mat4x4 tmp;
    for (int i = 0; i < 4; ++i)
        for (int j = 0; j < 4; ++j)
            tmp[i][j] = m[j][i];
    mat4x4_copy(M, tmp);
}

static inline void mat4x4_add(mat4x4 M, const mat4x4 a, const mat4x4 b) {
    for (int i = 0; i < 4; ++i)
        vec4_add(M[i], a[i], b[i]);
}

static inline void mat4x4_sub(mat4x4 M, const mat4x4 a, const mat4x4 b) {
    for (int i = 0; i < 4; ++i)
        vec4_sub(M[i], a[i], b[i]);
}

static inline void mat4x4_mul_scalar(mat4x4 M, const mat4x4 a, float s) {
    for (int i = 0; i < 4; ++i)
        vec4_scale(M[i], a[i], s);
}

static inline void mat4x4_mul(mat4x4 M, const mat4x4 a, const mat4x4 b) {
    mat4x4 tmp;
    for (int j = 0; j < 4; ++j) {
        vec4 row;
        mat4x4_row(row, a, j);
        for (int i = 0; i < 4; ++i)
            tmp[i][j] = vec4_dot(row, b[i]);
    }
    mat4x4_copy(M, tmp);
}

static inline void mat4x4_mul_vec4(vec4 r, const mat4x4 m, const vec4 v) {
    vec4 tmp;
    for (int j = 0; j < 4; ++j) {
        vec4 row;
        mat4x4_row(row, m, j);
        tmp[j] = vec4_dot(row, v);
    }
    vec4_copy(r, tmp);
}

static inline void mat4x4_invert(mat4x4 M, const mat4x4 m) {
    mat4x4 tmp;

    float s[6], c[6];
    s[0] = m[0][0] * m[1][1] - m[1][0] * m[0][1];
    s[1] = m[0][0] * m[1][2] - m[1][0] * m[0][2];
    s[2] = m[0][0] * m[1][3] - m[1][0] * m[0][3];
    s[3] = m[0][1] * m[1][2] - m[1][1] * m[0][2];
    s[4] = m[0][1] * m[1][3] - m[1][1] * m[0][3];
    s[5] = m[0][2] * m[1][3] - m[1][2] * m[0][3];

    c[0] = m[2][0] * m[3][1] - m[3][0] * m[2][1];
    c[1] = m[2][0] * m[3][2] - m[3][0] * m[2][2];
    c[2] = m[2][0] * m[3][3] - m[3][0] * m[2][3];
    c[3] = m[2][1] * m[3][2] - m[3][1] * m[2][2];
    c[4] = m[2][1] * m[3][3] - m[3][1] * m[2][3];
    c[5] = m[2][2] * m[3][3] - m[3][2] * m[2][3];

    /* Assumes it is invertible */
    float idet = 1.0f / (s[0] * c[5] - s[1] * c[4] + s[2] * c[3] + s[3] * c[2] - s[4] * c[1] + s[5] * c[0]);

    tmp[0][0] = (m[1][1] * c[5] - m[1][2] * c[4] + m[1][3] * c[3]) * idet;
    tmp[0][1] = (-m[0][1] * c[5] + m[0][2] * c[4] - m[0][3] * c[3]) * idet;
    tmp[0][2] = (m[3][1] * s[5] - m[3][2] * s[4] + m[3][3] * s[3]) * idet;
    tmp[0][3] = (-m[2][1] * s[5] + m[2][2] * s[4] - m[2][3] * s[3]) * idet;

    tmp[1][0] = (-m[1][0] * c[5] + m[1][2] * c[2] - m[1][3] * c[1]) * idet;
    tmp[1][1] = (m[0][0] * c[5] - m[0][2] * c[2] + m[0][3] * c[1]) * idet;
    tmp[1][2] = (-m[3][0] * s[5] + m[3][2] * s[2] - m[3][3] * s[1]) * idet;
    tmp[1][3] = (m[2][0] * s[5] - m[2][2] * s[2] + m[2][3] * s[1]) * idet;

    tmp[2][0] = (m[1][0] * c[4] - m[1][1] * c[2] + m[1][3] * c[0]) * idet;
    tmp[2][1] = (-m[0][0] * c[4] + m[0][1] * c[2] - m[0][3] * c[0]) * idet;
    tmp[2][2] = (m[3][0] * s[4] - m[3][1] * s[2] + m[3][3] * s[0]) * idet;
    tmp[2][3] = (-m[2][0] * s[4] + m[2][1] * s[2] - m[2][3] * s[0]) * idet;

    tmp[3][0] = (-m[1][0] * c[3] + m[1][1] * c[1] - m[1][2] * c[0]) * idet;
    tmp[3][1] = (m[0][0] * c[3] - m[0][1] * c[1] + m[0][2] * c[0]) * idet;
    tmp[3][2] = (-m[3][0] * s[3] + m[3][1] * s[1] - m[3][2] * s[0]) * idet;
    tmp[3][3] = (m[2][0] * s[3] - m[2][1] * s[1] + m[2][2] * s[0]) * idet;

    mat4x4_copy(M, tmp);
}

static inline void mat4x4_scale(mat4x4 M, const mat4x4 m, float x, float y, float z) {
    vec4_scale(M[0], m[0], x);
    vec4_scale(M[1], m[1], y);
    vec4_scale(M[2], m[2], z);
    vec4_copy(M[3], m[3]);
}

static inline void mat4x4_translation(mat4x4 M, float x, float y, float z) {
    mat4x4_identity(M);
    M[3][0] = x;
    M[3][1] = y;
    M[3][2] = z;
}

static inline void mat4x4_translate(mat4x4 M, const mat4x4 m, float x, float y, float z) {
    mat4x4 tmp;
    mat4x4_translation(tmp, x, y, z);
    mat4x4_mul(M, m, tmp);
}

static inline void mat4x4_rotation_x(mat4x4 M, float angle) {
    float s = sinf(angle);
    float c = cosf(angle);

    mat4x4_identity(M);
    M[1][1] = +c;
    M[1][2] = +s;
    M[2][1] = -s;
    M[2][2] = +c;
}

static inline void mat4x4_rotate_x(mat4x4 M, const mat4x4 m, float angle) {
    mat4x4 tmp;
    mat4x4_rotation_x(tmp, angle);
    mat4x4_mul(M, m, tmp);
}

static inline void mat4x4_rotation_y(mat4x4 M, float angle) {
    float s = sinf(angle);
    float c = cosf(angle);

    mat4x4_identity(M);
    M[0][0] = +c;
    M[0][2] = -s;
    M[2][0] = +s;
    M[2][2] = +c;
}

static inline void mat4x4_rotate_y(mat4x4 M, const mat4x4 m, float angle) {
    mat4x4 tmp;
    mat4x4_rotation_y(tmp, angle);
    mat4x4_mul(M, m, tmp);
}

static inline void mat4x4_rotation_z(mat4x4 M, float angle) {
    float s = sinf(angle);
    float c = cosf(angle);

    mat4x4_identity(M);
    M[0][0] = +c;
    M[0][1] = +s;
    M[1][0] = -s;
    M[1][1] = +c;
}

static inline void mat4x4_rotate_z(mat4x4 M, const mat4x4 m, float angle) {
    mat4x4 tmp;
    mat4x4_rotation_z(tmp, angle);
    mat4x4_mul(M, m, tmp);
}

static inline void mat4x4_rotate(mat4x4 M, const mat4x4 m, float yaw, float pitch, float roll) {
    mat4x4_rotate_y(M, m, yaw);
    mat4x4_rotate_x(M, m, pitch);
    mat4x4_rotate_z(M, m, roll);
}

static inline void mat4x4_ortho(mat4x4 M, float l, float r, float b, float t, float n, float f) {
    mat4x4_identity(M);
    M[0][0] = 2.0f / (r - l);
    M[1][1] = 2.0f / (t - b);
    M[2][2] = -2.0f / (f - n);
    M[3][0] = -(r + l) / (r - l);
    M[3][1] = -(t + b) / (t - b);
    M[3][2] = -(f + n) / (f - n);
}

static inline void mat4x4_perspective(mat4x4 M, float fov, float aspect, float n, float f) {
    float a = 1.0f / tanf(fov / 2.0f);

    mat4x4_zero(M);
    M[0][0] = a / aspect;
    M[1][1] = a;
    M[2][2] = -((f + n) / (f - n));
    M[2][3] = -1.0f;
    M[3][2] = -((2.0f * f * n) / (f - n));
}

static inline void mat4x4_look_at(mat4x4 M, const vec3 eye, const vec3 center, const vec3 up) {
    vec3 forward;
    vec3_sub(forward, center, eye);
    vec3_normalize(forward, forward);

    vec3 right;
    vec3_cross(right, forward, up);
    vec3_normalize(right, right);

    vec3 t;
    vec3_cross(t, right, forward);

    mat4x4_identity(M);
    M[0][0] = right[0];
    M[1][0] = right[1];
    M[2][0] = right[2];
    M[0][1] = t[0];
    M[1][1] = t[1];
    M[2][1] = t[2];
    M[0][2] = -forward[0];
    M[1][2] = -forward[1];
    M[2][2] = -forward[2];

    mat4x4_translate(M, M, -eye[0], -eye[1], -eye[2]);
}

#endif  // LINMATH_H
