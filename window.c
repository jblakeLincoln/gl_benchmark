#include <SDL2/SDL.h>
#include <GL/glew.h>

#include "window.h"

void window_create(Window *window) {
	if(SDL_Init(SDL_INIT_VIDEO) < 0) {
		printf("Failed SDL init\n");
		exit(1);
	}

	window->flags |= SDL_WINDOW_OPENGL;

	SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);
	SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
	SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);
	SDL_GL_SetAttribute(SDL_GL_MULTISAMPLEBUFFERS, 1);
	SDL_GL_SetAttribute(SDL_GL_MULTISAMPLESAMPLES, 4);

	window->win_handle = SDL_CreateWindow("Title", SDL_WINDOWPOS_UNDEFINED,
			SDL_WINDOWPOS_UNDEFINED, window->width, window->height, window->flags);

	window->context = SDL_GL_CreateContext(window->win_handle);

	if(window->context == NULL) {
		printf("Failed GL context creation\n");
		exit(1);
	}

	glewExperimental = GL_TRUE;

	if(glewInit() != GLEW_OK) {
		printf("Failed glew init\n");
		exit(1);
	}

	SDL_GL_SetSwapInterval(1);
	window->should_quit = false;
}

void window_destroy(Window *window) {
	SDL_GL_DeleteContext(window->context);
	SDL_DestroyWindow(window->win_handle);
}

