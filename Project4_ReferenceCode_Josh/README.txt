Name
====
Joshua Nahum

This code passes all the tests for Project 4. I was a bit lazy with the error
messages, but our tests only check that you raised an error, but not the
exact error message.

My makefile is different from Charles'. 'make' by default creates a tube executable
file, then renames it to the tube? (tube4 in this instance) according to the variable 
at the top. A helper target('make exam') makes the executable, then runs it against
the example.tube file, outputing the example.tic and TubeIC output for easy testing.
The make file also cleans up after itself on every build, leaving only the executable.
But 'make clean' removes the executable as well.
