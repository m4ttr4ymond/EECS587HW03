#include <stdio.h>
#include <omp.h>
#include <queue>
#include "point.h"
#include "f.h"
#include <float.h>
#include <chrono>

using namespace std;

// may need to make the priority queue copy by reference to make it faster
typedef queue<Point> point_queue;

inline void gen_model();
void push_points(queue<Point> &, Point *);

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
            printf("%d\n", pq.size());
        }

        free(temp);
    }

    chrono::steady_clock::time_point end = chrono::steady_clock::now();
    chrono::duration<double> time_span = chrono::duration_cast<chrono::duration<double>>(end - start);

    printf("Value: %f\n", current_max);
    printf("Time: %f seconds\n", time_span.count());

    return 0;
}

void push_points(queue<Point> &pq, Point *temp)
{
    *temp = pq.front();
    pq.pop();

    current_max = max(current_max, max(temp->f_c, temp->f_d));
    
    if(temp->t_max < (current_max+epsilon))
        return;

    temp->compute_f();
    temp->new_points(pq);
}