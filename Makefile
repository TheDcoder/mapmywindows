compiler = gcc
program = mapmywindows

$(program): $(program).c
	$(compiler) -lX11 -lxdo -Wall -o $(program) keyboard_shortcuts.c $(program).c
	
install: $(program)
	cp $(program) /usr/bin
	
uninstall:
	rm /usr/bin/$(program)
	
clean: $(program)
	rm $(program)
