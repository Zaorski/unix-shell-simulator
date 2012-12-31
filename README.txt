OWNER: Thomas Zaorski
EMAIL: zaorst@rpi.edu

This project is a simulation of the unix shell written in C. You should be able to use most of the commands you could in
the actual shell.  Enter & at the end of a command to run a process in the background. Redirect stdin with <== and redirect 
stdout with ==>. The special string --> can be used to redirct stdout in such a way that it appends to the end of the
given file instead of doing an overwrite.

Included is a file called sleeper.c which sleeps for a few seconds and then prints "HELLO". It was used to test if processes
could run in the background correctly

A few things of importance about my shell:

1) In order to run local directory commands in my shell you must type in "./" before the name of the file. For example, one must enter something like "./a.out". For system commands, you can simply type them out as you would normally. For example "ls -l -h".

2) If you run a program in the background that periodically prints out a statement, when it prints it will force 
the text prompt onto a new line without the "--|". After entering your next command it should once again appear on a line
with the "--|".

3) All commands can only accept up to 4 arguments in my shell.

4) If you run a child process that prints something after
a certain amount of time in the background and then entered "exit" or "quit" before it ends, my shell will print "bye" and 
then wait for the child to finish before returning you to the real shell.
