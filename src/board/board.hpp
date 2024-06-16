#pragma once

#include <string>

#include "types.hpp"

class Board
{

public:
    Board() {}
    ~Board() {}

    std::string toString() const;

    void loadFen(const std::string &fen);
    std::string fen() const;

    bool empty(Square square) const;

private:
    // bitboards

    uint64_t BPawns;
    uint64_t BKnights;
    uint64_t BBishops;
    uint64_t BRooks;
    uint64_t BQueens;
    uint64_t BKing;
    uint64_t WPawns;
    uint64_t WKnights;
    uint64_t WBishops;
    uint64_t WRooks;
    uint64_t WQueens;
    uint64_t WKing;

    uint64_t Black;
    uint64_t White;
    uint64_t AllPieces;

    // Game state

    Color sideToMove;
    bool castleKWhite;
    bool castleQWhite;
    bool castleKBlack;
    bool castleQBlack;
    Square enPassantSquare;
    int halfmove;
    int moveNumber;

    Piece getPiece(Square square) const;
    Color getPieceColor(Square square) const;
    char squareToChar(Square square) const;
    void putPiece(Piece piece, Square square);
    void deletePiece(Square square);
    uint64_t &pieceBitboard(Piece piece);
    void clearPieces();
    void checkAndModifyCastleRights();
    void checkAndModifyEnPassantRule();
};

/*
 *   Returns the color of the piece, if the square is empty the color is undefined
 */
inline Color Board::getPieceColor(Square square) const
{
    if (White & square.mask())
    {
        return Color::WHITE;
    }
    else
    {
        return Color::BLACK;
    }
}

/*
 *   Returns Char representation of the piece in that square
 */
inline char Board::squareToChar(Square square) const
{
    return pieceToChar(getPiece(square));
}

/*
 *   Input should be 0 <= row < 8 && 0 <= col < 8;
 *   Return true if the square is empty;
 */
inline bool Board::empty(Square square) const
{
    return (AllPieces & square.mask()) == 0;
}

/*
 *   Input should be 0 <= row < 8 && 0 <= col < 8 && piece != Empty;
 */
inline void Board::putPiece(Piece piece, Square square)
{

    uint64_t mask = square.mask();

    if (empty(square))
    {
        pieceBitboard(piece) |= mask;
    }
    else
    {
        // first delete the actual piece
        pieceBitboard(getPiece(square)) &= ~mask;
        White &= ~mask;
        Black &= ~mask;

        pieceBitboard(piece) |= mask;
    }

    if (color(piece) == Color::WHITE)
    {
        White |= mask;
    }
    else
    {
        Black |= mask;
    }

    AllPieces |= mask;
}

/*
 *   Input should be 0 <= row < 8 && 0 <= col < 8 && piece != Empty;
 */
inline void Board::deletePiece(Square square)
{

    uint64_t mask = square.mask();
    pieceBitboard(getPiece(square)) &= ~mask;
    White &= ~mask;
    Black &= ~mask;
    AllPieces &= ~mask;
}

/*
 *   Returns reference to the bitboard of the piece
 *   Piece should not be Empty
 */
inline uint64_t &Board::pieceBitboard(Piece piece)
{

    static uint64_t *pieceBitboards[12] = {
        &WPawns, &WKnights, &WBishops, &WRooks, &WQueens, &WKing,
        &BPawns, &BKnights, &BBishops, &BRooks, &BQueens, &BKing};

    return *pieceBitboards[static_cast<int>(piece)];
}

/*
 *   Remove all pieces on the board
 *   Do not modify the game state, just put all bitboards = 0
 */
inline void Board::clearPieces()
{
    BPawns = 0;
    BKnights = 0;
    BBishops = 0;
    BRooks = 0;
    BQueens = 0;
    BKing = 0;
    WPawns = 0;
    WKnights = 0;
    WBishops = 0;
    WRooks = 0;
    WQueens = 0;
    WKing = 0;
    Black = 0;
    White = 0;
    AllPieces = 0;
}