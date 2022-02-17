#include <stdio.h>
#include <errno.h>
#include <stdlib.h>
#include <string.h>


#include "input.h"
#include "output.h"


int main(int argc, char *argv[]) {
    tt.time = 60;
    tt.avglength = 4.79;
    tt.printindex = 0;
    int filename = 1;
    for (int i = 1; i < argc; i++) {
        filename = parse_argument(argv[i]);
    }
    if (filename) {
        print_help();
        return 0;
    }

  

    while (1) {
        process_keypress();
        if (check_timer() || tt.pos == tt.length) {
            print_score();
        }
    }

    return 0;
}
