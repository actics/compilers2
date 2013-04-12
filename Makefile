.PHONY: Build Lexer Parser

Build: Parser Lexer
	g++ -std=c++11 -o compiler ./generated/lexer.cpp ./generated/parser.cpp driver.cpp node.cpp compiler.cpp

Lexer: GenDir lexer.l
	flex -o ./generated/lexer.cpp lexer.l 

Parser: GenDir parser.y
	bison -d -o ./generated/parser.cpp parser.y

GenDir:
	$(if $(wildcard ./generated),,mkdir ./generated)
