#include <stdlib.h>
#include <stdio.h>
#include <queue>
#include "f.h"

using namespace std;

// typedef queue<Point> point_queue;

class Point
{
    public:
        double target;
        double t_max;
        double a_max;
        double f_c;
        double f_d;
        double c;
        double d;
        double s;

        Point(double, double, double, double, double);
        Point(double, double, double, double, double, double);
        void new_points(queue<Point> &, double, double);
        double compute_max();
};

inline Point::Point(double c, double d, double f_c, double f_d, double s) : c(c), d(d), s(s), f_c(f_c), f_d(f_d)
{
    // The point that will be calculated
    target = (c + d) / 2;
    // Compute the theoretical max of the function between two points
    t_max = (f_c + f_d + s * (d - c)) / 2;
}

inline Point::Point(double c, double d, double f_c, double f_d, double s, double t_max) : c(c), d(d), s(s), f_c(f_c), f_d(f_d), t_max(t_max)
{
    // The point that will be calculated
    target = (c + d) / 2;
}

inline double Point::compute_max() {
    return a_max = f(target);
}

void Point::new_points(queue<Point> &pq, double local_max, double current_max)
{
    double t_max = (f_c + local_max + s * (target - c)) / 2;
    // ToDo: See if precomputing to remove from queue speeds up execution
    // Left point
    if (t_max > current_max)
        pq.push(Point(c, target, f_c, local_max, s, t_max));

    t_max = (local_max + f_d + s * (d - target)) / 2;
    // Right point
    if (t_max > current_max)
        pq.push(Point(target, d, local_max, f_d, s, t_max));
}
