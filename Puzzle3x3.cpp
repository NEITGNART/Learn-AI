#include <iostream>
#include <vector>
#include <cstring>
#include <array>
#include <queue>
#include <unordered_map>

using namespace std;

array<int, 4> dx{1,0,0,-1};
array<int, 4> dy{0,1,-1,0};
const int N = 3;

struct Node {
    Node *parent;
    int matrix[N][N];
    int cost;
    int x, y;
    int height;
};

void PrintMatrix(int Matrix[N][N]) {
    for(int i = 0; i < N; ++i) {
        for(int j = 0; j < N; ++j) {
            cout << Matrix[i][j] << ' ';
        }
        cout << '\n';
    }
}

void Print(Node *root) {
    if (root == nullptr) {
        return;
    }
    Print(root->parent);
    PrintMatrix(root->matrix);
    cout << "\n\n";
}

int CalculateCost(int currentState[N][N], int finalState[N][N]) {
    int count = 0;
    for(int i = 0; i < N; ++i) {
        for(int j = 0; j < N; ++j) {
            if (currentState[i][j] && currentState[i][j] != finalState[i][j]) {
                ++count;
            }
        }
    }
    return count;
}

int Manhattan(int x1, int y1, int x2, int y2) {
    return abs(x1 - x2) + abs(y1 - y2);
}

int Heuristic(int currentState[N][N], int finalState[N][N]) {
    unordered_map<int, array<int,2>> M;
    for(int i = 0; i < N; ++i) {
        for(int j = 0; j < N; ++j) {
            if (finalState[i][j])
                M[finalState[i][j]] = {i,j};
        }
    }
    int total_cost = 0;
    for(int i = 0; i < N; ++i) {
        for(int j = 0; j < N; ++j) {
            if (currentState[i][j]) {
                auto dir = M[currentState[i][j]];
                total_cost += Manhattan(i, j, dir[0], dir[1]); 
            }
        }
    }
    return total_cost;
}

Node *newNode(int matrix[N][N], int x, int y, int dx, int dy, int level, Node* parent) {
    Node *p = new Node();
    p->x = dx;
    p->y = dy;
    p->height = level;
    p->parent = parent;
    p->cost = (1 << 30);
    memcpy(p->matrix, matrix, sizeof(p->matrix));
    std::swap(p->matrix[x][y], p->matrix[dx][dy]);
    return p;

}

void FindSolution(int startState[N][N], int finalState[N][N], int x, int y) {
    
    auto comp =  [&](Node* a, Node* b) { return a->cost + a->height > b->cost + b->height;};
    priority_queue<Node*, vector<Node*>, decltype(comp)> q(comp);
    Node *root = newNode(startState, x, y, x, y, 0, NULL);
//  root->cost = CalculateCost(startState, finalState);
    root->cost = Heuristic(startState, finalState);
    q.push(root);
    auto isSafe = [](int x, int y) -> bool {
        if (x >= 0 && x < N && y >= 0 && y < N) return true;
        return false;
    };
    while (!q.empty()) {
        auto min = q.top(); q.pop();
        
        if (min->cost == 0) {
            Print(min);
            return;
        }
        for(int i = 0; i < 4; ++i) {
            if (isSafe(min->x + dx[i], min->y + dy[i])) {
                Node *p = newNode(min->matrix, min->x,
                                 min->y, min->x + dx[i],
                                 min->y + dy[i],
                                 min->height + 1, min);
                      p->cost = Heuristic(p->matrix, finalState);
                      // p->cost = CalculateCost(p->matrix, finalState);

                q.push(p);
            }
        }
    }
}

int main(void) {
    // We assume that the puzzle solvable, to check it can or can not.Another ways implement effientcy, use the Fenwick tree to calculate
    // the number of inversion. If it odd then the puzzle can't solve
    int initial[N][N] =
    {
        {1, 2, 3},
        {0, 4, 6},
        {7, 5, 8}
    };
    int final[N][N] =
    {
        {1, 2, 3},
        {4, 5, 6},
        {7, 8, 0}
    };
    
    FindSolution(initial, final, 1, 0);
   
    return 1;
}
