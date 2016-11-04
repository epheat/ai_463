% Evan Heaton
% CS 465G: Artificial Intelligence
% Prolog2
% 10/31/2016

% USE GPROLOG TO RUN!!!

% maze(M), mazepath(1,1,M,Path,Score).
% mazepath(1,1,[[ o,  e,  j,  p,  o],[ o,  j,  o,  o,  o],[ o,  j, mt,  j,  o],[ o,  o,  e, o, o],[ p,  o,  j, mb,  o]], Path, Score).

maze([	[o,  j, mt],
	[o,  j,  e],
	[mb, o,  p]	]). 

invalidmaze([	[ o, o, mt],
		[mb, o,  e],
		[ j, j,  j],
		[ o, p,  o],
		[ o, o,  o]	]).

maze2([	[o, e, j,  p, o],
	[o, j, o,  o, o],
	[o, j, mt, j, o],
	[o, o, o,  j, o],
	[o, o, j, mb, o]	]).

% the have predicate is used to keep track of which items we have collected.
% i.e. have(egg), have(pika), and have(master) must be true before we search for mewtwo.
:- dynamic(have/1).

% mazepath/5 starts at (X, Y) and attempts to find a path that collects all items and then ends on the mewtwo.
mazepath(X, Y, Maze, Path, Score) :-
	findsubgoals(X, Y, XR, YR, Maze, [(X, Y)], [], PathThru, Score), 
	findmewtwo(XR, YR, Maze, [(XR, YR)], PathThru, Path, Score),
	
	calculatescore(Score),

	print('path taken: '),
	printlist(Path),
	nl,
	print('length of path: '),
	length(Path, N),
	Moves is N-1,
	print(Moves),
	nl,
	print('score: '),
	print(Score).

% calculatescore/1 returns the appropriate score based on the have/1 predicate.
% I had to use this predicate because I couldn't figure out how to increment score during the search
calculatescore(Score) :-
	have(pika),
	have(egg),
	Score is 11.
calculatescore(Score) :-
	have(pika),
	\+ have(egg),
	Score is 1.
calculatescore(Score) :-
	\+ have(pika),
	have(egg),
	Score is 10.

% find mewtwo base case (when we are on the mt space). Finalizes the path.
findmewtwo(X, Y, Maze, PathSoFar, [H|T], FinalPath, Score) :-
	mewtwo(X, Y, Maze),
	append(PathSoFar, T, RevFinalPath),
	reversepath(RevFinalPath, FinalPath).

findmewtwo(X, Y, Maze, PathSoFar, PathThroughItems, FinalPath, Score) :-
	adjacent(X, Y, NX, NY),
	openblock(NX, NY, Maze),
	notbeenthere(NX, NY, PathSoFar),
	append([(NX, NY)], PathSoFar, TempPath),
	findmewtwo(NX, NY, Maze, TempPath, PathThroughItems, FinalPath, Score).


% some base cases for findsubgoals
% finding an egg, pikachu, or master ball, or having all of them
findsubgoals(X, Y, XR, YR, Maze, PathFromLastItem, PathSoFar, FinalPath, Score) :-
	have(egg),
	have(pika),
	have(master),
	XR is X,
	YR is Y,
	append([(X,Y)], PathSoFar, FinalPath).

% I'm just making landing on an egg worth 10, rather than hatching it by walking
% Note: since I'm looking for the egg before any of the other items, I am guaranteed to walk 3 spaces by the time I hit the mewtwo. i.e. egg -> pika -> master ball -> mewtwo OR egg -> master ball -> pika -> mewtwo. Both of these have at minimum 3 spaces walked after finding the egg. So, the egg will be "hatched" by the time my path is complete. :^)
findsubgoals(X, Y, XR, YR, Maze, PathFromLastItem, PathSoFar, FinalPath, Score) :-
	\+ have(egg),
	egg(X, Y, Maze),
	assertz(have(egg)),
	append(PathFromLastItem, PathSoFar, [H|T]),
	%incr10(Score, NewScore),
	findsubgoals(X, Y, XR, YR, Maze, [(X, Y)], T, FinalPath, Score).

