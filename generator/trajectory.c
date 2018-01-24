#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <arpa/inet.h>

#include "trajectory.h"

/* Create a trajectory structi with a total of len points. The returned pointer must be freed with
 * trajectory_free. */
trajectory_t *trajectory_create(int len) {
   
    /* allocate space for the trajectory_t struct */ 
    trajectory_t *traj = (trajectory_t*) malloc(sizeof(trajectory_t));

    /* allocate an array of points for that trajectory */
    point_t *points = (point_t*) malloc(len * sizeof(point_t));

    traj->len = len;
    traj->points = points;

    return traj;
}


/* Free a trajectory_t, including its internal array of points. */
void trajectory_free(trajectory_t *traj) {
    free(traj->points);
    free(traj);
}

/* Push a uint32 onto the given char buffer, in network (big-endian) order.
 * Returns the advanced buffer pointer. */
static char* push_u32(char* buf, uint32_t val) {
    
    uint32_t val_be = htonl(val);
    memcpy(buf, &val_be, sizeof(uint32_t));

    return buf + sizeof(uint32_t);
}

/* Push a 64-bit double onto the given char buffer, in machine-dependent order.
 * Returns the advanced buffer pointer. */
static char* push_double(char* buf, double val) {

    memcpy(buf, &val, sizeof(val));
    return buf + sizeof(val);
}

/* Serialize a trajectory_t into a char buffer for network transmission. 
 * After calling this function, out will point to the char array. This buffer
 * must later be free()ed.
 *
 * traj : pointer to trajectory_t to serialize.
 * out  : pointer to resulting char* buffer. */
size_t trajectory_serialize(trajectory_t *traj, char **out) {

    /* Allocate buffer for serialized data. Total packet size (in bytes) :
     *
     * 4 (for # points) + (2 * 8 * num points)
     * */
    size_t buf_size = 4 + (2 * 8 * traj->len);

    /* allocate space for the serialized data */
    char *buf = (char *) malloc(buf_size);
    char *ptr = buf;
    
    ptr = push_u32(ptr, traj->len);

    for (int i = 0; i < traj->len; i++) {
        ptr = push_double(ptr, traj->points[i].x);
        ptr = push_double(ptr, traj->points[i].y);
    }

    *out = buf;
    return buf_size;
}

/* Print a trajectory_t's points. Useful for debugging. */
void trajectory_print(trajectory_t *traj) {

    printf("{\n");

    for (int i = 0; i < traj->len; i++) {
       printf("\t[ %.2f,%.2f ],\n", traj->points[i].x, traj->points[i].y); 
    }
    printf("}\n");

}
