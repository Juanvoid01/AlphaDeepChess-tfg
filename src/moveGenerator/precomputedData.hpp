#pragma once

#include <unordered_map>

#include "types.hpp"
#include "square.hpp"

class PrecomputedData
{
public:
    constexpr PrecomputedData() { initialize(); }

    ~PrecomputedData()
    {
        for (int i = 0; i < 64; i++)
        {
            delete rookMoves[i];
            delete bishopMoves[i];
        }
    }

    void calculateMoves()
    {
        initializeRookMoves();
        initializeBishopMoves();
    };
    /*
     *   Return the 64 bit mask with 1 on the squares that the piece in the provided square is attacking on an empty board
     *   square should be valid
     */
    inline uint64_t getKingAttacks(Square square) const { return kingAttacks[square]; }

    /*
     *   Return the 64 bit mask with 1 on the squares that the piece in the provided square is attacking on an empty board
     *   square should be valid
     */
    inline uint64_t getKnightAttacks(Square square) const { return knightAttacks[square]; }

    /*
     *   Return the 64 bit mask with 1 on the squares that the piece in the provided square is attacking on an empty board
     *   square should be valid
     */
    inline uint64_t getPawnWhiteAttacks(Square square) const { return pawnWhiteAttacks[square]; }

    /*
     *   Return the 64 bit mask with 1 on the squares that the piece in the provided square is attacking on an empty board
     *   square should be valid
     */
    inline uint64_t getPawnBlackAttacks(Square square) const { return pawnBlackAttacks[square]; }

    /*
     *   Return the 64 bit mask with 1 on the squares that the piece in the provided square is attacking on an empty board
     *   square should be valid
     */
    inline uint64_t getRookAttacks(Square square) const { return rookAttacks[square]; }

    /*
     *   Return the 64 bit mask with 1 on the squares that the piece in the provided square is attacking on an empty board
     *   square should be valid
     */
    inline uint64_t getBishopAttacks(Square square) const { return bishopAttacks[square]; }

    /*
     *   Return the 64 bit mask with 1 on the squares that the piece in the provided square is attacking on an empty board
     *   square should be valid
     */
    inline uint64_t getQueenAttacks(Square square) const { return queenAttacks[square]; }

    /*
     *   Lookup table for rook moves gives the rook square and the bitboard of blockers.
     */
    inline uint64_t getRookMoves(Square rookSquare, uint64_t blockers) const { return (*rookMoves[rookSquare])[blockers]; }

    /*
     *   Lookup table for bishop moves gives the bishop square and the bitboard of blockers.
     */
    inline uint64_t getBishopMoves(Square bishopSquare, uint64_t blockers) const { return (*bishopMoves[bishopSquare])[blockers]; }

    /*
     *   Lookup table for queen moves gives the queen square and the bitboard of blockers.
     *   Input should be the horizontal and vertical blockers(rook), and the diagonal blockers(bishop)
     */
    inline uint64_t getQueenMoves(Square queenSquare, uint64_t blockersRook, uint64_t blockersBishop) const
    {
        return getRookMoves(queenSquare, blockersRook) | getBishopMoves(queenSquare, blockersBishop);
    }

private:
    /*
     *   Array of Lookup tables for each square.
     *
     *   Basically this is a dictionary for the rook moves given
     *   the square of the rook and the bitboard of blocker pieces.
     */
    std::unordered_map<uint64_t, uint64_t> *rookMoves[64] = {nullptr};

    /*
     *   Array of Lookup tables for each square.
     *
     *   Basically this is a dictionary for the bishop moves given
     *   the square of the bishop and the bitboard of blocker pieces.
     */
    std::unordered_map<uint64_t, uint64_t> *bishopMoves[64] = {nullptr};

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

    void initializeRookMoves();
    void initializeBishopMoves();

    void calculateRookMoves(Square square);
    void calculateBishopMoves(Square square);

    uint64_t calculateLegalRookMoves(Square square, uint64_t blockerBB);
    uint64_t calculateLegalBishopMoves(Square square, uint64_t blockerBB);
};

constexpr void PrecomputedData::initialize()
{
    initializeKingAttacks();
    initializeKnightAttacks();
    initializePawnAttacks();
    initializeRookAttacks();
    initializeBishopAttacks();
    initializeQueenAttacks();
}

constexpr void PrecomputedData::initializeKingAttacks()
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

constexpr void PrecomputedData::initializeKnightAttacks()
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

constexpr void PrecomputedData::initializePawnAttacks()
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

constexpr void PrecomputedData::initializeRookAttacks()
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
constexpr void PrecomputedData::initializeBishopAttacks()
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

constexpr void PrecomputedData::initializeQueenAttacks()
{
    for (Square square = SQ_A1; square <= SQ_H8; square++)
    {
        queenAttacks[square] = rookAttacks[square] | bishopAttacks[square];
    }
}

