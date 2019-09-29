#include <iostream>
#include <limits.h>
#include <queue>
#include <vector>

bool BFS(std::vector<std::vector<int>> residual_graph, int src, int target, std::vector<int> *parent, std::vector<bool> *source_reachable, int n_vertex)
{
    (*source_reachable) = std::vector<bool>(n_vertex);
    std::queue<int> BFSQueue;
    BFSQueue.push(src);
    (*source_reachable)[src] = true;
    (*parent)[src] = src;

    while (!BFSQueue.empty())
    {
        if ((*source_reachable)[target])
        {
            return true;
        }

        int u = BFSQueue.front();
        BFSQueue.pop();

        for (int v = 0; v < n_vertex; v++)
        {
            if ((*source_reachable)[v] == false && residual_graph[u][v] > 0)
            {
                BFSQueue.push(v);
                (*source_reachable)[v] = true;
                (*parent)[v] = u;
            }
        }
    }

    return false;
}

int Edmonds_Karp(std::vector<std::vector<int>> graph, int source, int target, std::vector<bool> *source_reachable, int n_vertex, int min_cut)
{
    std::vector<std::vector<int>> residual_graph(n_vertex, std::vector<int>(n_vertex));
    residual_graph = graph;

    std::vector<int> parent(n_vertex);

    int max_flow = 0;

    while (max_flow < min_cut && BFS(residual_graph, source, target, &parent, source_reachable, n_vertex))
    {
        int min_path_flow = INT_MAX;

        for (int v = target; v != source; v = parent[v])
        {
            int u = parent[v];
            min_path_flow = std::min(min_path_flow, residual_graph[u][v]);
        }

        for (int v = target; v != source; v = parent[v])
        {
            int u = parent[v];
            residual_graph[u][v] -= min_path_flow;
            residual_graph[v][u] += min_path_flow;
        }
        max_flow += min_path_flow;
    }

    return max_flow;
}

void print_output(std::vector<bool> S_set, int min_cut)
{
    int S_size = 0;
    for (unsigned int i = 0; i < S_set.size(); i++)
    {
        if (S_set[i] == true)
        {
            S_size++;
        }
    }
    std::cout << S_size << std::endl;

    for (unsigned int i = 0; i < S_set.size(); i++)
    {
        if (S_set[i] == true)
        {
            std::cout << i << " ";
        }
    }
    std::cout << std::endl;

    std::cout << min_cut << std::endl;
}

int main()
{
    // Fast reading from input
    std::ios_base::sync_with_stdio(0);
    std::cin.tie(0);

    int n_vertex, n_edges;

    std::cin >> n_vertex >> n_edges;

    std::vector<std::vector<int>> graph(n_vertex, std::vector<int>(n_vertex));
    std::vector<bool> source_reachable(n_vertex);
    std::vector<bool> S_set(n_vertex);

    for (int i = 0; i < n_edges; i++)
    {
        int source, target, weight;
        std::cin >> source >> target >> weight;

        graph[source][target] = weight;
        graph[target][source] = weight;
    }

    int min_cut = INT_MAX;
    for (int i = 0; i < n_vertex; i++)
    {
        for (int j = i + 1; j < n_vertex; j++)
        {
            int cur_flow = Edmonds_Karp(graph, i, j, &source_reachable, n_vertex, min_cut);
            if (cur_flow < min_cut)
            {
                S_set = source_reachable;
                min_cut = cur_flow;
            }
        }
    }

    print_output(S_set, min_cut);

    return 0;
}
