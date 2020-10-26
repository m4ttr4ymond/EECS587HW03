#define testing_thread_number 8

#include <stdio.h>
#include <omp.h>
#include <queue>
#include "point.h"
#include "f.h"
#include <omp.h>
#include <float.h>
#include <chrono>
#include <unistd.h>
#include <string>

using namespace std;

void push_points(queue<Point> &, queue<Point> &);
int free_threads = 0;

double current_max = -DBL_MAX; // shared

int main(int argc, char *argv[])
{
    queue<Point> public_q; // shared

    int number_of_threads;
    chrono::steady_clock::time_point start;
    chrono::steady_clock::time_point end;
    chrono::duration<double> time_span;

    start = chrono::steady_clock::now(); // by single thread

    public_q.push(Point(a, b, f(a), f(b), s)); // by single thread

    number_of_threads = omp_get_num_threads();

    #pragma omp parallel shared(public_q, number_of_threads, free_threads, current_max) num_threads(stoi(argv[1]))
    {
        #pragma omp master
        {
            number_of_threads = omp_get_num_threads();

            free_threads = number_of_threads;
        }

        #pragma omp barrier

        while (public_q.size() || free_threads < number_of_threads)
        {
            if (public_q.size())
            {
                push_points(public_q, public_q);
            }
        }
    }

    end = chrono::steady_clock::now(); // by single thread
    time_span = chrono::duration_cast<chrono::duration<double>>(end - start); // by single thread

    if(argc > 2 && stoi(argv[2]))
    {
        printf("%d, %f, %f\n", number_of_threads, current_max, time_span.count());
    }
    else
    {
        printf("Total number of cores: %d\n", number_of_threads); // by single thread
        printf("Maximum Value: %f\n", current_max);               // by single thread
        printf("Time: %f seconds\n", time_span.count());          // by single thread
    }
    

    return 0;
}

void push_points(queue<Point> &public_q, queue<Point> &pq)
{
    #pragma omp atomic
    --free_threads;

    Point temp;
    #pragma omp critical(q)
    {
        if(pq.size())
        {
            temp = pq.front();
            pq.pop();
        }
    }

    #pragma omp critical(max)
    {
        current_max = max(current_max, max(temp.f_c, temp.f_d));
    }
    
    if (temp.t_max < (current_max + epsilon))
    {
        #pragma omp atomic
        ++free_threads; 
        
        return;
    }

    temp.compute_f();
    temp.new_points(public_q);

    #pragma omp atomic
    ++free_threads;
} 