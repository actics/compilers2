#include <cstdio>
#include <cstring>
#include <getopt.h>
#include "driver.hpp"

void help(void) 
{
    printf ("Usage: -p|-n [FILE]\n");
}

int main(int argc, char** argv)
{
    Driver driver;
    int c;

    while ( (c = getopt(argc, argv, "sph")) != -1 ) {
        switch (c) {
            case 's':
                driver.trace_scanning = true;
                break;
            case 'p':
                driver.trace_parsing = true;
                break;
            case 'h':
                help();
                return 0;
            case '?':
                return 1;
            default:
                abort();
        }
    }
    
    int noargs = argc - optind;

    if ( noargs > 1 ) {
        fprintf(stderr, "Too many filenames");
        return 1;
    }

    if ( noargs == 0 ) {
        help();
        return 0;
    }
    
    FILE * code_file;
    
    if ( !strcmp(argv[optind], "-") ) {
        code_file = stdin;
    }
    else {
        code_file = fopen(argv[optind], "r");
        if (!code_file) {
            printf("Can't open file '%s'.\n", argv[optind]);
            return 2;
        }
    }

    if (!driver.parse(code_file)) {
        // printf("%s\n", driver.getAstString().c_str());
        Module* module = driver.codeGeneration();
        verifyModule(*module);
        module->dump();
        driver.deleteAst();
    }

    fclose(code_file);
     
    return 0;
}
