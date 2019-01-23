#include "maze.h"

//FILE LOADER
/*This function opens the file provided from the command line argument. If it successfully opens the file it will take the first line of the file and load the two values * into the struct values holding the size dimensions, from those two values it will then dynamically allocate the 2-D array and then increment the handle variable
 * the while loop will keep looping until it hits the end of file however with the handle varible it will load the starting entry location fo the maze into the correct
 * struct values for the second line, and the ending location for the 3rd line. From there the rest of the file is the contents of the maze itself. It will then load the * contents into the allocated array one character at a time until the end of final is reached. After that the file is closed.
*/ 
int read_file(FILE * maze, char * argv[], struct data * m_data)
{
	int i, j, c, handle = 1;

	maze = fopen(argv[1], "r");			//open the file set to "read", argv takes in the file name from the command line using ./prog1 filename

	if(maze)					//check if the file has been correctly opened
	{
		while((c = getc(maze)) != EOF)		//keep moving through the file until the end of file is reached, (c) varible used as the spot im at in file	
		{
			ungetc(c, maze);		//remove buffer character loaded

			if(handle == 1)			//take the size dimensions of the matrix and dynamically create the array by the cols and rows in file
			{	
				fscanf(maze, "%d", &(m_data->cols));	//load # of rows and cols into the struct varibles	
				fgetc(maze);
				fscanf(maze, "%d", &(m_data->rows));
				fgetc(maze);

				m_data->array = (char**)malloc(sizeof(char*)*(m_data->rows));		//dynamically allocation the 2-D array by the size values

				for(i = 0; i < m_data->rows; i++)
				{
					m_data->array[i] = (char*)malloc(sizeof(char)*(m_data->cols));
				}

				++handle;

			}else if(handle == 2)		//take in the location of the start point of the maze in the array	
			{
				fscanf(maze, "%d", &(m_data->startc));		
				fgetc(maze);
				fscanf(maze, "%d", &(m_data->startr));
				fgetc(maze);

				++handle;

			}else if(handle == 3)		//take in the location of the end point of the maze in the array
			{
				fscanf(maze, "%d", &(m_data->endc));		
				fgetc(maze);
				fscanf(maze, "%d", &(m_data->endr));
				fgetc(maze);

				++handle;
			}else				//load the maze from file into the created array
			{
				for(i = 0; i < m_data->rows; ++i)			//load each character in the maze into the array
				{
					for(j = 0; j < m_data->cols; ++j)
						m_data->array[i][j] = getc(maze);	

					fgetc(maze);	
				}
			}
		}
	}else			
		return 0;				//opening of the file failed so return zero and end the program

	fclose(maze);					//close the file

	return 1;
}

//======================================================================================================================================================================
//======================================================================================================================================================================

//INTIAL ENTRY POINT ON THE MAZE
/* This function determines what side of the maze the entry / starting point is on the maze, using the starting values we can determine this with either the 0 row / col
 * and the MAX row and col. once determined the map enum value that determines our orientation is set dependant on where it enters
*/
int intial_dir(struct data * m_data, enum orientation * map)
{
	if( (m_data->startr != m_data->rows) && (m_data->startc == 0) )
	{
		(*map) = EAST;				//enters from the RIGHT

	}else if( (m_data->startr == 0) && (m_data->startc != m_data->cols) )
	{
		(*map) = SOUTH;				//enters from the TOP

	}else if( (m_data->startr == m_data->rows) && (m_data->startc == 0) )
	{
		(*map) = NORTH;				//enters from the BOTTOM

	}else if( (m_data->startr == 0) && (m_data->startc == m_data->cols) ) 
	{
		(*map) = WEST;				//enters from the LEFT

	}else
	{
		return 0;				//ERROR in finding the entry direction
	}

	return 1; 
}

//======================================================================================================================================================================
//======================================================================================================================================================================

