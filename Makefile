compiler = gcc
program = mapmywindows

$(program): $(program).c
	$(compiler) -lxdo -Wall -o $(program) $(program).c

install: $(program)
	cp $(program) /usr/bin
	
uninstall:
	rm /usr/bin/$(program)
	
clean: $(program)
	rm $(program)
