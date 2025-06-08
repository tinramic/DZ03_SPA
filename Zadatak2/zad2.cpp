#include <iostream>
#include <vector>
#include <queue>
#include <climits>
#include <algorithm>
#include <chrono>
#include <thread>
#include <cstdlib>

using namespace std;

const int RED = 20;
const int STUPAC = 40;

int toNode(int r, int s) {
    return r * STUPAC + s;
}

pair<int, int> toCoord(int node) {
    return { node / STUPAC, node % STUPAC };
}

vector<vector<int>> napraviEdgeList(vector<vector<char>>& mapa) {
    vector<vector<int>> edges;

    for (int i = 0; i < RED; i++) {
        for (int j = 0; j < STUPAC; j++) {
            if (mapa[i][j] == '#') continue;

            int u = toNode(i, j);

            int dr[] = { -1, 1, 0, 0 };
            int ds[] = { 0, 0, -1, 1 };

            for (int k = 0; k < 4; k++) {
                int ni = i + dr[k];
                int nj = j + ds[k];

                if (ni >= 0 && ni < RED && nj >= 0 && nj < STUPAC && mapa[ni][nj] != '#') {
                    int v = toNode(ni, nj);
                    edges.push_back({ u, v, 1 });
                }
            }
        }
    }

    return edges;
}

// Dijkstra funkcija
vector<int> dijkstra(int V, vector<vector<int>>& edges, int src) {
    vector<vector<vector<int>>> adj(V);
    for (auto& e : edges) {
        int u = e[0], v = e[1], w = e[2];
        adj[u].push_back({ v, w });
    }

    priority_queue<vector<int>, vector<vector<int>>, greater<vector<int>>> pq;
    vector<int> dist(V, INT_MAX);

    pq.push({ 0, src });
    dist[src] = 0;

    while (!pq.empty()) {
        int u = pq.top()[1];
        pq.pop();

        for (auto x : adj[u]) {
            int v = x[0];
            int weight = x[1];

            if (dist[v] > dist[u] + weight) {
                dist[v] = dist[u] + weight;
                pq.push({ dist[v], v });
            }
        }
    }

    return dist;
}

void ispisiMapu(vector<vector<char>>& mapa, int currR, int currS, int aR, int aS, int bR, int bS) {
#ifdef _WIN32
    system("cls");
#else
    system("clear");
#endif

    for (int i = 0; i < RED; i++) {
        for (int j = 0; j < STUPAC; j++) {
            if (i == aR && j == aS)
                cout << 'A';
            else if (i == bR && j == bS)
                cout << 'B';
            else if (i == currR && j == currS)
                cout << 'x';
            else
                cout << mapa[i][j];
        }
        cout << endl;
    }
}

int main() {
    vector<vector<char>> mapa(RED, vector<char>(STUPAC, '-'));

    for (int i = 2; i < 14; i++) {
        mapa[i][8] = '#';
    }

    int r1, s1, r2, s2;

    while (true) {
        cout << "Unesi redak i stupac pocetne tocke A (1-20 1-40): ";
        cin >> r1 >> s1;
        cout << "Unesi redak i stupac krajnje tocke B (1-20 1-40): ";
        cin >> r2 >> s2;

        r1--; s1--; r2--; s2--;

        if (r1 < 0 || r1 >= RED || s1 < 0 || s1 >= STUPAC ||
            r2 < 0 || r2 >= RED || s2 < 0 || s2 >= STUPAC ||
            mapa[r1][s1] == '#' || mapa[r2][s2] == '#') {
            cout << "Pogresan unos (mozda je na zidu). Pokusaj ponovo." << endl;
        }
        else {
            break;
        }
    }

    int V = RED * STUPAC;
    int start = toNode(r1, s1);
    int end = toNode(r2, s2);

    vector<vector<int>> edges = napraviEdgeList(mapa);
    vector<int> dist = dijkstra(V, edges, start);

    if (dist[end] == INT_MAX) {
        cout << "Nema puta do cilja." << endl;
        return 0;
    }

    vector<int> prev(V, -1);
    vector<int> d(V, INT_MAX);
    d[start] = 0;

    priority_queue<vector<int>, vector<vector<int>>, greater<vector<int>>> pq;
    pq.push({ 0, start });

    vector<vector<vector<int>>> adj(V);
    for (auto& e : edges) {
        adj[e[0]].push_back({ e[1], e[2] });
    }

    while (!pq.empty()) {
        int u = pq.top()[1];
        pq.pop();

        for (auto x : adj[u]) {
            int v = x[0], w = x[1];
            if (d[v] > d[u] + w) {
                d[v] = d[u] + w;
                prev[v] = u;
                pq.push({ d[v], v });
            }
        }
    }

    vector<int> put;
    for (int at = end; at != -1; at = prev[at])
        put.push_back(at);
    reverse(put.begin(), put.end());

    for (int node : put) {
        pair<int, int> pos = toCoord(node);
        ispisiMapu(mapa, pos.first, pos.second, r1, s1, r2, s2);
        this_thread::sleep_for(chrono::milliseconds(100));
    }

    cout << "Stigli smo do cilja!" << endl;
    return 0;
}
