The `delete` keyword besides being usseful for deleting files it can clear values from variables.

On this example, the value of the string `passwd` will be cleared:
```
var passwd as string

inp passwd

print "Password: " + passwd

delete passwd
```

It works on `String`, `Number` and `Boolean`.