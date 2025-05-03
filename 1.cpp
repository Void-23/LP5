#include <iostream>
#include <vector>
#include <queue>
#include <omp.h>
using namespace std;

// Parallel BFS
void parallel_bfs(const vector<vector<int>> &graph, int start) 
{
    int n = graph.size();
    vector<bool> visited(n, false);
    queue<int> q;
    visited[start] = true;
    q.push(start);


    cout << "Parallel BFS: ";
    while (!q.empty()) 
    {
        int sz = q.size();

        #pragma omp parallel for
        for (int i = 0; i < sz; ++i) 
        {
            int node;

            #pragma omp critical
            {
                if (!q.empty()) 
                {
                    node = q.front(); 
                    q.pop();
                    cout << node << " ";
                }
            }

            for (int neighbor : graph[node]) 
            {
                #pragma omp critical
                {
                    if (!visited[neighbor]) 
                    {
                        visited[neighbor] = true;
                        q.push(neighbor);
                    }
                }
            }
        }
    }
    cout << endl;
}

// Parallel DFS helper
void dfs_util(const vector<vector<int>> &graph, vector<bool> &visited, int node) 
{
    bool should_continue = true;
    
    #pragma omp critical
    {
        if (visited[node]) {
            should_continue = false;
        } else {
            visited[node] = true;
            cout << node << " ";
        }
    }
    
    if (!should_continue) {
        return;
    }

    #pragma omp parallel for
    for (int i = 0; i < graph[node].size(); ++i) 
    {
        int neighbor = graph[node][i];
        #pragma omp task
        if (!visited[neighbor])
            dfs_util(graph, visited, neighbor);
    }
}

// Parallel DFS
void parallel_dfs(const vector<vector<int>> &graph, int start) {
    vector<bool> visited(graph.size(), false);
    cout << "Parallel DFS: ";
    #pragma omp parallel
    #pragma omp single
    dfs_util(graph, visited, start);
    cout << endl;
}

// Main Function
int main() 
{
    vector<vector<int>> graph = 
    {
        {1, 2}, // Node 0
        {0, 3, 4}, // Node 1
        {0, 5}, // Node 2
        {1}, // Node 3
        {1}, // Node 4
        {2} // Node 5
    };
    parallel_bfs(graph, 0);
    parallel_dfs(graph, 0);
    return 0;
}