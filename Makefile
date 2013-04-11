.PHONY: Build Lexer Parser

Build: Parser Lexer
	g++ -o compiler lexer.cpp parser.cpp driver.cpp compiler.cpp

Lexer: lexer.l
	flex -o lexer.cpp lexer.l 

Parser: parser.y
	bison -d -o parser.cpp parser.y
