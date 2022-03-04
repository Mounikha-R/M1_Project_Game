/**
 * @file TypingGame_operations.h
 * @author Mounikha
 * @brief Header file for Typing Game application 
 *
 */

#ifndef __TypingGame_OPERATIONS_H__
#define __TypingGame_OPERATIONS_H__

int main(int argc, char *argv[]) {
    tt.time = 60;
    tt.avg_word_length = 4.79;
    tt.print_index = 0;
    int no_filename = 1;
    for (int i = 1; i < argc; i++) {
        no_filename = parse_argument(argv[i]);
    }
    if (no_filename) {
        print_help();
        return 0;
    }

    init_terminal();
    parse_text();
    print_text();

    while (1) {
        process_keypress();
        if (check_timer() || tt.pos == tt.length) {
            print_score();
        }
    }

    return 0;
}
