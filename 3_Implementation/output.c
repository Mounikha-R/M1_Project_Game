#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "data.h"
#include "helper.h"
#include "output.h"

void del() {
    crs;
    tt.pos--;
    printfrombuffer();
    crs;
}

void del_to_prev_line() {
    int i = 2;
    crsposup;
    while (tt.buffer[tt.pos - i] != '\n' && tt.pos - i != 0) {
        crsposf;
        i++;
    }
    if (tt.pos - i == 0) {
        crsposf;
    }
    tt.pos--;
    PRINT_TO_SCREEN("\u23CE");
    crsposb;
}

void jump_to_next_line() {
    write(STDOUT_FILENO, "\u23CE\n", strlen("\u23CE\n"));
}

void jump_to_next_word() {
    FONT_CLR_RED;
    while (tt.buffer[tt.pos] != ' ' && tt.buffer[tt.pos] != '\n') {
        printfrombuffer();
        tt.buffer_score[tt.pos] = -1;
        tt.pos++;
    }
    if (tt.buffer[tt.pos] == ' ') {
        fontclrgrn;
        printfrombuffer();
        tt.buffer_score[tt.pos] = 1;
        tt.pos++;
    }
}

void printfrombuffer() {
    write(STDOUT_FILENO, &tt.buffer[tt.pos], 1);
}


void print_score() {
    float wpm, kpm;
    int keystrokes = 0;
    int correct = 0;
    int wrong = 0;

    for (int i = 0; tt.buffer_score[i] != 0; i++) {
        keystrokes++;
        if (tt.buffer_score[i] == 1) correct++;
        else wrong++;
    }

    kpm = 60 / (float)tt.elapsed_time * (float)correct;
    wpm = kpm / tt.avg_word_length;

    clr_screen;
    crspostop;
    fontclrdef;
    printf("\x1b[32m%d", correct);
    printf("\x1b[31m%d", wrong);
    printf("\x1b[0m) %d\n", keystrokes);
    printf(" KPM:\t\t%.2f\n", kpm);
    printf(" WPM:\t\t%.2f\n\n", wpm);
    exit(0);
}
void refresh_screen() {
    clr_screen;
    crspoptop;
}

