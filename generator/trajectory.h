#ifndef __TRAJECTORY_H
#define __TRAJECTORY_H

#include "stdlib.h"

typedef struct point {
    double x;
    double y;
} point_t;

typedef struct trajectory {
    int len;
    point_t *points;
} trajectory_t;

trajectory_t *trajectory_create(int len);
void trajectory_free(trajectory_t *traj);

size_t trajectory_serialize(trajectory_t *traj, char **out);

void trajectory_print(trajectory_t *traj);

#endif
