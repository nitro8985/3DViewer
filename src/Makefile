CC=gcc
CFLAGS= -std=c11
LIBS=
SOURCES= parse.c matrix.c
GUI= gui.c

GTK3=`pkg-config --cflags --libs gtk+-3.0` -rdynamic

MW=`pkg-config --cflags --libs MagickWand`

OS=$(shell uname)
ifeq ($(OS), Linux)
#	LIBS=-lcheck -lpthread -lrt -lm
	LIBS=-lcheck -lpthread -lrt -lm -lsubunit 
	OPEN=xdg-open
else
	LIBS=-lcheck -lm
	OPEN=open
endif

all: viewer

re: clean viewer run

run:
	./viewer

viewer:
	mkdir -p pics
	$(CC) -o viewer $(GUI) $(SOURCES) $(GTK3) $(MW) $(CFLAGS) -lm

install: all
	-./install.sh 
	
uninstall:
	-./uninstall.sh

dvi:
	-texi2dvi -q readme.tex
	-texi2pdf -q readme.tex

dist:
	rm -rf 3DViewer_v1.0/
	mkdir 3DViewer_v1.0/
	mkdir 3DViewer_v1.0/src
	cp Makefile *.c *.h 3DViewer_v1.0/src/
	cp *.glade 3DViewer_v1.0/src/
	cp *.sh 3DViewer_v1.0/src/
	cp *.tex 3DViewer_v1.0//src/	
	tar cvzf 3DViewer_v1.0.tgz 3DViewer_v1.0/
	rm -rf 3DViewer_v1.0/

tests:
	$(CC) $(CFLAGS) test.c $(SOURCES) -o test.out $(LIBS)
	./test.out

gcov_report:
	$(CC) $(CFLAGS) -o test.out --coverage $(SOURCES) test.c $(LIBS)
	./test.out
	lcov -t "backend test" -o test.info -c -d .
	genhtml -o report test.info

clean:
	rm -rf viewer *.gcda *.gcno *.info *.dSYM *.info report *.cfg *.out *.tgz *.log *.aux *.out *.jpeg *.bmp *.gif *.pdf *.dvi pics/*.*

check: 
	cp ../materials/linters/CPPLINT.cfg ./CPPLINT.cfg
	python3 ../materials/linters/cpplint.py --quiet  --extension=c *.c *.h
	cppcheck --quiet --enable=all --suppress=missingIncludeSystem --suppress=unusedFunction *.c *.h
	rm *.cfg

leaks: clean tests
	CK_FORK=no leaks --atExit --  ./test.out

valgrind: clean tests
	CK_FORK=no valgrind --tool=memcheck --leak-check=full --show-leak-kinds=all --show-reachable=yes ./test.out

env:
ifeq ($(OS), Linux)
	sudo apt update
	sudo apt install -y libgtk-3-dev
	sudo apt install -y glade
	sudo apt install -y libmagickwand-dev
	sudo apt install -y texinfo
	sudo apt install -y texlive
	sudo apt install -y lcov
else
	echo "Please install the necessary modules if you have enough disk space"
	brew install gtk+3
	brew install glade
	brew install imagemagick
	brew install texinfo
	brew install texlive
endif

rebuild: clean all
