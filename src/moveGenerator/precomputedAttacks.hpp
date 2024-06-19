#pragma once

#include "types.hpp"
#include "square.hpp"

class PrecomputedAttacks
{
public:
    PrecomputedAttacks() { initialize(); }

    /*
     *   Return the 64 bit mask with 1 on the squares that the piece in the provided square is attacking
     *   square should be valid
     */
    inline uint64_t getKingAttacks(Square square) const { return kingAttacks[square]; }

    /*
     *   Return the 64 bit mask with 1 on the squares that the piece in the provided square is attacking
     *   square should be valid
     */
    inline uint64_t getKnightAttacks(Square square) const { return knightAttacks[square]; }

    /*
     *   Return the 64 bit mask with 1 on the squares that the piece in the provided square is attacking
     *   square should be valid
     */
    inline uint64_t getPawnWhiteAttacks(Square square) const { return pawnWhiteAttacks[square]; }

    /*
     *   Return the 64 bit mask with 1 on the squares that the piece in the provided square is attacking
     *   square should be valid
     */
    inline uint64_t getPawnBlackAttacks(Square square) const { return pawnBlackAttacks[square]; }

    /*
     *   Return the 64 bit mask with 1 on the squares that the piece in the provided square is attacking
     *   square should be valid
     */
    inline uint64_t getRookAttacks(Square square) const { return rookAttacks[square]; }

    /*
     *   Return the 64 bit mask with 1 on the squares that the piece in the provided square is attacking
     *   square should be valid
     */
    inline uint64_t getBishopAttacks(Square square) const { return bishopAttacks[square]; }

    /*
     *   Return the 64 bit mask with 1 on the squares that the piece in the provided square is attacking
     *   square should be valid
     */
    inline uint64_t getQueenAttacks(Square square) const { return queenAttacks[square]; }

private:
    uint64_t kingAttacks[64] = {0};
    uint64_t knightAttacks[64] = {0};
    uint64_t bishopAttacks[64] = {0};
    uint64_t rookAttacks[64] = {0};
    uint64_t queenAttacks[64] = {0};
    uint64_t pawnWhiteAttacks[64] = {0};
    uint64_t pawnBlackAttacks[64] = {0};

    constexpr void initialize();
    constexpr void initializeKingAttacks();
    constexpr void initializeKnightAttacks();
    constexpr void initializePawnAttacks();
    constexpr void initializeRookAttacks();
    constexpr void initializeBishopAttacks();
    constexpr void initializeQueenAttacks();
};

constexpr void PrecomputedAttacks::initialize()
{
    initializeKingAttacks();
    initializeKnightAttacks();
    initializePawnAttacks();
    initializeRookAttacks();
    initializeBishopAttacks();
    initializeQueenAttacks();
}

constexpr void PrecomputedAttacks::initializeKingAttacks()
{
    for (int row = ROW_1; row <= ROW_8; row++)
    {
        for (int col = COL_A; col <= COL_H; col++)
        {
            if (validCoord(row + 1, col))
                kingAttacks[Square(row, col)] |= 1UL << Square(row + 1, col);

            if (validCoord(row - 1, col))
                kingAttacks[Square(row, col)] |= 1UL << Square(row - 1, col);

            if (validCoord(row, col + 1))
                kingAttacks[Square(row, col)] |= 1UL << Square(row, col + 1);

            if (validCoord(row, col - 1))
                kingAttacks[Square(row, col)] |= 1UL << Square(row, col - 1);

            if (validCoord(row + 1, col + 1))
                kingAttacks[Square(row, col)] |= 1UL << Square(row + 1, col + 1);

            if (validCoord(row + 1, col - 1))
                kingAttacks[Square(row, col)] |= 1UL << Square(row + 1, col - 1);

            if (validCoord(row - 1, col + 1))
                kingAttacks[Square(row, col)] |= 1UL << Square(row - 1, col + 1);

            if (validCoord(row - 1, col - 1))
                kingAttacks[Square(row, col)] |= 1UL << Square(row - 1, col - 1);
        }
    }
}