//SOLVER
/* This function firstly sets our intial row and column values to be the starting entry location from the struct values copied from the file. The first start location is
 * set to be a 'W' as a walked path. the intial entry direction is determined to set the enum map value. The while loop will continue to loop until the current row and
 * column location is the same as the ending location. Following the wall follower algorithm the while loop will check if it can move to the right based on the enum map
 * orientation in the maze, if it cannot it will attemp to move forward, then left, otherwise it will attempt to make a 180 turn. If they all return false then it will
 * move into the back tracking function to make its way back until it encounters a '0' walkable path again. Once the current location matches the end location it will
 * set the last value to be a 'W' and then display the final solution to the maze.
*/ 
int solver(struct data * m_data, enum orientation map)
{
	int row = m_data->startr, col = m_data->startc, entry = 0;		//set the row and col intial values to be the starting location

	m_data->array[row][col] = 'W';						//set the starting location to be a 'W'	

	if(!intial_dir(m_data, &map))						//determine the entry location direction
	{
		printf("\n+Error in determining the intial entry direction.");
		return 0;
	}


	while(! ((row == m_data->endr) && (col == m_data->endc)) ) 		//loop until the current location is the ending location i.e solved the maze
	{	

		//printf("\n\n\tBEFORE CHANGE position: ROW: %d , COL: %d " , row ,col);

		if(move_east(m_data, &map, &row, &col))				//move right if possible
			;	
		else if(move_south(m_data, &map, &row, &col))			//move forward if possible
			;
		else if(move_west(m_data, &map, &row, &col))			//move left if possible
			;
		else if(move_north(m_data, &map, &row, &col))			//180 turn	
			;
		else if(back_track(m_data, &map, &row, &col, entry))		//backtrack until a '0' is encountered
			;
		else								 
			printf("\n++++++++++Solving Error++++++++++\n");	//error case, every previous function returned false, every direction is somehow a 'X"

		entry = 1;							//this makes sure we don't accidentally somehow we back track out of bounds on our 
										//start location. This makes sure we have moved at least once before backtracking
		////////////////////////////////
		/*
		printf("\n\n\tAFTER CHANGE position  BACK IN SOLVER FUNC: ROW: %d , COL: %d " , row ,col);
		printf("\n\n\tEXIT WHILE LOOP VALUES: ROW: %d , COL: %d " , m_data->endr ,m_data->endc);

		if(map == EAST)
			printf("\nEAST");
		else if(map == SOUTH)
			printf("\nSOUTH");
		else if(map == WEST)
			printf("\nWEST");
		else if(map == NORTH)
			printf("\nNORTH");


		display(m_data, 1);						//if you want to view each step and each update to the maze as it moves through
		*/								//	comment out this display function call
		///////////////////////////////////

	}	//end while 

	m_data->array[row][col] = 'W';						//set the ending location to be a 'W' since we solved it

	display(m_data, 3);							//display the final solution

	return 1; 
}

//======================================================================================================================================================================
//======================================================================================================================================================================

//MOVE EAST DIRECTION
/* In this function the the switch takes in the case of the map value (orientation values) dependant on that it will go to the specfic case that matches. It will then
 * check if it can move to the specified orientations "right" direction. if it can then it will move to the new position and then change the '0' to be a 'W'. it will 
 * set determine to be true to state that a movement happend and then return to the solver function skipping the other if else cases since it already was able to move
 * to the right. If it however cannot move to the "right" of the maps specified orientation then it will return false and move to the next if else case and attempt to
 * move there. If it can make a movement then it will set the new orientation to be "EAST" since that was its most recent movement direction
*/ 
bool move_east(struct data * m_data, enum orientation * map, int * row, int * col)
{
	bool determine = false;

	///////////////////////////////				//checker to determine what orientation were at going into the fucntion
	/*
	printf("\nIN THE RIGHT FUNCTION. ORIENT: ");
	if(*map == EAST)
		printf("\nEAST");
	else if(*map == SOUTH)
		printf("\nSOUTH");
	else if(*map == WEST)
		printf("\nWEST");
	else if(*map == NORTH)
		printf("\nNORTH");
	*/	
	//////////////////////////////

	switch( *map )							//determine what orientation we are currently at, to send it to the correct checks to check
	{								//	if we can move to that orientations "right" side
		case EAST:

			if(m_data->array[*row-1][*col] == '0') 		//check the EAST orientations right side to see if its a walkable path
			{
				--*row;					//if it is move to that location	
				m_data->array[*row][*col] = 'W';	//set that location to be a 'W" as a now walked path
				determine = true;			//set the return value to be true so we skip over the other movement functions	
			}	
			break;

		case SOUTH:						//*****SAME RULES APPLY TO EACH OF THE OTHER CASES******

			if(m_data->array[*row][*col+1] == '0')		
			{		
				++*col;		
				m_data->array[*row][*col] = 'W';
				*map = EAST;

				determine = true;		
			}	
			break;

		case WEST:

			if(m_data->array[*row+1][*col] == '0')		
			{
				++*row;	
				m_data->array[*row][*col] = 'W';
				*map = EAST;

				determine = true;		
			}	
			break;

		case NORTH:

			if(m_data->array[*row][*col-1] == '0')	//left	
			{
				--*col;	
				m_data->array[*row][*col] = 'W';
				*map = EAST;

				determine = true;		
			}	
			break;
	}

	return determine;
}	

