#include <iostream>
#include <limits.h>
#include <queue>
#include <vector>

using namespace std;

bool BFS(vector<vector<int>> residual_graph, int src, int target, vector<int> *parent, vector<bool> *source_reachable, int n_vertex)
{
    (*source_reachable) = vector<bool>(n_vertex);
    queue<int> BFSQueue;
    BFSQueue.push(src);
    (*source_reachable)[src] = true;
    (*parent)[src] = -1;

    while (!BFSQueue.empty())
    {
        if ((*source_reachable)[target] == true)
        {
            break;
        }

        int u = BFSQueue.front();
        BFSQueue.pop();

        for (int v = 0; v < n_vertex; v++)
        {
            if ((*source_reachable)[v] == false && residual_graph[u][v] > 0)
            {
                BFSQueue.push(v);
                (*parent)[v] = u;
                (*source_reachable)[v] = true;
            }
        }
    }
    return ((*source_reachable)[target] == true);
}

int EdmondsKarp(vector<vector<int>> graph, int source, int target, vector<bool> *source_reachable, int n_vertex)
{
    vector<vector<int>> residual_graph(n_vertex, vector<int>(n_vertex));

    residual_graph = graph;

    vector<int> parent(n_vertex);

    int max_flow = 0;

    while (BFS(residual_graph, source, target, &parent, source_reachable, n_vertex))
    {
        int path_flow = INT_MAX;

        for (int v = target; v != source; v = parent[v])
        {
            int u = parent[v];
            path_flow = min(path_flow, residual_graph[u][v]);
        }

        for (int v = target; v != source; v = parent[v])
        {
            int u = parent[v];
            residual_graph[u][v] -= path_flow;
            residual_graph[v][u] += path_flow;
        }
        max_flow += path_flow;
    }

    return max_flow;
}

void print_output(vector<bool> S_set, int max_flow)
{
    int S_size = 0;
    for (int i = 0; i < S_set.size(); i++)
    {
        if (S_set[i] == true)
        {
            S_size++;
        }
    }
    cout << S_size << endl;

    for (int i = 0; i < S_set.size(); i++)
    {
        if (S_set[i] == true)
        {
            cout << i << " ";
        }
    }
    cout << endl;

    cout << max_flow << endl;
}

int main()
{
    // Fast reading from input
    ios_base::sync_with_stdio(0);
    cin.tie(0);

    int n_vertex, n_edges;

    cin >> n_vertex >> n_edges;

    vector<vector<int>> graph(n_vertex, vector<int>(n_vertex));
    vector<bool> source_reachable(n_vertex);
    vector<bool> S_set(n_vertex);

    for (int i = 0; i < n_edges; i++)
    {
        int source, target, weight;
        cin >> source >> target >> weight;

        graph[source][target] = weight;
    }

    int max_flow = 0;
    for (int i = 0; i < n_vertex; i++)
    {
        for (int j = 0; j < n_vertex; j++)
        {
            if (i != j)
            {
                int cur_flow = EdmondsKarp(graph, i, j, &source_reachable, n_vertex);
                if (cur_flow > max_flow)
                {
                    S_set = source_reachable;
                    max_flow = cur_flow;
                }
            }
        }
    }

    print_output(S_set, max_flow);

    return 0;
}
