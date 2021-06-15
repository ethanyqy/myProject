//
//  main.cpp
//  CS341-A6
//
//  Created by Calvin Pan on 2020-07-30.
//  Copyright © 2020 Calvin Pan. All rights reserved.
//

#include <iostream>
#include <map>
#include <vector>
#include <queue>

using namespace std;

void printP(int u, int v, vector<int> &predecessor){
    int p = predecessor[u];
    //cout<<"u:"<<u<<" v:"<<v<<" p:"<<p<<endl;
    if(p != v){
        printP(p, v,predecessor);
        cout<<" "<<u;
    } else {
        cout<<v<<" "<<u;
    }
}

bool checkNC(int u, int start,vector<int> &predecessor){
    int p = predecessor[u];
    if(p == -1){
        return false;
    } else if(p == start){
        return true;
    }
    checkNC(p, start, predecessor);
    return false;
}

int main(int argc, const char * argv[]) {
    const int INF = 2000000000;
    int n,m,start;
    cin>>n>>m>>start;
    vector<vector<int>> G;
    vector<vector<int>> W;
    for(int i = 0;i<n;i++){
        G.push_back(vector<int>{});
        W.push_back(vector<int>{});
    }
    for(int i=0;i<m;i++){
        int u,v,w;
        cin>>u>>v>>w;
        W[u].push_back(w);
        G[u].push_back(v);
    }
    vector<int> distance;
    vector<int> predecessor;
    for(int i = 0; i<n;i++){
        distance.push_back(INF);
        predecessor.push_back(-1);
    }
    distance[start] = 0;
    bool changedAny = false;
    int vertexchanged = -1;
    for(int i = 0; i < n+1; i++){
        changedAny = false;
        for(int u = 0; u < n; u++){
            if(distance[u]!=INF){
                int size = G[u].size();
                for(int j = 0;j<size;j++){
                    int v = G[u][j];
                    //cout<<"comparing u:"<<u<<" v:"<<v<<endl;
                    if(distance[u] + W[u][j] < distance[v]){
                        if(i == n){
                            vertexchanged = u;
                        }
                        //cout<<"changes dist u:"<<distance[u]<<" W:"<<W[u][j]<<" v:"<<distance[v]<<endl;
                        changedAny=true;
                        distance[v] = distance[u] + W[u][j];
                        predecessor[v] = u;
                    }
                }
            }
        }
        if(!changedAny){
            break;
        }
    }
    if(changedAny){
        cout<<"ERROR: NEGATIVE WEIGHT CYCLE"<<endl;
        printP(vertexchanged,vertexchanged,predecessor);
        cout<<endl;
        return 0;
    }
    for(auto x:distance){
        if(x == INF){
            cout<<"inf"<<" ";
        } else {
            cout<<x<<" ";
        }
    }
    cout<<endl;
}
