.PHONY: Build Lexer Parser

flex_bison_cpp = ./generated/lexer.cpp ./generated/parser.cpp
ast_cpp = node.cpp collectionNode.cpp binaryNode.cpp llvmInterface.cpp codeScope.cpp
flags = -std=c++11 `llvm-config --cppflags --ldflags --libs`

Build: Parser Lexer
	g++ -o compiler driver.cpp compiler.cpp $(ast_cpp) $(flex_bison_cpp) $(flags)

Lexer: GenDir lexer.l
	flex -o ./generated/lexer.cpp lexer.l 

Parser: GenDir parser.y
	bison -d -o ./generated/parser.cpp parser.y

GenDir:
	$(if $(wildcard ./generated),,mkdir ./generated)
