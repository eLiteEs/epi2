Using the `external` keyword, you can run code from other programming languages.

This example runs code from Python:
```
external "python"
    print("Hello, World!")
    username = input("username: ")
    print("Hello, " + username)
```

You can also run `bash` and `batch` commands.


Something interesting about `external` keyword is that the following text which defines the language doesn't need to be pre-defined, it can be a variable. So you can make a quiz for the user like this:

```
print "Which programming language will run this?"
print "echo Hello"

in lang

external lang
    echo Hello
```