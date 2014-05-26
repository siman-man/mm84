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
const int EMPTY = -3;

const int MAX_N = 1000;

const int DX[4] = { -1, 1, 0, 0 };
const int DY[4] = { 0, 0, -1, 1 };

unsigned long long xor128() {
  static unsigned long long rx=123456789, ry=362436069, rz=521288629, rw=88675123;
  unsigned long long rt = (rx ^ (rx<<11));
  rx=ry; ry=rz; rz=rw;
  return (rw=(rw^(rw>>19))^(rt^(rt>>8)));
}

int LY[MAX_N];
int LX[MAX_N];
int RY[MAX_N];
int RX[MAX_N];

struct fieldInfo {
  int ly[MAX_N];
  int lx[MAX_N];
  int ry[MAX_N];
  int rx[MAX_N];
  int eval;

  bool operator >(const fieldInfo &e) const{
    return eval > e.eval;
  }    
};

struct rect_t {
  rect_t(short i = -1, short h = -1, short w = -1){
    id = i;
    height = h;
    width = w;
  }

  short id;
  short height;
  short width;

  bool operator<(const rect_t &e) const {
    return height*width > e.height*e.width;
  }
};

int N, setCnt;

vector<int> YS;
vector<int> XS;

vector<rect_t> rect_list(MAX_N);
vector< vector<int> > field;

class RectanglesAndHoles {
  public:

    /*
     * A = height, B = width.
     */
    vector<int> place( vector<int> A, vector<int> B ){
      N = A.size();
      vector<int> ret(3*N);

      init( A, B );

      for(int i = 0; i < 3*N; i++){
        ret[i*3] = xor128() % 1000000 * ( xor128()%2 )? -1 : 1;
        ret[i*3+1] = xor128() % 1000000 * ( xor128()%2 )? -1 : 1;
        ret[i*3+2] = 0;

        LY[i] = ret[i*3];
        LX[i] = ret[i*3];
        RY[i] = LY[i] + ( ret[i*3+2] )? A[i] : B[i];
        RX[i] = LX[i] + ( ret[i*3+2] )? B[i] : A[i];
      }
 
      /*
      for(int i = 0; i < N; i++){
        rect_t rect = rect_list[i];
        fprintf( stderr, "id = %d: height = %d, width = %d, area = %d\n",
            rect.id, rect.height, rect.width, rect.width * rect.height );
      }
      */
      fprintf( stderr, "N = %d\n", N );
      calcScore();

      return ret;
    };

    void init( vector<int> &A, vector<int> &B ){
      setCnt = 0;

      memset( LY, -1, sizeof(LY) );
      memset( LX, -1, sizeof(LX) );
      memset( RY, -1, sizeof(RY) );
      memset( RX, -1, sizeof(RX) );

      for(int i = 0; i < N; i++){
        rect_list[i] = rect_t( i, A[i], B[i]);
      }

      sort( rect_list.begin(), rect_list.begin()+N );
    }

    void gravity( int id, int rot ){
      int fromY = lower_bound( YS.begin(), YS.end(), LY[id] ) - YS.begin();
      int fromX = lower_bound( XS.begin(), XS.end(), LX[id] ) - XS.begin();

      vector< vector<int> > tmp_field;

      tmp_field = field;

      while( field[fromY][fromX] != EMPTY ){
      }
    }

    void createField(){
      YS = enumerateCoordinates( LY, RY );
      XS = enumerateCoordinates( LX, RX );

      vector< vector<int> > new_field(YS.size(), vector<int>(XS.size(), UNKNOWN ));
      field = new_field;

      for(int i = 0; i < N; i++){
        int fromY = lower_bound( YS.begin(), YS.end(), LY[i] ) - YS.begin();
        int fromX = lower_bound( XS.begin(), XS.end(), LX[i] ) - XS.begin();
        int   toY = lower_bound( YS.begin(), YS.end(), RY[i] ) - YS.begin();
        int   toX = lower_bound( XS.begin(), XS.end(), RX[i] ) - XS.begin();

        for(int y = fromY; y < toY; y++){
          for(int x = fromX; x < toX; x++){
            if( y == 0 || y == toY-1 || x == 0 || x == toX-1 ){
              field[y][x] = EMPTY;
            }else{
              field[y][x] = OBSTACLE;
            }
          }
        }
      }
    }

    ll calcScore(){

      createField();

      int ysize = YS.size();
      int xsize = XS.size();

      int cells = (ysize-1) * (xsize-1);

      int compCnt = 0;

      int QX[cells];
      int QY[cells];

      int qBeg = 0, qEnd = 0;

      for(int y = 0; y < ysize-1; y++){
        for(int x = 0; x < xsize-1; x++){
          if( field[y][x] == UNKNOWN ){
            QX[qBeg] = x;
            QY[qBeg++] = y;
            field[y][x] = compCnt;

            while( qEnd < qBeg ){
              int curX = QX[qEnd];
              int curY = QY[qEnd++];

              for(int d = 0; d < 4; d++){
                int nextX = curX + DX[d];
                int nextY = curY + DY[d];

                if( nextY >= 0 && nextY < ysize && nextX >= 0 && nextY < xsize && field[nextY][nextX] == UNKNOWN ){
                  QX[qBeg] = nextX;
                  QY[qBeg++] = nextY;
                  field[nextY][nextX] = compCnt;
                }
              }
            }

            compCnt++;
          }
        }
      }

      ll totArea = 0;

      for(int y = 0; y < ysize; y++){
        for(int x = 0; x < xsize; x++){
          if( field[y][x] > 0 ){
            totArea += (XS[x+1]-XS[x]) * (YS[y+1]-YS[y]);
          }
        }
      }

      compCnt--;

      fprintf( stderr, "compCnt = %d\n", compCnt );
      fprintf( stderr, "totArea = %lld\n", totArea );
      fprintf( stderr, "Current Score = %lld\n", totArea * compCnt * compCnt );

      return totArea * compCnt * compCnt;
    }

    bool overlap( int a, int b, int c, int d ){
      return max( a, c ) < min( b, d );
    }

    bool checkOverLap( int ly, int lx, int ry, int rx ){
      for(int i = 0; i < setCnt; i++){
        if( overlap( LY[i], RY[i], ly, ry ) && overlap( LX[i], RX[i], lx, rx ) ){
          return false;
        }
      }

      return true;
    }

    void setRect( int id, int y, int x, int rot ){
      LY[id] = y;
      LX[id] = x;
      RY[id] = y + ( rot )? rect_list[id].height : rect_list[id].width;
      RX[id] = y + ( rot )? rect_list[id].width : rect_list[id].height;
    }

    vector<int> enumerateCoordinates( int *A, int *B ){
      int min_value = INT_MAX;
      int max_value = INT_MIN;

      set<int> coords;

      for(int i = 0; i <= setCnt; i++){
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
