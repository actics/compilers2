build_dir = ./generated

program_name = calc
flex_name    = calc
bison_name   = calc

flex_file  = $(flex_name).l
bison_file = $(bison_name).y

flex_output  = $(build_dir)/$(flex_name)_lexer.cpp
bison_output = $(build_dir)/$(bison_name)_parser.cpp

build: calc.l calc.y
	$(if $(wildcard $(build_dir)),,mkdir $(build_dir))
	bison -o $(bison_output) $(bison_file) 
	flex  -o $(flex_output)  $(flex_file)
	g++ $(flex_output) $(bison_output) -o $(program_name) 
	
