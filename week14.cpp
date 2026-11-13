#include <bits/stdc++.h>
using namespace std;

const int INF = INT_MAX;

void printPath(const vector<int>& parent, int v) {
    if (v == -1) return;
    printPath(parent, parent[v]);
    cout << v << " ";
}

int main() {
    int n, src;

    cout << "Enter number of vertices: ";
    cin >> n;

    vector<vector<int>> graph(n, vector<int>(n));
    cout << "Enter adjacency matrix (0 if no edge):\n";
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            cin >> graph[i][j];
        }
    }

    cout << "Enter source vertex (0 to " << n - 1 << "): ";
    cin >> src;

    vector<int> dist(n, INF);
    vector<int> parent(n, -1);
    vector<bool> visited(n, false);

    dist[src] = 0;

    for (int step = 0; step < n - 1; step++) {
        int u = -1;
        for (int i = 0; i < n; i++) {
            if (!visited[i] && (u == -1 || dist[i] < dist[u])) {
                u = i;
            }
        }

        if (u == -1 || dist[u] == INF) break;

        visited[u] = true;

        for (int v = 0; v < n; v++) {
            int edgeWeight = graph[u][v];
            
            if (!visited[v] && edgeWeight != 0) {
                if (dist[u] + edgeWeight < dist[v]) {
                    dist[v] = dist[u] + edgeWeight;
                    parent[v] = u;
                }
            }
        }
    }

    cout << "\nShortest distances from source " << src << ":\n";
    for (int i = 0; i < n; i++) {
        cout << "To " << i << ": ";
        if (dist[i] == INF) {
            cout << "Unreachable\n";
        } else {
            cout << dist[i] << "\tPath: ";
            printPath(parent, i);
            cout << "\n";
        }
    }

    return 0;
}