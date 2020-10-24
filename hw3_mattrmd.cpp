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
void push_points(point_queue&);

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

    #pragma omp parallel
    {
        // while(pq.size())
            push_points(pq);
    }


    // printf("max: %f\nleft: %f\nright: %f\n", pq.top().t_max, pq.top().f_c, pq.top().f_d);

    return 0;
}

bool compare(Point a, Point b) {
    return (a.t_max < b.t_max);
}



void push_points(point_queue& pq) {
    Point temp;

    #pragma omp critical priority_queue
    {
        temp = pq.top();
        pq.pop();

        // Don't need to keep going if smaller
        if (temp.a_max <= current_max)
            return;
    }

    double new_max = temp.compute_max();

    // Also need to check to see if we have reached the cutoff point
    if (new_max <= (current_max + epsilon))
        return;

    Point *output = (Point *)malloc(sizeof(Point) * 2);

    // WARN computes function in here. Might not be a good idea
    temp.new_points(new_max, output);

    #pragma omp critical priority_queue
    {
        // Makes sure that the value is even worth looking at
        if(output[0].t_max > temp.a_max)
        {
            pq.push(output[0]);
            printf("%d\n", output[0].t_max);
        }

        if(output[1].t_max > temp.a_max)
        {
            pq.push(output[1]);
            printf("%d\n", output[1].t_max);
        }
    }

    free(output);
}