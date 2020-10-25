#include <stdio.h>
#include <omp.h>
#include <queue>
#include "point.h"
#include "f.h"
#include <float.h>

using namespace std;

bool compare(Point, Point);
// may need to make the priority queue copy by reference to make it faster
typedef priority_queue<Point, vector<Point>, decltype(&compare)> point_queue;

inline void gen_model();
void push_points(point_queue &, Point *);

// printf("here\n");
/*
    double a = 1.0;
    double b = 100.0;
    double epsilon = 1e-6;
    double s = 12.0;
*/

double current_max = -DBL_MAX;

int main(void) {
    point_queue pq(&compare);

    #pragma omp single
    {
        pq.push(Point(a, b, f(a), f(b), s));
    }

    #pragma omp barrier

    #pragma omp parallel
    {
        Point *temp = (Point *)malloc(sizeof(Point));
        while(pq.size() > 0)
        // for (size_t i = 0; i < 20; i++)
        {
            // printf("Iter %d: \n", i);
            push_points(pq, temp);
        }

        printf("%f\n", current_max);

        // TODO: the theoretical value appears to be below the actual value, which should not happen


        free(temp);
    }

    return 0;
}

bool compare(Point a, Point b) {
    return (a.t_max < b.t_max);
}



void push_points(point_queue& pq, Point *temp) {
    // Get values from the queue
    *temp = pq.top();
    pq.pop();

    // Don't need to keep going if smaller
    if (temp->t_max <= current_max) return;

    double new_max = temp->compute_max();

    // Also need to check to see if we have reached the cutoff point
    if (new_max > current_max)
        current_max = new_max;

    Point *output = (Point *)malloc(sizeof(Point) * 2);

    temp->new_points(new_max, output);

    // Makes sure that the value is even worth looking at
    if(output[0].t_max > temp->a_max)
    {
        pq.push(output[0]);
        // printf("%f\n", output[0].t_max);
    }

    if(output[1].t_max > temp->a_max)
    {
        pq.push(output[1]);
        // printf("%f\n", output[1].t_max);
    }

    free(output);
}