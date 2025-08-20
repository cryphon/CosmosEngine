layout(std140) uniform PerView {
    mat4 uView;
    mat4 uProjection;

    vec4 uViewPos;     // xyz used, w padding
    vec4 uLightPos;    // xyz used, w padding
    vec4 uLightColor;  // rgb used, a padding

    // pack scalars in a vec4 to keep std140 happy
    vec4 uParams; // x = reflectivity, y = alpha, z/w unused
};
