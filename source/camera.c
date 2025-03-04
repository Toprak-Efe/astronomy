#include "camera.h"

mat4 axis_remapping_matrix = {
    {0.0f, 0.0f,-1.0f, 0.0f},
    {-1.0f, 0.0f, 0.0f, 0.0f},
    {0.0f,-1.0f, 0.0f, 0.0f},
    {0.0f, 0.0f, 0.0f, 1.0f}
};

void camera_model_transform(camera_t *camera, mat4 out) {
    mat4 m_rotation, m_transform;
    vec3 v_rotation = {0.0f, -glm_rad(camera->direction.dec), glm_rad(camera->direction.ra)};
    vec4 v_transform = {camera->position.x, camera->position.y, camera->position.z, 0.0f};
    glm_euler_xyz(v_rotation, m_rotation);
    glm_translate_make(m_transform, v_transform);
    glm_mat4_mul(m_transform, m_rotation, out);
}

void camera_view_transform(camera_t *camera, mat4 out) {
    mat4 model_transform;
    camera_model_transform(camera, model_transform);
    glm_mat4_inv(model_transform, out);
}

void camera_move(camera_t *camera, vec3 delta) {
    mat4 transform;
    camera_model_transform(camera, transform);
    vec4 delta_p = {delta[0], delta[1], delta[2], 0.0f};
    vec4 output = {0.0f, 0.0f, 0.0f, 1.0f};
    glm_mat4_mulv(transform, delta_p, output);
    camera->position.x += output[0];
    camera->position.y += output[1];
    camera->position.z += output[2];
}

void camera_init(camera_t *camera) {
    camera->arclength = 90.0f;
    glm_perspective(glm_rad(camera->arclength), 16.0f/9.0f, 0.01f, 1000.0f, camera->projection);
    camera->direction.r = 1.0f;
    camera->direction.ra = 0.0f;
    camera->direction.dec = 0.0f;
    camera->position.x = 0.0f;
    camera->position.y = 0.0f;
    camera->position.z = 0.0f;
}