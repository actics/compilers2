build_dir = ./generated

program_name = calc
flex_name    = calc
bison_name   = calc

flex_file  = $(flex_name).l
bison_file = $(bison_name).y

flex_output  = $(build_dir)/$(flex_name)_lexer.cpp
bison_output = $(build_dir)/$(bison_name)_parser.cpp
bison_xml    = $(build_dir)/$(bison_name)_parser.xml

build: calc.l calc.y
	$(if $(wildcard $(build_dir)),,mkdir $(build_dir))
	flex  -o $(flex_output)  $(flex_file)
	bison -o $(bison_output) $(bison_file) 
	g++ -std=c++11 $(flex_output) $(bison_output) -o $(program_name) 
	
calc.html: calc.y
	bison -x -o $(bison_output) $(bison_file)
	xsltproc /usr/share/bison/xslt/xml2xhtml.xsl $(bison_xml) > calc.html

