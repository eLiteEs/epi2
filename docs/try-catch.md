If code might will throw an exception, you can use `try` and `catch` statements for avoiding that error stopping the code.
Example:
```
var num as number

try
    num = "string"
catch epi2.error.invalid_argument
    print "That's not a number."
```