# Functions
You can store runeable code inside a function for writing a lot of code faster, without repeating lines of code.
Example:
```
function helloWorld:
    print "Hello, World!"

#helloWorld
#helloWorld
#helloWorld
```
Output:
```
Hello, World!
Hello, World!
Hello, World!
```

Explanation:
1. `function helloWorld:`: Defines a function with the name `helloWorld`. it's important to have a `:` symbol at the end of the line so the program doesn't fails.
2. `    print "Hello, World!"`: This is the line that the function will have, is important to add 4 spaces before the code for making the compiler know that it's writing the function. You can add more than one line of code to a function.
3. `#helloWorld`: Runs the function