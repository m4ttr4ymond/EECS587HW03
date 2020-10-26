#include <stdio.h>
#include <omp.h>
#include <queue>
#include "point.h"
#include "f.h"
#include <omp.h>
#include <float.h>
#include <chrono>

using namespace std;

inline void gen_model();
void push_points(queue<Point> &, queue<Point> &, Point *);
int free_threads = 0;

double current_max = -DBL_MAX; // shared

int main(void) {
    queue<Point> public_q; // shared
    queue<Point> private_q; // private

    int number_of_threads;
    chrono::steady_clock::time_point start;
    chrono::steady_clock::time_point end;
    chrono::duration<double> time_span;

    start = chrono::steady_clock::now(); // by single thread

    #pragma omp parallel shared(public_q, number_of_threads, start, end, time_span, free_threads) num_threads(2) private(private_q)
    {
        #pragma omp master
        {
            number_of_threads = 2; //omp_get_num_threads();
            private_q.push(Point(a, b, f(a), f(b), s)); // by single thread
        }

        Point *temp = (Point *)malloc(sizeof(Point)); // private

        while (free_threads < number_of_threads) // not critical because it it runs an extra time it will do nothing bad
        {
            while (private_q.size() > 0)
            {
                push_points(public_q, private_q, temp);                                     // not critical
                // printf("%f Gb\n", ((double)private_q.size() * sizeof(Point)) / 1000000000); // going to be removed
            }

            #pragma omp atomic
            ++free_threads; // critical

            while (private_q.size() == 0 && free_threads < number_of_threads) // critical
            {
                #pragma omp critical(q)
                {       
                    if (public_q.size())
                    {                                     // critical
                        private_q.push(public_q.front()); // critical
                        public_q.pop();                   // critical

                        --free_threads; // critical
                    }
                }
            }
            
        }

        // ToDo: Code appears to be deadlocked

        printf("Thread %d free\n", omp_get_thread_num());
        free(temp); // private
    }

    end = chrono::steady_clock::now(); // by single thread
    time_span = chrono::duration_cast<chrono::duration<double>>(end - start); // by single thread

    printf("Total number of cores: %d\n", number_of_threads); // by single thread
    printf("Maximum Value: %f\n", current_max); // by single thread
    printf("Time: %f seconds\n", time_span.count()); // by single thread

    return 0;
}

void push_points(queue<Point> &public_q, queue<Point> &private_q, Point *temp)
{
    *temp = private_q.front(); // not critical
    private_q.pop(); // not critical

    #pragma omp critical(q)
    {
        current_max = max(current_max, max(temp->f_c, temp->f_d)); // critical
    }
    
    // May get evaluated twice, but won't change the truthfulness of the result
    if(temp->t_max < (current_max+epsilon)) // not critical
    {
        return;
    }

    temp->compute_f(); // not critical
    temp->new_points(public_q,private_q, free_threads); // not critical
}