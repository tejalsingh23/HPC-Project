#include <iostream>
#include <vector>
#include <chrono>
#include <climits>
#include <random>
#include <omp.h>

#define MAX_WEIGHT 100

int minDistance(const std::vector<int>& dist, const std::vector<bool>& sptSet) {
    int min = INT_MAX, min_index = -1;

    #pragma omp parallel
    {
        int local_min = INT_MAX, local_min_index = -1;
        #pragma omp for nowait
        for (int v = 0; v < dist.size(); v++) {
            if (!sptSet[v] && dist[v] < local_min) {
                local_min = dist[v];
                local_min_index = v;
            }
        }
        #pragma omp critical
        {
            if (local_min < min) {
                min = local_min;
                min_index = local_min_index;
            }
        }
    }

    return min_index;
}

void dijkstra(const std::vector<std::vector<int>>& graph, int src) {
    int V = graph.size();
    std::vector<int> dist(V, INT_MAX);
    std::vector<bool> sptSet(V, false);

    dist[src] = 0;

    for (int count = 0; count < V - 1; count++) {
        int u = minDistance(dist, sptSet);
        sptSet[u] = true;

        #pragma omp parallel for
        for (int v = 0; v < V; v++) {
            if (!sptSet[v] && graph[u][v] && dist[u] != INT_MAX &&
                dist[u] + graph[u][v] < dist[v]) {
                #pragma omp critical
                {
                    if (dist[u] + graph[u][v] < dist[v]) {
                        dist[v] = dist[u] + graph[u][v];
                    }
                }
            }
        }
    }
}

std::vector<std::vector<int>> generateGraph(int N) {
    std::vector<std::vector<int>> graph(N, std::vector<int>(N, 0));
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dis(1, MAX_WEIGHT);

    for (int i = 0; i < N; i++) {
        for (int j = i + 1; j < N; j++) {
            int weight = dis(gen);
            graph[i][j] = weight;
            graph[j][i] = weight; // Ensure the graph is undirected
        }
    }
    return graph;
}

int main() {
    std::vector<int> thread_counts = {1, 2, 4, 8, 16};
    std::vector<int> graph_sizes = {6, 10, 20, 40, 80};

    for (int N : graph_sizes) {
        std::vector<std::vector<int>> graph = generateGraph(N);
        std::cout << "Graph size: " << N << std::endl;

        for (int p : thread_counts) {
            omp_set_num_threads(p);
            auto start = std::chrono::high_resolution_clock::now();
            
            dijkstra(graph, 0); // Assuming 0 is always the source node

            auto end = std::chrono::high_resolution_clock::now();
            std::chrono::duration<double> diff = end - start;
            std::cout << "Threads: " << p << " Time: " << diff.count() << "s\n";
        }
    }

    return 0;
}
