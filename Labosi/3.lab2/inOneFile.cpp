#include <cstdint>
#include <ostream>
#include <array>
#include <cassert>

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

bool ColumnCondition(uint8_t row, uint8_t col, int8_t val, int(&field)[9][9]);
void FillColumn(uint8_t row, uint8_t col, int8_t val, CP_sudoku::Domains& domains);
void UnfillColumn(uint8_t row, uint8_t col, int8_t val, CP_sudoku::Domains& domains);

bool RowCondition(uint8_t row, uint8_t col, int8_t val, int(&field)[9][9]);
void FillRow(uint8_t row, uint8_t col, int8_t val, CP_sudoku::Domains& domains);
void UnfillRow(uint8_t row, uint8_t col, int8_t val, CP_sudoku::Domains& domains);

bool BoxCondition(uint8_t row, uint8_t col, int8_t val, int(&field)[9][9]);
void FillBox(uint8_t row, uint8_t col, int8_t val, CP_sudoku::Domains& domains);
void UnfillBox(uint8_t row, uint8_t col, int8_t val, CP_sudoku::Domains& domains);

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

int CP_sudoku::getBacktraceCalls() const
{
    return m_num;
}

void CP_sudoku::InitFill(int (&init_values)[9][9])
{
    m_field.InitFill(init_values);
    m_domains.InitFill(init_values);

    m_field.LinkFieldDomains(m_domains);
    m_domains.LinkFieldValues(m_field);
}

bool CP_sudoku::SolveRecursive()
{
    static std::uint32_t max_size = 0;

    m_num++;

    for(std::pair<int8_t, int8_t> domain_index : m_domains)
    {
        int8_t row = domain_index.first , col = domain_index.second;
        FieldDomain& currentDomain = m_domains.GetDomain(row, col);
        for(uint8_t val : currentDomain)
        {
            if(currentDomain.Size() > max_size)
            {
                max_size = currentDomain.Size();
            }
            m_field.SetFieldValue(row, col, val);
            if(SolveRecursive())
            {
                return true;
            }
            m_field.UnsetFieldValue(row, col);
        }
        return false;
    }

    return true;
}

int8_t &CP_sudoku::FieldDomain::operator[](uint8_t index)
{
    return m_domain[index];
}

CP_sudoku::FieldDomain::FieldDomainIter CP_sudoku::FieldDomain::begin()
{
    return { m_domain };
}

CP_sudoku::FieldDomain::FieldDomainIter CP_sudoku::FieldDomain::end()
{
    return { m_domain, 9 };
}

CP_sudoku::FieldDomain::FieldDomainIter::FieldDomainIter(CP_sudoku::FieldDomain::FieldDomainContainer &domain_container, uint8_t index)
        : m_domain_container(domain_container), m_index(index)
{
    FindNextIndex();
}

CP_sudoku::FieldDomain::FieldDomainIter &CP_sudoku::FieldDomain::FieldDomainIter::operator++()
{
    ++m_index;
    FindNextIndex();
    return *this;
}

uint8_t CP_sudoku::FieldDomain::FieldDomainIter::operator*()
{
    return m_index+1;
}

bool operator==(const CP_sudoku::FieldDomain::FieldDomainIter &iter1, const CP_sudoku::FieldDomain::FieldDomainIter &iter2)
{
    return iter1.m_index == iter2.m_index;
}

bool operator!=(const CP_sudoku::FieldDomain::FieldDomainIter &iter1, const CP_sudoku::FieldDomain::FieldDomainIter &iter2)
{
    return !(iter1 == iter2);
}

void CP_sudoku::FieldDomain::FieldDomainIter::FindNextIndex()
{
    for(;m_index < 9 && m_domain_container[m_index] != 0; ++m_index);
}

uint32_t CP_sudoku::FieldDomain::Size() const
{
    static uint8_t counter, index;

    for(index = counter = 0; index < 9; counter += (bool)!m_domain[index], ++index);

    return counter;
}

CP_sudoku::FieldDomain::FieldDomain()
        : m_domain()
{
    m_domain.fill(0);
}

void CP_sudoku::FieldDomain::Reset()
{
    m_domain.fill(0);
}

CP_sudoku::Domains::DomainsIter::DomainsIter(CP_sudoku::FieldDomain (&domain)[9][9], int* (&values)[9][9], bool end)
        : m_domain(domain), m_values(values), m_end(end)
{
    FindSmallestDomain();
}

CP_sudoku::Domains::DomainsIter &CP_sudoku::Domains::DomainsIter::operator++()
{
    FindSmallestDomain();
    return *this;
}

