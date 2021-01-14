//
// Created by hrvoje on 13. 01. 2021..
//

#ifndef INC_3_LAB_HELP_H
#define INC_3_LAB_HELP_H

#include "core.h"

bool ColumnCondition(uint8_t row, uint8_t col, int8_t val, int(&field)[9][9]);
void FillColumn(uint8_t row, uint8_t col, int8_t val, Field(&domains)[9][9]);
void UnfillColumn(uint8_t row, uint8_t col, int8_t val, Field(&domains)[9][9]);

bool RowCondition(uint8_t row, uint8_t col, int8_t val, int(&field)[9][9]);
void FillRow(uint8_t row, uint8_t col, int8_t val, Field(&domains)[9][9]);
void UnfillRow(uint8_t row, uint8_t col, int8_t val, Field(&domains)[9][9]);

bool BoxCondition(uint8_t row, uint8_t col, int8_t val, int(&field)[9][9]);
void FillBox(uint8_t row, uint8_t col, int8_t val, Field(&domains)[9][9]);
void UnfillBox(uint8_t row, uint8_t col, int8_t val, Field(&domains)[9][9]);


#endif //INC_3_LAB_HELP_H
