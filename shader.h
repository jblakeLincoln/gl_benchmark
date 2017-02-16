#ifndef SHADER_H
#define SHADER_H

#include <GL/glew.h>
#include <GL/gl.h>

static const char *vert_shader =
"#version 330\n"
"in vec2 verts;\n"
"in vec2 scale;\n"
"in vec2 pos;\n"
"in vec3 colour;\n"

"out vec3 Colour;\n"
"uniform vec2 resolution;\n"
""
"void main()\n"
"{\n"
"	Colour = colour;\n"
"	vec2 res = 1.f / resolution;\n"
"	vec2 pp = vec2(pos.x, pos.y - (resolution.y / 2.f));\n"
"	vec2 p = (verts * scale + pp);\n"
"	gl_Position = vec4(p * res, 0, 1);\n"
"}\n";

static const char *frag_shader =
"#version 330\n"
"in vec3 Colour;\n"
"out vec4 out_colour;\n"
"void main()\n"
"{\n"
"	out_colour = vec4(Colour, 1);\n"
"}\n"
;

GLuint shader_create(GLenum type);

#endif
