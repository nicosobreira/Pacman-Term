#include "vector.h"

/*
        -> = 1, 0
        |  = 0, 1
        v
        <- = -1, 0
        ^ = 0, -1
        |
*/

/* I had to change the formula a little bit
 * because the y coordinates are flipped
 * Original:
 *	temp_y = vec->y;
 *	vec->y = -vec->x;
 *	vec->x = temp_y;
 */
void rotateVectorClock(Vector *vec) {
    int temp_y = vec->y;
    vec->y = vec->x;
    vec->x = -temp_y;
}

/* Still don't work */
void rotateVectorCounterClock(Vector *vec) {
    int temp_x = vec->x;
    vec->x = vec->y;
    vec->y = -temp_x;
}

void rotateVector180(Vector *vec) {
    vec->x = -vec->x;
    vec->y = -vec->y;
}

/** Sum two vectors and stores the new value in the first one */
void sumVectors(Vector *vec1, Vector *vec2) {
    vec1->x += vec2->x;
    vec1->y += vec2->y;
}
