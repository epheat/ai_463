Evan Heaton
CS 463G: Artificial Intelligence
10/17/2016
Prolog Assignment 1

Your job, should you choose to accept it, is to navigate a city neighborhood in search of Pokemon.  You are on the wildcat blue team;  the villianous Team Rocket is hot on your heels.   For the honor of the team and your own satisfaction, you will collect as many of the elusive critters as you can.  You have a map of the city, but the threat of rampaging Mewtwos has forced Officer Jenny to closed off many of the streets and avenues.  Your map is of limited use, since it does not show where the streets are closed.  (And for some reason, no map programs will tell you.)

Goal:
Find a path from your residence (at the intersection of First Street and First Avenue) to the Pokemart (to buy Pokeballs)
Display the route, in terms of streets and avenues
Display the length of the path (number of blocks walked)

Maze:
The maze will be given as a 2 dimensional array of 1s and 0s.
A 0 represent a walkable block, while a 1 is a block that is occupied by Officer Jennies.
Your code should take a predicate in the form of a list of lists:
    maze([[0, 0, 0, 0],
          [0, 1, 1, 0],
          [0, 1, 0, 0],
          [0, 1, 0, 1],
          [0, 0, 0, 0]]).
This will encode the given maze. (The actual array will be different). Your code should work for any given start and end room in the maze. It should take specifications of rooms as (x,y) coordinates.  You should have a predicate
    citypath(X1,Y1,X2,Y2,Maze,Path) 
that instantiates Path to a path from (X1,Y1) to (X2,Y2) in Maze.
Even given an unsolvable maze, your code should still terminate, however you don't need to display anything in that case.

To submit via Canvas:
   Well documented Prolog code;
   a description of how the code works, including the maze representation and your pathfinding algorithm;
   a brief statement of what you learned.
