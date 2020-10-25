#include <stdio.h>
#include <omp.h>
#include <queue>
#include "point.h"
#include "f.h"
#include <float.h>
#include <chrono>

using namespace std;

bool compare(Point, Point);
// may need to make the priority queue copy by reference to make it faster
typedef queue<Point> point_queue;

inline void gen_model();
void push_points(queue<Point> &, Point *);

// printf("here\n");
/*
    double a = 1.0;
    double b = 100.0;
    double epsilon = 1e-6;
    double s = 12.0;
*/

double current_max = -DBL_MAX;
double last_max = 0;

int main(void) {
    queue<Point> pq;
    int max = 0;

    #pragma omp single
    {
        pq.push(Point(a, b, f(a), f(b), s));
    }

    #pragma omp barrier

    chrono::steady_clock::time_point start = chrono::steady_clock::now();

    #pragma omp parallel
    {
        Point *temp = (Point *)malloc(sizeof(Point));
        while (pq.size() > 0)
        {
            push_points(pq, temp);
            // printf("%d\n", pq.size());
            if (pq.size() > max)
                max = pq.size();
        }

        printf("max value: %d\n", max);
        free(temp);
    }

    chrono::steady_clock::time_point end = chrono::steady_clock::now();
    chrono::duration<double> time_span = chrono::duration_cast<chrono::duration<double>>(end - start);

    printf("Time: %f seconds\n", time_span.count());

    return 0;
}

bool compare(Point a, Point b) {
    return (a.t_max < b.t_max);
}

void push_points(queue<Point> &pq, Point *temp)
{
    // Get values from the queue
    *temp = pq.front();
    pq.pop();

    // Don't need to keep going if smaller
    if (temp->t_max <= current_max) return;

    double new_max = temp->compute_max();

    // Also need to check to see if we have reached the cutoff point
    if (new_max > current_max)
    {
        last_max = current_max;
        current_max = new_max;
        // ToDo maybe check to see if it should exit to skip extra computations
        if (current_max - last_max < epsilon) return;
        // probably not worth the extra cycles
    }


    temp->new_points(pq, new_max, current_max);

    // // Makes sure that the value is even worth looking at
    // if(output[0].t_max > current_max)
    //     pq.push(output[0]);

    // if(output[1].t_max > current_max)
    //     pq.push(output[1]);
}