//======================================================================================================================================================================
//======================================================================================================================================================================

//MOVE SOUTH DIRECTION
/* In this function the the switch takes in the case of the map value (orientation values) dependant on that it will go to the specfic case that matches. It will then
 * check if it can move to the specified orientations "forward" direction. if it can then it will move to the new position and then change the '0' to be a 'W'. it will 
 * set determine to be true to state that a movement happend and then return to the solver function skipping the other if else cases since it already was able to move
 * to the right. If it however cannot move to the "forward" of the maps specified orientation then it will return false and move to the next if else case and attempt to
 * move there. If it can make a movement then it will set the new orientation to be "SOUTH" since that was its most recent movement direction
*/ 
bool move_south(struct data * m_data, enum orientation * map, int * row, int * col)
{
	bool determine = false;

	/////////////////////////			//checker to determine what orientation were at going into the fucntion
	/*
	printf("\nIN THE FORWARD FUNC: ORIENT:");	
	if(*map == EAST)
		printf("\nEAST");
	else if(*map == SOUTH)
		printf("\nSOUTH");
	else if(*map == WEST)
		printf("\nWEST");
	else if(*map == NORTH)
		printf("\nNORTH");
	*/		
	////////////////////////

	switch( *map )							//determine what orientation we are currently at, to send it to the correct checks to check
	{								//	if we can move to that orientations "forward" side
		case EAST:

			if(m_data->array[*row][*col+1] == '0') 		//check the EAST orientations forward side to see if its a walkable path
			{						//if it is move to that location	
				++*col;					//set that location to be a 'W" as a now walked path	
				m_data->array[*row][*col] = 'W';
				*map = SOUTH;				//set the return value to be true so we skip over the other movement functions	

				determine = true;				
			}	
			break;
		
 		case SOUTH:

			if(m_data->array[*row+1][*col] == '0')		//*****SAME RULES APPLY TO EACH OF THE OTHER CASES******
			{		
				++*row;		
				m_data->array[*row][*col] = 'W';

				determine = true;		
			}	
			break;
	
 		case WEST:

			if(m_data->array[*row][*col-1] == '0')		
			{
				--*col;	
				m_data->array[*row][*col] = 'W';
				*map = SOUTH;

				determine = true;		
			}	
			break;

		case NORTH:

			if(m_data->array[*row-1][*col] == '0')		
			{
				--*row;	
				m_data->array[*row][*col] = 'W';
				*map = SOUTH;

				determine = true;		
			}	
			break;
	}

	return determine;
}	

//======================================================================================================================================================================
//======================================================================================================================================================================

//MOVE WEST DIRECTION
/* In this function the the switch takes in the case of the map value (orientation values) dependant on that it will go to the specfic case that matches. It will then
 * check if it can move to the specified orientations "left" direction. if it can then it will move to the new position and then change the '0' to be a 'W'. it will 
 * set determine to be true to state that a movement happend and then return to the solver function skipping the other if else cases since it already was able to move
 * to the right. If it however cannot move to the "left" of the maps specified orientation then it will return false and move to the next if else case and attempt to
 * move there. If it can make a movement then it will set the new orientation to be "WEST" since that was its most recent movement direction
*/ 
bool move_west(struct data * m_data, enum orientation * map, int * row, int * col)
{
	bool determine = false;

	/////////////////////////////				//checker to determine what orientation were at going into the fucntion
	/*
	printf("\nIN THE LEFT MOVE FUNC: ORIENT: ");	
	if(*map == EAST)
		printf("\nEAST");
	else if(*map == SOUTH)
		printf("\nSOUTH");
	else if(*map == WEST)
		printf("\nWEST");
	else if(*map == NORTH)
		printf("\nNORTH");
	*/	
	//////////////////////////////	

	switch( *map )							//determine what orientation we are currently at, to send it to the correct checks to check
	{								//	if we can move to that orientations "left" side
		case EAST:

			if(m_data->array[*row+1][*col] == '0') 		//check the EAST orientations left side to see if its a walkable path
			{						//if it is move to that location	
				++*row;	
				m_data->array[*row][*col] = 'W';	//set that location to be a 'W" as a now walked path
				*map = WEST;
									//set the return value to be true so we skip over the other movement functions	
				determine = true;		
			}							

			break;

		case SOUTH:

			if(m_data->array[*row][*col-1] == '0')		//*****SAME RULES APPLY TO EACH OF THE OTHER CASES******	
			{		
				--*col;		
				m_data->array[*row][*col] = 'W';
				*map = WEST;

				determine = true;		
			}	

			break;

		case WEST:

			if(m_data->array[*row-1][*col] == '0')		
			{
				--*row;	
				m_data->array[*row][*col] = 'W';

				determine = true;		
			}	

			break;

		case NORTH:

			if(m_data->array[*row][*col+1] == '0')		
			{
				++*col;	
				m_data->array[*row][*col] = 'W';
				*map = WEST;

				determine = true;		
			}	

			break;
	}

	return determine;
}	

