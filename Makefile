BINDIR = /usr/local/bin/

ifeq ($(SUDO_USER),)
  USER_HOME = $(HOME)
else
  USER_HOME = $(shell getent passwd $(SUDO_USER) | cut -d: -f6)
endif

DATADIR = $(USER_HOME)/.local/share/wordle

all:
	gcc main.c -o wordle

install:
	install -Dm755 wordle $(BINDIR)
	mkdir $(DATADIR)
	cp words.txt $(DATADIR)

uninstall:
	rm $(BINDIR)/wordle
	rm -r $(DATADIR)


clean:
	rm -f wordle