void CP_sudoku::Domains::DomainsIter::FindSmallestDomain()
{
    uint8_t smallest = 10;
    uint8_t smallestRow, smallestCol;

    for(uint8_t row = 0; row < 9; ++row)
    {
        for(uint8_t col = 0; col < 9; ++col)
        {
            if(*m_values[row][col] != -1)
            {
                continue;
            }

            if(m_domain[row][col].Size() < smallest)
            {
                smallest = m_domain[row][col].Size();
                smallestRow = row;
                smallestCol = col;
            }

            if(smallest == 0)
            {
                goto BreakBothLoops;
            }
        }
    }
    BreakBothLoops:

    if(smallest == 10)
    {
        m_end = true;
    }
    else
    {
        m_current_index.first = smallestRow;
        m_current_index.second = smallestCol;
    }
}

std::pair<int8_t, int8_t> CP_sudoku::Domains::DomainsIter::operator*()
{
    return m_current_index;
}

CP_sudoku::FieldDomain &CP_sudoku::Domains::GetDomain(uint8_t row, uint8_t col)
{
    return m_domains[row][col];
}

const CP_sudoku::FieldDomain &CP_sudoku::Domains::GetDomain(uint8_t row, uint8_t col) const
{
    return m_domains[row][col];
}

CP_sudoku::Domains::DomainsIter CP_sudoku::Domains::begin()
{
    return { m_domains, m_values };
}

CP_sudoku::Domains::DomainsIter CP_sudoku::Domains::end()
{
    return { m_domains, m_values, true };
}

bool operator==(const CP_sudoku::Domains::DomainsIter &iter1, const CP_sudoku::Domains::DomainsIter &iter2)
{
    return iter1.m_end == iter2.m_end && iter1.m_end;
}

bool operator!=(const CP_sudoku::Domains::DomainsIter &iter1, const CP_sudoku::Domains::DomainsIter &iter2)
{
    return !(iter1 == iter2);
}

void CP_sudoku::Domains::InitFill(int (&init_values)[9][9])
{
    // Reset domains on consecutive calls
    for(uint8_t row = 0; row < 9; ++row)
    {
        for(uint8_t col = 0; col < 9; ++col)
        {
            m_domains[row][col].Reset();
        }
    }

    for(uint8_t row = 0; row < 9; ++row)
    {
        for(uint8_t col = 0; col < 9; ++col)
        {
            if(init_values[row][col] != -1)
            {
                FillRow(row, col, init_values[row][col], *this);
                FillColumn(row, col, init_values[row][col], *this);
                FillBox(row, col, init_values[row][col], *this);
            }
        }
    }

}

void CP_sudoku::Domains::LinkFieldValues(CP_sudoku::SudokuField &field)
{
    for(uint8_t row = 0; row < 9; ++row)
    {
        for(uint8_t col = 0; col < 9; ++col)
        {
            m_values[row][col] = &field.field[row][col];
        }
    }
}

int CP_sudoku::SudokuField::GetFieldValue(uint8_t row, uint8_t col) const
{
    return field[row][col];
}

const CP_sudoku::FieldDomain &CP_sudoku::SudokuField::GetFieldDomain(uint8_t row, uint8_t col) const
{
    return m_domains->GetDomain(row, col);
}

void CP_sudoku::SudokuField::SetFieldValue(uint8_t row, uint8_t col, uint8_t val)
{
    assert(val > 0 && val < 10);

    FillRow(row, col, val, *m_domains);
    FillColumn(row, col, val, *m_domains);
    FillBox(row, col, val, *m_domains);

    field[row][col] = val;

}

void CP_sudoku::SudokuField::UnsetFieldValue(uint8_t row, uint8_t col)
{
    uint8_t val = field[row][col];

    UnfillRow(row, col, val, *m_domains);
    UnfillColumn(row, col, val, *m_domains);
    UnfillBox(row, col, val, *m_domains);

    field[row][col] = -1;
}

void CP_sudoku::SudokuField::InitFill(int (&init_values)[9][9])
{
    for(uint8_t row = 0; row < 9; ++row)
    {
        for(uint8_t col = 0; col < 9; ++col)
        {
            field[row][col] = init_values[row][col];
        }
    }
}

void CP_sudoku::SudokuField::LinkFieldDomains(CP_sudoku::Domains &domains)
{
    m_domains = &domains;
}

CP_sudoku::SudokuField::SudokuField(const CP_sudoku::SudokuField &other)
        : field(), m_domains(nullptr)
{
    for(uint8_t row = 0; row < 9; ++row)
    {
        for(uint8_t col = 0; col < 9; ++col)
        {
            field[row][col] = other.field[row][col];
        }
    }
}

//
// Created by hrvoje on 14. 01. 2021..
//

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

