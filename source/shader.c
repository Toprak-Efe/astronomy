#include "shader.h"

shader_t *g_star_shader = NULL;
shader_t *g_vertex_shader = NULL;
shader_t *g_geometry_shader = NULL;
shader_t *g_fragment_shader = NULL;

void initialize_shaders() {
    logprint(LOG_INFO, "Initializing shaders.");
    g_star_shader = malloc(sizeof(shader_t));
    g_vertex_shader = malloc(sizeof(shader_t));
    g_geometry_shader = malloc(sizeof(shader_t));
    g_fragment_shader = malloc(sizeof(shader_t));
    if (!g_star_shader || !g_vertex_shader || ! g_geometry_shader || !g_fragment_shader) {
        logprint(LOG_ERROR, "Failed to allocate memory for shaders.");
        exit(1);
    }
    g_star_shader->u_trans_id = -1;
    g_star_shader->u_color_id = -1;
    g_vertex_shader->u_trans_id = -1;
    g_vertex_shader->u_color_id = -1;
    g_geometry_shader->u_trans_id = -1;
    g_geometry_shader->u_color_id = -1;
    g_fragment_shader->u_trans_id = -1;
    g_fragment_shader->u_color_id = -1;

    const char *vertex_shader_start = (char *)_binary__vertex_start;
    size_t vertex_shader_size = (size_t) (_binary__vertex_end - _binary__vertex_start);
    const char *geometry_shader_start = (char *)_binary__geometry_start;
    size_t geometry_shader_size = (size_t) (_binary__geometry_end - _binary__geometry_start);
    const char *fragment_shader_start = (char *)_binary__fragment_start;
    size_t fragment_shader_size = (size_t) (_binary__fragment_end - _binary__fragment_start);

    char *vertex_source_str = malloc(sizeof(char) * vertex_shader_size + 1);
    memcpy(vertex_source_str, vertex_shader_start, vertex_shader_size);
    vertex_source_str[vertex_shader_size] = '\0';

    char *geometry_source_str = malloc(sizeof(char) * geometry_shader_size + 1);
    memcpy(geometry_source_str, geometry_shader_start, geometry_shader_size);
    geometry_source_str[geometry_shader_size] = '\0';

    char *fragment_source_str = malloc(sizeof(char) * fragment_shader_size + 1);
    memcpy(fragment_source_str, fragment_shader_start, fragment_shader_size);
    fragment_source_str[fragment_shader_size] = '\0';

    GLuint vs = glCreateShader((GLenum) GL_VERTEX_SHADER);
    glShaderSource(vs, 1, (const GLchar * const *)& vertex_source_str, NULL);
    glCompileShader(vs);
    g_vertex_shader->shader_id = vs;

    GLuint gs = glCreateShader((GLenum) GL_GEOMETRY_SHADER);
    glShaderSource(gs, 1, (const GLchar * const *)& geometry_source_str, NULL);
    glCompileShader(gs);
    g_geometry_shader->shader_id = gs;

    GLuint fs = glCreateShader((GLenum) GL_FRAGMENT_SHADER);
    glShaderSource(fs, 1, (const GLchar * const *) &fragment_source_str, NULL);
    glCompileShader(fs);
    g_fragment_shader->shader_id = fs;
    
    GLint success;
    glGetShaderiv(vs, GL_COMPILE_STATUS, &success);
    if (!success) {
        char info_log[512];
        glGetShaderInfoLog(vs, sizeof(info_log), NULL, info_log);
        logprint(LOG_ERROR, "Vertex shader failed to compile: %s", info_log);
    }
    glGetShaderiv(gs, GL_COMPILE_STATUS, &success);
    if (!success) {
        char info_log[512];
        glGetShaderInfoLog(gs, sizeof(info_log), NULL, info_log);
        logprint(LOG_ERROR, "Geometry shader failed to compile: %s", info_log);
    }
    glGetShaderiv(fs, GL_COMPILE_STATUS, &success);
    if (!success) {
        char info_log[512];
        glGetShaderInfoLog(fs, sizeof(info_log), NULL, info_log);
        logprint(LOG_ERROR, "Fragment shader failed to compile: %s", info_log);
    }

    GLuint program = glCreateProgram();
    glAttachShader(program, vs);
    glAttachShader(program, gs);
    glAttachShader(program, fs);
    glLinkProgram(program);
    glGetProgramiv(program, GL_LINK_STATUS, &success);
    if (!success) {
        char info_log[512];
        glGetProgramInfoLog(program, sizeof(info_log), NULL, info_log);
        logprint(LOG_ERROR, "Program failed to link: %s", info_log);
    }
    g_star_shader->shader_id = program;
    g_star_shader->u_trans_id = glGetUniformLocation(program, "transformation");
    g_star_shader->u_color_id = glGetUniformLocation(program, "color");

    free(vertex_source_str);
    free(fragment_source_str);
    atexit(uninitialize_shaders);
}

void uninitialize_shaders() {
    logprint(LOG_INFO, "Uninitializing shaders.");
    if (g_vertex_shader) {
        if (glIsProgram(g_vertex_shader->shader_id)) {
            glDeleteProgram(g_vertex_shader->shader_id);
        }
        free(g_vertex_shader);
    }
    if (g_geometry_shader) {
        if (glIsProgram(g_geometry_shader->shader_id)) {
            glDeleteProgram(g_geometry_shader->shader_id);
        }
        free(g_geometry_shader);
    }
    if (g_fragment_shader) {
        if (glIsProgram(g_fragment_shader->shader_id)) {
            glDeleteProgram(g_fragment_shader->shader_id);
        }
        free(g_fragment_shader);
    }
    if (g_star_shader) {
        if (glIsProgram(g_star_shader->shader_id)) {
            glDeleteProgram(g_star_shader->shader_id);
        }
        free(g_star_shader);
    }
}
