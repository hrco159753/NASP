//
// Created by hrvoje on 14. 01. 2021..
//

#include "help.h"

bool ColumnCondition(uint8_t row, uint8_t col, int8_t val, int(&field)[9][9])
{
    for(uint8_t i = 0; i < 9; ++i) {
        if(field[i][col] == val && i != row) return false;
    }

    return true;
}

void FillColumn(uint8_t row, uint8_t col, int8_t val, CP_sudoku::Domains& domains)
{
    for(uint8_t i = 0; i < 9; ++i) {
        domains.GetDomain(i, col)[val-1]++;
    }
}

void UnfillColumn(uint8_t row, uint8_t col, int8_t val, CP_sudoku::Domains& domains)
{
    for(uint8_t i = 0; i < 9; ++i) {
        domains.GetDomain(i, col)[val-1]--;
    }
}

bool RowCondition(uint8_t row, uint8_t col, int8_t val, int(&field)[9][9])
{
    for(uint8_t i = 0; i < 9; ++i) {
        if(field[row][i] == val && i != col) return false;
    }

    return true;
}

void FillRow(uint8_t row, uint8_t col, int8_t val, CP_sudoku::Domains& domains)
{
    for(uint8_t i = 0; i < 9; ++i) {
        domains.GetDomain(row, i)[val-1]++;
    }
}

void UnfillRow(uint8_t row, uint8_t col, int8_t val, CP_sudoku::Domains& domains)
{
    for(uint8_t i = 0; i < 9; ++i) {
        domains.GetDomain(row, i)[val-1]--;
    }
}

bool BoxCondition(uint8_t row, uint8_t col, int8_t val, int(&field)[9][9])
{
    uint8_t box_row = (row / 3), box_col = (col / 3);

    for(uint8_t i = 0; i < 3; ++i)
    {
        for(uint8_t j = 0; j < 3; ++j)
        {
            if(field[3*box_row + i][3*box_col + j] == val && (3*box_row + i != row && 3*box_col + j != col)) return false;
        }
    }

    return true;
}

void FillBox(uint8_t row, uint8_t col, int8_t val, CP_sudoku::Domains& domains)
{
    uint8_t box_row = (row / 3), box_col = (col / 3);

    for(uint8_t i = 0; i < 3; ++i)
    {
        for(uint8_t j = 0; j < 3; ++j)
        {
            domains.GetDomain(3*box_row + i, 3*box_col + j)[val-1]++;
        }
    }
}

void UnfillBox(uint8_t row, uint8_t col, int8_t val, CP_sudoku::Domains& domains)
{
    uint8_t box_row = (row / 3), box_col = (col / 3);

    for(uint8_t i = 0; i < 3; ++i)
    {
        for(uint8_t j = 0; j < 3; ++j)
        {
            domains.GetDomain(3*box_row + i, 3*box_col + j)[val-1]--;
        }
    }
}

