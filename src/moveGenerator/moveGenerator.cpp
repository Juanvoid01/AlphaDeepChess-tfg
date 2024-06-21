#include <bit>

#include "moveGenerator.hpp"
#include "precomputedData.hpp"

static PrecomputedData precomputedData;

static Color sideToMove;
static uint64_t pinMask;
static uint64_t checkMask;
static uint64_t enemyBB;
static uint64_t friendlyBB;
static Dir pawnMoveDir;
static int pawnPrePromotionRow;
static int pawnInitialRow;

static void initializeVariables(MoveList &moves, const Board &board);
static void calculatePinMask(const Board &board);

static void generatePawnMoves(MoveList &moves, Square square, const Board &board);
static void generateRookMoves(MoveList &moves, Square square, const Board &board);
static void generateKnightMoves(MoveList &moves, Square square, const Board &board);
static void generateBishopMoves(MoveList &moves, Square square, const Board &board);
static void generateQueenMoves(MoveList &moves, Square square, const Board &board);
static void generateKingMoves(MoveList &moves, Square square, const Board &board);

void generateLegalMoves(MoveList &moves, const Board &board)
{
    initializeVariables(moves, board);

    for (Square square = SQ_A1; square <= SQ_H8; square++)
    {
        if (board.empty(square) || board.getPieceColor(square) != sideToMove)
            continue;

        switch (board.getPieceType(square))
        {
        case PieceType::PAWN:
            // generatePawnMoves(moves, square, board);
            break;
        case PieceType::KNIGHT:
            // generateKnightMoves(moves, square, board);
            break;
        case PieceType::BISHOP:
            //generateBishopMoves(moves, square, board);
            break;
        case PieceType::ROOK:
            // generateRookMoves(moves, square, board);
            break;
        case PieceType::QUEEN:
            generateQueenMoves(moves, square, board);
            break;
        case PieceType::KING:
            // generateKingMoves(moves, square, board);
            break;
        default:
            break;
        }
    }
}

static void initializeVariables(MoveList &moves, const Board &board)
{
    moves.clear();
    sideToMove = board.sideToMove;
    pawnMoveDir = sideToMove == Color::WHITE ? Dir::UP : Dir::DOWN;
    pawnPrePromotionRow = sideToMove == Color::WHITE ? ROW_7 : ROW_2;
    pawnInitialRow = sideToMove == Color::WHITE ? ROW_2 : ROW_7;
    enemyBB = board.enemyBB(sideToMove);
    friendlyBB = board.friendlyBB(sideToMove);

    precomputedData.calculateMoves();

    calculatePinMask(board);
}

static void generatePawnMoves(MoveList &moves, Square square, const Board &board)
{

    const int row = square.row();
    const int pushSquare = square + pawnMoveDir;

    // get all the squares that the pawn attacks
    uint64_t pawnAttacks =
        sideToMove == Color::WHITE ? precomputedData.getPawnWhiteAttacks(square)
                                   : precomputedData.getPawnBlackAttacks(square);

    // en passant
    if (board.enPassantSquare.isValid() && pawnAttacks & (1UL << board.enPassantSquare))
    {
        moves.add(Move(square, board.enPassantSquare, MoveType::EN_PASSANT));
    }

    // only get the squares with enemy piece
    pawnAttacks &= board.enemyBB(sideToMove);

    while (pawnAttacks != 0)
    {
        // Find the index of the least significant set bit
        uint8_t squareTo = std::countr_zero(pawnAttacks);
        // Clear the least significant set bit
        pawnAttacks &= (pawnAttacks - 1);

        if (row != pawnPrePromotionRow) // diagonal captures
        {
            moves.add(Move(square, squareTo));
        }
        else // diagonal captures and promotion
        {
            moves.add(Move(square, squareTo, MoveType::PROMOTION, PieceType::KNIGHT));
            moves.add(Move(square, squareTo, MoveType::PROMOTION, PieceType::BISHOP));
            moves.add(Move(square, squareTo, MoveType::PROMOTION, PieceType::ROOK));
            moves.add(Move(square, squareTo, MoveType::PROMOTION, PieceType::QUEEN));
        }
    }

    // forward push and double push

    if (row == pawnInitialRow)
    {
        if (board.empty(pushSquare))
        {
            moves.add(Move(square, pushSquare)); // pawn push

            const int doublePushSquare = pushSquare + pawnMoveDir;
            if (board.empty(doublePushSquare))
                moves.add(Move(square, doublePushSquare)); // initial double push
        }
    }
    else if (row == pawnPrePromotionRow)
    {
        if (board.empty(pushSquare)) // push and promotion
        {
            moves.add(Move(square, pushSquare, MoveType::PROMOTION, PieceType::KNIGHT));
            moves.add(Move(square, pushSquare, MoveType::PROMOTION, PieceType::BISHOP));
            moves.add(Move(square, pushSquare, MoveType::PROMOTION, PieceType::ROOK));
            moves.add(Move(square, pushSquare, MoveType::PROMOTION, PieceType::QUEEN));
        }
    }
    else
    {
        if (board.empty(pushSquare))
            moves.add(Move(square, pushSquare)); // normal pawn push
    }
}

