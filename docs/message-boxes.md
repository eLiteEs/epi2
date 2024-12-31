You can make a message box to inform the user about something.

For this, you'll need to use `~include` function for initializing that function.

And then you can use `msgbox` function. Example:
```
~include gui

msgbox "Hello World!"
```

You can set a title to the message box by adding a second parameter. Example:
```
~include gui

msgbox "Hello World!", "MyProgram"
```

The main thread of the program stops when a message box is shown.
