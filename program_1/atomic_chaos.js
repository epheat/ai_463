//I'll want to remove this global variable at some point
//To do that, I will have to convert to "instance mode" I guess?
//https://github.com/processing/p5.js/wiki/p5.js-overview#instantiation--namespace
//no.. I'll keep a global variable but make it into a global object from a class
//see: https://p5js.org/examples/examples/Objects_Objects.php

var console_mode = false;

//setup works like main. Called once at the beginning.
function setup() {
  var puzzle = new AtomicChaosPuzzle();
  puzzle.state = puzzle.create_state_array();

  //window stuff please ignore (lol)
  createCanvas(601, 301);
  ellipseMode(CORNER);
  var clockwise_button = createButton('clockwise');
  clockwise_button.position(400, 350);
  clockwise_button.mousePressed(function() {puzzle.clockwise();});
  var counter_button = createButton('counter-clockwise');
  counter_button.position(400, 370);
  counter_button.mousePressed(function() {puzzle.counter_clockwise();});
  var flip_button = createButton('flip');
  flip_button.position(400,400);
  flip_button.mousePressed(function() {puzzle.flip();});
  var debug_button = createButton('debug');
  debug_button.position(400,450);
  debug_button.mousePressed(function() {
    puzzle.print_state();
    var solution_string = puzzle.scramble(10);
    console.log(puzzle.print_solution(solution_string));});
  var heuristic_button = createButton('heuristic');
  heuristic_button.position(500, 450);
  heuristic_button.mousePressed(function() {console.log("heuristic_value: " + puzzle.get_heuristic_value(puzzle.state));});

  var goal_button = createButton('isgoal');
  goal_button.position(500, 425);
  goal_button.mousePressed(function() {console.log("solved: "+puzzle.is_goal_state());});

  var solve_button = createButton('solve');
  solve_button.position(500,400);
  solve_button.mousePressed(function() {puzzle.solve();});

  puzzle.render();

}

