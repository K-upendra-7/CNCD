#include <bits/stdc++.h>
using namespace std;

int INF = INT_MAX;

void printPath(const vector<int>& parent, int v) {
    if (v == -1)
        return;
    printPath(parent, parent[v]);
    cout << v << " ";
}

int main() {
    int n, src;

    cout << "Enter number of vertices: ";
    if (!(cin >> n) || n <= 0) return 1;


    vector<vector<int>> cost(n, vector<int>(n));

    vector<int> dist(n, INF);

    vector<int> parent(n, -1);

    vector<bool> visited(n, false);

    cout << "Enter adjacency matrix (0 if no edge):\n";
   
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            cin >> cost[i][j];
        }
    }

    cout << "Enter source vertex (0 to " << n - 1 << "): ";
    cin >> src;

    dist[src] = 0;

    for (int count = 0; count < n - 1; count++) {
        int u = -1;
        int min_dist = INF;

        for (int i = 0; i < n; i++) {
            if (!visited[i] && dist[i] < min_dist) {
                min_dist = dist[i];
                u = i;
            }
        }

        if (u == -1)
            break;

        visited[u] = true;

        for (int j = 0; j < n; j++) {
            if (!visited[j] && cost[u][j] != 0 && dist[u] + cost[u][j] < dist[j]) {
                dist[j] = dist[u] + cost[u][j];
                parent[j] = u;
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