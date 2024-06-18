/*
    Uci protocol specifications
    https://gist.github.com/DOBRO/2592c6dad754ba67e6dcaec8c90165bf#file-uci-protocol-specification-txt
*/

#include "uci.hpp"

#include <iostream>
#include <string>
#include <sstream>

constexpr auto StartFEN = "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1";
constexpr auto EnPassantFEN = "rnbqkb1r/2pp2pn/1p6/pP1PppPp/8/2N5/P1P1PP1P/R1BQKBNR w KQkq f6 0 8";
constexpr auto PromotionFEN = "r1b1kb1r/p1pqn1P1/1pn4p/8/2P5/2N5/PPQBN1pP/R3KB1R w KQkq - 0 12";

void Uci::loop()
{
    std::string command;
    std::string line;

    bool exit = false;

    board.loadFen(PromotionFEN);
    board.makeMove(Move(SQ_G7, SQ_F8, MoveType::PROMOTION, PieceType::QUEEN));
    board.makeMove(Move(SQ_G2, SQ_H1, MoveType::PROMOTION, PieceType::QUEEN));
    do
    {
        std::getline(std::cin, line);

        // convert the input line into a stream of words
        std::istringstream iss(line);

        // avoid a stale if blank line
        command.clear();

        iss >> std::skipws >> command;

        if (command == "uci")
        {
            uciCommandAction();
        }
        else if (command == "isready")
        {
            isReadyCommandAction();
        }
        else if (command == "ucinewgame")
        {
            newgameCommandAction();
        }
        else if (command == "go")
        {
            goCommandAction();
        }
        else if (command == "stop")
        {
            stopCommandAction();
        }
        else if (command == "eval")
        {
            evalCommandAction();
        }
        else if (command == "position")
        {
            positionCommandAction();
        }
        else if (command == "d")
        {
            diagramCommandAction();
        }
        else if (command == "help")
        {
            helpCommandAction();
        }
        else if (command == "quit")
        {
            quitCommandAction();
            exit = true;
        }
        else
        {
            unknownCommandAction();
        }

    } while (!exit);
}

/*
    respond with "uciok"

    tell engine to use the uci (universal chess interface),
    this will be sent once as a first command after program boot
*/
void Uci::uciCommandAction()
{
    std::cout << "uciok" << std::endl;
}

/*
    respond with "readyok"

    this command can be used to wait for the engine to be ready again or
    to ping the engine to find out if it is still alive.
*/
void Uci::isReadyCommandAction()
{
    std::cout << "readyok" << std::endl;
}

void Uci::newgameCommandAction()
{
    std::cout << "Not implemented yet!" << std::endl;
}

/*
    start calculating on the current position
*/
void Uci::goCommandAction()
{
    std::cout << "Not implemented yet!" << std::endl;
}

/*
    stop calculating as soon as possible,
*/
void Uci::stopCommandAction()
{
    std::cout << "Not implemented yet!" << std::endl;
}

void Uci::evalCommandAction()
{
    std::cout << "Not implemented yet!" << std::endl;
}

/*
    position [fen <fenstring> | startpos ]  moves <move1> .... <movei>
    set up the position described in fenstring on the internal board
*/
void Uci::positionCommandAction()
{
    std::cout << "Not implemented yet!" << std::endl;
}

/*
    handle "d" command
    generates diagram of the chess position
*/
void Uci::diagramCommandAction()
{
    std::cout << board.toString() << std::endl;
}

void Uci::helpCommandAction()
{
    std::cout << "Commands:\n"
                 "----------------------------------------\n"
                 "uci\n"
                 "\tTell engine to use the UCI (Universal Chess Interface).\n"
                 "\tThe engine must respond with 'uciok'.\n\n"

                 "isready\n"
                 "\tSynchronize the engine with the GUI. The engine must respond with 'readyok'.\n\n"

                 "ucinewgame\n"
                 "\tStart of a new game.\n\n"

                 "position [fen <fenstring> | startpos ] moves <move1> .... <movei>\n"
                 "\tSet up the position on the internal board.\n\n"

                 "go\n"
                 "\tStart calculating.\n"
                 "\tOptional parameters: searchmoves, ponder, wtime, btime, winc, binc, movestogo, depth, nodes, mate, movetime, infinite.\n\n"

                 "stop\n"
                 "\tStop calculating.\n\n"

                 "quit\n"
                 "\tQuit the program.\n\n"

                 "d\n"
                 "\tDisplay the current position on the board.\n\n"

              << std::endl;
}

/*
    quit the program as soon as possible
*/
void Uci::quitCommandAction()
{
    std::cout << "goodbye" << std::endl;
}

void Uci::unknownCommandAction()
{
    std::cout << "Unknown command, type help for more information" << std::endl;
}