//creating an "AtomicChaosPuzzle" class:
//In pretty much the jankest way possible. I don't know Javascript very well:
function AtomicChaosPuzzle() {
  //looking back, I probably should have made separate variables here for each tube, gravity, rotation, etc.
  //but I already wrote everything to work with the state[] array so I just kept it.
  //to be fair, we were very rushed with this assignment.
  this.state;

  //attempts to solve the puzzle using A*
  this.solve = function() {
    console.log("starting solving process");
    var initial_state = deep_copy(this.state);
    var frontier = new PriorityQueue(); //make the frontier
    frontier.push({ state: deep_copy(this.state), g: 0, path: "" }, this.get_heuristic_value(this.state)); //put the start state on the frontier

    var found_solution = false;

    while (!found_solution) {
      var current_node = frontier.pop();
      // console.log("gval: "+current_node.g);
      // console.log("state: "+current_node.state);
      this.state = deep_copy(current_node.state); //pop the puzzle state with the lowest f-val
      if (this.is_goal_state()) {
        found_solution = true;
        console.log("found!!!");
        console.log(this.print_solution(current_node.path));
        this.state = deep_copy(initial_state);
        this.render();
        // found the solution!!!
        // trace back and print out the solution
      } else {
        console.log("enqueueing children...");
        // enqueue the children
        var save_state = deep_copy(this.state);
        this.clockwise();
        frontier.push({ state: deep_copy(this.state), g: current_node.g+1, path: current_node.path+"U" }, current_node.g+1 + this.get_heuristic_value(this.state));
        this.state = deep_copy(save_state);
        this.counter_clockwise();
        frontier.push({ state: deep_copy(this.state), g: current_node.g+1, path: current_node.path+"D" }, current_node.g+1 + this.get_heuristic_value(this.state));
        this.state = deep_copy(save_state);
        this.flip();
        frontier.push({ state: deep_copy(this.state), g: current_node.g+1, path: current_node.path+"F" }, current_node.g+1 + this.get_heuristic_value(this.state));
      }
    }

  }

  //returns true if the puzzle is in the goal state. returns false otherwise.
  this.is_goal_state = function() {
    if (this.state[1] != 0) // make sure the tubes are aligned, as part of the solved state
      return false;
    var found_bad_ball = false;
    for (var i=2;i<8;i++) { // look through left side
      for (var j=0; j<this.state[i].length; j++) {
        if (this.state[i][j] != 8-i)
          found_bad_ball = true;
      }
    }
    if (!found_bad_ball)
      return true;
    found_bad_ball = false;
    for (var i=8;i<14;i++) { // look through left side
      for (var j=0; j<this.state[i].length; j++) {
        if (this.state[i][j] != 14-i)
          found_bad_ball = true;
      }
    }
    return !found_bad_ball;
  }

  //returns true if the puzzle is in a "potentially solved" state. i.e. tubes aligned and all balls on one side.
  //returns false otherwise.
  this.is_flat_state = function() {

    if (this.state[1] != 0) // make sure the tubes are aligned, as part of the solved state
      return false;
    else {
      var found_zero = false, found_non_zero = false;
      for (var i=2;i<8;i++) { // look through just the left-hand side of the puzzle. That's all we need.
        for (var j=0; j<this.state[i].length; j++) {
          if (this.state[i][j])
            found_non_zero = true;
          else
            found_zero = true;
        }
      }
      // returns true if either of the found_zeroes stays false
      // it can be all 0s or all !0s, but not a mix.
      return ( !(found_zero) || !(found_non_zero) );
    }
  }

  // This algorithm scrambles the puzzle such that it may be solved in no more than [k] moves.
  // Thanks to: Anton Vinogradov, for presenting this method on the discussion boards.
  // it returns a solution_string: made up of
  //    U: clockwise
  //    D: counter_clockwise
  //    F: flip
  // use this.print_solution() to get a readable print-out
  this.scramble = function(k) {

    // first, make sure the puzzle is sufficiently scrambled for our initial state
    // I believe this is the proper way, because the Atomic Chaos has no unsolvable states, but does have unreachable states (As all but proven by Emory on the discussion boards).
    // We can dodge those unreachable states by only using a valid arrangement of balls. Which re reached by valid scrambles.
    for (var i=0; i<100; i++) { //I thought 100 would be good enough to scramble. I'm pretty sure it is.
      var rand = getRandomInt(0,3);
      if (rand == 0)
        this.clockwise();
      else if (rand == 1)
        this.counter_clockwise();
      else
        this.flip();
    }

    //save the completely random state
    var save_state = deep_copy(this.state);

    //mark the balls 1-21 in the save_state
    var ball_number = 1;
    for (var i=2; i<14; i++) {
      for (var j=0; j<save_state[i].length; j++) {
        if (save_state[i][j])
          save_state[i][j] = ball_number++;
      }
    }

    var solution_string = "";
    var rejected_strings = 0;

    //now, we find the path to a flat state.
    do {
      //load from the save state
      this.state = deep_copy(save_state);
      solution_string = "";
      rejected_strings++;

      var last_move = -1;
      var last_drop = true;
      var move_lookup = ["U","D","F"];

      // make k random solving moves (no 2 flips in a row)
      // interestingly, this move generator does not account for the circular property of the puzzle.
      // that would be my next area of improvement
      for (var i=0; i<k; i++) {
        var random_move;

        if (last_move == 2) //make sure we have a turn if the last move was a flip
          random_move = getRandomInt(0,2);
        else {
          if (!last_drop) { //if the last move didn't drop a ball, don't turn the opposite direction as last move.
            if (last_move == 0)
              random_move = 2*getRandomInt(0,2); //either a 0 or a 2
            else
              random_move = getRandomInt(1,3); //either a 1 or a 2
          } else {
            random_move = getRandomInt(0,3);
          }
        }

        if (random_move == 0) {
          last_move = 0;
          last_drop = this.clockwise();
        }
        else if (random_move == 1) {
          last_move = 1;
          last_drop = this.counter_clockwise();
        }
        else {
          last_move = 2;
          last_drop = this.flip();
        }
        solution_string+=move_lookup[last_move];
      }
    } while (!this.is_flat_state() || last_move == 2); //make sure we end in a flat state

    // console.log("rejected " + rejected_strings + " strings");

    //OKAY! we finally have an initial scrambled state (save_state) and a mixed flat state (this.state)
    //now the magic really begins.
    //let's pretend like the flat state we're in now is the solved state.
    //then go back in time to make that initial sequence of [k] moves from the initial state so we fall into place

    if (console_mode) {
      console.log("initial scrambled state (save_state):");
      console.log(save_state);
      console.log("flat state (this.state):");
      console.log(this.state);
      console.log(solution_string);
    }

    //Now we need to go through the flat state, and find out which numbers are associated with which color ball.
    var colors = [0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0]; //I'm sure there's a better way to do this lol
    for (var i=2; i<14; i++) {
      for (var j=0; j<this.state[i].length; j++) {
        if (this.state[i][j])
          colors[this.state[i][j]] = this.state[i].length;
      }
    }

    //The final step. We have now the colors that the balls in the save_state should be. Let's go back in time and fill those in.
    for (var i=2; i<14; i++) {
      for (var j=0; j<save_state[i].length; j++) {
        if (save_state[i][j])
          //get the color for the ball from the lookup table "colors".
          save_state[i][j] = colors[save_state[i][j]];
      }
    }
    //load from the newly reassigned save_state...
    this.state = deep_copy(save_state);

    //and we have a puzzle state solvable in no more than k moves.
    this.render();
    return solution_string;
  }

  //Because of dumb modulo implementation which is apparently inherited from C,
  //I had to do the ol' (((x)+n)%n) trick...
  //see: http://stackoverflow.com/questions/18618136/how-to-calculate-modulo-of-negative-integers-in-javascript
  //Basically, % is not modulo in the traditional sense. It is "remainder". So it keeps the sign of the numerator.
  //I wanted my result to always be positive, so I first add 6 before taking the modulus.
  //Since I'm only ever decrementing by 1, this should do the trick. vvvvv

  //returns true if the turn dropped a ball. False if it did not drop a ball
  this.clockwise = function() {
    this.state[1] = ((this.state[1]-1)+6)%6;
    var last_drop = this.resolve();
    this.render();
    return last_drop;
  }
  //returns true if the turn dropped a ball. False if it did not drop a ball
  this.counter_clockwise = function() {
    this.state[1] = (this.state[1]+1)%6;
    var last_drop = this.resolve();
    this.render();
    return last_drop;
  }
  this.flip = function() {
    this.state[0] = !this.state[0];
    this.resolve();
    this.render();
  }
  this.print_state = function() {
    console.log(this.state);
    for (var row=0;row<6;row++) {
      var rowlen = this.get_rowlen(row);
      for (var col=0; col<rowlen; col++) {
        console.log("state["+this.get_state_index(row,col)+"]["+this.get_col_index(row,col)+"] = "+this.state[this.get_state_index(row,col)][this.get_col_index(row,col)]);
      }
    }
  }

  // This function takes a solution_string and returns a large chunk of text that displays the move sequence nicely.
  this.print_solution = function(solution_string) {
    var text_block = "The known solution is " + solution_string.length + " moves long: \n";
    for (var i = 0; i < solution_string.length; i++) {
      text_block += i;
      // Thanks to: http://www.w3schools.com/jsref/jsref_charat.asp
      if (solution_string.charAt(i) == "U") {
        text_block += ". Clockwise\n";
      } else if (solution_string.charAt(i) == "D") {
        text_block += ". Counter-clockwise\n";
      } else {
        text_block += ". Flip\n";
      }
    }
    text_block += "\n Solution complete.\n";
    return text_block;
  }

  //This is a function that resolves the puzzle according to gravity (state[0]) and to the position (state[1])
  //returns true if a ball dropped in that move. False if a ball did not drop.
  this.resolve = function() {
    //if state[1] == 0
    //  2 <--> 8;   3 <--> 9;   4 <--> 10;  5 <--> 11;  6 <--> 12;  7 <--> 13;
    //if state[1] == 1
    //  2 <--> 13;  3 <--> 8;   4 <--> 9;   5 <--> 10;  6 <--> 11;  7 <--> 12;
    //if state[1] == 2
    //  2 <--> 12;  3 <--> 13;  4 <--> 8;   5 <--> 9;   6 <--> 10;  7 <--> 11;
    //if state[1] == 3
    //  2 <--> 11;  3 <--> 12;  4 <--> 13;  5 <--> 8;   6 <--> 9;   7 <--> 10;
    //if state[1] == 4
    //  2 <--> 10;  3 <--> 11;  4 <--> 12;  5 <--> 13;  6 <--> 8;   7 <--> 9;
    //if state[1] == 5
    //  2 <--> 9;   3 <--> 10;  4 <--> 11;  5 <--> 12;  6 <--> 13;  7 <--> 8;
    //lol
    var ball_dropped = false;

    if (this.state[0]) { //If gravity is to the left <----
      //start from the top-left-most ball
      for (var row=0; row<6; row++) { //loop through each row, starting with the top row
        var rowlen = this.get_rowlen(row);
        for (var col=0; col<rowlen; col++) { //go through each cell in the particular row.

          //find the "correct" place to look in the state array. This is necessary because arrays line up differently based on rotation
          var col_index = this.get_col_index(row, col);
          var state_index = this.get_state_index(row, col);

          if (this.state[state_index][col_index] != 0) { //Wait till we find a ball
            var decreasing_col = col;
            //while we're not out of bounds, drop down the ball into empty slots below it.
            while (decreasing_col > 0 && this.state[this.get_state_index(row, decreasing_col-1)][this.get_col_index(row, decreasing_col-1)] == 0) {
              //Swap ball with empty slot below
              this.state[this.get_state_index(row, decreasing_col-1)][this.get_col_index(row, decreasing_col-1)] = this.state[this.get_state_index(row, decreasing_col)][this.get_col_index(row, decreasing_col)];
              this.state[this.get_state_index(row, decreasing_col)][this.get_col_index(row, decreasing_col)] = 0;
              decreasing_col--;
              //IF IT GETS HERE, A BALL DROPPED!
              ball_dropped = true;
            }
          }
        }
      }
    } else { //If gravity is to the right ---->
      //start from the top-right-most ball
      for (var row=0; row<6; row++) {
        var rowlen = this.get_rowlen(row);
        for (var col=rowlen-1; col>=0; col--) {

          var col_index = this.get_col_index(row, col);
          var state_index = this.get_state_index(row, col);

          if (this.state[state_index][col_index] != 0) { //if theres an empty slot, go to the next iteration
            var increasing_col = col;
            //while we're not out of bounds, drop down the ball into empty slots below it
            while (increasing_col < rowlen-1 && this.state[this.get_state_index(row, increasing_col+1)][this.get_col_index(row, increasing_col+1)] == 0) {
              //Swap ball with empty slot below
              this.state[this.get_state_index(row, increasing_col+1)][this.get_col_index(row, increasing_col+1)] = this.state[this.get_state_index(row, increasing_col)][this.get_col_index(row, increasing_col)];
              this.state[this.get_state_index(row, increasing_col)][this.get_col_index(row, increasing_col)] = 0;
              increasing_col++;
              //OR IF IT GETS HERE, A BALL DROPPED!
              ball_dropped = true;
            }
          }
        }
      }
    }

    return ball_dropped;

  }
  //returns the length of a row in the puzzle
  this.get_rowlen = function(row) {
    return this.state[row+2].length + this.state[8+(row-this.state[1]+6)%6].length;
  }
  //Returns a in state[a][b] based on row and col
  //row should be [0-5], representing the row (duh)
  //col should be [0-11], but not OOB on a particular row (maximum is get_rowlen(row)-1)
  this.get_state_index = function(row, col) {
    if (col < 0 || col >= this.state[row+2].length + this.state[8+(row-this.state[1]+6)%6].length)
      return -1;
    if (col >= this.state[row+2].length) {
      return 8+(row-this.state[1]+6)%6;
    } else {
      return row+2;
    }
  }
  //Returns b in state[a][b]
  this.get_col_index = function(row, col) {
    if (col < 0 || col >= this.state[row+2].length + this.state[8+(row-this.state[1]+6)%6].length)
      return -1;
    if (col >= this.state[row+2].length) {
      return col - this.state[row+2].length;
    } else {
      return col;
    }
  }

  this.get_heuristic_value = function(current_state) {
    //max(int number_of_tubes_with_an_incorrectly_colored_ball_on_the_lower_half, int number_of_turns_to_properly_align_tubes);
    var turns = current_state[1];
    if (turns > 3) // state[1]==4 => turns==2, state[1]==5 => turns==1
      turns = 6-turns;
    var tubes = 0;
    if (current_state[0]) { // <--- grav
      for (var i = 2; i < 8; i++) {
        var found_wrong_ball = false;
        for (var j = 0; j < current_state[i].length; j++) {
          if (current_state[i][j] != 8-i && current_state[i][j] != 0) //if the ball is incorrectly colored and not blank
            found_wrong_ball = true;
        }
        if (found_wrong_ball)
          tubes++;
      }
    } else {
      for (var i = 8; i < 14; i++) {
        var found_wrong_ball = false;
        for (var j = 0; j < current_state[i].length; j++) {
          if (current_state[i][j] != 14-i && current_state[i][j] != 0) //if the ball is incorrectly colored and not blank
            found_wrong_ball = true;
        }
        if (found_wrong_ball)
          tubes++;
      }
    }
    return Math.max(tubes, turns);
  }

  //This is a function to create the Array to define the current state of the atomic_chaos
  //The first number in the array has a value [0,1]: representing the direction in which the puzzle is flipped.
  //The second number in the array has a value [0,5]: representing the relative positions of the two sides of the puzzle
  //The next 6 arrays represent what each tube on the left side of the puzzle contains
  //The next 6 arrays represent what each tube on the right side of the puzzle contains
  this.create_state_array = function() {
    var arr = new Array(14);
    arr[0] = 1; //gravity
    arr[1] = 0; //
    var len=6;
    var position_in_arr = 2;
    //Left 6 tubes
    while (len--) {
      arr[position_in_arr] = new Array(len+1);
      //populate the array with balls
      for (var i=0;i<arr[position_in_arr].length;i++)
        arr[position_in_arr][i] = len+1;
      position_in_arr++;
    }
    len=6;
    //Right 6 tubes
    while (len--) {
      arr[position_in_arr] = new Array(len+1);
      //populate the array with zeroes
      for (var i=0;i<arr[position_in_arr].length;i++)
        arr[position_in_arr][i] = 0;
      position_in_arr++;
    }
    return arr;
  }
  //when called, renders the puzzle on the screen
  this.render = function() {
    // clear artifacts from last state
    clear();

    fill("#ffffff");
    //Left side of atomic_chaos
    rect(0, 0, 300, 50);
    rect(50, 50, 250, 50);
    rect(100, 100, 200, 50);
    rect(150, 150, 150, 50);
    rect(200, 200, 100, 50);
    rect(250, 250, 50, 50);

    var right_side_top = this.state[1]*50;
    //Right side of atomic_chaos
    rect(300, (right_side_top+0)%300, 300, 50);
    rect(300, (right_side_top+50)%300, 250, 50);
    rect(300, (right_side_top+100)%300, 200, 50);
    rect(300, (right_side_top+150)%300, 150, 50);
    rect(300, (right_side_top+200)%300, 100, 50);
    rect(300, (right_side_top+250)%300, 50, 50);
    //render the balls on the left
    for (var i=2; i<8; i++) {
      var iteration = i-2;
      for (var j=0; j<this.state[i].length; j++) {
        if (this.state[i][j] > 0) {
          fill(this.getRGB(this.state[i][j]));
          ellipse(iteration*50 + j*50, iteration*50, 50);
        }
      }
    }
    //render the balls on the right
    for (var i=8; i<14; i++) {
      var iteration = i-8;
      for (var j=this.state[i].length-1;j>=0; j--) {
        if (this.state[i][j] > 0) {
          fill(this.getRGB(this.state[i][j]));
          // ellipse(550-(iteration*50 + j*50), (iteration*50 + state[1]*50)%300, 50);
          ellipse(300+(j*50),(iteration*50 + this.state[1]*50)%300, 50);
        }
      }
    }
  }
  this.getRGB = function(color) {
    if (color == 6)
      return "#ff3333";
    else if (color == 5)
      return "#5cd65c";
    else if (color == 4)
      return "#e6e600";
    else if (color == 3)
      return "#3385ff"
    else if (color == 2)
      return "#d9d9d9"
    else
      return "#333333"
  }
}

