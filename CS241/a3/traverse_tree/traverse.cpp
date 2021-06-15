#include <iostream>
#include <vector>
using namespace std;

int printPost(vector<vector<int>> &trees, int node) {
    if (trees[node][1] == 0) {
        cout << trees[node][0] << " " << trees[node][1] << endl;
        return 0;
    } else {
        int numOfChild = trees[node][1];
        int total = 0;
        for (int i = 1; i <= numOfChild; i++) {
            total += printPost(trees, node + i + total);
        }
        cout << trees[node][0] << " " << trees[node][1] << endl;
        total += numOfChild;
        return total;
    }
}

int main() {
    vector<vector<int>> trees;
    
    while (true) {
        int value,numOfChild;
        cin >> value ;
        if (cin.eof()) break;
        cin >> numOfChild;
        vector<int> node;
        node.push_back(value);
        node.push_back(numOfChild);
        trees.push_back(node);
    }
    
    printPost(trees, 0);
}
