#define _CRT_SECURE_NO_WARNINGS

#include <iostream>
#include <vector>
#include <fstream>
#include <algorithm>
#include <queue>

using namespace std;
struct Bucket {
    vector<int> B;
    vector<int> fw;
    vector<int> bw;
    Bucket(int n, int mx) {
        fw.assign(n, -1);
        bw.assign(n, -1);
        B.assign(n*mx, -1);
    }
    int get(int dist) {
        int v = -1;
        v = B[dist];
        if (v != -1) {
            B[dist] = fw[v];
        }
        return v;
    }
    void insert(int v, int k) {
        int now = B[k];
        B[k] = v;
        fw[v] = now;
        if (now != -1) {
            bw[now] = v;
        }
    }
    void remove(int v, int k) {
        int nex = fw[v];
        int bac = bw[v];
        if (B[k] == v) {
            B[k] = nex;
        }
        else {
            fw[bac] = nex;
            if (nex != -1) {
                bw[nex] = bac;
            }
        }
    }
};
class Graph {
public:
    int N, M;
    const int INF = 1e9;
    vector<pair<int, int> > IJ;
    vector<int> H;
    vector<int> L;
    vector<int> W;
    vector<int> d;
    vector<int> del;
    vector<int> p;
    vector<int> color;
    Graph(int N):N(N), M(0) {
        H.assign(N, -1);
    }
    void add(int i, int j, int w) {
        IJ.push_back({ i,j });
        L.push_back(H[i]);
        W.push_back(w);
        del.push_back(0);
        H[i] = M;
        M++;
    }
    Graph(const vector<int>& I, const vector<int>& J, int N) :M(0), N(N) {
        if (I.size() != J.size()) throw out_of_range("I size not equal J size!");
        if (N < 0) throw invalid_argument("N cannot be negative!");
        H.assign(N, -1);
        color.assign(N, -1);
        for (int i = 0; i < I.size(); ++i) {
            add(I[i], J[i], 1);
        }
    };
    Graph(const vector<int>& I, const vector<int>& J,const vector<int>& Weight, int N) :M(0), N(N) {
        if (I.size() != J.size()) throw out_of_range("I size not equal J size!");
        if (N < 0) throw invalid_argument("N cannot be negative!");
        H.assign(N,-1);
        color.assign(N, -1);
        for (int i = 0; i < I.size(); ++i) {
            add(I[i], J[i], Weight[i]);
        }
    };
    void Bellman_Ford(const int& s) {
        d.assign(N, INF);
        d[s] = 0;
        while (true) {
            bool flag = false;
            for (int i = 0; i < IJ.size(); ++i) {
                if (d[IJ[i].first] < INF) {
                    if (d[IJ[i].second] > d[IJ[i].first] + W[i]) {
                        d[IJ[i].second] = d[IJ[i].first] + W[i];
                        flag = true;
                    }
                }
            }
            if (!flag) break;
        }
    }
    void print(ofstream& Os, const string& path) {
        Os.open(path);
        Os << "digraph G { \n";
        for (int i = 0; i < M; i++) {
            if (!del[i]) {
                Os << IJ[i].first + 1 << " -> " << IJ[i].second + 1 << ";\n";
            }
        }
        Os << "}";
        Os.close();
    }
    void del_arc(const int& a,const int& b) {
        for (int i = H[a]; i != -1; i = L[i]) {
            if (i == H[a] && IJ[i].second == b) {
                del[H[a]] = 1;
                H[a] = L[H[a]];
                break;
            }
            if (L[i] != -1 && IJ[L[i]].second == b) {
                del[L[i]] = 1;
                L[i] = L[L[i]];
                break;
            }
        }
    }
    void del_arc(const int& ind) {
        if (ind < 0 || ind>=M) throw invalid_argument("You can't delete unexisting edge!");
        del_arc(IJ[ind].first, IJ[ind].second);
    }
    void Djikstra(const int& s) {
        d.assign(N, -1);
        d[s] = 0;
        p.assign(N, -1);
        priority_queue<pair<int, int> > q;
        q.push({ 0,s });
        while (!q.empty()) {
            int dist = -q.top().first;
            int v = q.top().second;
            q.pop();
            if (dist > d[v]) continue;
            for (int i = H[v]; i != -1; i = L[i]) {
                int w = W[i];
                int to = IJ[i].second;
                if (d[to] == -1 || d[to] > d[v] + w) {
                    d[to] = d[v] + w;
                    p[to] = v;
                    q.push({ -d[to], to });
                }
            }
        }
    }
    void ch_Djikstra(const int& s) {
        d.assign(N, -1);
        p.assign(N, -1);
        int mx = 0;
        for (int i = 0; i < M; ++i) {
            mx = max(mx, W[i]);
        }
        Bucket ch = Bucket(N, mx);
        d[s] = 0;
        ch.insert(s, 0);
        for (int cur = 0; cur < ch.B.size(); ++cur) {
            while (true) {
                int v = ch.get(cur);
                if (v == -1) break;
                for (int i = H[v]; i != -1; i = L[i]) {
                    int to = IJ[i].second;
                    int w = W[i];
                    if (d[to] == -1 || d[to] > d[v] + w) {
                        if (d[to] != -1) ch.remove(to, d[to]);
                        d[to] = cur + w;
                        p[to] = v;
                        ch.insert(to, d[to]);
                    }
                }
            }
        }
    }

    void print_distances(ofstream& Os, const string& path) {
        Os.open(path);
        Os << "digraph G { \n";
        for (int i = 0; i < N; ++i) {
            if (p[i] == -1) continue;
            Os << p[i] + 1 << "->" << i + 1 << "[label = " << d[i] - d[p[i]] << "];\n";
        }
        Os << "}";
        Os.close();
    }
    void find_components() {
        int c = 0;
        for (int i = 0; i < N; ++i) {
            if (color[i] == -1) {
                dfs(i,c++);
            }
        }
        for (int i = 0; i < N; ++i) {
            cout << i + 1 << ' ' << color[i] << '\n';
        }
    }
    void dfs(int v, int c) {
        color[v] = c;
        for (int i = H[v]; i != -1; i = L[i]) {
            if (color[IJ[i].second] == -1) {
                dfs(IJ[i].second, c);
            }
        }
    }
};


int main()
{
    freopen("input.txt", "r", stdin);
   // freopen("output.txt", "w", stdout);
    int n, m;
    cin >> n >> m;
    vector<int> I(m);
    vector<int> J(m);
    vector<int> W(m);
    for (int i = 0; i < m; ++i) {
        int a, b, c;
        cin >> a >> b >> c;
        a--; b--;
        I[i] = a;
        J[i] = b;
        W[i] = c;
    }
    Graph a = Graph(I, J, W, n);
    a.ch_Djikstra(0);
    ofstream output_file;
    a.print_distances(output_file, "output.txt");
}