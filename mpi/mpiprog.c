#include <omp.h>
#include <mpi.h>
#include <stdio.h>
#include <math.h>
#include <time.h>
#include <stdlib.h>

double EPS = 1e-6;

int n_proc;
int my_id;

//constants to use
//#define LOGS
//#define LOGS_2
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
    MPI_Bcast(&EPS, 1, MPI_DOUBLE, 0, MPI_COMM_WORLD);
#ifdef LOGS
	printf("CalcIntegral from %lf to %lf, n = %d\n", a, b, n);
#endif
    const double dx = (b - a)/n;
    double sum = 0.;

    #pragma omp parallel for reduction (+:sum)
    for(int i = 1; i < n; i+=n_proc){
        sum += f(a + i*dx);
    }
    double result;
    MPI_Reduce(&sum, &result, 1, MPI_LONG_DOUBLE, MPI_SUM, 0, MPI_COMM_WORLD);
    sum = result;

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

double start, end;

double MeasureTime(void (*f)(void))
{
    if(my_id == 0) {
        start = MPI_Wtime();
    }
    f();
    if(my_id == 0) {
        end = MPI_Wtime();
        return end - start;
    }
    return 0;
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

    int rc;
    if (rc = MPI_Init(&argc, &argv))
    {
        printf("Error: MPI_Init - Aborting...\n");
        MPI_Abort(MPI_COMM_WORLD, rc);
    } else {
        //printf("Success: MPI_Init\n");
    }

    // get number of processes and id of the current process
    MPI_Comm_size(MPI_COMM_WORLD,&n_proc);
    MPI_Comm_rank(MPI_COMM_WORLD,&my_id);

    if(my_id == 0) {
        printf("EPS: %.1e, process count: %d, mean time: %lf\n", EPS,
               n_proc, MeasureMeanTime(Run, 20));
    }


    MPI_Finalize();
    return 0;
}
