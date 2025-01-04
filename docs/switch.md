On epi², there's something like an if that checks if something is something. Example:

```
printc "Enter username: "
in username

switch username
    case "admin"
        print "Hello, admin"
        print "Admin menu:"
    case "dog"
        print "Access ungranted"
    default
        print "Hi, " + username + "!"
```

The code in `default` is runned if no other option was `true`.