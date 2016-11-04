It's time to catch 'em all.  You really want to collect a Mewtwo, but for that you need a Master Ball.  You also want to hatch some baby Pokemon.  This requires you to find the eggs, then walk for at least 3 blocks.  Can you accomplish this, and also find the Pikachu?


Changes to the maze:
    You will not be given the X, Y coordinates for anything.
    The Master Ball location will be marked with a "mb".
    The Mewtwo will be marked with a "mt".
    The eggs will be marked with "e".  There will be at least one egg.
       Hatched eggs are worth 10 points per egg.
    Pikachu''s location will be marked with a "p".
       Pikachus are worth 1 point each.
    There will also be unavailable blocks, marked with a "j".
    All other empty blocks will be marked with an "o" (the letter o).
     
Example:
    [[ o,  e,  j,  p,  o],
     [ o,  j,  o,  o,  o],
     [ o,  j, mt,  j,  o],
     [ o,  o,  e, o, o],
     [ p,  o,  j, mb,  o]]

You must find a path through the maze:
    Find the Master Ball before you capture the Mewtwo;
    Find a Pikachu;
    Find an egg, and then walk 3 spaces with it;
    Calculate your points as you go.
    Go to the Mewtwo after you have gotten the Master Ball.


Prolog Code
    As before, your code should contain a predicate with the following name and signature

    mazepath(X, Y, Maze, Path,Score)
        X,Y
            These are the X and Y of the room you begin the maze in. It is guaranteed to be walkable.
        Maze
            This is a 2d array of atoms, defined above, that represents the maze. You can travel to any walkable orthogonally-adjacent room. The maze coordinate system starts at (1, 1) in the top left corner. The Y-coordinates are the rows, and the X-coordinates are the columns.
        Path
            This should be a list of either 2-tuples or lists (i.e. in the form [(x1, y1), (x2, y2), ..] or [[x1, y1], [x2, y2], ...]) Each element of the path is the coordinates of an intersection traversed. It should also contain the
starting and ending intersections.

         Score 

This should be the points you accumulate from eggs and Pikachus.   Notice that you only get credit if you succeed in capturing the Mewtwo.

    You should _return_ the Path, not just print it out.
    You do not need to find the length of the path.

To Turn In:

        Description of what you did to implement the changes
        Well documented Prolog code.
        Learning Outcome

Check the forum for handy code snippets and library functions. Remember to document your code well. Prolog is not particularly readable by itself.
For good style, especially for documenting predicate signatures, see
http://www.cs.ubbcluj.ro/~csatol/log_funk/prolog/Covington_PrologGuidelines.pdf
