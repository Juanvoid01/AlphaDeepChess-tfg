#include <cstdint>

#define ROW_1 0
#define ROW_2 1
#define ROW_3 2
#define ROW_4 3
#define ROW_5 4
#define ROW_6 5
#define ROW_7 6
#define ROW_8 7

#define COL_A 0
#define COL_B 1
#define COL_C 2
#define COL_D 3
#define COL_E 4
#define COL_F 5
#define COL_G 6
#define COL_H 7

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

/*
 *   White = 0
 *   Black = 1
 */
enum class Color
{
    WHITE = 0,
    BLACK = 1
};

/*
 *   Represents an square on the board
 *   int row
 *   int col
 */
struct Square
{

    int row;
    int col;

    Square(int row = -1, int col = -1)
        : row(row), col(col)
    {
    }

    /*
     * Setter for row and col
     */
    inline void set(int Row, int Col)
    {
        row = Row;
        col = Col;
    }

    /*
     * Returns true if 0 <= row < 8 && 0 <= col < 8;
     */
    inline bool isValid() const
    {
        return row >= ROW_1 && row <= ROW_8 && col >= COL_A && col <= COL_H;
    }

    /*
     *   Put row = -1 and col = -1
     */
    inline void setInvalid()
    {
        row = -1;
        col = -1;
    }

    /*
     *   Return the 64 bit mask, all zeros except 1 in the (row * 8 + col) bit;
     */
    inline uint64_t mask() const
    {
        return 1UL << ((row << 3) + col);
    }

    /*
     *   Return string representation. E.g : e4, d6, h1, e8
     */
    inline std::string toString() const
    {
        return std::string(1, 'a' + col) + static_cast<char>('1' + row);
    }
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
