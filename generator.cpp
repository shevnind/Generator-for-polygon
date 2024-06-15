#include <bits/stdc++.h>
#include "testlib.h"

//struct RandomMock {
//    std::mt19937 rnd;
//
//    int next(int l, int r) {
//        return rnd() % (r - l + 1) + l;
//    }
//} rnd;

namespace gen_functions {
    template <typename T>
    void shuffle(std::vector<T>& v, double cnt = 10) { //TODO
        if (v.empty()) return;
        for (int i = 0; i < cnt * v.size(); ++i) {
            int j = rnd.next(0, int(v.size()) - 1);
            int h = rnd.next(0, int(v.size()) - 1);
            std::swap(v[j], v[h]);
        }
    }


    template <typename T>
    void add_random_place(std::deque<T>& d, const T& t) { //TODO
        d.push_back(t);
        int j = rnd.next(0, int(d.size()) - 1);
        std::swap(d[j], d.back());
    }

    template <typename T>
    T& get_random_element(const std::deque<T>& d) { //TODO
        if (d.empty()) return -1;
        int j = rnd.next(0, int(d.size()) - 1);
        return d[j];
    }

    template <typename T>
    T get_and_erase_random_element(std::deque<T>& d) { //TODO
        if (d.empty()) return -1;
        int j = rnd.next(0, int(d.size()) - 1);
        int ans = d[j];
        std::swap(d[j], d.back());
        d.pop_back();
        return ans;
    }
} //namespace gen_funcions


class Tree {
private:
    int n;
    std::vector<std::vector<int> > g;

    void dfs_get_parents(int v, std::vector<int>& pr, int p = -1) {
        pr[v] = p;
        for (auto to : g[v]) {
            if (to != p) dfs_get_parents(to, pr, v);
        }
    }

public:
    Tree(int maxn) {
        n = rnd.next(1, maxn);
        g.resize(n);
    }



    void gen_random() {
        for (int i = 1; i < n; ++i) {
            int pr = rnd.next(0, i - 1);
            g[pr].push_back(i);
            g[i].push_back(pr);
        }
        renumerate();
    }

    void gen_sun() {
        for (int i = 1; i < n; ++i) {
            g[0].push_back(i);
            g[i].push_back(0);
        }
        if (rnd.next(0, 1)) renumerate();
    }

    void gen_bambuk() {
        for (int i = 1; i < n; ++i) {
            g[i - 1].push_back(i);
            g[i].push_back(i - 1);
        }
        renumerate();
    }

    void gen_sqrt_children() { // generate tree where any vertex has one or sqrt(n) children
        int k = ceil(sqrt(n));
        int cnt_main = 1;
        std::deque<int> main_vertexes;
        for (int i = 0; i < k; ++i) {
            gen_functions::add_random_place(main_vertexes, 0);
        }
        for (int i = 1; i < n; ++i) {
            int pr = gen_functions::get_and_erase_random_element(main_vertexes);
            g[i].push_back(pr);
            g[pr].push_back(i);
            if (/*rnd.next(0, main_vertexes.size()) <= 1 && */cnt_main < k) {
                for (int j = 0; j < k; ++j) {
                    gen_functions::add_random_place(main_vertexes, i);
                }
                ++cnt_main;
            }
        }
        renumerate();
    }

    void gen_small_height(double coeff = 10) { // generate parent for vertex i from [0; i / coeff]
        if (coeff == 0) throw("coeff must not be zero");

        for (int i = 1; i < n; ++i) {
            int pr = rnd.next(0, std::min(i - 1, (int)ceil(i / coeff)));
            g[i].push_back(pr);
            g[pr].push_back(i);
        }
        renumerate();
    }

    void gen(int random_part = 4, int sun_part = 1, int bambuk_part = 1, int sqrt_part = 1, int small_height_part = 1) { //TODO
        int sum = random_part + sun_part + bambuk_part + sqrt_part + small_height_part;
        int k = rnd.next(1, sum);
        int curr_sum = random_part;
        if (k <= curr_sum) {
            gen_random();
            return;
        }
        
        curr_sum += sun_part;
        if (k <= curr_sum) {
            gen_sun();
            return;
        }

        curr_sum += bambuk_part;
        if (k <= curr_sum) {
            gen_bambuk();
            return;
        }

        curr_sum += sqrt_part;
        if (k <= curr_sum) {
            gen_sqrt_children();
            return;
        }

        gen_small_height();
    }



    void print() {
        std::cout << n << '\n';
        std::vector<std::pair<int, int> > edges;
        for (int i = 0; i < n; ++i) {
            for (auto j : g[i]) {
                edges.push_back({i + 1, j + 1});
            }
        }
        gen_functions::shuffle(edges);
        for (auto e : edges) std::cout << e.first << ' ' << e.second << '\n';
    }

    void print_parents(int start_vertex = 0, bool print_for_start_vertex = false) {
        std::vector<int> pr = get_parents(start_vertex);
        std::cout << n << '\n';
        bool need_space = false;
        for (int i = 0; i < n; ++i) {
            if (i != start_vertex || print_for_start_vertex) {
                if (need_space) std::cout << ' ';
                std::cout << pr[i];
                need_space = true;
            }
        }
    }

    void print_children(int start_vertex = 0) {
        std::vector<int> pr = get_parents(start_vertex);
        std::cout << n << '\n';
        for (int i = 0; i < n; ++i) {
            int cnt = 0;
            for (auto j : g[i]) {
                if (j != pr[i]) ++cnt;
            }

            std::cout << cnt;
            for (auto j : g[i]) {
                if (j != pr[i]) std::cout << ' ' << j + 1;
            }
            std::cout << '\n';
        }
    }



    std::vector<int> get_parents(int start_vertex) {
        std::vector<int> pr(n);
        dfs_get_parents(start_vertex, pr);
        return pr;
    }



    void renumerate() {
        std::vector<int> v;
        for (int i = 0; i < n; ++i) v.push_back(i);
        gen_functions::shuffle(v);
        std::vector<std::vector<int> > new_g(n);
        for (int i = 0; i < n; ++i) {
            for (auto& j : g[i]) j = v[j];
        }
        for (int i = 0; i < n; ++i) {
            new_g[v[i]] = std::move(g[i]);
        }
        g = new_g;
    }
};


int main(int argc, char** argv) {
    registerGen(argc, argv, 1);
    int maxn = opt<int>("maxn");

    Tree t(maxn);
    t.gen();
    t.print_children();

    return 0;
}