//I plan to write my own "deep copy" function at some point. For now it's this one:
//http://blog.andrewray.me/how-to-clone-a-nested-array-in-javascript/
function arrayClone( arr ) {

    var i, copy;

    if( Array.isArray( arr ) ) {
        copy = arr.slice( 0 );
        for( i = 0; i < copy.length; i++ ) {
            copy[ i ] = arrayClone( copy[ i ] );
        }
        return copy;
    } else if( typeof arr === 'object' ) {
        throw 'Cannot clone array containing an object!';
    } else {
        return arr;
    }

}

//here's my deep copy function. It copies values from the source state array (first argument) into the destination state array (returned)
function deep_copy( source_state_array ) {
  var arr = [];
  arr.push(source_state_array[0]); //copy gravity
  arr.push(source_state_array[1]); //copy relative tube position
  for (var i = 2; i < 14; i++) {
    var inner_arr = [];
    for (var j = 0; j < source_state_array[i].length; j++) {
      inner_arr.push(source_state_array[i][j]);
    }
    arr.push(inner_arr);
  }
  return arr;
}

//Returns a random integer in [min, max)
function getRandomInt(min, max) {
  return Math.floor(Math.random() * (max - min)) + min;
  //Thanks to:
  //https://www.freecodecamp.com/challenges/generate-random-whole-numbers-with-javascript
}

