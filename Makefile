compiler = gcc
program = mapmywindows
man = $(program).1

.PHONY: install uninstall clean

$(program): $(program).c
	$(compiler) -Wall -o $(program) xkeymacro/xkeymacro.c $(program).c -lX11 -lxdo

$(man).gz: $(program)
	help2man --no-info --output $(man) ./$(program)
	gzip $(man)

install: $(program) $(man).gz
	install $(program) /usr/bin
	install $(man).gz /usr/share/man/man1

uninstall:
	rm /usr/bin/$(program)
	rm /usr/share/man/man1/$(man).gz

clean:
	-rm $(program)
	-rm $(man).gz
