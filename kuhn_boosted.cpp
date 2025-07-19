#include <iostream>
#include <vector>

using namespace std;

int l_size, r_size; // размеры долей

vector<vector<int>> graph; // граф в виде списка смежности
vector<bool> used; // посещённые вершины правой доли
vector<int> match; // само паросочетание: для каждой вершины правой доле - её "друг" из левой

bool try_kuhn (int v) { // true - если нашлась цепь 
	if (used[v])  return false;
	used[v] = true;
	for (auto& to : graph[v]) {
        // если to никуда ещё не привязана или нашлась цепь из to
		if (match[to] == -1 || try_kuhn (match[to])) {
            // чередуем паросочетание
			match[to] = v;
			return true;
		}
	}
	return false;
}

int main() {
    // входные данные
    l_size = r_size = 4;
    graph = {
        /* 0: */ {0, 1},
        /* 1: */ {0, 1},
        /* 2: */ {1},
        /* 3: */ {3},
    };
    used.resize(r_size, false);
    match.resize(r_size, -1);

    // жадная раздача вершинам из правой доли друзей из левой
    vector<int> used_pre(l_size);
    for (int i=0; i<l_size; ++i) {
        for (auto& to : graph[i]) {
            if (match[to] == -1) {
                match[to] = i;
                used_pre[i] = true;
                break;
            }
        }
    }


    // для каждой вершины левой доли запускаем поиск увел. цепи
    for(int i=0; i<l_size; ++i) {
        if (used_pre[i]) continue; // уже жадно подружили
        used.assign(r_size, false);
        try_kuhn(i);
    }

    // вывод
    cout << "Pairs:\n";
    for(int i=0;i<r_size; ++i) {
        if (match[i] != -1) {
            cout << match[i] << " - " << i << "\n";
        }
    }
}