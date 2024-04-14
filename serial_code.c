#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/time.h>

#define N 2048
#define SOURCE 1
#define MAXINT 9999999

void dijkstra(int graph[N][N], int source);

int main(int argc, char *argv[]) {
    int weight[N][N];
    int i, j;
    char fn[255];
    FILE *fp;
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
            fscanf(fp, "%d", &weight[i][j]);
        }
    }
    fclose(fp);

    dijkstra(weight, SOURCE);

    printf("\n");
    printf("Nodes: %d ", N);

    gettimeofday(&tv, &tz);
    time_end = (double)tv.tv_sec + (double)tv.tv_usec / 1000000.00;
    printf("Time cost is %f\n", time_end - time_start);
    printf("\n");

    return 0;
}

void dijkstra(int graph[N][N], int source) {
    int distance[N];
    int visited[N];
    int i, j, nextNode, minDistance;

    for (i = 0; i < N; i++) {
        distance[i] = MAXINT;
        visited[i] = 0;
    }

    distance[source] = 0;

    for (i = 0; i < N; i++) {
        minDistance = MAXINT;
        for (j = 0; j < N; j++) {
            if (!visited[j] && distance[j] < minDistance) {
                minDistance = distance[j];
                nextNode = j;
            }
        }
        visited[nextNode] = 1;
        for (j = 0; j < N; j++) {
            if (!visited[j] && graph[nextNode][j] && minDistance + graph[nextNode][j] < distance[j]) {
                distance[j] = minDistance + graph[nextNode][j];
            }
        }
    }

    // Print the distance values if needed
    // printf("\nThe distance vector is\n");
    // for (i = 0; i < N; i++) {
    //     printf("%d ", distance[i]);
    // }
    // printf("\n");
}
