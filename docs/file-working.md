You can edit a file content on epi² using the `File` variable and a few functions.

To create a variable which stores the file information, do this:
```
File file is "file.txt"
```

Then, you can write text to it by uding the `write` function. Example: 
```
write "Hello" to file
```

You can also append text to it. Example:
```
append " World!" to file
```

If you haven't defined the filename when you created the `File` variable, you can do it later using the `open` function. Example:
```
open file at "file.txt"
```
And you can check if the file was opened by using the `open` function. Example:
```
print open file
```
That returns `true` if the file was opened and `false` if not.

You can read a file with the `read` function. Example:
```
print read file
```

For deleting a file you can use the `delete` function. Example:
```
delete file
```
