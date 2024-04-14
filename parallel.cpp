#include <iostream>
#include <vector>
#include <limits>
#include <omp.h>
#include <chrono>

#define INF std::numeric_limits<int>::max()

void GFG(const std::vector<std::vector<int>>& graph, int start) {
    int num_nodes = graph.size();
    std::vector<int> dist(num_nodes, INF);
    std::vector<bool> visited(num_nodes, false);
    dist[start] = 0;

    for (int count = 0; count < num_nodes - 1; count++) {
        // Find the minimum distance vertex from the set of vertices not yet processed.
        // This part is done sequentially.
        int u = -1;
        int min_dist = INF;

        for (int i = 0; i < num_nodes; i++) {
            if (!visited[i] && dist[i] < min_dist) {
                min_dist = dist[i];
                u = i;
            }
        }

        // Mark the picked vertex as processed
        visited[u] = true;

        // Update dist value of the adjacent vertices of the picked vertex.
        #pragma omp parallel for schedule(static)
        for (int v = 0; v < num_nodes; v++) {
            // Update dist[v] only if is not in sptSet, there is an edge from u to v, and
            // total weight of path from start to v through u is smaller than current value of dist[v]
            if (!visited[v] && graph[u][v] && dist[u] != INF && dist[u] + graph[u][v] < dist[v]) {
                dist[v] = dist[u] + graph[u][v];
            }
        }
    }

    // Print the shortest distances
    std::cout << "Vertex \t Distance from Source\n";
    for (int i = 0; i < num_nodes; ++i) {
        std::cout << i << "\t\t" << dist[i] << "\n";
    }
}

int main() {
    std::vector<std::vector<int>> graph = {
        {0, 4, 0, 0, 0, 0, 0, 8, 0},
        {4, 0, 8, 0, 0, 0, 0, 11, 0},
        {0, 8, 0, 7, 0, 4, 0, 0, 2},
        {0, 0, 7, 0, 9, 14, 0, 0, 0},
        {0, 0, 0, 9, 0, 10, 0, 0, 0},
        {0, 0, 4, 14, 10, 0, 2, 0, 0},
        {0, 0, 0, 0, 0, 2, 0, 1, 6},
        {8, 11, 0, 0, 0, 0, 1, 0, 7},
        {0, 0, 2, 0, 0, 0, 6, 7, 0}
    };
    GFG(graph, 0);
    return 0;
}

