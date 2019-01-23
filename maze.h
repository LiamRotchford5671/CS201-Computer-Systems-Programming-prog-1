#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>

//orientation values used in movement functions
enum orientation {EAST, SOUTH, WEST, NORTH}; 

//this struct holds all the data loaded from the external file to be used in all of the solving functions
struct data
{
	char ** array;		//array holding the maze
	
	int cols;		//size dimensions of the maze
	int rows;

	int startc;		//entry location of the maze (columns , rows)
	int startr;

	int endc;		//exit location of the maze (columns , rows)
	int endr;
};

//external file loader
int read_file(FILE * maze, char * argv[], struct data * m_data);

//determine intial direction of the entry point to the maze
int intial_dir(struct data * m_data, enum orientation * map);

//Move through the maze implementing the wall follower algorithm
int solver(struct data * m_data, enum orientation map);

//Movement functions
bool move_east(struct data * m_data, enum orientation * map, int * row, int * col);
bool move_south(struct data * m_data, enum orientation * map, int * row, int * col);
bool move_west(struct data * m_data, enum orientation * map, int * row, int * col);
bool move_north(struct data * m_data, enum orientation * map, int * row, int * col);

//Backtracking movement function
bool back_track(struct data * m_data, enum orientation * map, int * row, int * col, int entry);

//maze display
int display(struct data * m_data, int result);

//deconstructor of dynamically allocated 2-D array holding the maze
int destroy(struct data * m_data);
