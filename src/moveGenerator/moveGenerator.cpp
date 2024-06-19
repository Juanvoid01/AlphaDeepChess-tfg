#include <bit>

#include "moveGenerator.hpp"
#include "precomputedAttacks.hpp"

static const PrecomputedAttacks precomputedAttacks;

static void generatePawnMoves(MoveList &moves, Square square, Color sideToMove);
static void generateRookMoves(MoveList &moves, Square square, Color sideToMove);
static void generateKnightMoves(MoveList &moves, Square square, Color sideToMove);
static void generateBishopMoves(MoveList &moves, Square square, Color sideToMove);
static void generateQueenMoves(MoveList &moves, Square square, Color sideToMove);
static void generateKingMoves(MoveList &moves, Square square, Color sideToMove);

void generateLegalMoves(MoveList &moves, const Board &board)
{
    for (Square square = SQ_A1; square < SQ_H8; square++)
    {
        if (board.empty(square) || board.getPieceColor(square) != board.sideToMove)
            continue;

        switch (board.getPieceType(square))
        {
        case PieceType::PAWN:
            generatePawnMoves(moves, square, board.sideToMove);
            break;
        case PieceType::KNIGHT:
            generateKnightMoves(moves, square, board.sideToMove);
            break;
        case PieceType::BISHOP:
            generateBishopMoves(moves, square, board.sideToMove);
            break;
        case PieceType::ROOK:
            generateRookMoves(moves, square, board.sideToMove);
            break;
        case PieceType::QUEEN:
            generateQueenMoves(moves, square, board.sideToMove);
            break;
        case PieceType::KING:
            generateKingMoves(moves, square, board.sideToMove);
            break;
        default:
            break;
        }
    }
}

static void generatePawnMoves(MoveList &moves, Square square, Color sideToMove)
{

    uint64_t pawnAttacks = sideToMove == Color::WHITE ? precomputedAttacks.getPawnWhiteAttacks(square)
                                                      : precomputedAttacks.getPawnBlackAttacks(square);

    while (pawnAttacks != 0)
    {
        // Find the index of the least significant set bit
        uint8_t squareTo = std::countr_zero(pawnAttacks);
        // Clear the least significant set bit
        pawnAttacks &= (pawnAttacks - 1);

        moves.add(Move(square, squareTo));
    }
}

static void generateRookMoves(MoveList &moves, Square square, Color sideToMove)
{
    uint64_t rookAttacks = precomputedAttacks.getRookAttacks(square);

    while (rookAttacks != 0)
    {
        // Find the index of the least significant set bit
        uint8_t squareTo = std::countr_zero(rookAttacks);
        // Clear the least significant set bit
        rookAttacks &= (rookAttacks - 1);

        moves.add(Move(square, squareTo));
    }
}

static void generateKnightMoves(MoveList &moves, Square square, Color sideToMove)
{
    uint64_t knightAttacks = precomputedAttacks.getKnightAttacks(square);

    while (knightAttacks != 0)
    {
        // Find the index of the least significant set bit
        uint8_t squareTo = std::countr_zero(knightAttacks);
        // Clear the least significant set bit
        knightAttacks &= (knightAttacks - 1);

        moves.add(Move(square, squareTo));
    }
}

static void generateBishopMoves(MoveList &moves, Square square, Color sideToMove)
{
    uint64_t bishopAttacks = precomputedAttacks.getBishopAttacks(square);

    while (bishopAttacks != 0)
    {
        // Find the index of the least significant set bit
        uint8_t squareTo = std::countr_zero(bishopAttacks);
        // Clear the least significant set bit
        bishopAttacks &= (bishopAttacks - 1);

        moves.add(Move(square, squareTo));
    }
}

static void generateQueenMoves(MoveList &moves, Square square, Color sideToMove)
{
    uint64_t queenAttacks = precomputedAttacks.getQueenAttacks(square);

    while (queenAttacks != 0)
    {
        // Find the index of the least significant set bit
        uint8_t squareTo = std::countr_zero(queenAttacks);
        // Clear the least significant set bit
        queenAttacks &= (queenAttacks - 1);

        moves.add(Move(square, squareTo));
    }
}

static void generateKingMoves(MoveList &moves, Square square, Color sideToMove)
{
    uint64_t kingAttacks = precomputedAttacks.getKingAttacks(square);

    while (kingAttacks != 0)
    {
        // Find the index of the least significant set bit
        uint8_t squareTo = std::countr_zero(kingAttacks);
        // Clear the least significant set bit
        kingAttacks &= (kingAttacks - 1);

        moves.add(Move(square, squareTo));
    }
}