// I found a javascript implementation of a priority queue online, written by user GRIFFnDOOR
// http://fiddle.jshell.net/GRIFFnDOOR/r7tvg/
function Node (data, priority) {
    this.data = data;
    this.priority = priority;
}

// takes an array of objects with {data, priority}
function PriorityQueue (arr) {
    this.heap = [null];
    if (arr) for (i=0; i< arr.length; i++)
        this.push(arr[i].data, arr[i].priority);
}

PriorityQueue.prototype = {
    push: function(data, priority) {
        var node = new Node(data, priority);
        this.bubble(this.heap.push(node) -1);
    },

    // removes and returns the data of highest priority
    pop: function() {
        var topVal = this.heap[1].data;
        this.heap[1] = this.heap.pop();
        this.sink(1); return topVal;
    },

    // bubbles node i up the binary tree based on
    // priority until heap conditions are restored
    bubble: function(i) {
        while (i > 1) {
            var parentIndex = i >> 1; // <=> floor(i/2)

            // if equal, no bubble (maintains insertion order)
            if (!this.isHigherPriority(i, parentIndex)) break;

            this.swap(i, parentIndex);
            i = parentIndex;
    }   },

    // does the opposite of the bubble() function
    sink: function(i) {
        while (i*2 < this.heap.length) {
            // if equal, left bubbles (maintains insertion order)
            var leftHigher = !this.isHigherPriority(i*2 +1, i*2);
            var childIndex = leftHigher? i*2 : i*2 +1;

            // if equal, sink happens (maintains insertion order)
            if (this.isHigherPriority(i,childIndex)) break;

            this.swap(i, childIndex);
            i = childIndex;
    }   },

    // swaps the addresses of 2 nodes
    swap: function(i,j) {
        var temp = this.heap[i];
        this.heap[i] = this.heap[j];
        this.heap[j] = temp;
    },

    // returns true if node i is higher priority than j
    isHigherPriority: function(i,j) {
        return this.heap[i].priority < this.heap[j].priority;
    }
}
