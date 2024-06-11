#pragma once

/*
    Uci protocol specifications
    https://gist.github.com/DOBRO/2592c6dad754ba67e6dcaec8c90165bf#file-uci-protocol-specification-txt
*/



class Uci
{
public:
    Uci() {}

    ~Uci() {}

    void loop();

private:
    void uciCommandAction();
    void isReadyCommandAction();
    void newgameCommandAction();
    void goCommandAction();
    void stopCommandAction();
    void evalCommandAction();
    void positionCommandAction();
    void diagramCommandAction();
    void helpCommandAction();
    void quitCommandAction();
    void unknownCommandAction();
};