## Evan Heaton
### 09/22/2016
### Satisfiability Programming Assignment

This assignment uses several heuristic search techniques to find possibly optimal truth assignments for variables in Boolean formulas.
The formulas are in conjunctive normal form (ANDs of ORs). The fitness of an assignment is the number of clauses (ORs) that the assignment satisfies. If there are c clauses, then the highest fitness is bounded by c. However, if the formula is not satisfiable, then you cannot simultaneously make all c clauses true. You will use three of the following four techniques:

* a genetic algorithm;
* hill climbing;
* constraint solving with forward search;
* the DPLL algorithm;
* the WalkSAT algorithm;
* resolution.

### To do:

Download these sets of formulasView in a new window. Run each of your three algorithms on each formula. If your algorithm uses randomness, do 10 runs per formula. Collect data, for each formula and each run, on the time taken, and the highest value of c found. Put these data in tables or graphs (clearly labeled!)

### To turn in:

A brief description of each algorithm, including design decisions (how you set your parameters in the genetic algorithm, for instance);
your carefully commented code;
instructions on how to compile/run your code;
the data you collected;
a brief statement of what you learned.

[Format info!](http://www.satcompetition.org/2004/format-solvers2004.html)
