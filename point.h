#include <stdlib.h>
#include <stdio.h>
#include <queue>
#include "f.h"
#include <omp.h>

using namespace std;

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
        Point();
        void new_points(queue<Point> &);
        double compute_f();
};

inline Point::Point(double c, double d, double f_c, double f_d, double s) : c(c), d(d), s(s), f_c(f_c), f_d(f_d)
{
    // The point that will be calculated
    target = (c + d) / 2;
    // Compute the theoretical max of the function between two points
    t_max = (f_c + f_d + s * (d - c)) / 2;
}

inline double Point::compute_f() {
    return a_max = f(target);
}

void Point::new_points(queue<Point> &pq)
{
    #pragma omp critical(q)
    {
        // left point
        pq.push(Point(c, target, f_c, a_max, s));

        // right point
        pq.push(Point(target, d, a_max, f_d, s));
    }
}

inline Point::Point()
{}