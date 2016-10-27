% Evan Heaton
% CS 465G: Artificial Intelligence
% Prolog1
% 10/18/2016

% USE GPROLOG TO RUN!!!

maze([	[0, 0, 0, 0],
	[0, 1, 1, 0],
	[0, 1, 0, 0],
	[0, 1, 0, 1],
	[0, 0, 0, 0]	]).

% call citypath(Xstart, Ystart, Xfinish, Yfinish, Maze, Path) to see if there is a valid path from (Xstart, Ystart) to (Xfinish, Yfinish).
% If citypath returns true (meaning, there is a path), then Path will contain the blocks traversed to get to the finish.
citypath(X1, Y1, Xf, Yf, Maze, Path) :-
	maze(Maze),
	buildpath(X1, Y1, Xf, Yf, Maze, [(X1, Y1)], Path),
	print('path taken: '),
	printlist(Path),
	nl,
	print('length of path: '),
	length(Path, N),
	Moves is N-1,
	print(Moves).

% once buildpath/7 reaches the final block, CurrentPath's contents are reversed and stored in FinalPath.
buildpath(X, Y, Xf, Yf, Maze, CurrentPath, FinalPath) :-
	X =:= Xf,
	Y =:= Yf,
	reversepath(CurrentPath, FinalPath).

% buildpath/7 finds a path from (X, Y) to (Xf, Yf) in Maze, saving its progress in CurrentPath.
buildpath(X, Y, Xf, Yf, Maze, CurrentPath, FinalPath) :-
	adjacent(X, Y, NextX, NextY),
	openblock(NextX, NextY, Maze),
	notbeenthere(NextX, NextY, CurrentPath),
	append([(NextX, NextY)], CurrentPath, PathSoFar),
	buildpath(NextX, NextY, Xf, Yf, Maze, PathSoFar, FinalPath).

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

% openblock(X, Y, Maze) is true when the space at (X, Y) [origin at top left] is a 0.
openblock(X, Y, Maze) :-
	nth1(Y, Maze, Row),
	nth1(X, Row, 0).

printlist([]).
printlist([A|B]) :-
	print(A),
	print(' -> '),
	printlist(B).
