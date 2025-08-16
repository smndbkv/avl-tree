FLAGS=-O3 -mfpmath=sse -fstack-protector-all -g -W -Wall -Wextra -Wunused -Wcast-align -Werror -pedantic -pedantic-errors -Wfloat-equal -Wpointer-arith -Wformat-security -Wmissing-format-attribute -Wformat=1 -Wwrite-strings -Wcast-align -Wno-long-long -Woverloaded-virtual -Wnon-virtual-dtor -Wcast-qual -Wno-suggest-attribute=format

all: a.out git

git:
	git add .

a.out: main.o
	g++ $(FLAGS) $^  -o $@

main.o: main.cpp avl_tree.h
	g++ -c $(FLAGS) $< -o $@

clean:
	rm -f *.o *.out

zip:
	zip avl_tree.zip *.cpp *.h Makefile

delete_zip:
	rm -f *.zip	
