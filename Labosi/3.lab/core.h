//
// Created by hrvoje on 13. 01. 2021..
//

#ifndef INC_3_LAB_CORE_H
#define INC_3_LAB_CORE_H

#include <cstdint>
#include <array>
#include <ostream>
#include <iostream>

class Field
{
    using FieldDomainContainer = std::array<bool, 9>;
    struct FieldIterator
    {
        FieldIterator(FieldDomainContainer& domainContainer, uint8_t index = 0); // end is 9

        uint32_t operator*();
        FieldIterator& operator++();

        void FindNextIndex();

        uint8_t m_index;
        FieldDomainContainer& m_domainContainer;
    };

public:
    explicit Field(int8_t val = -1);

    uint32_t Size() const;
    bool HasSetValue() const;
    int32_t GetCurrentValue() const;
    bool& operator[](int8_t index);
    FieldIterator begin();
    FieldIterator end();

    friend bool operator!=(const FieldIterator& iter1, const FieldIterator& iter2);
    friend bool operator==(const FieldIterator& iter1, const FieldIterator& iter2);
    friend class CP_sudoku;

private:
    // elements of the domain are with the "false" value
    FieldDomainContainer m_domain;
    int8_t m_curr_val;
};

bool operator<(const Field& domain1, const Field& domain2);

class CP_sudoku
{
    struct GridIndexIterator
    {
        GridIndexIterator(Field (&grid)[9][9], bool end = false);

        std::pair<int8_t, int8_t> operator*(); // {-1, -1} if there is no more
        GridIndexIterator& operator++();

        void FindSmallestField();

        std::pair<int8_t, int8_t> m_current_smallest_indexes;
        Field (&m_grid)[9][9];
        bool m_end;
    };

public:
    struct SudokuField {
        int field[9][9];
    };

    CP_sudoku() = default;

    template <std::size_t Rows, std::size_t Cols>
    SudokuField *solve(int (&init_values)[Rows][Cols]);
    int getBacktraceCalls() const;

    void SetCurrentValue(uint8_t row, uint8_t col, const int8_t value);
    int GetCurrentValue(uint8_t row, uint8_t col) const;

    GridIndexIterator begin();
    GridIndexIterator end();

    void FillInitValues(int (&init_values)[9][9]);
    bool SolveRecursive();

    friend bool operator!=(const GridIndexIterator& iter1, const GridIndexIterator& iter2);
    friend bool operator==(const GridIndexIterator& iter1, const GridIndexIterator& iter2);
    friend std::ostream& operator<<(std::ostream& os, const SudokuField& field);
    
    static SudokuField* ToSudokuField(Field (&grid)[9][9]);
private:

    Field m_grid[9][9];
    mutable uint32_t m_num = 0U;
};

std::ostream& operator<<(std::ostream& os, const int(&field)[9][9]);
std::ostream& operator<<(std::ostream& os, const Field(&field)[9][9]);

template<std::size_t Rows, std::size_t Cols>
CP_sudoku::SudokuField *CP_sudoku::solve(int (&init_values)[Rows][Cols])
{
    FillInitValues(init_values);
    m_num = 0;

    std::cout << m_grid << std::endl;

    if(SolveRecursive())
    {
        return CP_sudoku::ToSudokuField(m_grid);
    }
    else
    {
        return nullptr;
    }
}

#endif //INC_3_LAB_CORE_H
