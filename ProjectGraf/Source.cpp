#define _CRT_SECURE_NO_WARNINGS

#include <iostream>
#include <vector>
#include <fstream>

using namespace std;

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
            add(J[i], I[i], 1);
        }
    };
    Graph(const vector<int>& I, const vector<int>& J,const vector<int>& Weight, int N) :M(0), N(N) {
        if (I.size() != J.size()) throw out_of_range("I size not equal J size!");
        if (N < 0) throw invalid_argument("N cannot be negative!");
        H.assign(N,-1);
        color.assign(N, -1);
        for (int i = 0; i < I.size(); ++i) {
            add(I[i], J[i], Weight[i]);
            add(J[i], I[i], Weight[i]);
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
int n, m;
vector < vector<long long> > d;


void calc(int x = 0, int y = 0, int mask = 0, int next_mask = 0)
{
    if (x == n)
        return;
    if (y >= m)
        d[x + 1][next_mask] += d[x][mask];
    else
    {
        int my_mask = 1 << y;
        if (mask & my_mask)
            calc(x, y + 1, mask, next_mask);
        else
        {
            calc(x, y + 1, mask, next_mask | my_mask);
            if (y + 1 < m && !(mask & my_mask) && !(mask & (my_mask << 1)))
                calc(x, y + 2, mask, next_mask);
        }
    }
}

int main()
{
    freopen("input.txt", "r", stdin);
    freopen("output.txt", "w", stdout);
    cin >> n >> m;

    d.resize(n + 1, vector<long long>(1 << m));
    d[0][0] = 1;
    for (int x = 0; x<n; ++x)
        for (int mask = 0; mask<(1 << m); ++mask)
            calc(x, 0, mask, 0);

    for (int i = 0; i <= n; ++i) {
        for (int j = 0; j < (1 << m); ++j) {
            cout << d[i][j] << ' ';
        }
        cout << '\n';
    }
    cout << d[n][0];
    //ofstream output_file;
 //   g.print(output_file, "output.txt");
 //   g.del_arc(1, 4);
//    g.print(output_file, "output.txt");
}