#include "board.hpp"

#include <sstream>

std::string Board::toString() const
{
    std::ostringstream diagram;
    diagram << "\n +---+---+---+---+---+---+---+---+\n";

    Square square;

    for (int row = ROW_8; row >= ROW_1; --row)
    {
        for (int col = COL_A; col <= COL_H; ++col)
        {
            square.set(row, col);
            diagram << " | " << squareToChar(square);
        }
        diagram << " | " << row + 1 << "\n +---+---+---+---+---+---+---+---+\n";
    }
    diagram << "   a   b   c   d   e   f   g   h\n";
    diagram << "\n\nFen: " << fen();

    return diagram.str();
}

/*
 *   Set the position on the board stored in fen notation
 *   Return false if fen is incorrectly formatted or error happens
 */
void Board::loadFen(const std::string &fen)
{

    /*
        Fen notation info :

        https://www.chess.com/terms/fen-chess
        https://en.wikipedia.org/wiki/Forsyth%E2%80%93Edwards_Notation
    */

    int row = ROW_8, col = COL_A;
    char token;
    Piece piece;
    std::istringstream ss(fen);

    ss >> std::noskipws;

    // 1. Piece placement
    while ((ss >> token) && !isspace(token))
    {
        if (isdigit(token))
        {
            col += (token - '0'); // Advance the given number of columns
        }
        else if (token == '/')
        {
            row -= 1; // Advance to the next row
            col = COL_A;
        }
        else if ((piece = charToPiece(token)) != Piece::Empty)
        {
            putPiece(piece, {row, col});
            col++;
        }
    }

    // 2. Active color
    ss >> token;
    sideToMove = (token == 'w' ? Color::WHITE : Color::BLACK);
    ss >> token;

    // 3. Castling availability.
    while ((ss >> token) && !isspace(token))
    {
        if (token == 'K')
        {
            // white king castle
            castleKWhite = true;
        }
        else if (token == 'Q')
        {
            // white queen castle
            castleQWhite = true;
        }
        else if (token == 'k')
        {
            // black king castle
            castleKBlack = true;
        }
        else if (token == 'q')
        {
            // black queen castle
            castleQBlack = true;
        }

        checkAndModifyCastleRights();
    }

    // 4. En passant square.

    if ((ss >> col) && (ss >> row))
    {
        row = row - '1';
        col = col - 'a';

        enPassantSquare = Square(row, col);
        checkAndModifyEnPassantRule();
    }

    // 5-6. Halfmove clock and fullmove number
    ss >> std::skipws >> halfmove >> moveNumber;
}

/*
    Returns fen representation of the position
*/
std::string Board::fen() const
{
    int emptyCounter;
    std::ostringstream fen;

    for (int row = ROW_8; row >= ROW_1; row--)
    {
        for (int col = COL_A; col <= COL_H; col++)
        {
            emptyCounter = 0;
            while (col <= COL_H && empty(Square(row, col)))
            {
                emptyCounter++;
                col++;
            };

            if (emptyCounter)
                fen << emptyCounter;

            if (col <= 7)
                fen << squareToChar(Square(row, col));
        }

        if (row > 0)
            fen << '/';
    }

    fen << (sideToMove == Color::WHITE ? " w " : " b ");

    if (castleKWhite)
        fen << 'K';
    if (castleQWhite)
        fen << 'Q';
    if (castleKBlack)
        fen << 'k';
    if (castleQBlack)
        fen << 'q';

    if (!castleKWhite && !castleQWhite && !castleKBlack && !castleQBlack)
    {
        fen << '-';
    }

    fen << (!enPassantSquare.isValid() ? " - " : " " + enPassantSquare.toString() + " ")
        << halfmove << " " << moveNumber;

    return fen.str();
}

/*
 *   Returns the piece in the square, if square is not valid returns empty
 */
Piece Board::getPiece(Square square) const
{
    uint64_t mask = square.mask();

    if (!square.isValid() || (AllPieces & mask) == 0)
        return Piece::Empty;

    if (BPawns & mask)
        return Piece::BPawn;
    if (BKnights & mask)
        return Piece::BKnight;
    if (BBishops & mask)
        return Piece::BBishop;
    if (BRooks & mask)
        return Piece::BRook;
    if (BQueens & mask)
        return Piece::BQueen;
    if (BKing & mask)
        return Piece::BKing;
    if (WPawns & mask)
        return Piece::WPawn;
    if (WKnights & mask)
        return Piece::WKnight;
    if (WBishops & mask)
        return Piece::WBishop;
    if (WRooks & mask)
        return Piece::WRook;
    if (WQueens & mask)
        return Piece::WQueen;
    if (WKing & mask)
        return Piece::WKing;

    return Piece::Empty;
}

/*
 *  Used in set fen to check if castle is really avaliable
 *  put castle rights to false if they are not avaliable
 */
void Board::checkAndModifyCastleRights()
{

    // Check for white king-side castling rights
    if (castleKWhite)
    {
        castleKWhite = getPiece({ROW_1, COL_E}) == Piece::WKing &&
                       getPiece({ROW_1, COL_H}) == Piece::WRook &&
                       empty({ROW_1, COL_F}) && empty({ROW_1, COL_G});
    }
    // Check for white queen-side castling rights
    if (castleQWhite)
    {
        castleQWhite = getPiece({ROW_1, COL_E}) == Piece::WKing &&
                       getPiece({ROW_1, COL_A}) == Piece::WRook &&
                       empty({ROW_1, COL_B}) && empty({ROW_1, COL_C}) && empty({ROW_1, COL_D});
    }

    // Check for black king-side castling rights
    if (castleKBlack)
    {
        castleKBlack = getPiece({ROW_8, COL_E}) == Piece::BKing &&
                       getPiece({ROW_8, COL_H}) == Piece::BRook &&
                       empty({ROW_8, COL_F}) && empty({ROW_8, COL_G});
    }

    // Check for black queen-side castling rights
    if (castleQBlack)
    {
        castleQBlack = getPiece({ROW_8, COL_E}) == Piece::BKing &&
                       getPiece({ROW_8, COL_A}) == Piece::BRook &&
                       empty({ROW_8, COL_B}) && empty({ROW_8, COL_C}) && empty({ROW_8, COL_D});
    }
}

/*
 *  Used in set fen to check if en passant is really avaliable
 *  put en passant square to not valid if is not avaliable
 */
void Board::checkAndModifyEnPassantRule()
{
    // En passant square will be considered only if
    // a) side to move have a pawn threatening enPassantSquare
    // b) there is an enemy pawn in front of enPassantSquare
    // c) there is no piece on enPassantSquare or behind enPassantSquare

    bool valid = false;

    if (enPassantSquare.isValid())
    {

        int col = enPassantSquare.col;
        if (enPassantSquare.row == ROW_6)
        {

            if ((getPiece({ROW_5, col - 1}) == Piece::WPawn || getPiece({ROW_5, col + 1}) == Piece::WPawn) &&
                getPiece({ROW_5, col}) == Piece::BPawn &&
                empty(enPassantSquare) && empty({ROW_7, col}))
            {
                valid = true;
            }
        }
        else if (enPassantSquare.row == ROW_3)
        {
            if ((getPiece({ROW_4, col - 1}) == Piece::BPawn || getPiece({ROW_4, col + 1}) == Piece::BPawn) &&
                getPiece({ROW_5, col}) == Piece::WPawn &&
                empty(enPassantSquare) && empty({ROW_2, col}))
            {
                valid = true;
            }
        }

        if (!valid)
        {
            // invalid enPassant row
            enPassantSquare.setInvalid();
        }
    }
}