#include <iostream>
#include <ctime>
#include <cstdlib>
#include <fstream>
#include <queue>
#include <vector>
#include <utility>
using namespace std;

double prob(){
    return static_cast<double> (rand())/RAND_MAX;
}

struct node{
    int num;
    int cost;

    node(int n, int c) : num(n), cost(c){};
    bool operator>(const node& other) const{
        return cost > other.cost;
    }
};

int main()
{
    int size=15; double density;
    cout << "graph size ?" <<endl;
    cin >> size;
    cout << "graph density (0,1)?" <<endl;
    cin >> density;
    bool** graph;
    int** color;
    int** cost;
    srand(time(0));
    graph = new bool*[size];
    color = new int*[size];
    cost = new int*[size];
    for (int i=0; i<size; i++){
        graph[i] = new bool[size];
        color[i] = new int[size];
        cost[i] = new int[size];
    }
    // generate undirected graph
    for (int i=0; i<size; ++i){
        for (int j=i; j<size; ++j){
            if (i == j) graph[i][j] = false;
            else graph[i][j] = graph[j][i] = (prob() < density);
        }
    }
    for (int i=0; i<size; ++i){
        for (int j=i; j<size; ++j){
            if (graph[i][j]){
                color[i][j] = color[j][i] = rand() % 3 + 1;
                cost[i][j] = cost[j][i] = prob()*30;
            }
        }
    }
    ofstream outp("data.txt");
    outp << size << "\n";
    for (int i=0; i<size; ++i){
        for (int j=0; j<size; ++j){
            if (graph[i][j])
                outp << i << "->" << j << " cost : " << cost[i][j] << endl;
        }
    }


    //MST
    int start = rand() % size;
    priority_queue<node, vector<node>, greater<node>> pq;
    bool* visited = new bool[size];
    int num_visited = 0;
    for (int i = 0; i < size; i++)
    {
        visited[i] = false;
    }
    visited[start] = true;
    num_visited++;
    vector<vector<int> *> MST;
    for (int i=0; i<size; ++i){
        MST.push_back(new vector<int>);
    }
    int curr = start;
    for(int i=0; i<size; i++){
        if (graph[curr][i]){
            pq.push(node(i, cost[curr][i]));
        }
    }

    while(!pq.empty() && (num_visited < size)){
        int next_node = pq.top().num;
        int next_cost = pq.top().cost;
        pq.pop();

        if (!visited[next_node]){
            visited[next_node] = true;
            num_visited++;
            MST[curr]
                ->push_back(next_node);
            curr = next_node;
            for (int i = 0; i < size; i++)
            {
                if (graph[curr][i])
                {
                    pq.push(node(i, cost[curr][i]));
                }
            }
        }else{
            continue;
        }
    }
    cout << "MST is:" << endl;
    for (int i = 0; i < MST.size(); i++)
    {
        for (int j = 0; j < MST[i]->size(); j++)
        {
            int next_node = (*MST[i])[j];
            int edge_cost = cost[i][next_node];
            cout << i << "-> " << next_node << " cost : " << edge_cost << endl;
        }
    }

    for (int i = 0; i < size; i++)
    {
        delete[] graph[i];
        delete[] color[i];
        delete[] cost[i];
        delete MST[i];
    }
    delete[] graph;
    delete[] color;
    delete[] cost;
    delete[] visited;
    return 0;
}