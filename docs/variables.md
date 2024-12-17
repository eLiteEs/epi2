You can use variables for storing text, numbers or a boolean.

You can define a variable by two ways.

**1. Setting a default value to the variable**
 Example:
```
var name = "name"
```
This would create a new variable with the name `name` and the text `name` to it, so the type of that variable will be `string`. 
Another example:

```
var num = 10
```
This example creates a variable with the name `num` and the number `10` as the content. The variable will have the type `number`.

**2. Setting the variable type to the variable**
Example:
```
var name as string
```
That creates an empty variable with the name `name` and the type `string`.

Example with all the types of variables:

```
var name as string
var num as number
var flag as boolean

name = "Some text"
num = 10
flag = true

num = "aaa" @ This would throw invalid_argument
flag = "more text" @ This also throws invalid_argumnet

name = true @ This is correct, sets the variable content to "true"
```