//
// Created by hrvoje on 14. 01. 2021..
//

#ifndef INC_3_LAB2_HELP_H
#define INC_3_LAB2_HELP_H

#include "core.h"

bool ColumnCondition(uint8_t row, uint8_t col, int8_t val, int(&field)[9][9]);
void FillColumn(uint8_t row, uint8_t col, int8_t val, CP_sudoku::Domains& domains);
void UnfillColumn(uint8_t row, uint8_t col, int8_t val, CP_sudoku::Domains& domains);

bool RowCondition(uint8_t row, uint8_t col, int8_t val, int(&field)[9][9]);
void FillRow(uint8_t row, uint8_t col, int8_t val, CP_sudoku::Domains& domains);
void UnfillRow(uint8_t row, uint8_t col, int8_t val, CP_sudoku::Domains& domains);

bool BoxCondition(uint8_t row, uint8_t col, int8_t val, int(&field)[9][9]);
void FillBox(uint8_t row, uint8_t col, int8_t val, CP_sudoku::Domains& domains);
void UnfillBox(uint8_t row, uint8_t col, int8_t val, CP_sudoku::Domains& domains);

#endif //INC_3_LAB2_HELP_H
