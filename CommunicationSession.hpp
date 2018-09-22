#include <string>

//externs
//bool ANALYZE_MODE;
bool EDIT_MODE;
bool SHOW_WB_THINKING_OUTPUT;

//function conducting comunication with external interface app. via Winboard protocol
void makeWinboardSession();

//function returns changed gl_move if move is valid and in turn, else it returns zero
//changes gl_move
//uses Game data
unsigned int GetMove(std::string move);