static void generateRookMoves(MoveList &moves, Square square, const Board &board)
{

    uint64_t blockers = board.AllPiecesBB & precomputedData.getRookAttacks(square);

    // filter the moves so we cant take a friendly piece
    uint64_t rookMoves = precomputedData.getRookMoves(square, blockers) & ~friendlyBB;

    while (rookMoves != 0)
    {
        // Find the index of the least significant set bit
        uint8_t squareTo = std::countr_zero(rookMoves);
        // Clear the least significant set bit
        rookMoves &= (rookMoves - 1);

        moves.add(Move(square, squareTo));
    }
}

static void generateKnightMoves(MoveList &moves, Square square, const Board &board)
{
    // get all the squares that the knight attacks
    uint64_t knightAttacks = precomputedData.getKnightAttacks(square);

    // only get the squares empty or with enemy piece
    knightAttacks &= board.enemyOrEmptyBB(sideToMove);

    while (knightAttacks != 0)
    {
        // Find the index of the least significant set bit
        uint8_t squareTo = std::countr_zero(knightAttacks);
        // Clear the least significant set bit
        knightAttacks &= (knightAttacks - 1);

        moves.add(Move(square, squareTo));
    }
}

static void generateBishopMoves(MoveList &moves, Square square, const Board &board)
{
    uint64_t blockers = board.AllPiecesBB & precomputedData.getBishopAttacks(square);

    // filter the moves so we cant take a friendly piece
    uint64_t bishopMoves = precomputedData.getBishopMoves(square, blockers) & ~friendlyBB;

    while (bishopMoves != 0)
    {
        // Find the index of the least significant set bit
        uint8_t squareTo = std::countr_zero(bishopMoves);
        // Clear the least significant set bit
        bishopMoves &= (bishopMoves - 1);

        moves.add(Move(square, squareTo));
    }
}

static void generateQueenMoves(MoveList &moves, Square square, const Board &board)
{
    uint64_t rookBlockers = board.AllPiecesBB & precomputedData.getRookAttacks(square);
    uint64_t bishopBlockers = board.AllPiecesBB & precomputedData.getBishopAttacks(square);

    // filter the moves so we cant take a friendly piece, we pass the orthogonal and diagonal blockers
    uint64_t queenMoves = precomputedData.getQueenMoves(square, rookBlockers, bishopBlockers) & ~friendlyBB;

    while (queenMoves != 0)
    {
        // Find the index of the least significant set bit
        uint8_t squareTo = std::countr_zero(queenMoves);
        // Clear the least significant set bit
        queenMoves &= (queenMoves - 1);

        moves.add(Move(square, squareTo));
    }
}

static void generateKingMoves(MoveList &moves, Square square, const Board &board)
{
    // get all the squares that the king attacks
    uint64_t kingAttacks = precomputedData.getKingAttacks(square);

    // only get the squares empty or with enemy piece
    kingAttacks &= board.enemyOrEmptyBB(sideToMove);

    while (kingAttacks != 0)
    {
        // Find the index of the least significant set bit
        uint8_t squareTo = std::countr_zero(kingAttacks);
        // Clear the least significant set bit
        kingAttacks &= (kingAttacks - 1);

        moves.add(Move(square, squareTo));
    }

    // castling

    if (sideToMove == Color::WHITE)
    {
        if (board.getPiece(SQ_E1) == Piece::WKing)
        {
            if (board.castleKWhite && board.empty(SQ_F1) && board.empty(SQ_G1) &&
                board.getPiece(SQ_H1) == Piece::WRook)
            {
                moves.add(Move::castleWking());
            }

            if (board.castleQWhite && board.empty(SQ_D1) && board.empty(SQ_C1) &&
                board.empty(SQ_B1) && board.getPiece(SQ_H1) == Piece::WRook)
            {
                moves.add(Move::castleWqueen());
            }
        }
    }
    else
    {
        if (board.getPiece(SQ_E8) == Piece::BKing)
        {
            if (board.castleKBlack && board.empty(SQ_F8) && board.empty(SQ_G8) &&
                board.getPiece(SQ_H8) == Piece::BRook)
            {
                moves.add(Move::castleBking());
            }

            if (board.castleQBlack && board.empty(SQ_D8) && board.empty(SQ_C8) &&
                board.empty(SQ_B8) && board.getPiece(SQ_H8) == Piece::BRook)
            {
                moves.add(Move::castleBqueen());
            }
        }
    }
}

static void calculatePinMask(const Board &board)
{
}