#include <iostream>
#include "mazeio.h"
#include "queue.h"

using namespace std;

int maze_search(char**, int, int);

// main function to read, solve maze, and print result
int main(int argc, char* argv[]) {
   int rows, cols, result;
   char** mymaze=NULL;
   
   if(argc < 2)
   {
       cout << "Please provide a maze input file" << endl;
       return 1;
   }
   mymaze = read_maze(argv[1], &rows, &cols);
   
   if (mymaze == NULL) {
      cout << "Error, input format incorrect" << endl;
      return 1;
   }

   result = maze_search(mymaze, rows, cols);
    
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
    
   for (int i=0; i<rows; i++)
   {
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
int maze_search(char** maze, int rows, int cols) 
{
    
    bool** visited = new bool*[rows]; // allocates 2D array of bools to track visited
    for (int i=0; i<rows; i++)
    {
        visited[i] = new bool[cols];
    }
    for (int i=0; i<rows; i++)
    {
        for (int j=0; j<cols; j++)
        {
            visited[i][j] = 0;
        }
    }
    
    Location** predecessor = new Location*[rows]; // allocates 2D array of Locations to keep track of predecessors 
    for (int i=0; i<rows; i++)
    {
        predecessor[i] = new Location[cols];
    }
    for (int i=0; i<rows; i++)
    {
        for (int j=0; j<cols; j++)
        {
            predecessor[i][j].row = -1; // initializes to (-1, -1)
            predecessor[i][j].col = -1;
        }
    }
    
    Location start;
    Location finish;
    
    int is_start = false;
    int is_finish = false;
    bool is_end = false; // specifically to track if the maze is doable
    
    for (int i=0; i<rows; i++) // searches maze for starting and finishing location
    {
        for (int j=0; j<cols; j++)
        {
            if (maze[i][j] == 'S')
            {
                start.row = i;
                start.col = j;
                visited[i][j] = true;
                is_start++;
            }
            if (maze[i][j] == 'F')
            {
                finish.row = i;
                finish.col = j;
                is_finish++;
            }
        }
    }
    if (is_finish != 1 || is_start != 1)
    {
        for (int i=0; i<rows; i++)
        {
            delete [] visited[i];
            delete [] predecessor[i];
        } 
        delete [] visited;
        delete [] predecessor;
        return -1;
    }
    
    Queue q(rows*cols);
    q.add_to_back(start);
    
    Location north;
    Location east;
    Location south;
    Location west;
  
    while (!q.is_empty())
    {
        Location loc = q.remove_from_front();
        
        if (((loc.col-1) >= 0)  // checks if in bounds
            && ((loc.col-1) <= cols-1)
            && ((maze[loc.row][loc.col-1] == '.')
            || (maze[loc.row][loc.col-1] == 'F'))
            // checks if it's an open space or the finish, i.e. not a wall
            && (!visited[loc.row][loc.col-1])) // checks if it's been visited before
        {
            north.col = loc.col-1;
            north.row = loc.row;
            visited[loc.row][loc.col-1] = 1; // marks it as visited
            q.add_to_back(north); // adds new discovered location to queue
            predecessor[loc.row][loc.col-1] = loc; // tracks predecessor
        }
        if (((loc.row+1) >= 0)
            && ((loc.row+1) <= rows-1)
            && ((maze[loc.row+1][loc.col] == '.')
            || (maze[loc.row+1][loc.col] == 'F'))
            && (!visited[loc.row+1][loc.col]))
        {
            east.col = loc.col;
            east.row = loc.row+1;
            visited[loc.row+1][loc.col] = 1;
            q.add_to_back(east);
            predecessor[loc.row+1][loc.col] = loc;
        } 
        if (((loc.col+1) >= 0)
            && ((loc.col+1) <= cols-1)
            && ((maze[loc.row][loc.col+1] == '.')
            || (maze[loc.row][loc.col+1] == 'F'))
            && (!visited[loc.row][loc.col+1]))
        {
            south.col = loc.col+1;
            south.row = loc.row;
            visited[loc.row][loc.col+1] = 1;
            q.add_to_back(south);
            predecessor[loc.row][loc.col+1] = loc;
        } 
        if (((loc.row-1) >= 0) 
            && (((loc.row-1) <= rows-1))
            && ((maze[loc.row-1][loc.col] == '.')
            || (maze[loc.row-1][loc.col] == 'F'))
            && (!visited[loc.row-1][loc.col]))
        {
            west.col = loc.col;
            west.row = loc.row-1;
            visited[loc.row-1][loc.col] = 1;
            q.add_to_back(west);
            predecessor[loc.row-1][loc.col] = loc;
        } 
        if ((loc.col == finish.col) && (loc.row == finish.row))
        {
            is_end = true;
            break;
        }
    }
    if (!is_end)
    {
        for (int i=0; i<rows; i++)
        {
            delete [] visited[i];
            delete [] predecessor[i];
        }
        delete [] visited;
        delete [] predecessor;
        return 0;
    }

    Location previous_loc = predecessor[finish.row][finish.col];
 
    while ((previous_loc.col != start.col) || (previous_loc.row != start.row))
    {
        maze[previous_loc.row][previous_loc.col] = '*';
        previous_loc = predecessor[previous_loc.row][previous_loc.col];
        
    }
    
    for (int i=0; i<rows; i++)
    {
        delete [] visited[i];
        delete [] predecessor[i];
    } 
    delete [] visited;
    delete [] predecessor;
    
    if (is_end)
    {
        return 1;
    }
    else
    {
        return 0;
    }
}
