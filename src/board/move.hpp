#pragma once

#include "types.hpp"
#include "square.hpp"

enum class MoveType
{
    NORMAL = 0,
    PROMOTION = 1,
    EN_PASSANT = 2,
    CASTLING = 3
};

// A move needs 16 bits to be stored
//
// bit  0- 5: destination square (from 0 to 63)
// bit  6-11: origin square (from 0 to 63)
// bit 12-13: promotion piece type
// bit 14-15: special move flag: promotion (1), en passant (2), castling (3)
// En passant bit is set only when a pawn can be captured
// while Move::none() and Move::null() have the same origin and destination square.

class Move
{
public:
    constexpr Move() : data(0b0000'0000'0000'0000) {}

    constexpr explicit Move(std::uint16_t data) : data(data) {}

    constexpr Move(Square from, Square to, MoveType type = MoveType::NORMAL, PieceType promotionPiece = PieceType::KNIGHT)
        : data((from.value() << 6) | to.value() |
               (static_cast<uint16_t>(promotionPiece) << 12) |
               (static_cast<uint16_t>(type) << 14))
    {
    }

    constexpr inline Square squareFrom() const
    {
        // bit  6-11: origin square (from 0 to 63)
        return Square((data & 0b0000'1111'1100'0000) >> 6);
    }

    constexpr inline Square squareTo() const
    {
        // bit  0- 5: destination square (from 0 to 63)
        return Square(data & 0b0000'0000'0011'1111);
    }

    constexpr inline MoveType type() const
    {
        /*
            bit 12-13: promotion piece type :
                00 = NORMAL
                01 = PROMOTION
                10 = EN_PASSANT
                11 = CASTLING
        */
        return static_cast<MoveType>(data >> 14);
    }

    constexpr inline PieceType promotionPiece() const
    {
        /*
            bit 12-13: promotion piece type :
                00 = KNIGHT
                01 = BISHOP
                10 = ROOK
                11 = QUEEN
        */
        return static_cast<PieceType>((data & 0b0011'0000'0000'0000) >> 12);
    }

    // move is not null and is not none
    constexpr bool isValid() const { return none().data != data && null().data != data; }

    static constexpr Move null()
    {
        // Move::null() has the same origin and destination square.
        return Move(0b0000'0000'0100'0001);
    }

    static constexpr Move none()
    {
        // Move::none() is 0 and has the same origin and destination square.
        return Move(0b0000'0000'0000'0000);
    }

    static constexpr Move castleWking() { return Move(SQ_E1, SQ_G1, MoveType::CASTLING); }
    static constexpr Move castleWqueen() { return Move(SQ_E1, SQ_C1, MoveType::CASTLING); }
    static constexpr Move castleBking() { return Move(SQ_E8, SQ_G8, MoveType::CASTLING); }
    static constexpr Move castleBqueen() { return Move(SQ_E8, SQ_C8, MoveType::CASTLING); }

    constexpr bool operator==(const Move &m) const { return data == m.data; }
    constexpr bool operator!=(const Move &m) const { return data != m.data; }

private:
    std::uint16_t data;
};