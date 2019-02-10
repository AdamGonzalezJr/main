#include <iostream>
#include <vector>
#include <algorithm>
#include <limits.h>
#include <list>
#include <iterator>
#define INF INT_MAX

using namespace std;
int height,startX,startY;
int width,goalX,goalY;
struct Node
{
    /* data */
    int x_cordinates = 0;
    int y_cordinates = 0;
    char currentState = 0;
    int gScore = 0;
    int fScore = 0;
    bool closed;
    Node* cameFrom;
   Node(int x, int y, char state, int scoreF, int scoreG, Node* next, bool setCheck)
    {
        x_cordinates = x;
        y_cordinates = y;
        currentState = state;
        fScore = scoreF;
        gScore = scoreG;
        cameFrom = next;
        closed = setCheck;

    }
};


vector <Node*> getNeighbors(Node* current, Node*** arr);
void AStarSearch(int startX, int startY, int goalX, int goalY, Node*** arr);
int heuristic(int x, int y, int xGoal, int yGoal);
int distance(Node* current);
void reconstructPath(Node* current, Node* start, Node*** arr);
Node* change(Node* current, Node*** &arr, Node* start);



int main(int argc, char* argv[]){
   width = stoi(argv[1]);
   height = stoi(argv[2]);
   startX = 0, startY = 0, goalX = height - 1, goalY = width - 1;
   char state;

   if(argc > 3){
      startX = stoi(argv[3]);
      startY = stoi(argv[4]);
   }
   if(argc > 5){
      goalX = stoi(argv[6]);
      goalY = stoi(argv[5]);
   }

   Node*** grid = new Node**[width];
   for(int i = 0; i < width; i++){
      grid[i] = new Node*[height];
   }
   for(int i = 0; i < height; i++){
      for(int j = 0; j < width; j++){
         cin >> state;
         grid[i][j] = new Node(i,j,state, 0, 0, NULL, false);
      }
      cin.ignore();
   }
   AStarSearch(startX, startY, goalX, goalY, grid);
   return 0;
}

int heuristic(int x, int y, int xGoal, int yGoal){
   return abs(x - xGoal) + abs(y - yGoal);
}

void AStarSearch(int startX, int startY, int goalX, int goalY, Node*** arr){
  Node* current;

  list <Node*> openSet;
  list <Node*> closedSet;
  vector <Node*> neighbors;

  openSet.push_front(arr[startX][startY]);
  arr[startX][startY] -> gScore = 0;
  arr[startX][startY] -> fScore = heuristic(startX, startY, goalX, goalY);


  while(!openSet.empty()){
    auto lowFscore = [] (const Node* f1, const Node* f2) -> bool
      {
          return f1 -> fScore < f2 -> fScore;
      };

      openSet.sort(lowFscore);
      current = openSet.front();

      if(current == arr[goalX][goalY]){
        reconstructPath(current, arr[startX][startY], arr);
        return;
      }
      openSet.remove(current);
      current -> closed = true;
      closedSet.push_front(current);

      neighbors = getNeighbors(current, arr);
      for(int i = 0; i < neighbors.size(); i++){
        if(neighbors[i] -> closed == true){
          continue;
        }

        int score = current -> gScore + distance(neighbors[i]);

        if(neighbors[i] -> closed == false){
          openSet.push_front(neighbors[i]);
        }
        else if(score >= neighbors[i] -> gScore){
          continue;
        }
        neighbors[i] -> cameFrom = current;
        neighbors[i] -> gScore = score;
        neighbors[i] -> fScore = neighbors[i] -> gScore + heuristic(neighbors[i] -> x_cordinates, neighbors[i] -> y_cordinates, goalX, goalY);

      }
  }

}

vector <Node*> getNeighbors(Node* current, Node*** arr){
  int x = current -> x_cordinates;
  int y = current -> y_cordinates;
  vector <Node*> neighbors;
  if(current -> x_cordinates - 1 >= 0){
    neighbors.push_back(arr[x-1][y]);
  }
  if(current -> x_cordinates + 1 < width){
    neighbors.push_back(arr[x+1][y]);
  }
  if(current -> y_cordinates + 1 < height){
    neighbors.push_back(arr[x][y+1]);
  }
  if(current -> y_cordinates - 1 >= 0){
    neighbors.push_back(arr[x][y-1]);
  }
  // if(current -> x_cordinates - 1 >= 0){
  //   neighbors.push_back(arr[x-1][y]);
  // }
  // if(current -> x_cordinates + 1 < width){
  //   neighbors.push_back(arr[x+1][y]);
  // }
  return neighbors;
}

int distance(Node* current){
  int total = 0;
  if(current -> currentState == '.'){
    total = 1;
      return total;
    }
    if(current -> currentState == '*'){
      total = 3;
      return total;
    }
    if(current -> currentState == 'o'){
      total = 5;
      return total;
    }
    if(current -> currentState == '~'){
      total = 7;
      return total;
    }
    if(current -> currentState == 'x' )
      return INT_MAX;
    return 0;
}
void reconstructPath(Node* current, Node* start, Node*** arr){
  list <Node*> temp;
  while(current != start){
      temp.push_front(change(current, arr, start));
      current = current -> cameFrom;
    }
    arr[startX][startY] -> currentState = 'S';
    arr[goalX][goalY] -> currentState = 'G';
    for(int i = 0; i < height; i++){
      for(int j = 0; j < width; j++){
        cout << arr[i][j] -> currentState;
      }
      cout << endl;
    }

}

Node* change(Node* current, Node*** &arr, Node* start){

  int x = current -> x_cordinates;
  int x1 = current -> cameFrom -> x_cordinates;
  int y = current -> y_cordinates;
  int y1 = current -> cameFrom -> y_cordinates;

  if(x1 == x - 1 && current -> cameFrom != start){
    arr[x1][y1] -> currentState = 'v';
    return current;
  }
  if(y1 == y - 1 && current -> cameFrom != start){
    arr[x1][y1] -> currentState = '>';
    return current;
  }
  if(x1 == x + 1 && current -> cameFrom != start){
    arr[x1][y1] -> currentState = '^';
    return current;
  }
  if(y1 == y + 1 && current -> cameFrom != start){
    arr[x1][y1] -> currentState = '<';
    return current;
  }
  return NULL;
}
