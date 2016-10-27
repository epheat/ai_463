Evan Heaton
CS 463G: Artificial Intelligence
10/17/2016
Prolog Assignment 1

RUN evan\_maze.pl WITH GPROLOG!!!

### Program Description
The maze is stored as a list of lists, with X coordinates representing the col#, and y coordinates representing the row#. The origin is in the top left corner, thus the indices start with 0.

My program builds a path from (X1, Y1) to (Xf, Yf) through a depth first search.
Every iteration, I first check if I have reached (Xf, Yf). If not (which is usually the case), my program will do the following:
* generate an (NextX, NextY) that is adjacent to the current (X, Y)
* verify that (NextX, NextY) is a walkable square
* verify that the path does not already travel through (NextX, NextY)
* now we're certain that (NextX, NextY) is valid, so we add it to the Current Path.
* finally, recursively build the rest of the path starting at (NextX, NextY).

and that's about it!

### Learning Statement
Well for one, I learned prolog, and how difficult it can be to make a functional logic program. This programming assignment reaaaaally stretched my brain - I'm so used to programming with direct manipulation of control flow that I nearly couldn't complete it. I'm a little disappointed in how finnicky prolog can be, but I suppose that's necessary with this sort of language. Anyways, I'm hyped that I actually finished the programming assignment, and I think it was a really fun exercise! cheers. 
