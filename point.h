#include <stdlib.h>
#include <stdio.h>
#include "f.h"

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
        void new_points(double, Point*);
        double compute_max();
};

inline Point::Point(double c, double d, double f_c, double f_d, double s) : c(c), d(d), s(s), f_c(f_c), f_d(f_d)
{
    // The point that will be calculated
    target = (c + d) / 2;
    // Compute the theoretical max of the function between two points
    t_max = (f_c + f_d + s * (d - c)) / 2;
}

inline double Point::compute_max() {
    return a_max = f(target);
}

void Point::new_points(double computed_max, Point *out)
{   
    // Left point
    out[0] = Point(c, target, f_c, computed_max, s);

    // Right point
    out[1] = Point(target, d, computed_max, f_d, s);
}
