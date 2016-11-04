Evan Heaton
CS465g: Artificial Intelligence
Prolog 2
11/3/2016

### Explanation

My program works by first finding a path to the egg. Once the egg is found, it will find a path to pikachu and the master ball (in any order). Finally, once all these items are found, it will make a path to the mewtwo, and terminate.
The reason my program finds the egg first is to guarantee that the egg will be hatched by the time it terminates on the mewtwo square. At that point, it can count the egg as being worth 10 points. By finding the egg first, the path will consist of the following: egg -> pika -> masterball -> mewtwo OR egg -> masterball -> pika -> mewtwo. Both of these cases have at minimum 3 spaces walked after finding the egg.
At the end of the pathfinding, the score is counting by seeing which items were collected. If the egg was collected, +10. If the pikachu was collected, +1.
View the comments in the source file for more detailed information.

### Learning Outcome

From this assignment, I learned that sometimes I have to come up with cheeky solutions to problems when I'm not familiar enough with the language I'm writing the solution in. I'd say that's actually my favorite part of programming: finding clever solutions to problems to save either real time or computational time. Additionally, I learned a lot more about how Lists work in prolog. I had trouble figuring out how to make te score count work in this program, because I couldn't get the Score to be initialized at zero without making the Score variable unify with 0... Eventually I found a workaround.
All in all, this program was pretty difficult, so I feel pretty good about finishing it. :)  

