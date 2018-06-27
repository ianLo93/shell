#include <iostream>
#include <cstdlib>
#include <fstream>
#include <vector>
#include <climits>
#define N 200

using namespace std;

vector<pair<bool, int> > mark;

void percolate_up (vector<pair<int, int> >& G_x, int pos);
void percolate_down (vector<pair<int, int> >& G_x, int pos);
void insert (vector<pair<int, int> >& G_x, pair<int, int>& node);
pair<int,int> pop (vector<pair<int, int> >& G_x);
void erase (vector<pair<int, int> >& G_x, int pos);

int main(int argc, char* argv[]) {
  if (argc != 2) {
    cerr << "Command failed.." << endl;
    return 1;
  }
  
  FILE* input = fopen(argv[1], "r");
  if (!input) {
    cerr << "open() failed.." << endl;
    return 1;
  }

  int head, tail, d, k;
  vector<pair<int, int> > temp;
  vector<vector<pair<int, int> > > G(N, temp);
  vector<pair<int, int> > G_x;
  while (1) {
    k = fscanf(input, "%d,%d", &tail, &d);
    if (k == 1)
      head = tail-1;
    else if (k == 2)
      G[head].push_back( make_pair(tail-1, d) );
    else break;
  }

  mark = vector<pair<bool, int> >( N, make_pair(false, 0) );
  G_x.push_back(make_pair(0, 0));
  mark[0].second = 0;
  for (int i=1; i<N; i++) {
    G_x.push_back(make_pair(i, INT_MAX));
    mark[i].second = i;
  }
  vector<int> G_y(N, 0);
  while(G_x.size() > 0) {
    auto x = pop(G_x);
    int num = x.first;
    int dijVal = x.second;
    G_y[num] = dijVal;
    for (int i=0; i<G[num].size(); i++) {
      int next = G[num][i].first;
      int dist = G[num][i].second;
      if (mark[next].first) continue;
      int pos = mark[next].second;
      int next_dijVal = dijVal+dist;
      if (next_dijVal < G_x[pos].second) {
        erase(G_x, pos);
        auto new_x = make_pair(next, next_dijVal);
        insert(G_x, new_x);
      }
    }
  }
  vector<int> thePos = {7,37,59,82,99,115,133,165,188,197};
  int i=0;
  for (; i<thePos.size()-1; i++) {
    cout << G_y[thePos[i]-1] << ",";
  }
  cout << G_y[thePos[i]-1] << endl;

  return 0;
}

void percolate_up (vector<pair<int, int> >& G_x, int pos) {
  if (pos == 0) return;
  while (G_x[(pos-1)/2].second > G_x[pos].second) {
    mark[G_x[(pos-1)/2].first].second = pos;
    mark[G_x[pos].first].second = (pos-1)/2;
    swap(G_x[(pos-1)/2], G_x[pos]);
    pos = (pos-1)/2;
    if (pos == 0) break;
  }
}

void percolate_down (vector<pair<int, int> >& G_x, int pos) {
  while ( 2*pos+1 < G_x.size() ) {
    int child;
    if (2*pos+2 < G_x.size() && G_x[2*pos+2].second < G_x[2*pos+1].second)
      child = 2*pos+2;
    else
      child = 2*pos+1;
    if (G_x[child].second < G_x[pos].second) {
      mark[G_x[child].first].second = pos;
      mark[G_x[pos].first].second = child;
      swap(G_x[pos], G_x[child]);
      pos = child;
    } else break;
  }
}

pair<int,int> pop(vector<pair<int, int> > & G_x) {
  int n = G_x.size()-1;
  pair<int, int> ans = G_x[0];
  mark[G_x[0].first].first = true;
  mark[G_x[n].first].second = 0;
  swap(G_x[0], G_x[n]);
  G_x.pop_back();
  percolate_down(G_x, 0);
  return ans;
}

void insert(vector<pair<int, int> > & G_x, pair<int, int>& node) {
  int n = G_x.size();
  G_x.push_back(node);
  mark[node.first].second = n;
  percolate_up(G_x, n);
}

void erase(vector<pair<int, int> > & G_x, int pos) {
  int n = G_x.size()-1;
  mark[G_x[n].first].second = pos;
  swap(G_x[n], G_x[pos]);
  G_x.pop_back();
  percolate_up(G_x, pos);
  percolate_down(G_x, pos);
}
