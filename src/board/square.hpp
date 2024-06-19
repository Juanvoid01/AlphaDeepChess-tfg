#pragma once

#include <string>
#include <cstdint>

/*
 * Enum representing all squares on a chess board from A1 to H8
 */
enum SQ : uint8_t
{
    SQ_A1 = 0,
    SQ_B1,
    SQ_C1,
    SQ_D1,
    SQ_E1,
    SQ_F1,
    SQ_G1,
    SQ_H1,
    SQ_A2,
    SQ_B2,
    SQ_C2,
    SQ_D2,
    SQ_E2,
    SQ_F2,
    SQ_G2,
    SQ_H2,
    SQ_A3,
    SQ_B3,
    SQ_C3,
    SQ_D3,
    SQ_E3,
    SQ_F3,
    SQ_G3,
    SQ_H3,
    SQ_A4,
    SQ_B4,
    SQ_C4,
    SQ_D4,
    SQ_E4,
    SQ_F4,
    SQ_G4,
    SQ_H4,
    SQ_A5,
    SQ_B5,
    SQ_C5,
    SQ_D5,
    SQ_E5,
    SQ_F5,
    SQ_G5,
    SQ_H5,
    SQ_A6,
    SQ_B6,
    SQ_C6,
    SQ_D6,
    SQ_E6,
    SQ_F6,
    SQ_G6,
    SQ_H6,
    SQ_A7,
    SQ_B7,
    SQ_C7,
    SQ_D7,
    SQ_E7,
    SQ_F7,
    SQ_G7,
    SQ_H7,
    SQ_A8,
    SQ_B8,
    SQ_C8,
    SQ_D8,
    SQ_E8,
    SQ_F8,
    SQ_G8,
    SQ_H8,
    SQ_NONE = 255
};

/*
 *   Represents an square on the board form 0(row 0, col 0) to 63(row 7, col 7)
 */
class Square
{

public:
    constexpr Square() : squareValue(255) {}

    /*
     * Create square by row and col
     * row and col should be between 0 and 7 
     */
    constexpr Square(int row, int col) : squareValue((row << 3) + col) {}

    /*
     * Create square by squareValue (0 to 63)
     */
    constexpr Square(uint8_t squareValue) : squareValue(squareValue) {}

    inline constexpr int row() const { return squareValue >> 3; }    // row = squareValue / 8
    inline constexpr int col() const { return squareValue & 0b111; } // col = squareValue % 8

    // return the square value from 0 to 63
    inline constexpr uint8_t value() const { return squareValue; }

    /*
     * Setter for squareValue
     */
    inline constexpr void set(uint8_t newSquareValue)
    {
        squareValue = newSquareValue;
    }

    /*
     * Setter for row and col
     * row and col should be between 0 and 7 
     */
    inline constexpr void set(int row, int col)
    {
        squareValue = (row << 3) + col;
    }

    /*
     * Returns true if squareValue >=0 && squareValue < 64
     */
    inline constexpr bool isValid() const { return squareValue < 64; }

    /*
     *   Invalidates the square, Put squareValue to 255
     */
    inline constexpr void setInvalid() { squareValue = 255; }

    /*
     *   Return the 64 bit mask, all zeros except 1 in the (row * 8 + col) bit;
     *   SquareValue should be valid
     */
    inline uint64_t mask() const
    {
        return 1UL << squareValue;
    }

    /*
     *   Return string representation. E.g : e4, d6, h1, e8
     */
    inline std::string toString() const
    {
        return std::string(1, 'a' + col()) + static_cast<char>('1' + row());
    }

    // Implicit conversion operator to uint8_t
    constexpr operator uint8_t() const { return squareValue; }

// Pre-increment operator overload
    constexpr Square& operator++() { 
        ++squareValue; 
        return *this; 
    }

    // Post-increment operator overload
    constexpr Square operator++(int) {
        Square temp = *this;
        ++(*this);
        return temp;
    }

    // Pre-decrement operator overload
    constexpr Square& operator--() {
        --squareValue;
        return *this;
    }

    // Post-decrement operator overload
    constexpr Square operator--(int) {
        Square temp = *this;
        --(*this);
        return temp;
    }

private:
    uint8_t squareValue;
};