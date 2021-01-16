//
// Created by hrvoje on 14. 01. 2021..
//

#ifndef INC_3_LAB2_CORE_H
#define INC_3_LAB2_CORE_H

#include <cstdint>
#include <ostream>
#include <array>
#include <iostream>

class CP_sudoku
{
public:
    class SudokuField;
    class FieldDomain;
    class Domains;

    class FieldDomain
    {
    public:
        struct FieldDomainIter;

        using FieldDomainContainer = std::array<int8_t, 9>;

        FieldDomain();

        int8_t& operator[](uint8_t index);
        uint32_t Size() const;
        void Reset();

        FieldDomainIter begin();
        FieldDomainIter end();

        friend class CP_sudoku;

        FieldDomainContainer m_domain;

        struct FieldDomainIter
        {
            FieldDomainIter(FieldDomainContainer& domain_container, uint8_t index = 0);

            FieldDomainIter& operator++();
            uint8_t operator*();

            friend bool operator==(const FieldDomainIter& iter1, const FieldDomainIter& iter2);
            friend bool operator!=(const FieldDomainIter& iter1, const FieldDomainIter& iter2);
        private:
            void FindNextIndex();

            uint8_t m_index;
            FieldDomainContainer& m_domain_container;
        };

    };

    class Domains
    {
    public:
        struct DomainsIter
        {
            DomainsIter(FieldDomain (&domain)[9][9], int* (&values)[9][9], bool end = false);

            DomainsIter& operator++();
            std::pair<int8_t, int8_t> operator*();

            friend bool operator==(const DomainsIter& iter1, const DomainsIter& iter2);
            friend bool operator!=(const DomainsIter& iter1, const DomainsIter& iter2);
        private:
            void FindSmallestDomain();

            FieldDomain (&m_domain)[9][9];
            int* (&m_values)[9][9];
            bool m_end;
            std::pair<int8_t, int8_t> m_current_index;
        };

        FieldDomain& GetDomain(uint8_t row, uint8_t col);
        const FieldDomain& GetDomain(uint8_t row, uint8_t col) const;
        DomainsIter begin();
        DomainsIter end();

        friend class CP_sudoku::SudokuField;
        friend class CP_sudoku;

        friend std::ostream& operator<<(std::ostream& os, const Domains& domains);
    private:
        void InitFill(int(&init_values)[9][9]);
        void LinkFieldValues(SudokuField& field);
        FieldDomain m_domains[9][9];
        int *m_values[9][9];
    };

    class SudokuField
    {
    public:
        SudokuField() = default;
        SudokuField(const SudokuField& other);

        int GetFieldValue(uint8_t row, uint8_t col) const;
        const FieldDomain& GetFieldDomain(uint8_t row, uint8_t col) const;

        void SetFieldValue(uint8_t row, uint8_t col, uint8_t val);
        void UnsetFieldValue(uint8_t row, uint8_t col);

        int field[9][9];

        friend class CP_sudoku;
        friend std::ostream& operator<<(std::ostream& os, const SudokuField& field);
    private:
        void InitFill(int(&init_values)[9][9]);
        void LinkFieldDomains(Domains& domains);

        Domains* m_domains = nullptr;
    };

    friend std::ostream& operator<<(std::ostream& os, const int(&field)[9][9]);


    template <std::size_t Rows, std::size_t Cols>
    SudokuField *solve(int (&init_values)[Rows][Cols]);

    int getBacktraceCalls() const;

private:

    bool SolveRecursive();
    void InitFill(int(&init_values)[9][9]);

    SudokuField m_field;
    Domains m_domains;
    mutable uint32_t m_num;
};

template<std::size_t Rows, std::size_t Cols>
CP_sudoku::SudokuField *CP_sudoku::solve(int (&init_values)[Rows][Cols])
{
    InitFill(init_values);
    m_num = 0;

    if(SolveRecursive())
    {
        SudokuField* newField = new CP_sudoku::SudokuField(m_field);
        return newField;
    }
    else
    {
        return nullptr;
    }
}


#endif //INC_3_LAB2_CORE_H
