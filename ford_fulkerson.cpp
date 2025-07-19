#include <iostream>
#include <vector>
#include <climits>
using namespace std;

struct Edge {
    int to;
    bool enabled; // = текущее направление ребра совпадает с тем, которое мы ожидаем ? 1 : 0
    Edge* back; // обратное к этому

};

int l_size, r_size; // размеры долей
int s, t; // номера истока и стока
vector<vector<Edge>> graph; // Граф в виде списка смежности
vector<bool> used;

vector<int> match; // само паросочетание - для правой доли - "друзья" из левой

bool dfs(int v) {
    if (v == t) {return true;}
    if (used[v]) {return false;}
    used[v] = true;
    for(auto& edge : graph[v]) {
        if (edge.enabled && !used[edge.to]) {
            bool ans = dfs(edge.to);
            if (ans) {
                edge.enabled = false;
                (*edge.back).enabled = true; // инвертировали рёбра
                return true;
            }
        }
    }
    return false;
}

void ford_fulkerson() {
    used.resize(r_size, false);
    // пока находится, ищем
    while(dfs(s)) {
        used.assign(r_size, false);
    }
}

int main() {
    // Пример невзвешенного графа (матрица смежности)
    l_size = r_size = 4;
    vector<vector<int>> given_graph = {
        /* 0: */ {0, 1},
        /* 1: */ {0, 1},
        /* 2: */ {1},
        /* 3: */ {3},
    };

    s = l_size + r_size;
    t = l_size + r_size + 1;

    // строим граф
    graph.resize(l_size + r_size + 2);
    // данные рёбра
    for (int i=0; i<l_size; ++i) {
        for (auto& el : given_graph[i]) {
            graph[i].push_back({l_size + el, true, nullptr}); // прямое
            graph[l_size + el].push_back({i, false, nullptr}); // обратное
            // слинковали прямое и обратное
            graph[i].back().back = &graph[l_size + el].back();
            graph[l_size + el].back().back = &graph[i].back();
        }
    }

    // из S в левую долю
    for (int i=0; i<l_size; ++i) {
        graph[s].push_back({i, true, nullptr}); // прямое S -> L
        graph[i].push_back({s, false, nullptr}); // обратное L -> S
        // линкуем
        graph[s].back().back = &graph[i].back();
        graph[i].back().back = &graph[s].back();
    }
    // из правой доли в T
    for (int i=0; i<r_size; ++i) {
        graph[l_size+i].push_back({t, true, nullptr}); // прямое S -> L
        graph[t].push_back({l_size + i, false, nullptr}); // обратное L -> S
        // линкуем
        graph[l_size + i].back().back = &graph[t].back();
        graph[t].back().back = &graph[l_size + i].back();
    }

    ford_fulkerson();

    cout << "Pairs:\n";
    for (int i=0; i<l_size; ++i) {
        for (auto& el : graph[i]) {
            if (!el.enabled && el.to != s) {
                cout << i << " - " << el.to - l_size << "\n";
            }
        }
    }
    return 0;
}