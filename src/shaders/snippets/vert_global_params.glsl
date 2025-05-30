/*

This vertex shader snippet defines the input attributes, uniforms, and varyings
used in both the vertex and fragment shaders.

@in vec3 a_Position - Vertex position
@in vec3 a_Normal - Vertex normal
@in vec2 a_TexCoord - Vertex texture coordinate
@uniform mat3 u_NormalMatrix - Transforms normals to view space
@uniform mat3 u_TextureTransform - Applies texture coordinate transformations
@uniform mat4 u_ModelView - Model-view transformation matrix
@uniform mat4 u_Projection - Projection transformation matrix
@out float v_ViewDepth - Depth of the vertex in view space
@out vec2 v_TexCoord - Transformed texture coordinates for the fragment shader
@out vec3 v_Normal - Transformed normal vector in view space
@out vec3 v_ViewDir - View direction vector for lighting calculations
@out vec4 v_Position - Vertex position in view space

*/

in vec3 a_Position;
in vec3 a_Normal;
in vec2 a_TexCoord;

uniform mat3 u_NormalMatrix;
uniform mat3 u_TextureTransform;
uniform mat4 u_ModelView;
uniform mat4 u_Projection;

out float v_ViewDepth;
out vec2 v_TexCoord;
out vec3 v_Normal;
out vec3 v_ViewDir;
out vec4 v_Position;