//======================================================================================================================================================================
//======================================================================================================================================================================

//MOVE NORTH DIRECTION
/* In this function the the switch takes in the case of the map value (orientation values) dependant on that it will go to the specfic case that matches. It will then
 * check if it can move to the specified orientations "180" direction. if it can then it will move to the new position and then change the '0' to be a 'W'. it will 
 * set determine to be true to state that a movement happend and then return to the solver function skipping the other if else cases since it already was able to move
 * to the right. If it however cannot move to the "180" of the maps specified orientation then it will return false and move to the next if else case and attempt to
 * move there. If it can make a movement then it will set the new orientation to be "NORTH" since that was its most recent movement direction
*/ 
bool move_north(struct data * m_data, enum orientation * map, int * row, int * col)
{
	bool determine = false;

	///////////////////////////////				//checker to determine what orientation were at going into the fucntion
	/*
	printf("\nIN THE BACK FUNCTION. ORIENT: ");
	if(*map == EAST)
		printf("\nEAST");
	else if(*map == SOUTH)
		printf("\nSOUTH");
	else if(*map == WEST)
		printf("\nWEST");
	else if(*map == NORTH)
		printf("\nNORTH");
	*/	
	//////////////////////////////

	switch( *map )							//determine what orientation we are currently at, to send it to the correct checks to check
	{								//	if we can move to that orientations "180" side
		case EAST:

			if(m_data->array[*row][*col-1] == '0') 		//check the EAST orientations 180 side to see if its a walkable path
			{						//if it is move to that location	
				--*col;	
				m_data->array[*row][*col] = 'W';	//set that location to be a 'W" as a now walked path
				*map = NORTH;	
									//set the return value to be true so we skip over the other movement functions	
				determine = true;		
			}	
			break;

		case SOUTH: 

			if(m_data->array[*row-1][*col] == '0')		//*****SAME RULES APPLY TO EACH OF THE OTHER CASES******
			{		
				--*row;		
				m_data->array[*row][*col] = 'W';
				*map = NORTH;

				determine = true;		
			}	
			break;
	
		case WEST:

			if(m_data->array[*row][*col+1] == '0')		
			{
				++*col;	
				m_data->array[*row][*col] = 'W';
				*map = NORTH;

				determine = true;		
			}	
			break;

		case NORTH:

			if(m_data->array[*row+1][*col] == '0')		
			{
				++*row;	
				m_data->array[*row][*col] = 'W';

				determine = true;		
			}	
			break;
	}

	return determine;
}	

//======================================================================================================================================================================
//======================================================================================================================================================================

//BACKTRACKER
/* In this function we firstly check that we are not somehow at the starting location (if we backtrack somehow we would be out of array bounds and seg fault). we then
 * check to be sure that there is no walkable path '0' in any direction from our current position. It then checks if there is a already walked path in the wall follower
 * order of precedence (right, forward, left, 180) if and when it finds it the current postion is set to a 'T' to represent a backtracked path and then moves to the
 * new position where the 'W' was found. then we return true to state back to the solver function that we moved our location.
*/

