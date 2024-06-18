#pragma once

#include <cstdint>

/*
 * Enum representing all rows on a chess board from ROW_1 to ROW_8
 */
enum Row : uint8_t {
    ROW_1 = 0,
    ROW_2,
    ROW_3,
    ROW_4,
    ROW_5,
    ROW_6,
    ROW_7,
    ROW_8
};

/*
 * Enum representing all columns on a chess board from COL_A to COL_H
 */
enum Column : uint8_t {
    COL_A = 0,
    COL_B,
    COL_C,
    COL_D,
    COL_E,
    COL_F,
    COL_G,
    COL_H
};

/*
    WPawn = 0,
    WKnight = 1,
    WBishop = 2,
    WRook = 3,
    WQueen = 4,
    WKing = 5,
    BPawn = 6,
    BKnight = 7,
    BBishop = 8,
    BRook = 9,
    BQueen = 10,
    BKing = 11,
    Empty = 12
*/

enum class PieceType
{
    KNIGHT = 0,
    BISHOP = 1,
    ROOK = 2,
    QUEEN = 3,
    KING = 5,
    PAWN = 0,
    EMPTY = 6
};

enum class Piece
{
    WPawn = 0,
    WKnight = 1,
    WBishop = 2,
    WRook = 3,
    WQueen = 4,
    WKing = 5,
    BPawn = 6,
    BKnight = 7,
    BBishop = 8,
    BRook = 9,
    BQueen = 10,
    BKing = 11,
    Empty = 12
};

constexpr inline int index(Piece piece)
{
    return static_cast<int>(piece);
}

/*
 *   White = 0
 *   Black = 1
 */
enum class Color
{
    WHITE = 0,
    BLACK = 1
};

static constexpr char pieceRepresentation[13] = {'P', 'N', 'B', 'R', 'Q', 'K', 'p', 'n', 'b', 'r', 'q', 'k', ' '};

/*
 *   Returns char piece representation
 *   {'P', 'N', 'B', 'R', 'Q', 'K', 'p', 'n', 'b', 'r', 'q', 'k', ' '}
 */
constexpr inline char pieceToChar(Piece piece)
{
    return pieceRepresentation[static_cast<int>(piece)];
}

// Returns the piece that corresponds to the character
constexpr inline Piece charToPiece(char pieceChar)
{
    switch (pieceChar)
    {
    case 'P':
        return Piece::WPawn;
    case 'N':
        return Piece::WKnight;
    case 'B':
        return Piece::WBishop;
    case 'R':
        return Piece::WRook;
    case 'Q':
        return Piece::WQueen;
    case 'K':
        return Piece::WKing;
    case 'p':
        return Piece::BPawn;
    case 'n':
        return Piece::BKnight;
    case 'b':
        return Piece::BBishop;
    case 'r':
        return Piece::BRook;
    case 'q':
        return Piece::BQueen;
    case 'k':
        return Piece::BKing;
    default:
        return Piece::Empty;
    }
}

// returns piece color
constexpr inline Color color(Piece piece)
{
    if (static_cast<int>(piece) <= 5)
    {
        return Color::WHITE;
    }

    return Color::BLACK;
}
