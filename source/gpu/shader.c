// Copyright (c) 2025 Elias Engelbert Plank

/// Compiles the specified shader type from source
static u32 shader_compile(BinaryBuffer const *source, u32 const type) {
    u32 const program = glCreateShader(type);
    const GLchar *shader_source = source->data;
    glShaderSource(program, 1, &shader_source, NULL);
    glCompileShader(program);

    s32 success;
    glGetShaderiv(program, GL_COMPILE_STATUS, &success);
    if (!success) {
        s32 info_length;
        glGetShaderiv(program, GL_INFO_LOG_LENGTH, &info_length);

        BinaryBuffer failure_info;
        failure_info.data = (char *) malloc((size_t) info_length);
        failure_info.size = (u32) info_length;
        glGetShaderInfoLog(program, info_length, &info_length, failure_info.data);
        glDeleteProgram(program);
        fprintf(stderr, "compilation failed: %s\n", failure_info.data);
        free(failure_info.data);
        failure_info.size = 0;
        return 0;
    }
    return program;
}

/// Creates a shader from the given vertex and fragment shader files
static b32 shader_create(Shader *self, const char *vertex, const char *fragment) {
    BinaryBuffer vertex_source, fragment_source;
    if (!file_read(&vertex_source, vertex) || !file_read(&fragment_source, fragment)) {
        free(vertex_source.data);
        free(fragment_source.data);
        vertex_source.size = 0;
        fragment_source.size = 0;
        return false;
    }

    u32 const vertex_program = shader_compile(&vertex_source, GL_VERTEX_SHADER);
    u32 const fragment_program = shader_compile(&fragment_source, GL_FRAGMENT_SHADER);
    free(vertex_source.data);
    free(fragment_source.data);
    vertex_source.size = 0;
    fragment_source.size = 0;

    u32 const handle = glCreateProgram();
    glAttachShader(handle, vertex_program);
    glAttachShader(handle, fragment_program);
    glLinkProgram(handle);

    s32 link_success;
    glGetProgramiv(handle, GL_LINK_STATUS, &link_success);
    if (!link_success) {
        s32 info_length;
        glGetProgramiv(handle, GL_INFO_LOG_LENGTH, &info_length);

        BinaryBuffer failure_info;
        failure_info.data = (char *) malloc(info_length);
        failure_info.size = (u32) info_length;
        glGetProgramInfoLog(handle, info_length, NULL, failure_info.data);
        glDeleteProgram(handle);
        glDeleteProgram(vertex_program);
        glDeleteProgram(fragment_program);
        fprintf(stderr, "shader linking failed: %.*s\n", failure_info.size, failure_info.data);
        free(failure_info.data);
        failure_info.size = 0;
        return false;
    }

    s32 uniform_count;
    glGetProgramiv(handle, GL_ACTIVE_UNIFORMS, &uniform_count);

    s32 uniform_length;
    glGetProgramiv(handle, GL_ACTIVE_UNIFORM_MAX_LENGTH, &uniform_length);

    if (uniform_count > 0 && uniform_length > 0) {
        BinaryBuffer uniform_name;
        uniform_name.data = (char *) malloc(uniform_length);
        uniform_name.size = (u32) uniform_length;

        for (u32 i = 0; i < (u32) uniform_count; i++) {
            s32 length, size;
            u32 data_type;

            glGetActiveUniform(handle, i, uniform_length, &length, &size, &data_type, uniform_name.data);
            s32 const location = glGetUniformLocation(handle, uniform_name.data);
            fprintf(stderr, "shader [%s, %s] uniform %.*s has location %d\n", vertex, fragment, uniform_name.size,
                    uniform_name.data, location);
        }
        free(uniform_name.data);
        uniform_name.size = 0;
    }
    self->handle = handle;
    return true;
}

/// Destroys the specified shader
static void shader_destroy(Shader const *self) {
    glDeleteProgram(self->handle);
}

/// Sets a sampler2D (texture) uniform
static void shader_uniform_sampler(Shader const *self, const char *name, u32 const slot) {
    shader_uniform_s32(self, name, (s32) slot);
}

/// Sets an integer (s32) uniform
static void shader_uniform_s32(Shader const *self, const char *name, s32 const value) {
    glUseProgram(self->handle);
    glUniform1i(glGetUniformLocation(self->handle, name), value);
}

/// Sets an 2D integer (S32Vector2) uniform
static void shader_uniform_s32vec2(Shader const *self, const char *name, S32Vector2 const *value) {
    glUseProgram(self->handle);
    glUniform2i(glGetUniformLocation(self->handle, name), value->x, value->y);
}

/// Sets an 3D integer (S32Vector3) uniform
static void shader_uniform_s32vec3(Shader const *self, const char *name, S32Vector3 const *value) {
    glUseProgram(self->handle);
    glUniform3i(glGetUniformLocation(self->handle, name), value->x, value->y, value->z);
}

/// Sets an 4D integer (S32Vector4) uniform
static void shader_uniform_s32vec4(Shader const *self, const char *name, S32Vector4 const *value) {
    glUseProgram(self->handle);
    glUniform4i(glGetUniformLocation(self->handle, name), value->x, value->y, value->z, value->w);
}

/// Sets a float (f32) uniform
static void shader_uniform_f32(Shader const *self, const char *name, f32 const value) {
    glUseProgram(self->handle);
    glUniform1f(glGetUniformLocation(self->handle, name), value);
}

/// Sets an 2D float (f32vec2_t) uniform
static void shader_uniform_f32vec2(Shader const *self, const char *name, F32Vector2 const *value) {
    glUseProgram(self->handle);
    glUniform2f(glGetUniformLocation(self->handle, name), value->x, value->y);
}

static void shader_uniform_f32vec3(Shader const *self, const char *name, F32Vector3 const *value) {
    glUseProgram(self->handle);
    glUniform3f(glGetUniformLocation(self->handle, name), value->x, value->y, value->z);
}

/// Sets an 4D float (f32vec4_t) uniform
static void shader_uniform_f32vec4(Shader const *self, const char *name, F32Vector4 const *value) {
    glUseProgram(self->handle);
    glUniform4f(glGetUniformLocation(self->handle, name), value->x, value->y, value->z, value->w);
}

/// Sets an 4x4 matrix (f32mat4_t) uniform
static void shader_uniform_f32mat4(Shader const *self, const char *name, F32Mat4 const *value) {
    glUseProgram(self->handle);
    glUniformMatrix4fv(glGetUniformLocation(self->handle, name), 1, GL_FALSE, &value->value[0].x);
}

/// Binds the specified shader
static void shader_bind(Shader const *self) {
    glUseProgram(self->handle);
}

/// Unbinds the currently bound shader
static void shader_unbind(void) {
    glUseProgram(0);
}
