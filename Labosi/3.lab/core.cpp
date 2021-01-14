//
// Created by hrvoje on 13. 01. 2021..
//

#include "core.h"
#include "help.h"

#include <iostream>
#include <iomanip>

void CP_sudoku::FillInitValues(int (&init_values)[9][9]) {
    for(uint8_t row = 0; row < 9; ++row)
    {
        for(uint8_t col = 0; col < 9; ++col)
        {
            if(init_values[row][col] != -1)
            {
                SetCurrentValue(row, col, init_values[row][col]);
            }
        }
    }
}

void CP_sudoku::SetCurrentValue(uint8_t row, uint8_t col, const int8_t value)
{
    m_grid[row][col].m_curr_val = value;

    if(!m_grid[row][col].HasSetValue())
    {
        UnfillRow(row, col, value, m_grid);
        UnfillColumn(row, col, value, m_grid);
        UnfillBox(row, col, value, m_grid);
    }
    else
    {
        FillRow(row, col, value, m_grid);
        FillColumn(row, col, value, m_grid);
        FillBox(row, col, value, m_grid);
    }

}

int CP_sudoku::GetCurrentValue(uint8_t row, uint8_t col) const
{
    return m_grid[row][col].GetCurrentValue();
}

std::ostream& operator<<(std::ostream& os, const int(&field)[9][9])
{
    for(std::uint8_t row = 0; row < 9; ++row)
    {
        for(std::uint8_t col = 0; col < 9; ++col)
        {
            std::cout << std::setw(2) << field[row][col] << ", ";
        }
        std::cout << std::endl;
    }

    return os;
}

std::ostream& operator<<(std::ostream& os, const CP_sudoku::SudokuField& field)
{
    std::cout << field.field << std::endl;

    return os;
}

std::ostream& operator<<(std::ostream& os, const Field(&field)[9][9])
{
    for(std::uint8_t row = 0; row < 9; ++row)
    {
        for(std::uint8_t col = 0; col < 9; ++col)
        {
            std::cout << std::setw(2) << (int32_t)field[row][col].GetCurrentValue() << ", ";
        }
        std::cout << std::endl;
    }

    return os;
}

CP_sudoku::GridIndexIterator CP_sudoku::begin()
{
    return { m_grid };
}

CP_sudoku::GridIndexIterator CP_sudoku::end()
{
    return { m_grid, true };
}

Field::Field(int8_t val)
    : m_domain(), m_curr_val(val)
{

}

uint32_t Field::Size() const
{
    static uint8_t counter, i;

    for(i = counter = 0; i < 9; counter += (uint8_t)!m_domain[i], ++i);

    return counter;
}

bool Field::HasSetValue() const
{
    return m_curr_val != -1;
}

int32_t Field::GetCurrentValue() const
{
    return m_curr_val;
}

bool &Field::operator[](int8_t index)
{
    return m_domain[index];
}

Field::FieldIterator Field::begin()
{
    return { m_domain };
}

Field::FieldIterator Field::end() {
    return { m_domain, 9 };
}

bool operator!=(const Field::FieldIterator &iter1, const Field::FieldIterator &iter2)
{
    return !(iter1 == iter2);
}

bool operator==(const Field::FieldIterator &iter1, const Field::FieldIterator &iter2)
{
    return iter1.m_index == iter2.m_index;
}

Field::FieldIterator::FieldIterator(Field::FieldDomainContainer &domainContainer, uint8_t index)
    : m_domainContainer(domainContainer), m_index(index)
{
    FindNextIndex();
}

uint32_t Field::FieldIterator::operator*()
{
    return m_index+1;
}

Field::FieldIterator &Field::FieldIterator::operator++()
{
    ++m_index;
    FindNextIndex();
    return *this;
}

void Field::FieldIterator::FindNextIndex()
{
    for(; m_index < 9 && m_domainContainer[m_index]; ++m_index);
}

CP_sudoku::GridIndexIterator::GridIndexIterator(Field (&grid)[9][9], bool end)
    : m_current_smallest_indexes(), m_grid(grid), m_end(end)
{

}

std::pair<int8_t, int8_t> CP_sudoku::GridIndexIterator::operator*()
{
    return m_current_smallest_indexes;
}

void CP_sudoku::GridIndexIterator::FindSmallestField()
{
    static uint8_t current_size;

    current_size = 10;
    m_current_smallest_indexes.first = m_current_smallest_indexes.second = -1;

    for(uint8_t row = 0; row < 9; ++row)
    {
        for(uint8_t col = 0; col < 9; ++col)
        {
            Field& field = m_grid[row][col];
            if(!field.HasSetValue() && field.Size() < current_size)
            {
                current_size = field.Size();
                m_current_smallest_indexes.first = row;
                m_current_smallest_indexes.second = col;
            }
        }
    }

    if(m_current_smallest_indexes.first == -1 && m_current_smallest_indexes.second == -1)
    {
        m_end = true;
    }
}

CP_sudoku::GridIndexIterator &CP_sudoku::GridIndexIterator::operator++()
{
    FindSmallestField();
    return *this;
}

bool operator==(const CP_sudoku::GridIndexIterator &iter1, const CP_sudoku::GridIndexIterator &iter2)
{
    return iter1.m_end == iter2.m_end && iter1.m_end;
}

bool operator!=(const CP_sudoku::GridIndexIterator &iter1, const CP_sudoku::GridIndexIterator &iter2)
{
    return !(iter1 == iter2);
}

CP_sudoku::SudokuField *CP_sudoku::ToSudokuField(Field (&grid)[9][9])
{
    CP_sudoku::SudokuField& newSudokuField = *new CP_sudoku::SudokuField;

    for(uint8_t row = 0; row < 9; ++row)
    {
        for(uint8_t col = 0; col < 9; ++col)
        {
            newSudokuField.field[row][col] = grid[row][col].GetCurrentValue();
        }
    }

    return &newSudokuField;
}

bool CP_sudoku::SolveRecursive()
{
    m_num++; // increment number of recursive calls

    std::cout << m_grid << std::endl;
    for(std::pair<int8_t, int8_t> indexes : *this)
    {
        Field& current_field = m_grid[indexes.first][indexes.second];
        for(uint8_t val : current_field)
        {
            SetCurrentValue(indexes.first, indexes.second, val);
            if(SolveRecursive())
            {
                return true;
            }
            SetCurrentValue(indexes.first, indexes.second, -1); // unset
        }

        return false;
    }

    return true;
}

int CP_sudoku::getBacktraceCalls() const {
    return m_num;
}


