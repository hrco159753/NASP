//
// Created by hrvoje on 13. 01. 2021..
//

#include "help.h"

bool ColumnCondition(uint8_t row, uint8_t col, int8_t val, int(&field)[9][9]) {
    for(uint8_t i = 0; i < 9; ++i) {
        if(field[i][col] == val && i != row) return false;
    }

    return true;
}

void FillColumn(uint8_t row, uint8_t col, int8_t val, Field(&domains)[9][9]){
    for(uint8_t i = 0; i < 9; ++i) {
        if(i != row) {
            domains[i][col][val-1] = true;
        }
    }
}

void UnfillColumn(uint8_t row, uint8_t col, int8_t val, Field(&domains)[9][9]) {
    for(uint8_t i = 0; i < 9; ++i) {
        if(i != row) {
            domains[i][col][val-1] = false;
        }
    }
}

bool RowCondition(uint8_t row, uint8_t col, int8_t val, int(&field)[9][9]) {
    for(uint8_t i = 0; i < 9; ++i) {
        if(field[row][i] == val && i != col) return false;
    }

    return true;
}

void FillRow(uint8_t row, uint8_t col, int8_t val, Field(&domains)[9][9]) {
    for(uint8_t i = 0; i < 9; ++i) {
        if(i != col) {
            domains[row][i][val-1] = true;
        }
    }
}

void UnfillRow(uint8_t row, uint8_t col, int8_t val, Field(&domains)[9][9]) {
    for(uint8_t i = 0; i < 9; ++i) {
        if(i != col) {
            domains[row][i][val-1] = false;
        }
    }
}

bool BoxCondition(uint8_t row, uint8_t col, int8_t val, int(&field)[9][9]) {
    uint8_t box_row = (row / 3), box_col = (col / 3);

    for(uint8_t i = 0; i < 3; ++i)
    {
        for(uint8_t j = 0; j < 3; ++j)
        {
            if(field[3*box_row + i][3*box_col + j] == val && (i != box_row && j != box_col)) return false;
        }
    }

    return true;
}

void FillBox(uint8_t row, uint8_t col, int8_t val, Field(&domains)[9][9]) {
    uint8_t box_row = (row / 3), box_col = (col / 3);

    for(uint8_t i = 0; i < 3; ++i)
    {
        for(uint8_t j = 0; j < 3; ++j)
        {
            if((i != box_row && j != box_col)) {
                domains[i][j][val-1] = true;
            }
        }
    }
}

void UnfillBox(uint8_t row, uint8_t col, int8_t val, Field(&domains)[9][9]) {
    uint8_t box_row = (row / 3), box_col = (col / 3);

    for(uint8_t i = 0; i < 3; ++i)
    {
        for(uint8_t j = 0; j < 3; ++j)
        {
            if((i != box_row && j != box_col)) {
                domains[i][j][val-1] = false;
            }
        }
    }
}


