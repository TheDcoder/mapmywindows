compiler = gcc
program = mapmywindows

$(program): $(program).c
	$(compiler) -Wall -o $(program) xkeymacro/xkeymacro.c $(program).c -lX11 -lxdo
	
install: $(program)
	cp $(program) /usr/bin
	
uninstall:
	rm /usr/bin/$(program)
	
clean: $(program)
	rm $(program)
