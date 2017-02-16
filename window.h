#ifndef WINDOW_H
#define WINDOW_H

#include <stdbool.h>
#include <stdint.h>

typedef struct Window_ {
	struct SDL_Window *win_handle;
	struct SDL_GLContext *context;
	uint32_t flags;
	uint32_t width;
	uint32_t height;
	bool should_quit;
} Window;

void window_create(Window *window);
void window_destroy(Window *window);

#endif
