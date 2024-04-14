#include <iostream>
#include <vector>
#include <limits>
#include <omp.h>
#include <chrono>

#define INF std::numeric_limits<int>::max()

double runDijkstra(const std::vector<std::vector<int>>& graph, int start, int num_threads) {
    omp_set_num_threads(num_threads); // Set the number of threads for OpenMP

    int num_nodes = graph.size();
    std::vector<int> dist(num_nodes, INF);
    std::vector<bool> visited(num_nodes, false);
    dist[start] = 0;

    auto start_time = std::chrono::high_resolution_clock::now();

    for (int count = 0; count < num_nodes - 1; count++) {
        int u = -1;
        int min_dist = INF;

        for (int i = 0; i < num_nodes; i++) {
            if (!visited[i] && dist[i] < min_dist) {
                min_dist = dist[i];
                u = i;
            }
        }

        visited[u] = true;

        #pragma omp parallel for schedule(static)
        for (int v = 0; v < num_nodes; v++) {
            if (!visited[v] && graph[u][v] && dist[u] != INF && dist[u] + graph[u][v] < dist[v]) {
                dist[v] = dist[u] + graph[u][v];
            }
        }
    }

    auto end_time = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double, std::milli> elapsed = end_time - start_time;
    return elapsed.count(); // Return elapsed time in milliseconds
}

int main() {
    // Example: Graph of size N
    int N = 100; // Example graph size
    std::vector<std::vector<int>> graph(N, std::vector<int>(N, 0));
    // Fill graph with some weights (could be random or patterned)

    int num_threads[] = {1, 2, 4, 8, 16};
    for (int t : num_threads) {
        double time = runDijkstra(graph, 0, t);
        std::cout << "Time with " << t << " threads: " << time << " ms" << std::endl;
    }

    return 0;
}
