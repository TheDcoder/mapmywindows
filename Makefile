compiler = gcc
program = mapmywindows

mapmywindows: $(program).c
	$(compiler) -lxdo -Wall -o $(program) $(program).c

clean: $(program)
	rm $(program)
