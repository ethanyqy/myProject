#include <iostream>
#include <vector>
using namespace std;

bool func(int *A, int i, int j,int l, vector<int> &P, bool push){
    
    // called from counting i in subset S
    if (push) {
        P.push_back(i);
    }
    
    // base cases:
    if (i < j) {
        P.pop_back();
        return 0;
    }
    if (j == 1) {
        for (int k = 0; k < i; k++) {
            if ((l-A[k]) == 0) {
                P.push_back(k);
                return 1;
            }
        }
        P.pop_back();
        return 0;
    }
   
    return func(A, i-1, j-1, l - A[i-1], P, 1)|| func(A, i-1, j, l, P, 0);;
}


int main() {
    int m,n;
    cin >> n;
    cin >> m;
    int i = n;
    int j = n/2;
    int A[n];
    vector<int> P;
    int totalsum = 0;
    bool t = 0;
    
    for (int k = 0; k < n; k++) {
        cin >> A[k];
        totalsum += A[k];
    }
    
    // to go through the possible picks for P and then check A\P
    for (int l = m*n/4+1; l < m*n/2+1; l++) {
        t = func(A, i, j, l, P, 0);
        if (t) {
            int sum1 = 0;
            for (int k = 0; k < j; k++) {
                sum1 += A[P[k]];
            }
            if ( (totalsum - sum1) > (m*n/4)) {
                break;
            }
        }
        P.clear();
    }
    
    if (!t) {
        cout << "false"<< endl;
    } else {
        cout << "true" << endl;
        for(int i=P.size()-1; i > -1; i--) {
            cout <<(P[i]+1);
            if (i != 0) {
                cout << " ";
            }
        }
    }
    
    
}
