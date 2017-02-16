#if 0
time clang --std=c99 -Werror -Wall -Wpedantic \
			   main.c shader.c window.c       \
			   -lGL -lGLEW -lSDL2             \
			   -o a.out &&                    \
			   ./a.out
exit
#endif

#include <GL/glew.h>
#include "shader.h"
#include "timer.h"
#include "window.h"

#include <time.h>
#include <GL/gl.h>
#include <SDL2/SDL.h>

static float verts[] = {
	-0.5f, -0.5f,
	 0.f,   0.5f,
	 0.5f, -0.5f,
};

typedef struct Vec2 {
	float x;
	float y;
} vec2;

typedef struct Vec3 {
	float x;
	float y;
	float z;
} vec3;


void handle_sdl_window_events(Window *window, SDL_Event *event) {
	switch(event->window.event) {
		case SDL_WINDOWEVENT_SIZE_CHANGED:
			window->width = event->window.data1;
			window->height = event->window.data1;
			glViewport(0, 0, window->width, window->height);
			break;
	}
}

void handle_sdl_events(Window *window) {
	SDL_Event event;

	while(SDL_PollEvent(&event)) {
		switch(event.type) {
			case SDL_QUIT:
				window->should_quit = true;
				break;
			case SDL_WINDOWEVENT:
				handle_sdl_window_events(window, &event);
				break;
		}
	}
}

typedef struct {
	vec2 pos;
	vec2 scale;
	vec3 colour;
} Data;

struct GL {
	GLuint shader_program;

	struct Attribs {
		GLint verts;
		GLint pos;
		GLint scale;
		GLint colour;
	} attribs;

	struct Uniforms {
		GLint resolution;
	} uniforms;

	struct Buffers {
		GLuint vao;
		GLuint verts;
		GLuint data;
	} buffers;
} gl;


void gl_setup(void) {
	GLuint shader_vert = shader_create(GL_VERTEX_SHADER);
	GLuint shader_frag = shader_create(GL_FRAGMENT_SHADER);

	/* Program compilation */
	gl.shader_program = glCreateProgram();
	glAttachShader(gl.shader_program, shader_vert);
	glAttachShader(gl.shader_program, shader_frag);
	glLinkProgram(gl.shader_program);
	glUseProgram(gl.shader_program);

	glDeleteShader(shader_vert);
	glDeleteShader(shader_frag);

	/* Buffer setup */
	glGenVertexArrays(1, &gl.buffers.vao);
	glGenBuffers(1, &gl.buffers.verts);
	glGenBuffers(1, &gl.buffers.data);

	glBindVertexArray(gl.buffers.vao);
	gl.attribs.verts = glGetAttribLocation(gl.shader_program, "verts");
	gl.attribs.pos = glGetAttribLocation(gl.shader_program, "pos");
	gl.attribs.scale = glGetAttribLocation(gl.shader_program, "scale");
	gl.attribs.colour = glGetAttribLocation(gl.shader_program, "colour");

	glEnableVertexAttribArray(gl.attribs.verts);
	glEnableVertexAttribArray(gl.attribs.pos);
	glEnableVertexAttribArray(gl.attribs.scale);
	glEnableVertexAttribArray(gl.attribs.colour);
	glVertexAttribDivisor(gl.attribs.verts, 0);
	glVertexAttribDivisor(gl.attribs.pos, 1);
	glVertexAttribDivisor(gl.attribs.scale, 1);
	glVertexAttribDivisor(gl.attribs.colour, 1);

	glBindBuffer(GL_ARRAY_BUFFER, gl.buffers.verts);
	glVertexAttribPointer(gl.attribs.verts, 2, GL_FLOAT, GL_FALSE,
			2 * sizeof(GL_FLOAT), 0);

	glBindBuffer(GL_ARRAY_BUFFER, gl.buffers.data);
	glVertexAttribPointer(gl.attribs.pos, 2, GL_FLOAT, GL_FALSE,
			sizeof(Data), 0);
	glVertexAttribPointer(gl.attribs.scale, 2, GL_FLOAT, GL_FALSE,
			sizeof(Data), (GLvoid*)(2 * sizeof(GLfloat)));
	glVertexAttribPointer(gl.attribs.colour, 3, GL_FLOAT, GL_FALSE,
			sizeof(Data), (GLvoid*)(4 * sizeof(GLfloat)));

	gl.uniforms.resolution =
		glGetUniformLocation(gl.shader_program, "resolution");

	glBindBuffer(GL_ARRAY_BUFFER, gl.buffers.verts);
	glBufferData(GL_ARRAY_BUFFER, sizeof(verts), verts, GL_DYNAMIC_DRAW);
}

enum UpdateType {
	UPDATE_DATA,
	UPDATE_SUBDATA,
	UPDATE_MAP
} myenum;

Data *data;
size_t data_count = 300000;

void update_data_buffer(enum UpdateType type) {
	float *p_data;
	switch(type) {
		case UPDATE_DATA:
			data[0].pos.x++;
			glBufferData(GL_ARRAY_BUFFER, sizeof(Data) * data_count, data, GL_DYNAMIC_DRAW);
			break;
		case UPDATE_SUBDATA:
			data[0].pos.x++;
			glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(Data) * data_count, data);

		case UPDATE_MAP:
			p_data = glMapBuffer(GL_ARRAY_BUFFER, GL_WRITE_ONLY);
			p_data[0]++;
			glUnmapBuffer(GL_ARRAY_BUFFER);
			break;
	}
}

int main()
{
	char time_str[128];

	Window window = {
		.flags = SDL_WINDOW_RESIZABLE,
		.width = 640,
		.height = 480,
	};

	window_create(&window);
	gl_setup();
	data = malloc(sizeof(Data) * data_count);

	for(size_t i = 0; i < data_count; ++i) {
		data[i].pos.x = i * 220;
		data[i].pos.y = 0;
		data[i].scale.x = 200;
		data[i].scale.y = 200;
		data[i].colour.x = 1;
		data[i].colour.y = 1;
		data[i].colour.z = 1;
	}

	glBindBuffer(GL_ARRAY_BUFFER, gl.buffers.data);
	glBufferData(GL_ARRAY_BUFFER, sizeof(Data) * data_count, data, GL_DYNAMIC_DRAW);

	while(!window.should_quit) {
		uint64_t time_start;
		uint64_t time_end;

		glClearColor(0.f, 0.f, 0.f, 1.f);
		glClear(GL_COLOR_BUFFER_BIT);
		glUniform2f(gl.uniforms.resolution, window.width, window.height);
		glViewport(0, 0, window.width, window.height);

		time_start = get_time_ms();
		glBindBuffer(GL_ARRAY_BUFFER, gl.buffers.data);

		update_data_buffer(UPDATE_MAP);

		time_end = get_time_ms();
		glDrawArraysInstanced(GL_TRIANGLES, 0, 3, data_count);

		handle_sdl_events(&window);
		SDL_GL_SwapWindow(window.win_handle);

		sprintf(time_str, "%" PRIu64, time_end - time_start);
		SDL_SetWindowTitle(window.win_handle, time_str);
	}

	window_destroy(&window);
	SDL_Quit();

	return 0;
}
