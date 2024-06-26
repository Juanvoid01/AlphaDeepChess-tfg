#pragma once

#include "move.hpp"

class Board
{

public:
    Board() { clearPosition(); }
    ~Board() {}

    std::string toString() const;

    void loadFen(const std::string &fen);
    std::string fen() const;

    bool empty(Square square) const;
    void makeMove(Move move);

    // board with the pieces
    Piece boardPieces[64];

    /*
        index of each bitboard

        WPawn = 0
        WKnight = 1
        WBishop = 2
        WRook = 3
        WQueen = 4
        WKing = 5
        BPawn = 6
        BKnight = 7
        BBishop = 8
        BRook = 9
        BQueen = 10
        BKing = 11
    */
    uint64_t bitBoards[11];

    uint64_t BlackBB;     // bitboard for black pieces
    uint64_t WhiteBB;     // bitboard for white pieces
    uint64_t AllPiecesBB; // bitboard for all pieces

    uint64_t pinMask;
    uint64_t checkMask;

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
    PieceType getPieceType(Square square) const;
    Color getPieceColor(Square square) const;
    char squareToChar(Square square) const;
    void putPiece(Piece piece, Square square);
    void deletePiece(Square square);
    uint64_t friendlyBB(Color color) const;
    uint64_t enemyBB(Color color) const;
    uint64_t enemyOrEmptyBB(Color color) const;
    void clearPosition();
    void checkAndModifyCastleRights();
    void checkAndModifyEnPassantRule();

    void makeCastle(Move move);
    void makeEnPassant(Move move);
};

/*
 *   Returns the piece in the square, if square should be valid
 */
inline Piece Board::getPiece(Square square) const
{
    return boardPieces[square];
}

inline PieceType Board::getPieceType(Square square) const
{
    return pieceToPieceType(boardPieces[square]);
}

/*
 *   Returns the color of the piece, if the square is empty the color is undefined
 */
inline Color Board::getPieceColor(Square square) const
{
    if (WhiteBB & square.mask())
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
 *   Input should be a valid square;
 *   Return true if the square is empty;
 */
inline bool Board::empty(Square square) const
{
    return boardPieces[square] == Piece::Empty;
}

/*
 *   Input should be a valid square && piece != Empty;
 */
inline void Board::putPiece(Piece piece, Square square)
{

    uint64_t mask = square.mask();
    int newPieceIndex = index(piece);

    if (!empty(square))
    {
        // first delete the actual piece
        bitBoards[index(getPiece(square))] &= ~mask;
        WhiteBB &= ~mask;
        BlackBB &= ~mask;
    }

    bitBoards[newPieceIndex] |= mask;
    boardPieces[square] = piece;

    if (color(piece) == Color::WHITE)
    {
        WhiteBB |= mask;
    }
    else
    {
        BlackBB |= mask;
    }

    AllPiecesBB = WhiteBB | BlackBB;
}

/*
 *   Input should be a valid square && piece != Empty;
 */
inline void Board::deletePiece(Square square)
{
    uint64_t mask = square.mask();
    bitBoards[index(getPiece(square))] &= ~mask;
    WhiteBB &= ~mask;
    BlackBB &= ~mask;
    boardPieces[square] = Piece::Empty;
    AllPiecesBB = WhiteBB | BlackBB;
}

/*
 *   Remove all pieces on the board
 *   Do not modify the game state, just put all bitboards = 0
 */
inline void Board::clearPosition()
{
    BlackBB = 0;
    WhiteBB = 0;
    AllPiecesBB = 0;

    for (int i = 0; i < 12; i++)
        bitBoards[i] = 0;

    for (int i = 0; i < 64; i++)
        boardPieces[i] = Piece::Empty;
}

/*
 * Return the bitboard of all the squares with a friendly piece
 */
inline uint64_t Board::friendlyBB(Color color) const
{
    return color == Color::BLACK ? BlackBB : WhiteBB;
}

/*
 * Return the bitboard of all the squares with an enemy piece
 */
inline uint64_t Board::enemyBB(Color color) const
{
    return color == Color::WHITE ? BlackBB : WhiteBB;
}

/*
 * Return the bitboard of all the squares empty or with an enemy piece
 */
inline uint64_t Board::enemyOrEmptyBB(Color color) const
{
    return (~AllPiecesBB) | (color == Color::WHITE ? BlackBB : WhiteBB);
}
