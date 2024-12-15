# Variable Functions
This are functions that are useful for working with variables.
There are 2 types of Variable Functions.
1. **Multi-Type functions**, works with any type of variables. Examples: `Type`, `Equals`, etc...
2. **Single-Type functions**, this functions only work for one type of functions, specially strings. Examples: `Length`, `Substring`, `Find`, etc...

Here you will find a full reference of this functions.
### Multi-type Functions
| Name | Call | Returns | Arguments | Explanation | Example |
|-|-|-|-|-|-|
| Type | `var.Type` | `string` | - | Returns the type of the variable: `string`, `number` or `boolean` | `name.Type`
| Value | `var.Value` | Depending on the variable content |-| Returns the content of the variable | `name.Value`
| Equals | `var.Equals(arg)` | `boolean` | `arg` (any): The another value to compare | Returns `true` if the two values are equal, if not, returns `false`. | `name.Equals("admin")`

### Single-Type Functions
#### String Functions
On epi, this functions were called `String Functions`.
| Name | Call | Returns | Arguments | Explanation | Example |
|-|-|-|-|-|-|
| Length | `str.Length` | `number` | - | Returns the size of the `string` | `text.Length`
| Empty | `str.Empty` | `boolean` | - | Returns `true` if the `string` is empty | `text.Empty`
| Is Number | `str.IsNumber` | `boolean` | - | Returns `true` if the variable can be converted to a `number` | `str.IsNum`
| Is Boolean | `str.IsBoolean` | `boolean` | - | Returns `true` if the variable can be converted to a `boolean` | `str.IsBoolean`
| Substring | `str.Substring(arg1, arg2)` | `string` | `arg1` (`number`): Index of start of substring of the string. `arg2` (`number`): End of the substring of a string. | Returns the substring of a string between `arg1` and `arg2`. | `text.Substring(2,4)`
| Find | `str.Find(arg)` | `number` | `arg` (any): Query to search on the `string` | Returns the index of the first ocurence of `arg` inside the variable. | `text.Find("Text")`