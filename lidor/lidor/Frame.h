#pragma once

#ifndef FRAME_H
#define FRAME_H

#include <stdio.h>

struct Frame
{
	char  *name;
	unsigned int duration;
	char *path;
};
typedef struct Frame frame_t;

#define MAX_PATH_SIZE (256)
#define MAX_NAME_SIZE (50)

#endif //FRAME_H
