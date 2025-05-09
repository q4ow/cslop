#include <stdio.h>
#include <stdlib.h>
#include "../include/cslop.h"


int main(int argc, char *argv[]) {
    struct arguments arguments;
    
    init_cli(argc, argv, &arguments);

    if (!arguments.quiet) {
        print_program_header();
    }
    
    if (arguments.verbose) {
        print_success("Verbose mode enabled");
        
        if (arguments.output_file) {
            print_info("Output file", arguments.output_file);
        }
        
        if (arguments.arg_count > 0) {
            char buffer[32];
            sprintf(buffer, "%d", arguments.arg_count);
            print_info("Arguments provided", buffer);
            
            for (int i = 0; i < arguments.arg_count; i++) {
                char label[32];
                sprintf(label, "Argument %d", i+1);
                print_info(label, arguments.args[i]);
            }
        } else {
            print_warning("No arguments provided");
        }
    }

    return 0;
}