//for this version DON'T use the test maze. The wall values are hard coded 
//using maze_wallinput so just run the whole thing.

#include <cmath>
#include <stdlib.h>
#include "floodfill.h"

struct graph m[SIZ][SIZ];
//-----------stack-----------

struct Stack stack;
Stack* s = &stack;
struct vertex c;

//--- Optimal Path Stack ------ //
struct Stack optimal;
Stack* op = &optimal;
struct graph f[SIZ][SIZ];

void Stack_Init(Stack *S)
{
    S->size = 0;
}

struct vertex Stack_Top(Stack *S)
{
  struct vertex v;
  v.x = -1;
  v.y = -1;

    if (S->size == 0) {
        return v;
    } 

    return S->data[S->size-1];
}

struct vertex Stack_Second_Top(Stack *S)
{
  struct vertex v;
  v.x = -1;
  v.y = -1;

    if (S->size < 2) {
        return v;
    } 

    return S->data[S->size-2];
}

void Stack_Push(Stack *S, struct vertex d)
{
    if (S->size < STACK_MAX)
        S->data[S->size++] = d;
}

void Stack_Pop(Stack *S)
{
        S->size--;
}


void init_adjacency() {
	int i,j;
    for(i = 0;i < SIZ; i++) {
    	for(j=0;j<SIZ;j++) {
                      
                      m[i][j].v[0].x = i; m[i][j].v[0].y = j-1;
                      m[i][j].v[2].x = i; m[i][j].v[2].y = j+1;
                      m[i][j].v[1].x = i+1; m[i][j].v[1].y = j;
                      m[i][j].v[3].x = i-1; m[i][j].v[3].y = j;
     	}
 	}
     
}

void assign_dist() {
	int i,j;
    for(i = 0;i < SIZ; i++) {
    	for(j = 0;j < SIZ;j++) {
    		if((i < (SIZ/2))&&(j < (SIZ/2))) {m[i][j].dist = (SIZ-2)-j-i;}
            if((i>=(SIZ/2))&&(j<(SIZ/2))) {m[i][j].dist = m[(SIZ-1)-i][j].dist;}
            if((i<(SIZ/2))&&(j>=(SIZ/2))) {m[i][j].dist = m[i][(SIZ-1)-j].dist;}
            if((i>=(SIZ/2))&&(j>=(SIZ/2))) {m[i][j].dist = m[(SIZ-1)-i][(SIZ-1)-j].dist;}
    	}
    }
    for(i = 0; i <= SIZ; i++) {
      for(j = 0; j <= SIZ; j++) {
        m[i][j].x = i;
        m[i][j].y = j;
        if(i == SIZ) {m[i][j].dist = -1;}
        if(j == SIZ) {m[i][j].dist = -1;}
        m[i][j].visited = false;
      }
    }
    for(i = 0; i < SIZ; ++i) {
      if(i<(SIZ/2)) {m[i][0].dist = SIZ-2-i;}
      if(i>=(SIZ/2)) {m[i][0].dist = SIZ-1-(SIZ-i);}
    }
    
}

void maze_wallinput(int x,int y,int i) {
  struct vertex curr;
	struct vertex adj;
  curr.x = x;
  curr.y = y;

  if(m[x][y].v[i].x!=-1&&m[x][y].v[i].x!=SIZ&&m[x][y].v[i].y!=-1&&m[x][y].v[i].y!=SIZ) //if it is not a border wall
  {
    if(i == 1) { 
      adj.x = m[x][y].v[i].x;
      adj.y = m[x][y].v[i].y;
      m[(m[x][y].v[i].x)][(m[x][y].v[i].y)].v[3].x=-1;m[(m[x][y].v[i].x)][(m[x][y].v[i].y)].v[3].y=-1;
    }
    else {
      adj.x = m[x][y].v[i].x;
      adj.y = m[x][y].v[i].y;
      m[(m[x][y].v[i].x)][(m[x][y].v[i].y)].v[abs(2-i)].x=-1;m[(m[x][y].v[i].x)][(m[x][y].v[i].y)].v[abs(2-i)].y=-1;
    } 

    m[x][y].v[i].x=-1;m[x][y].v[i].y=-1;
  Stack_Push(s,curr);
  Stack_Push(s,adj);
  }
//set x,y values of both cells to -1 in adjacency lists

  
}    

