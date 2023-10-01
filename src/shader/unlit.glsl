@ctype mat4 HMM_Mat4

@vs vs
uniform vs_params {
    mat4 mvp;
};

in vec3 position;
in vec3 normal0;
in vec2 uv0;

out vec3 normal;
out vec2 uv;

void main() {
    gl_Position = mvp * vec4(position, 1.0);
    normal = normal0;
    uv = uv0;
}
@end

@fs fs

in vec3 normal;
in vec2 uv;

out vec4 frag_color;

void main() {
    frag_color = vec4(normal.x, normal.y, uv.x, 1.0f);
}
@end

@program unlit vs fs
