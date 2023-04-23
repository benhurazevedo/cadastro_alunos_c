#My first makefile

all: cadastro_alunos

cadastro_alunos: 
	gcc -o cadastro_alunos cadastro_alunos.c -W -Wall -ansi -pedantic

clean:
	rm -rf cadastro_alunos