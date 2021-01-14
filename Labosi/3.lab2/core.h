//
// Created by hrvoje on 14. 01. 2021..
//

#ifndef INC_3_LAB2_CORE_H
#define INC_3_LAB2_CORE_H

class CP_sudoku {
public:
    struct SudokuField {
        int field[9][9];
    };
    template <size_t rows, size_t cols>
    SudokuField *solve(int (&init_values)[rows][cols]);
    int getBacktraceCalls();

};


#endif //INC_3_LAB2_CORE_H