constexpr void PrecomputedAttacks::initializeKnightAttacks()
{
    for (int row = ROW_1; row <= ROW_8; row++)
    {
        for (int col = COL_A; col <= COL_H; col++)
        {
            if (validCoord(row + 2, col + 1))
                knightAttacks[Square(row, col)] |= 1UL << Square(row + 2, col + 1);

            if (validCoord(row + 2, col - 1))
                knightAttacks[Square(row, col)] |= 1UL << Square(row + 2, col - 1);

            if (validCoord(row - 2, col + 1))
                knightAttacks[Square(row, col)] |= 1UL << Square(row - 2, col + 1);

            if (validCoord(row - 2, col - 1))
                knightAttacks[Square(row, col)] |= 1UL << Square(row - 2, col - 1);

            if (validCoord(row + 1, col + 2))
                knightAttacks[Square(row, col)] |= 1UL << Square(row + 1, col + 2);

            if (validCoord(row + 1, col - 2))
                knightAttacks[Square(row, col)] |= 1UL << Square(row + 1, col - 2);

            if (validCoord(row - 1, col + 2))
                knightAttacks[Square(row, col)] |= 1UL << Square(row - 1, col + 2);

            if (validCoord(row - 1, col - 2))
                knightAttacks[Square(row, col)] |= 1UL << Square(row - 1, col - 2);
        }
    }
}

constexpr void PrecomputedAttacks::initializePawnAttacks()
{
    for (int row = ROW_1; row <= ROW_8; row++)
    {
        for (int col = COL_A; col <= COL_H; col++)
        {
            if (validCoord(row + 1, col + 1))
                pawnWhiteAttacks[Square(row, col)] |= 1UL << Square(row + 1, col + 1);

            if (validCoord(row + 1, col - 1))
                pawnWhiteAttacks[Square(row, col)] |= 1UL << Square(row + 1, col - 1);

            if (validCoord(row - 1, col + 1))
                pawnBlackAttacks[Square(row, col)] |= 1UL << Square(row - 1, col + 1);

            if (validCoord(row - 1, col - 1))
                pawnBlackAttacks[Square(row, col)] |= 1UL << Square(row - 1, col - 1);
        }
    }
}

constexpr void PrecomputedAttacks::initializeRookAttacks()
{
    for (int row = ROW_1; row <= ROW_8; row++)
    {
        for (int col = COL_A; col <= COL_H; col++)
        {
            int auxRow = ROW_1;
            while (auxRow <= ROW_8)
            {
                if (validCoord(auxRow, col) && auxRow != row)
                    rookAttacks[Square(row, col)] |= 1UL << Square(auxRow, col);

                auxRow++;
            }

            int auxCol = COL_A;

            while (auxCol <= COL_H)
            {
                if (validCoord(row, auxCol) && auxCol != col)
                    rookAttacks[Square(row, col)] |= 1UL << Square(row, auxCol);

                auxCol++;
            }
        }
    }
}
constexpr void PrecomputedAttacks::initializeBishopAttacks()
{
    for (int row = ROW_1; row <= ROW_8; row++)
    {
        for (int col = COL_A; col <= COL_H; col++)
        {
            int auxRow = row > col ? row - col : ROW_1;
            int auxCol = row > col ? COL_A : col - row;

            while (auxRow <= ROW_8 && auxCol <= COL_H)
            {
                if (row != auxRow && col != auxCol)
                    bishopAttacks[Square(row, col)] |= 1UL << Square(auxRow, auxCol);

                auxRow++;
                auxCol++;
            }

            auxRow = (7 - row) > col ? row + col : ROW_8;
            auxCol = (7 - row) > col ? COL_A : col - (7 - row);

            while (auxRow >= ROW_1 && auxCol <= COL_H)
            {
                if (row != auxRow && col != auxCol)
                    bishopAttacks[Square(row, col)] |= 1UL << Square(auxRow, auxCol);

                auxRow--;
                auxCol++;
            }
        }
    }
}
constexpr void PrecomputedAttacks::initializeQueenAttacks()
{
    for (Square square = SQ_A1; square <= SQ_H8; square++)
    {
        queenAttacks[square] = rookAttacks[square] | bishopAttacks[square];
    }
}