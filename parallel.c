#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <sys/time.h>
#include <omp.h>

#define N 2048
#define SOURCE 1
#define MAXINT 9999999

void dijkstra(int graph[N][N], int source);

int main(int argc, char **argv) {
    int i, j;
    char fn[255];
    FILE *fp;
    int graph[N][N];
    int threads;
    printf("Please enter number of threads: ");
    scanf("%d", &threads);
    omp_set_num_threads(threads);
    double time_start, time_end;
    struct timeval tv;
    struct timezone tz;
    gettimeofday(&tv, &tz);
    time_start = (double)tv.tv_sec + (double)tv.tv_usec / 1000000.00;
    strcpy(fn, "input2048.txt");
    fp = fopen(fn, "r");
    if (fp == NULL) {
        printf("Can't open the input file: %s\n\n", fn);
        exit(1);
    }
    for (i = 0; i < N; i++) {
        for (j = 0; j < N; j++) {
            fscanf(fp, "%d", &graph[i][j]);
        }
    }
    fclose(fp);
    dijkstra(graph, SOURCE);
    gettimeofday(&tv, &tz);
    time_end = (double)tv.tv_sec + (double)tv.tv_usec / 1000000.00;
    printf("Nodes: %d\n", N);
    printf("Time cost is %f\n", time_end - time_start);
    return 0;
}

void dijkstra(int graph[N][N], int source) {
    int visited[N] = {0};
    int distance[N];
    int i;

    for (i = 0; i < N; i++) {
        distance[i] = graph[source][i];
    }
    visited[source] = 1;

    #pragma omp parallel shared(graph, distance, visited) private(i)
    {
        int md;
        int mv;
        int my_first;
        int my_last;
        int my_md;
        int my_mv;
        int my_step;
        int nth;

        int my_id = omp_get_thread_num();
        nth = omp_get_num_threads();
        my_first = (my_id * N) / nth;
        my_last = ((my_id + 1) * N) / nth - 1;

        for (my_step = 1; my_step < N; my_step++) {
            #pragma omp single
            {
                md = MAXINT;
                mv = -1;
            }
            my_md = MAXINT;
            my_mv = -1;
            for (i = my_first; i <= my_last; i++) {
                if (!visited[i] && distance[i] < my_md) {
                    my_md = distance[i];
                    my_mv = i;
                }
            }
            #pragma omp critical
            {
                if (my_md < md) {
                    md = my_md;
                    mv = my_mv;
                }
            }
            #pragma omp barrier
            #pragma omp single
            {
                if (mv != -1) {
                    visited[mv] = 1;
                }
            }
            #pragma omp barrier
            if (mv != -1) {
                for (i = my_first; i <= my_last; i++) {
                    if (!visited[i] && graph[mv][i] < MAXINT && distance[mv] + graph[mv][i] < distance[i]) {
                        distance[i] = distance[mv] + graph[mv][i];
                    }
                }
            }
            #pragma omp barrier
        }
    }
}