% pika base case
findsubgoals(X, Y, XR, YR, Maze, PathFromLastItem, PathSoFar, FinalPath, Score) :-
	have(egg),
	\+ have(pika),
	pika(X, Y, Maze),
	assertz(have(pika)),
	append(PathFromLastItem, PathSoFar, [H|T]),
	% incr(Score, NewScore),
	findsubgoals(X, Y, XR, YR, Maze, [(X, Y)], T, FinalPath, Score).

% master ball base case
findsubgoals(X, Y, XR, YR, Maze, PathFromLastItem, PathSoFar, FinalPath, Score) :-
	have(egg),
	\+ have(master),
	master(X, Y, Maze),
	assertz(have(master)),
	append(PathFromLastItem, PathSoFar, [H|T]),
	findsubgoals(X, Y, XR, YR, Maze, [(X, Y)], T, FinalPath, Score).

% recursive case: move to an adjacent open square and repeat until base case.
findsubgoals(X, Y, XR, YR, Maze, PathFromLastItem, PathSoFar, FinalPath, Score) :-
	adjacent(X, Y, NX, NY),
	openblock(NX, NY, Maze),
	notbeenthere(NX, NY, PathFromLastItem),
	append([(NX, NY)], PathFromLastItem, TempPath),
	findsubgoals(NX, NY, XR, YR, Maze, TempPath, PathSoFar, FinalPath, Score).

% These increment predicates are unused.
incr10(Score, NewScore) :-
	NewScore is Score+10.
incr(Score, NewScore) :-
	NewScore is Score+1.

% mewtwo/3 returns true if mewtwo is at (X,Y) in Maze.
mewtwo(X, Y, Maze) :-
	nth1(Y, Maze, Row),
	nth1(X, Row, mt).

% egg/3 returns true if the egg is at (X,Y) in Maze.
egg(X, Y, Maze) :-
	nth1(Y, Maze, Row),
	nth1(X, Row, e).

% pika/3 returns true if pikachu is at (X,Y) in Maze.
pika(X, Y, Maze) :-
	nth1(Y, Maze, Row),
	nth1(X, Row, p).

% master/3 returns true if the master ball is at (X,Y), in Maze.
master(X, Y, Maze) :-
	nth1(Y, Maze, Row),
	nth1(X, Row, mb).

% notbeenthere/3 returns true if (X,Y) does not appear in Path.
notbeenthere(X, Y, Path) :-
	\+ member((X,Y), Path).

% reversepath(Path, Reversed) is true when Reversed is Path in reverse order 
reversepath(P, R) :-
	accRev(P, [], R).

% http://www.learnprolognow.org/lpnpage.php?pagetype=html&pageid=lpn-htmlse25
accRev([], A, A).
accRev([H|T], A, R) :-
	accRev(T, [H|A], R).

% adjacent(X1, Y1, X2, Y2) sets (X2, Y2) to be directly adjacent to (X1, Y1).
adjacent(X1, Y1, X2, Y2) :-
	X2 is X1,
	Y2 is Y1 + 1.
adjacent(X1, Y1, X2, Y2) :-
	X2 is X1,
	Y2 is Y1 - 1.
adjacent(X1, Y1, X2, Y2) :-
	Y2 is Y1,
	X2 is X1 + 1.
adjacent(X1, Y1, X2, Y2) :-
	Y2 is Y1,
	X2 is X1 - 1.

% openblock(X, Y, Maze) is true when the space at (X, Y) is not j. The indexing starts at (1,1) in the top left corner. X is the column number, Y is the row number.
openblock(X, Y, Maze) :-
	nth1(Y, Maze, Row),
	nth1(X, Row, Cell),
	\+ Cell = 'j'.

% printlist/1 prints a list (normally a Path in this program) with arrows between each element.
printlist([]).
printlist([A|B]) :-
	print(' -> '),
	print(A),
	printlist(B).
