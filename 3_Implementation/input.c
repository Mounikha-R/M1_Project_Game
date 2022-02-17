#include <ctype.h>
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>



#include "input.h"
#include "output.h"


void parse_text() {
    tt.buffer = 0;
    tt.bufferscore = 0;
    tt.isnewline = 0;
    tt.isnewpage = 0;
    FILE * f = fopen(tt.filename, "rb");

    unsigned int i, j;

    if (f == NULL) {
        die(tt.filename, 0);
    }

    fseek(f, 0, SEEK_END);
    tt.length = ftell(f);
    fseek(f, 0, SEEK_SET);
    tt.buffer = malloc(tt.length);
    tt.bufferscore = malloc(tt.length);
    tt.isnewline = malloc(tt.length);
    tt.isnewpage = malloc(tt.length);
    if (tt.buffer) fread(tt.buffer, 1, tt.length, f);
    fclose(f);

    for (i = 0; i < tt.length; i++) {
        tt.bufferscore[i] = 0;
        if ((tt.buffer[i+1] == ' ' && tt.buffer[i] == ' ')
                || (tt.buffer[i+1] == '\n' && tt.buffer[i] == ' ')
                || (tt.buffer[i+1] == '\n' && tt.buffer[i] == '\n')) {
            for (j = i; j < tt.length - 1; j++) {
                tt.buffer[j] = tt.buffer[j+1];
            }
            tt.length--;
            i--;
        }
    }

    int textrows = 1;
    int textcolumn = 1;
    for (i = 0; i < tt.length; i++) {
        if (tt.buffer[i] == '\n' || textcolumn == tt.term_cols) {
            textrows++;
            textcols = 0;
            tt.isnewline[i+1] = 1;
        } else {
            tt.isnewline[i+1] = 0;
        }
        if (text_rows == tt.term_rows - 1 && textcolumn == 0) {
            tt.isnewpage[i+1] = 1;
        } else {
            tt.isnewpage[i+1] = 0;
        }
        textcolumn++;
    }
}

void process_keypress() {
    char c = read_key();

    FONT_DEF;

    switch (c) {
        case ctrlkey('c'):
        case ctrlkey('q'):
            clr_screen;
            crspostop;
            exit(0);
        case ctrlkey('r'):
            refresh_screen();
            parse_text();
            print_text();
            tt.pos = 0;
            tt.print_index = 0;
            first_key_read = 0;
            break;
        case 127:
            if (tt.pos == 0 || tt.is_new_page[tt.pos]) {
                break;
            } else if (tt.buffer[tt.pos - 1] == '\n') {
                del_to_prev_line();
                tt.buffer_score[tt.pos] = 0;
                break;
            } else {
                del();
                break;
            }
        case '\r':
            if (tt.buffer[tt.pos] != '\n') break;
            FONT_CLR_GRN;
            jump_next_line();
            tt.buffer_score[tt.pos] = 1;
            tt.pos++;
            break;
        default:
            if (iscntrl(c) && c != '\r') {
                break;
            } else if (c == tt.buffer[tt.pos]) {
                FONT_CLR_GRN;
                print_from_buffer();
                tt.buffer_score[tt.pos] = 1;
            } else {
                FONT_CLR_RED;
                if (tt.buffer[tt.pos] == '\n') {
                    jump_to_next_line();
                } else {
                    print_from_buffer();
                }
                tt.buffer_score[tt.pos] = -1;
            }
            tt.pos++;
    }

    if (tt.pos == tt.print_index) {
        print_text();
    }
}

