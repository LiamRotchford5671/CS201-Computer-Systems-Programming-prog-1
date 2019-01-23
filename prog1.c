#include "maze.h"

int main(int argc, char * argv[])
{
	FILE * maze;				//external file loading varible
	struct data m_data;			//struct object for maze data	
	enum orientation map;			//enum object for orientation as you move through the maze	

	printf("\nHello, We will now load the maze file.");

	if(!read_file(maze, argv, &m_data))				//load the maze and maze data from suppiled test file from the command line argument
		printf("\nCannot open file.");
	else								//only execute if the file is opened and loaded properly					
	{		
		if(!display(&m_data, 2))				//display the intial unsolved maze and display maze data
			printf("\nCannot print maze.");

		if(!solver(&m_data, map))				//Determine the maze solution using the wall follower algorithm
			printf("\nError in solving maze.");

		if(!destroy(&m_data))					//destroy the dynamically allocated 2-D array holding the maze
			printf("\nCannot delete contents.");
	}

	return 0;
}								//end of program