bool back_track(struct data * m_data, enum orientation * map, int * row, int * col, int entry)
{
	bool determine = false;

	if(entry == 1)			//confirm that we arent at the starting location on the maze
	{				//make sure that there arent any walkable paths in any direction at our current position
		if((m_data->array[*row-1][*col] != '0') && (m_data->array[*row][*col+1] != '0') && (m_data->array[*row+1][*col] != '0') && (m_data->array[*row][*col-1] != '0')) 
		{
			if(m_data->array[*row][*col+1] == 'W')		//right EAST back track to the east
			{
				m_data->array[*row][*col] = 'T';	//set the back track character to 'T'	
				++*col;					//move to the east from current postion
				determine = true;			//set the return value to true to state that we successfully moved

			}else if(m_data->array[*row+1][*col] == 'W')	//forward SOUTH
			{
				m_data->array[*row][*col] = 'T';	
				++*row;
				determine = true;

			}else if(m_data->array[*row][*col-1] == 'W')	//left WEST
			{
				m_data->array[*row][*col] = 'T';	
				--*col;
				determine = true;

			}else if(m_data->array[*row-1][*col] == 'W')	//180 NORTH
			{
				m_data->array[*row][*col] = 'T';	
				--*row;
				determine = true;
			}
		}else
			printf("\n\t-There is a zero around, in the move right func");	//found a walkable path, if this displays we have a solving error
	}

	return determine;
}

//======================================================================================================================================================================
//======================================================================================================================================================================

//DISPLAY
/* In this function we display the contents of the maze, dependant on the int value of result given by the function call certain printf's will take place. if 1 the 
 * title will be the for solving the maze as we solve it, if its a 2 we havn't begun solving the maze so we display the contents of the maze data and unsolved maze,
 * 3 it will display the fully solved maze changing every back track path 'T' back into a 'W' for a final solution.
*/
int display(struct data * m_data, int result)
{
	int i, j;	

	if(m_data->array[0] == '\0')		//precaution, make sure we're not displaying NULL contents
		return 0;

	printf("\n\n========================================================================================================");

	if( (result == 1 || result == 2) )	//result 1 or 2 is just to determine what printf's to execute, 2 is for the intial display, disp all data
	{
		if(result == 1)			//print for the solving maze when you need to see each maze step
		{
			printf("\n+Solving Maze: \n");

		}else if(result == 2)		//print the intial data about the maze (for the intially loaded maze)
		{
			printf("\n+Intial unsolved maze setup: \n+++++++++++++++++++++++++++++++++++++++++++++++++++++++\n");

			printf("\n+The maze has been loaded from file specified. \n\n+Dimensions: \n\t-Columns: %d \n\t-Rows: %d", m_data->cols, m_data->rows);	

			printf("\n+Starting location on maze: \n\t-Column: %d \n\t-Row: %d", m_data->startc, m_data->startr);	
			printf("\n+Ending location on the maze: \n\t-Column: %d \n\t-Row: %d", m_data->endc, m_data->endr);	
		}	
		
		printf("\n\n========================================================================================================\n");
		
		for(i = 0; i < m_data->rows; ++i)	//print the contents of the maze
		{
			printf("\t\n");

			for(j = 0; j < m_data->cols; ++j)	
				printf(" %c " , m_data->array[i][j]);	
		}	

		printf("\n\n========================================================================================================\n");

	}else		//print the solved version of the maze
	{
	
		printf("\n\n+Solved Maze: ('W' path from start to finish) \n");
		printf("========================================================================================================\n");
		
		for(i = 0; i < m_data->rows; ++i)
		{
			printf("\t\n");

			for(j = 0; j < m_data->cols; ++j)		//prints the contents of the solved maze with only the taken path 'W' and walls 'X'	
			{
				if(m_data->array[i][j] == 'T')		//if you wish to view the back tracking path: comment out this if statement condition
					m_data->array[i][j] = 'W';

				printf(" %c " , m_data->array[i][j]);	
			}
		}	

		printf("\n\n========================================================================================================\n");
	}

	return 1;
}

//======================================================================================================================================================================
//======================================================================================================================================================================

//DECONSTRUCTOR
/* This function moves through each row freeing each point on the array setting it to NULL, once it has gone through every row it free's the array itself deallocating
 * all memory allocated in creating the array itself to avoid lost memory or memory leaks
*/ 
int destroy(struct data * m_data)
{
	int i;

	for(i = 0; i < m_data->rows; ++i)		//move through every row
	{
		if(m_data->array[i] == '\0')		//if the row is null dont fre it again (precaution)
			return 0;
		else
			free(m_data->array[i]);		//free each row
	}

	free(m_data->array);				//free the dynamic array

	return 1;
}