void PrecomputedData::initializeRookMoves()
{
    for (Square square = SQ_A1; square <= SQ_H8; square++)
    {
        rookMoves[square] = new std::unordered_map<uint64_t, uint64_t>();
        calculateRookMoves(square);
    }
}

void PrecomputedData::initializeBishopMoves()
{
    for (Square square = SQ_A1; square <= SQ_H8; square++)
    {
        bishopMoves[square] = new std::unordered_map<uint64_t, uint64_t>();
        calculateBishopMoves(square);
    }
}

void PrecomputedData::calculateRookMoves(Square square)
{
    uint64_t moveMask = getRookAttacks(square);

    int indicesInMoveMask[64] = {0};
    int numIndices = 0;

    // Create a list of the indices of the bits that are set to 1 in the movement mask
    for (int i = 0; i < 64; i++)
    {
        if (moveMask & (1UL << i))
        {
            indicesInMoveMask[numIndices++] = i;
        }
    }

    // Calculate total number of different bitboards
    int numPatterns = 1 << numIndices; // 2^n

    // Create all bitboards
    for (int patternIndex = 0; patternIndex < numPatterns; patternIndex++)
    {
        // calculate blocker bitboard
        uint64_t blockerBitboard = 0;
        for (int bitIndex = 0; bitIndex < numIndices; bitIndex++)
        {
            uint64_t bit = (patternIndex >> bitIndex) & 1;
            blockerBitboard |= bit << indicesInMoveMask[bitIndex];
        }

        // create entry in rook lookupTable
        (*rookMoves[square])[blockerBitboard] = calculateLegalRookMoves(square, blockerBitboard);
    }
}

void PrecomputedData::calculateBishopMoves(Square square)
{
    uint64_t moveMask = getBishopAttacks(square);

    int indicesInMoveMask[64] = {0};
    int numIndices = 0;

    // Create a list of the indices of the bits that are set to 1 in the movement mask
    for (int i = 0; i < 64; i++)
    {
        if (moveMask & (1UL << i))
        {
            indicesInMoveMask[numIndices++] = i;
        }
    }

    // Calculate total number of different bitboards
    int numPatterns = 1 << numIndices; // 2^n

    // Create all bitboards
    for (int patternIndex = 0; patternIndex < numPatterns; patternIndex++)
    {
        // calculate blocker bitboard
        uint64_t blockerBitboard = 0;
        for (int bitIndex = 0; bitIndex < numIndices; bitIndex++)
        {
            uint64_t bit = (patternIndex >> bitIndex) & 1;
            blockerBitboard |= bit << indicesInMoveMask[bitIndex];
        }

        // create entry in rook lookupTable
        (*bishopMoves[square])[blockerBitboard] = calculateLegalBishopMoves(square, blockerBitboard);
    }
}

uint64_t PrecomputedData::calculateLegalRookMoves(Square square, uint64_t blockerBB)
{
    uint64_t movesBitboard = 0;

    int row = square.row();
    int col = square.col();

    int rookDirections[4][2] = {
        {1, 0},  // Up
        {-1, 0}, // Down
        {0, 1},  // Right
        {0, -1}  // Left
    };

    for (int i = 0; i < 4; ++i)
    {
        int dirRow = rookDirections[i][0];
        int dirCol = rookDirections[i][1];

        int auxRow = row + dirRow;
        int auxCol = col + dirCol;

        while (validCoord(auxRow, auxCol))
        {
            Square auxSquare = Square(auxRow, auxCol);

            // set the square to 1 in the bitboard
            movesBitboard |= auxSquare.mask();

            // if there is a blocker in the square we stop in this direction
            if (blockerBB & auxSquare.mask())
            {
                break;
            }

            auxRow += dirRow;
            auxCol += dirCol;
        }
    }

    return movesBitboard;
}

uint64_t PrecomputedData::calculateLegalBishopMoves(Square square, uint64_t blockerBB)
{
    uint64_t movesBitboard = 0;

    int row = square.row();
    int col = square.col();

    int bishopDirections[4][2] = {
        {1, 1},  // Up-right
        {1, -1}, // Up-left
        {-1, 1}, // Down-right
        {-1, -1} // Down-left
    };

    for (int i = 0; i < 4; ++i)
    {
        int dirRow = bishopDirections[i][0];
        int dirCol = bishopDirections[i][1];

        int auxRow = row + dirRow;
        int auxCol = col + dirCol;

        while (validCoord(auxRow, auxCol))
        {
            Square auxSquare = Square(auxRow, auxCol);

            // set the square to 1 in the bitboard
            movesBitboard |= auxSquare.mask();

            // if there is a blocker in the square we stop in this direction
            if (blockerBB & auxSquare.mask())
            {
                break;
            }

            auxRow += dirRow;
            auxCol += dirCol;
        }
    }

    return movesBitboard;
}