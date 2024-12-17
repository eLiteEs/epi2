## Output Text

For showing text into the console, you can use `print` or `out` functions. Example:

```
print "Hello World!"
out "Hello World!"
```
That outputs:
```
Hello World!
Hello World!
```
You can use `printc` for not adding a new-line after the printed text. Example:
```
printc "Hello, "
print "World!"
```
Output:
```
Hello, World!
```

## Input Text
For letting the user to introduce text in the console and then store it on a variable, you can use `in` function. Example:
```
printc "Enter username: "
in username
print "Hello, " + username
```
Output:
```
Enter username: Cat
Hello, Cat
```
In that example I use `printc` function for making the user-input more comfortable.

You can define previously a variable and then use it on an user-input. Example:
```
var username as string
in username
```
The `in` function will check if the text introduced is the correct tpe of the variable.
For example, if you enter the text `Hello` on a user-input that uses a variable with the type `number`, it would throw the exception `invalid_argument`. Example:
```
var name as string @ This creates a variable of the type "string"
in name @ The user can introduce any type of variable here

var num as number @ This creates a variable of the type "number"
in num @ The user must introduce a number here
```

You can make an user-input which hides the text that the user introduced or puts other character for hiding it. You can use `inn` and `inp` functions. Example:
```
printc "Enter password: "
inn password @ This will let the user enter text, but it will not show it
```
Example of `inp` function:
```
printc "Enter password: "
inp password "*" @ This will let the user enter text, but show the symbol * instead of the text.
```