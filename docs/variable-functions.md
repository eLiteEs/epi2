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
| Substring | `str.Substring(arg1, arg2)` | `string` | `arg1` (`number`): Index of start of substring of the string.<br> `arg2` (`number`): End of the substring of a string. | Returns the substring of a string between `arg1` and `arg2`. | `text.Substring(2,4)`
| Find | `str.Find(arg)` | `number` | `arg` (any): Query to search on the `string` | Returns the index of the first ocurence of `arg` inside the variable. | `text.Find("Text")`
| Contains | `var.Contains(arg)` | `boolean` | `arg` (any): Query to search on the `string` | Returns `true` if the `string` contains `arg`, `false` if not. | `text.Contains("text")`
| Add | `var.Add(arg)` | `string` | `arg` (any): Text to add to the `string` | Returns the concatenation of the `string` and `arg`. | `text.Add("More text")`
| Insert | `var.Insert(arg1, arg2)` | `string` | `arg1` (`number`): Index to add the text `arg2`<br>`arg2` (any): Text to insert in the index `arg1` | Returns the `string` with the text of `arg2` inserted at the `string` index of `arg1`. | `var.Insert(2, "Text")`
| At | `var.At(arg)` | `string` | `arg` (`number`): Index to search on the `string` | Returns the character at the index `arg` on the `string` | `text.At(9)`
| Split | `var.Split(arg1, arg2)` | `string` | `arg1` (any): Text to use for splitting the `string`<br>`arg2` (`number`): Index to get on the splt function | Returns the text between the ocurence `arg2` of `arg1` twice. | `text.Split(";", 1)`
| Crypt | `var.Crypt(arg)` | `string` | `arg` (any): Key to use for encrypting the `strring` | Returns the encrypted text of `atring` using as key `arg` | `text.Crypt("mykey")`
| Replace Index | `var.ReplaceIndex(arg1, arg2, arg3)` | `string` | `arg1` (number): Index to replace the occurences<br>`arg2` (any): Text to find on the `string`<br>`arg3` (any): Replace this with the `arg2` | Returns the `string` replacing `arg2` with `arg3` on the index `arg1` | `text.ReplaceIndex(0, "dog", "cat")`
| Replace All | `var.ReplaceAll(arg1, arg2)` | `string` | `arg1` (any): Text to find on the `string` for replacing it.<br>`arg2` (any): Replace thw occurences of `arg1` with this | Returns the `string` replacing all the occurences of `arg1` with `arg2` | `text.ReplaceAll("dog", "cat")`