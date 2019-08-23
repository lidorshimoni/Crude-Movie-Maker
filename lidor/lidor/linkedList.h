#pragma once

#ifndef LINKEDLISTH
#define LINKEDLISTH

#include "Frame.h"

struct Link
{
	frame_t *frame;
	struct Link *next;
};

typedef struct Link link_t;

void addNewFrame(link_t ** head);
void removeFrame(link_t ** head);
void changeFrameIndex(link_t ** head);
void changeFrameDura(link_t ** head);
void changeAllDura(link_t ** head);
void freeAll(link_t * del);
frame_t* setFrameFromUser(link_t ** head);
void reverse(link_t** head_ref);

#endif