void updateDist() {
	struct vertex push;
  //printf("HERE");
  int i = 0;
  struct vertex top = Stack_Top(s);
  //printf("TEST");
  struct vertex min = Stack_Top(s);
  int x1, y1, min_x, min_y;
  min_x = min.x;
  min_y = min.y;

  //printf("ONE");
  //printf("\n(%d,%d)\n",min.x,min.y);

  for (i=0; i<4; i++) {

    x1 = m[top.x][top.y].v[i].x;
    //printf("\n%d\n",top.y);
    y1 = m[top.x][top.y].v[i].y;
    //printf("\n(%d,%d)\n",x1,y1);
    /*if(x1 < 0 || x1 > SIZ) {
      if(i == 0) {x1 = top.x;}
      if(i == 1) {x1 = top.x + 1;}
      if(i == 2) {x1 = top.x;}
      if(i == 3) {x1 = top.x - 1;}
    }
    if(y1 < 0 || y1 > SIZ) {
      if(i == 0) {y1 = top.y - 1;}
      if(i == 1) {y1 = top.y;}
      if(i == 2) {y1 = top.y + 1;}
      if(i == 3) {y1 = top.y;}
    }*/
    if(x1 >= 0 && y1 >= 0 && x1 < SIZ && y1 < SIZ) {
     // printf("HERE");
      if(m[x1][y1].dist < m[min_x][min_y].dist) {
       
        min_x = x1;
        min_y = y1;
      }
    }
}

  if(m[top.x][top.y].dist != m[min_x][min_y].dist + 1 && m[top.x][top.y].dist != 0) {
      m[top.x][top.y].dist = m[min_x][min_y].dist + 1;
      Stack_Pop(s);
      for(i = 0; i < 4; ++i) {
        if(i==0) {push.x = top.x; push.y = top.y-1;}
        else if(i==1) {push.x = top.x+1; push.y = top.y;}
        else if(i==2) {push.x = top.x; push.y = top.y+1;}
        else if(i==3) {push.x = top.x-1; push.y = top.y;}
        if(push.x >= 0 && push.y >= 0 && push.x < SIZ && push.y < SIZ) {
          Stack_Push(s, push);
        }
      }
    } else {Stack_Pop(s);}
}

struct vertex floodfill(int x, int y) {
  int i;
  int x1, y1;
  int next_x = x;
  int next_y = y;
  int next_dist = 100;
  struct vertex nextCell;

  for(i = 0; i < 4; i++) {
    // Serial.println("2");
    x1 = m[x][y].v[i].x; //x,y at vertices of m[x][y]
    y1 = m[x][y].v[i].y;

    if(x1 != -1 && x1 != SIZ && y1 != -1 && y1 != SIZ) {
      if(m[x1][y1].dist < next_dist && m[x1][y1].visited != true) {
        next_dist = m[x1][y1].dist;
        next_x = x1;
        next_y = y1;
      }
      
    }
  }
  if(next_x == x && next_y == y) {
    for(i = 0; i < 4; i++) {
      // Serial.println("3");
    x1 = m[x][y].v[i].x; //x,y at vertices of m[x][y]
    y1 = m[x][y].v[i].y;

    if(x1 != -1 && x1 != SIZ && y1 != -1 && y1 != SIZ) {
      if(m[x1][y1].dist < next_dist) {
        next_dist = m[x1][y1].dist;
        next_x = x1;
        next_y = y1;
      }
      
    }
  }
  }

  if(next_x - m[x][y].x == 1) {
    nextCell = m[x][y].v[1];
    nextCell.x = next_x;
    nextCell.y = next_y;
  }
  else if(next_x - m[x][y].x == -1) {
    nextCell = m[x][y].v[3];
    nextCell.x = next_x;
    nextCell.y = next_y;
  }
  else if(next_y - m[x][y].y == 1) {
    nextCell = m[x][y].v[2];
    nextCell.x = next_x;
    nextCell.y = next_y;
  }
  else if(next_y - m[x][y].y == -1){
    nextCell = m[x][y].v[0];
    nextCell.x = next_x;
    nextCell.y = next_y;
  }
  
  while(stack.size != 0) {
    updateDist();
    // Serial.println("4");
  }
  m[nextCell.x][nextCell.y].visited = true;
  return nextCell;
} 

struct vertex speed_floodfill(int x, int y) {
   int i;
  int x1, y1;
  int next_x = x;
  int next_y = y;
  int next_dist = 100;
  struct vertex nextCell;

  for(i = 0; i < 4; i++) {
    // Serial.println("2");
    x1 = m[x][y].v[i].x; //x,y at vertices of m[x][y]
    y1 = m[x][y].v[i].y;

    if(x1 != -1 && x1 != SIZ && y1 != -1 && y1 != SIZ) {
      if(m[x1][y1].dist < next_dist && m[x1][y1].visited == true) {
        next_dist = m[x1][y1].dist;
        next_x = x1;
        next_y = y1;
      }
      
    }
  }
  if(next_x == x && next_y == y) {
    for(i = 0; i < 4; i++) {
      // Serial.println("3");
    x1 = m[x][y].v[i].x; //x,y at vertices of m[x][y]
    y1 = m[x][y].v[i].y;

    if(x1 != -1 && x1 != SIZ && y1 != -1 && y1 != SIZ) {
      if(m[x1][y1].dist < next_dist) {
        next_dist = m[x1][y1].dist;
        next_x = x1;
        next_y = y1;
      }
      
    }
  }
  }

  if(next_x - m[x][y].x == 1) {
    nextCell = m[x][y].v[1];
    nextCell.x = next_x;
    nextCell.y = next_y;
  }
  else if(next_x - m[x][y].x == -1) {
    nextCell = m[x][y].v[3];
    nextCell.x = next_x;
    nextCell.y = next_y;
  }
  else if(next_y - m[x][y].y == 1) {
    nextCell = m[x][y].v[2];
    nextCell.x = next_x;
    nextCell.y = next_y;
  }
  else if(next_y - m[x][y].y == -1){
    nextCell = m[x][y].v[0];
    nextCell.x = next_x;
    nextCell.y = next_y;
  }
  
  while(stack.size != 0) {
    updateDist();
    // Serial.println("4");
  }
  // m[nextCell.x][nextCell.y].visited = true;
  return nextCell;

}
