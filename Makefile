file = in.txt

run:
	gcc compiler.c -o compiler
	./compiler $(file)