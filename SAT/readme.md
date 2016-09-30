## Evan Heaton
### 09/22/2016
### Satisfiability Programming Assignment

This assignment uses several heuristic search techniques to find possibly optimal truth assignments for variables in Boolean formulas.
The formulas are in conjunctive normal form (ANDs of ORs). The fitness of an assignment is the number of clauses (ORs) that the assignment satisfies. If there are c clauses, then the highest fitness is bounded by c. However, if the formula is not satisfiable, then you cannot simultaneously make all c clauses true. You will use three of the following four techniques:

* a genetic algorithm
* hill climbing
* constraint solving with forward search
* the DPLL algorithm
* the WalkSAT algorithm
* resolution

### To do:

Download these sets of formulasView in a new window. Run each of your three algorithms on each formula. If your algorithm uses randomness, do 10 runs per formula. Collect data, for each formula and each run, on the time taken, and the highest value of c found. Put these data in tables or graphs (clearly labeled!)

### To turn in:

* A brief description of each algorithm, including design decisions (how you set your parameters in the genetic algorithm, for instance)
* your carefully commented code
* instructions on how to compile/run your code
* the data you collected
* a brief statement of what you learned

[Format info!](http://www.satcompetition.org/2004/format-solvers2004.html)

________________________________________________________________________________________________________________

### The three algorithms that I implemented are:
* DPLL
  * I had to add an extra check for consistency and empty clauses AFTER the unit-propogation/pure-literal-assignment
  * For my choose_literal function, I chose a random literal that existed in the formula.
* WalkSAT
  * I chose to make 500 mutations to the solution, looking for a satisfying assignment. If it's not found in 500 mutations, a new random assignment is used (restart)
  * I chose to use at max 500 restarts.
  * This combination yielded an average running time of about 3 minutes, which is what I was aiming for to finish in time for the deadline.
* Hill Climbing (repeated Local Search)
  * I chose to allow 1000 restarts, every time a plateau is found. This didn't seem to be enough for the hard cases, it timed out almost every time before finding the satisfying assignment.

### Running Instructions:
Type 'make' on the command line to compile the code, and ./satisfiability [easy/~hard] [Formula#] to test a particular Formula. OR, use the included shell script "./all_sat_testing.sh [easy/~hard]" to test all 100 formulas of a particular difficulty.

### Data collected:
see the excel sheet I included in the .zip

### Learning Outcome:
I had never heard of Satisfiability before this assignment, but now I know all about it! It's one of those categories that can be applied to many problems, so hopefully having these solutions implemented will serve me well in the future. I also learned how to keep track of time in c++ through the use of clock(), and re-learned how to link together many c++ files into a single executable through the use of a Makefile. I'm finally back in the groove of using unix as a development environment!
