/**
#include <algorithm>
#include <vector>
#include <iostream>

using namespace std;

class Edge {
public:
    int u, v, weight;
    Edge(int u, int v, int weight) : u(u), v(v), weight(weight) {}
    bool operator<(Edge const& other) {
        return weight < other.weight;
    }
};

class DisjointSet {
private:
    vector<int> parent, rank;
public:
    DisjointSet(int n) {
        parent.resize(n);
        rank.resize(n);
        for (int i = 0; i < n; i++) {
            parent[i] = i;
            rank[i] = 0;
        }
    }
    int find_set(int v) {
        if (v == parent[v])
            return v;
        return parent[v] = find_set(parent[v]);
    }
    void union_sets(int a, int b) {
        a = find_set(a);
        b = find_set(b);
        if (a != b) {
            if (rank[a] < rank[b])
                swap(a, b);
            parent[b] = a;
            if (rank[a] == rank[b])
                rank[a]++;
        }
    }
};

vector<Edge> kruskal(vector<Edge>& edges, int n) {
    DisjointSet ds(n);
    sort(edges.begin(), edges.end());
    vector<Edge> result;
    for (Edge e : edges) {
        if (ds.find_set(e.u) != ds.find_set(e.v)) {
            result.push_back(e);
            ds.union_sets(e.u, e.v);
        }
    }
    return result;
}

int main() {
    int n = 5;
    vector<Edge> edges = {
            Edge(0, 1, 1),
            Edge(0, 2, 3),
            Edge(1, 2, 2),
            Edge(1, 3, 3),
            Edge(2, 3, 1),
            Edge(3, 4, 4)
    };

    vector<Edge> mst = kruskal(edges, n);

    cout << "Edges of MST are \n";
    for (Edge e : mst) {
        cout << e.u << " -- " << e.v << " == " << e.weight << endl;
    }

    return 0;
}
**/
#include <cstdio>
#include <string>
#include <vector>
#include <iostream>
#include <chrono>
#include <fstream>
#include <cstdlib>
#include <ctime>
#include <random>
#include <algorithm>


using namespace std;

// Define the Edge class
class Edge {
public:
    int u, v, weight; // Each edge has two vertices (u, v) and a weight
    Edge(int u, int v, int weight) : u(u), v(v), weight(weight) {} // Constructor
    bool operator<(Edge const& other) const { // Overload the < operator for sorting
        return weight < other.weight;
    }
};

// Define the DisjointSet class
class DisjointSet {
private:
    vector<int> parent, rank; // Each element has a parent and a rank
    int num_sets; // The number of disjoint sets
public:
    explicit DisjointSet(int n) : num_sets(n) { // Constructor
        parent.resize(n);
        rank.resize(n);
        for (int i = 0; i < n; i++) {
            parent[i] = i; // Initially, each element is its own parent
            rank[i] = 0; // The rank is 0
        }
    }
    int find_set(int v) { // Find the parent of v
        if (v == parent[v])
            return v;
        return parent[v] = find_set(parent[v]); // Path compression
    }
    void union_sets(int a, int b) { // Merge the sets containing a and b
        a = find_set(a);
        b = find_set(b);
        if (a != b) {
            if (rank[a] < rank[b])
                swap(a, b); // Ensure that a is the set with higher rank
            parent[b] = a; // Attach the set of b to the set of a
            if (rank[a] == rank[b])
                rank[a]++; // Increase the rank of a if necessary
            num_sets--; // Decrease the number of sets
        }
    }
    int get_num_sets() const { // Get the number of disjoint sets
        return num_sets;
    }
};

