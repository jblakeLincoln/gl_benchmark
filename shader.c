#include "shader.h"

#include <stdio.h>

GLuint shader_create(GLenum type) {
	const char *type_str;
	const char *src;
	GLuint id;
	GLint status;

	switch(type) {
		case GL_VERTEX_SHADER:
			type_str = "Vertex";
			src = vert_shader;
			break;
		case GL_FRAGMENT_SHADER:
			type_str = "Fragment";
			src = frag_shader;
			break;
		case GL_COMPUTE_SHADER:
			type_str = "Compute";
			src = NULL;
			break;
		default:
			type_str = "Other";
			src = NULL;
			break;
	}

	id = glCreateShader(type);
	glShaderSource(id, 1, &src, (GLvoid*)0);
	glCompileShader(id);

	glGetShaderiv(id, GL_COMPILE_STATUS, &status);

	printf("%s shader compilation %s\n", type_str,
			status == GL_TRUE ? "successful" : "failed");

	if(status != GL_TRUE) {
		char err_buf[1024];
		glGetShaderInfoLog(id, sizeof(err_buf), NULL, err_buf);
		printf("%s\n\n", err_buf);
	}

	return id;
}


