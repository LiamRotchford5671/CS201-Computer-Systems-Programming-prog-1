# CS201-Computer-Systems-Programming-prog-1

~PROGRAM BRIEF DESCRIPTION:

This program utilizes external file reading, allocation & deallocation of dynamic memory, and multidimensional arrays. In this program specfically a 2-D array of rows and columns have been used. 

++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

~PROGRAM SUMMARY

This program begins by loading the size dimensions of the maze, starting and ending locations of the maze, and the maze contents from the external file supplied. It then creates the dynamic 2-D array from the size dimenstions and loads the maze contents into the array. After determineing the direction (side of box / maze) the starting point enters the maze from the programs uses the "Wall Follower Algorithm" to traverse through the maze from start to end. It will check if it can move to its right first, if not then forward, if not that then it will try its left, If not of those work it will make a 180 degree turn. In the maze '0's represent a walkable path, 'W's represent a already walked path, and 'X's represent a wall.

In the event that the program cannot move right, forward, or left it will back track through its path until it encounters another walkable path '0'. The solver will continue to loop through until the current location matches the exit point of the maze. It should be mentioned if you remove the commented out display in the solver while loop to view every movement update to the maze any time the solver needs to backtrack its steps to find a new walkable path it will change the 'W' (path already walked i.e our backtrack path) will be changed to 'T's to visually view the back track path. These 'T's are changed back into 'W's for the final display of the solution.  

The organization of the program follows the directions of the maze by the arrows of a compass with the top being NORTH, bottom SOUTH, right EAST, and left WEST
++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

~RUNNING INSTUCTIONS

	1) To create / compile program executable: enter " make hw1 " on the command line

	2) To execute the program: enter " ./hw1 ***external text file*** " on the command line

	3) To delete all files created during compliation process: enter " make clean " on the command line 