// Define the Graph class
class Graph {
private:
    vector<Edge> edges; // The edges of the graph
    int n; // The number of vertices
public:
    explicit Graph(int n) : n(n) {} // Constructor
    void add_edge(int u, int v, int weight) { // Add an edge to the graph
        if (u < 0 || v < 0 || u >= n || v >= n) { // Check if the vertices are valid
            cout << "Invalid edge: " << u << " -- " << v << ". Vertices should be between 0 and " << n-1 << ".\n";
            return;
        }
        if (weight < 0) { // Check if the weight is valid
            cout << "Warning: negative weight: " << weight << ".\n";
        }
        edges.emplace_back(u, v, weight); // Add the edge
    }
    void kruskal() { // Implement the Kruskal's algorithm
        auto start = chrono::high_resolution_clock::now(); // Start the timer

        DisjointSet ds(n); // Create a disjoint set data structure
        sort(edges.begin(), edges.end()); // Sort the edges by weight
        vector<Edge> result; // The edges of the minimum spanning tree
        int total_weight = 0; // The total weight of the minimum spanning tree
        for (Edge e : edges) { // For each edge
            if (ds.find_set(e.u) != ds.find_set(e.v)) { // If the edge connects two different trees
                result.push_back(e); // Add the edge to the minimum spanning tree
                ds.union_sets(e.u, e.v); // Merge the two trees
                total_weight += e.weight; // Add the weight of the edge to the total weight
            }
        }

        auto stop = chrono::high_resolution_clock::now(); // Stop the timer
        auto duration = chrono::duration_cast<chrono::microseconds>(stop - start); // Calculate the duration

        cout << "Edges of MST are \n"; // Print the edges of the minimum spanning tree
        for (Edge e : result) {
            cout << e.u << " -- " << e.v << " == " << e.weight << endl;
        }
        cout << "Total weight of MST is " << total_weight << endl; // Print the total weight
        cout << "Time taken: " << duration.count() << " microseconds" << endl; // Print the time taken

        // Check if the graph is acyclic
        int num_sets = ds.get_num_sets();
        if (result.size() == n - num_sets) {
            cout << "The graph is acyclic.\n";
        } else {
            cout << "The graph is not acyclic.\n";
        }
    }
//    void plot() {
//        FILE* gnuplotPipe = popen("gnuplot -persistent", "w");
//        fprintf(gnuplotPipe, "set title 'Graph'\n");
//        for (Edge e : edges) {
//            fprintf(gnuplotPipe, "plot '-' with lines\n");
//            fprintf(gnuplotPipe, "%d %d\n", e.u, e.weight);
//            fprintf(gnuplotPipe, "e\n");
//        }
//        fflush(gnuplotPipe);
//    }
};

// Generate a random graph
void generate_random_graph(int n, Graph& g) {
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dis(1, 100);

    // Ensure the graph is connected by creating a random spanning tree
    for (int i = 1; i < n; i++) {
        int u = i;
        int v = rand() % i; // Ensure there's a path from 0 to i
        int w = dis(gen); // Random weight between 1 and 100
        g.add_edge(u, v, w);
    }

    // Add additional edges
    int m = rand() % (n * (n - 1) / 2); // Random number of edges
    for (int i = 0; i < m; i++) {
        int u = rand() % n;
        int v = rand() % n;
        if (u != v) {
            int w = dis(gen); // Random weight between 1 and 100
            g.add_edge(u, v, w);
            cout << "Added edge: " << u << " -- " << v << " == " << w << endl;
        }
    }
}

int main() {
    while (true) {
        cout << "Enter 1 to manually input the graph, or 2 to generate a random graph: ";
        int option;
        if (!(cin >> option)) {
            cout << "Invalid input. Please enter 1 or 2.\n";
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            continue;
        }

        int n, m;
        if (option == 1) {
            cout << "Enter the number of vertices: ";
            if (!(cin >> n)) {
                cout << "Invalid input. Please enter an integer.\n";
                cin.clear();
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
                continue;
            }
            cout << "Enter the number of edges: ";
            if (!(cin >> m)) {
                cout << "Invalid input. Please enter an integer.\n";
                cin.clear();
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
                continue;
            }

            Graph g(n);
            cout << "Enter the edges (u, v, weight):\n";
            for (int i = 0; i < m; i++) {
                int u, v, w;
                if (!(cin >> u >> v >> w)) {
                    cout << "Invalid input. Please enter three integers.\n";
                    cin.clear();
                    cin.ignore(numeric_limits<streamsize>::max(), '\n');
                    i--; // Repeat the same iteration
                    continue;
                }
                g.add_edge(u, v, w);
            }

            g.kruskal();
        } else if (option == 2) {
            cout << "Enter the number of vertices: ";
            if (!(cin >> n)) {
                cout << "Invalid input. Please enter an integer.\n";
                cin.clear();
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
                continue;
            }

            Graph g(n);
            generate_random_graph(n, g);

            g.kruskal();
        } else {
            cout << "Invalid option. Please enter 1 or 2.\n";
            continue;
        }

        char choice;
        cout << "Do you want to continue? (y/n): ";
        cin >> choice;
        if (choice == 'n' || choice == 'N') {
            break;
        }
    }

    return 0;
}
