#include <omp.h>
#include <stdio.h>
#include <math.h>
#include <time.h>
#include <stdlib.h>

double EPS = 1e-6;

//constants to use
//#define LOGS
//#define LOGS_2
#define USE_OPENMP
#define USE_OPENMP_PARALLEL

struct Integral
{
    double upper;
    double lower;
};

double user_function(double x)
{
    return (x > 0) ? 1+sin(x) : cos(x);
}

struct Integral CalcIntegralAdv(double a, double b, int n, double (*f)(double x));
double CalcIntegral(double a, double b);
double func(double x);
void Run();
double MeasureTime(void (*f)(void));
double MeasureMeanTime(void (*f)(void), int count);


struct Integral CalcIntegralAdv(double a, double b, int n, double (*f)(double x))
{
#ifdef LOGS
	printf("CalcIntegral from %lf to %lf, n = %d\n", a, b, n);
#endif
    const double dx = (b - a)/n;
    double sum = 0.;

#ifdef USE_OPENMP_PARALLEL
    #pragma omp parallel for reduction (+:sum)
#endif
    for(int i = 1; i < n; ++i){
        sum += f(a + i*dx);
    }

    struct Integral integral;
    integral.upper = (sum + f(b))*dx;
    integral.lower = (sum + f(a))*dx;
#ifdef LOGS
    printf("lower: %lf ", integral.lower);
    printf("upper: %lf\n", integral.upper);
#endif
    return integral;
}


double func(double x)
{
    return user_function(x);
}


double MeasureTime(void (*f)(void))
{
#ifdef USE_OPENMP
    double start, end;
    start = omp_get_wtime();
#else
    time_t start, end;
    time (&start);
#endif // USE_OPENMP
    f();
#ifdef USE_OPENMP
    end = omp_get_wtime();
    return end - start;
#else
    time (&end);
    return difftime (end,start);
#endif // USE_OPENMP
}

void Run()
{
	double a = -1;
    double b = 2;
    double result = CalcIntegral(a, b);
#ifdef LOGS_2
    printf("Value: %lf\n", result);
#endif
}

double CalcIntegral(double a, double b)
{
    struct Integral integral;
    double delta = 2*EPS;
    for(int n = 1; delta > EPS; n *= 2){
        integral = CalcIntegralAdv(a, b, n, func);
        delta = integral.upper - integral.lower;
    }

    return (integral.upper + integral.lower) / 2;
}

double MeasureMeanTime(void (*f)(void), int count)
{
	double sum = 0;
    for(int i = 0; i < count; ++i) {
		double time = (double)(MeasureTime(f));
#ifdef LOGS_2
		printf("Time: %lf\n\n", time);
#endif
        sum += time;
	}

    return sum / count;
}


int main(int argc, char *argv[])
{
    if(argc > 1){
        sscanf(argv[1], "%lf", &EPS);
    }
    printf("EPS: %.1e, ", EPS);

#pragma omp parallel
    {
#pragma omp master
        {
            int cntThreads=omp_get_max_threads();
            printf("threads: %d, ", cntThreads);
        }

    }
    printf("mean time: %lf\n", MeasureMeanTime(Run, 20));
    return 0;
}
