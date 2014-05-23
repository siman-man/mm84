#include <iostream>
#include <vector>
#include <map>
#include <algorithm>
#include <limits.h>
#include <string>
#include <string.h>
#include <sstream>
#include <set>
#include <cstdio>
#include <cstdlib>
#include <cmath>
#include <stack>
#include <queue>

using namespace std;

typedef long long ll;

const int UNKNOWN = -1;
const int OBSTACLE = -2;

unsigned long long xor128() {
  static unsigned long long rx=123456789, ry=362436069, rz=521288629, rw=88675123;
  unsigned long long rt = (rx ^ (rx<<11));
  rx=ry; ry=rz; rz=rw;
  return (rw=(rw^(rw>>19))^(rt^(rt>>8)));
}

class RectanglesAndHoles {
  public:
    vector<int> place( vector<int> A, vector<int> B ){
      int n = A.size();
      vector<int> ret(3*n);

      for(int i = 0; i < 3*n; i++){
        ret[i*3] = xor128() % 400000;
        ret[i*3+1] = xor128() % 400000;
        ret[i*3+2] = 0;
      }

      return ret;
    };

    int calcScore( vector<int> LX, vector<int> RX, vector<int> LY, vector<int> RY ){
      vector<int> XS = enumerateCoordinates( LX, RX );
      vector<int> YS = enumerateCoordinates( LY, RY );

      int map[XS.size()-1][YS.size()-1];
    }

    vector<int> enumerateCoordinates( vector<int> A, vector<int> B ){
      int min_value = INT_MAX;
      int max_value = INT_MIN;
      int size = A.size();

      set<int> coords;

      for(int i = 0; i < size; i++){
        max_value = max( max_value, max( A[i], B[i] ) );
        min_value = min( min_value, min( A[i], B[i] ) );

        coords.insert(A[i]);
        coords.insert(B[i]);
      }

      set<int>::iterator it = coords.begin();
      
      vector<int> res;

      while( it != coords.end() ){
        res.push_back(*it);
        it++;
      }

      sort(res.begin(), res.end());

      return res;
    }
};

int main(){
  int n;
  cin >> n;
  RectanglesAndHoles rh;
  vector<int> A(n);
  vector<int> B(n);

  for(int i = 0; i < n; i++){
    cin >> A[i];
  }

  for(int i = 0; i < n; i++){
    cin >> B[i];
  }

  vector<int> ret = rh.place( A, B );

  for(int i = 0; i < 3*n; i++){
    printf("%d\n", ret[i] );
  }

  return 0;
}
