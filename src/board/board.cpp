#include "board.hpp"

#include <sstream>
#include <stdexcept>

std::string Board::toString() const
{
    std::ostringstream diagram;
    diagram << "\n +---+---+---+---+---+---+---+---+\n";

    for (int row = ROW_8; row >= ROW_1; --row)
    {
        for (int col = COL_A; col <= COL_H; ++col)
        {
            diagram << " | " << squareToChar({row, col});
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

    clearPosition();

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
    }

    checkAndModifyCastleRights();

    // 4. En passant square.

    ss >> std::skipws >> token;
    if (token != '-')
    {
        col = token - 'a';
        ss >> token;
        row = token - '1';

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
 *  Used in set fen to check if castle is really avaliable
 *  put castle rights to false if they are not avaliable
 */
void Board::checkAndModifyCastleRights()
{

    // Check for white king-side castling rights
    if (castleKWhite)
    {

        castleKWhite = getPiece({ROW_1, COL_E}) == Piece::WKing &&
                       getPiece({ROW_1, COL_H}) == Piece::WRook;
    }
    // Check for white queen-side castling rights
    if (castleQWhite)
    {
        castleQWhite = getPiece({ROW_1, COL_E}) == Piece::WKing &&
                       getPiece({ROW_1, COL_A}) == Piece::WRook;
    }

    // Check for black king-side castling rights
    if (castleKBlack)
    {
        castleKBlack = getPiece({ROW_8, COL_E}) == Piece::BKing &&
                       getPiece({ROW_8, COL_H}) == Piece::BRook;
    }

    // Check for black queen-side castling rights
    if (castleQBlack)
    {
        castleQBlack = getPiece({ROW_8, COL_E}) == Piece::BKing &&
                       getPiece({ROW_8, COL_A}) == Piece::BRook;
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

        int col = enPassantSquare.col();
        if (enPassantSquare.row() == ROW_6)
        {

            if ((getPiece({ROW_5, col - 1}) == Piece::WPawn || getPiece({ROW_5, col + 1}) == Piece::WPawn) &&
                getPiece({ROW_5, col}) == Piece::BPawn &&
                empty(enPassantSquare) && empty({ROW_7, col}))
            {
                valid = true;
            }
        }
        else if (enPassantSquare.row() == ROW_3)
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

/*
 *   Throw runtime error "Invalid move"
 */
void Board::makeMove(Move move)
{
    MoveType moveType = move.type();

    if (!move.isValid())
    {
        throw std::runtime_error("Invalid move");
    }

    if (moveType == MoveType::NORMAL)
    {
    }
    else if (moveType == MoveType::CASTLING)
    {
        makeCastle(move);
    }
    else if (moveType == MoveType::EN_PASSANT)
    {
    }
    else if (moveType == MoveType::PROMOTION)
    {
    }
}

void Board::makeCastle(Move move)
{
    /*
     * squareTo() should be the king origin and squareFrom() is the king end square
     */

    deletePiece(move.squareFrom());

    if (move.squareTo() == SQ_G1)
    {
        putPiece(Piece::WKing, SQ_G1);
        deletePiece(SQ_H1);
        putPiece(Piece::WRook, SQ_F1);
    }
    else if (move.squareTo() == SQ_C1)
    {
        putPiece(Piece::WKing, SQ_C1);
        deletePiece(SQ_A1);
        putPiece(Piece::WRook, SQ_D1);
    }
    else if (move.squareTo() == SQ_G8)
    {
        putPiece(Piece::BKing, SQ_G8);
        deletePiece(SQ_H8);
        putPiece(Piece::BRook, SQ_F8);
    }
    else if (move.squareTo() == SQ_C8)
    {
        putPiece(Piece::BKing, SQ_C8);
        deletePiece(SQ_A8);
        putPiece(Piece::BRook, SQ_D8);
    }
}
void Board::makeEnPassant(Move move)
{
}