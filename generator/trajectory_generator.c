#include "stdlib.h"

#include "trajectory_generator.h"
#include "trajectory.h"

/* Crate a new trajectory_t and fill it with a nice curvy line. */
trajectory_t *trajectory_generate(void) {
   
    /* Create a new trajectory_t with room for 100 points. */ 
    trajectory_t *traj = trajectory_create(100);

    /* Get random coefficients for a cubic parametric polynomial */
    double ax = (double) rand() /  (double) RAND_MAX - 0.5;
    double bx = (double) rand() /  (double) RAND_MAX - 0.5;
    double cx = (double) rand() /  (double) RAND_MAX - 0.5;


    double ay = (double) rand() /  (double) RAND_MAX - 0.5;
    double by = (double) rand() /  (double) RAND_MAX - 0.5;
    double cy = (double) rand() /  (double) RAND_MAX - 0.5;

    for(int i = 0; i < 100; i++) {

        double t = i / 100.0;
        
        double x = ax*t*t*t + bx*t*t + cx*t + 0.0;
        double y = ay*t*t*t + bx*t*t + cx*t + 0.0;

        traj->points[i] = ((point_t) {x, y});
    }

    return traj;

}
