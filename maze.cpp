/* 
maze.cpp

Author: Rhea Koparde

Short description of this file:
tries to find the shortest path and mark it with stars
uses BFS to explore all positions till path is found
*/

#include <iostream>
#include "mazeio.h"
#include "queue.h"

using namespace std;

// Prototype for maze_search, which you will fill in below.
int maze_search(char**, int, int);

// main function to read, solve maze, and print result
int main() {
   int rows, cols, result;
   char** mymaze;
   
   mymaze = read_maze(&rows, &cols);
   
   if (mymaze == NULL) {
      cout << "Error, input format incorrect" << endl;
      return 1;
   }

   // when working on Checkpoint 3, you will call maze_search here.
   // here. but for Checkpoint 1, just assume we found the path.
   result= maze_search(mymaze, rows, cols); 

   // examine value returned by maze_search and print appropriate output
   if (result == 1) { // path found!
      print_maze(mymaze, rows, cols);
   }
   else if (result == 0) { // no path :(
      cout << "No path could be found!" << endl;
   }
   else { // result == -1
      cout << "Invalid maze." << endl;
   }

   // ADD CODE HERE to delete all memory 
   // that read_maze allocated   
   for(int i=0; i<rows; i++){
      delete [] mymaze[i];
   }
   delete [] mymaze;

   return 0;
}

/**************************************************
 * Attempt to find shortest path and return:
 *  1 if successful
 *  0 if no path exists
 * -1 if invalid maze (not exactly one S and one F)
 *
 * If path is found fill it in with '*' characters
 *  but don't overwrite the 'S' and 'F' cells
 *************************************************/
int maze_search(char** maze, int rows, int cols) {
   int** mazeSearch = new int* [rows]; //visited depth maze

   for(int m=0; m<rows; m++){
      mazeSearch[m]= new int [cols];
      for(int n=0; n<cols; n++){ //sets all to no path at the moment
         mazeSearch[m][n]=0; 
      }
   }

   //predecessor maze
   Location** predecessor = new Location*[rows];
   for(int k=0; k<rows; k++){
      predecessor[k]= new Location [cols];
   }

   //looking at maze file
   int countS=0;
   int countF=0;
   Queue list(rows*cols);
   bool found=false;
   Location startPosition; //add start postiton to queue
   for(int i=0; i<rows; i++){
      for(int j=0; j<cols; j++){
         if(maze[i][j]=='S'){
            found=true;
            countS++;
            startPosition.row=i;
            startPosition.col=j;
            list.add_to_back(startPosition);
            mazeSearch[i][j]=1;
         } //making sure there is a start and end
         if(maze[i][j]=='F'){
            found=true;
            countF++;
         }
      }
   }

   if(!found || countS != 1 || countF != 1){ //if no start or end
      for(int i=0; i<rows; i++){
         delete [] mazeSearch[i];
      }
      delete [] mazeSearch;
      for(int j=0; j<rows; j++){
         delete [] predecessor[j];
      }
      delete [] predecessor;
      return -1;
      }


   Location position;
   Location north;
   Location south;
   Location west;
   Location east;
   bool final=false; //if the next position is F
   while(!(list.is_empty())){
    //start with first position in the list
      position = list.remove_from_front();
      mazeSearch[position.row][position.col]=1;

      //neighbors to the north
      north.row = position.row - 1;
      north.col=position.col;
      if(north.row >= 0 && north.row<rows &&
         north.col>=0 && north.col<cols){
         
         if(maze[north.row][north.col]== 'F'){ 
            final=true; 
            break;
         }
         else if(maze[north.row][north.col]=='.'){
            if(mazeSearch[north.row][north.col] !=1){
               list.add_to_back(north);
               mazeSearch[north.row][north.col]=1; //set as found
               predecessor[north.row][north.col]=position;   
            }
         }
         

      }

      //neighbors to the south
      south.row=position.row +1;
      south.col=position.col;
      if(south.row >= 0 && south.row<rows 
         && south.col>=0 && south.col<cols){
         
         if(maze[south.row][south.col]=='F'){
            final=true;
            break; 
         }
         else if(maze[south.row][south.col]=='.'){
            if(mazeSearch[south.row][south.col]!=1){
               list.add_to_back(south);
               mazeSearch[south.row][south.col]=1;
               predecessor[south.row][south.col]=position;
            }
         }
      }

      //neighbors to the east
      east.row=position.row;
      east.col=position.col+1;
      if(east.row >= 0 && east.row<rows 
         && east.col>=0 && east.col<cols){
        
         if(maze[east.row][east.col]=='F'){
            final=true;
            break; 
         }
         else if(maze[east.row][east.col]=='.'){
            if(mazeSearch[east.row][east.col]!=1){
               list.add_to_back(east);
               mazeSearch[east.row][east.col]=1;
               predecessor[east.row][east.col]=position;
            }
         }
      }
      
      //neighbors to the west
      west.row=position.row;
      west.col=position.col -1;
      if(west.row >= 0 && west.row<rows 
         && west.col>=0 && west.col<cols){
         
         if(maze[west.row][west.col]=='F'){
            final=true;
            break; 
         }
         else if(maze[west.row][west.col]=='.'){
            if(mazeSearch[west.row][west.col]!=1){
               list.add_to_back(west);
               mazeSearch[west.row][west.col]=1;
               predecessor[west.row][west.col]=position;
            }
         }
      }


   }

   
   for(int i=0; i<rows; i++){
      delete [] mazeSearch[i];
   } 
   delete [] mazeSearch;  

   //walk backwards and check if there is a path

   if(!final){ //if no F was found
      for(int l=0; l<rows; l++){
      delete [] predecessor[l];
      }
      delete [] predecessor;
   
      return 0;
   }
   else{
      while(maze[position.row][position.col]!='S'){ //create path with *
         maze[position.row][position.col]='*';
         position=predecessor[position.row][position.col];
      }

      for(int l=0; l<rows; l++){
         delete [] predecessor[l];
      }
      delete [] predecessor;

      return 1;
   }


}
