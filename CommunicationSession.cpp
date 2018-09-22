#include <iostream>
#include <sstream>
//#include <fstream>
#include <iomanip>
#include <cstdlib>
#ifndef EXTERN_DECLARATIONS_INCL
				#define EXTERN_DECLARATIONS_INCL
				#include "ExternDeclarations.hpp"
#endif
#include "CommunicationSession.hpp"
using namespace std;

unsigned int GetMove(string move)
{
///////////////////////////////////

  #ifdef DEBUG_VERSION
	//cout<<"in GetMove() before function execution and affecting global move: gl_move == "<<printMove(gl_move)<<endl;
  #endif

  //find and set from_square (from_square_num)
	string froms = move.substr(0, 2);
  short from_square_num = 0;

  while (from_square_num < 64) {
    if (froms == SQUARE[from_square_num]) {
      break;
    };

    from_square_num++;
  };

  if (from_square_num == 64) {
    cout << "tellusererror Illegal move (from square)" << endl;
  };

//find and set to_square (to_square_num)
  string tosqu = move.substr(2, 2);

  short to_square_num = 0;

  while (to_square_num < 64) {
    if (tosqu == SQUARE[to_square_num]) {
      break;
    };

    to_square_num++;
  };

  if (to_square_num == 64) {
    cout << "tellusererror Illegal move (to square):" << endl;
  };

//find and set promotion piece
  string promo = move.substr(4, 1);

  short promo_piece_type = 0;

  if ((promo == "q") || (promo == "Q")) {
    if (GameWtm[GMI] == true) {
      promo_piece_type = 11;
    } else {
      promo_piece_type = 12;
    };
  };

  if ((promo == "r") || (promo == "R")) {
    if (GameWtm[GMI] == true) {
      promo_piece_type = 3;
    } else {
      promo_piece_type = 4;
    };
  };

  if ((promo == "b") || (promo == "B")) {
    if (GameWtm[GMI] == true) {
      promo_piece_type = 5;
    } else {
      promo_piece_type = 6;
    };
  };

  if ((promo == "n") || (promo == "N")) {
    if (GameWtm[GMI] == true) {
      promo_piece_type = 7;
    } else {
      promo_piece_type = 8;
    };
  };

//get list of valid moves of Game board and data
//copy board to proccess move search at B[] to proccess new FULL_PLY
  for (int i = 0; i < 64; i++) {
    B[i] = GameBoard[GMI][i];
  };

//set board data
  WpaNum = 0;

  BpaNum = 0;

  WroNum = 0;

  BroNum = 0;

  WbiNum = 0;

  BbiNum = 0;

  WknNum = 0;

  BknNum = 0;

  WquNum = 0;

  BquNum = 0;

  MaterialScore = 0;

  for (int i = 0; i < 64; i++) {
    switch (B[ws_r]) {
      case
          sWpa: {
          WpaNum++;
          MaterialScore += evPawnMaterialValue;
        };

        break;
      case
          sBpa: {
          BpaNum++;
          MaterialScore -= evPawnMaterialValue;
        };

        break;
      case
          sWro: {
          WroNum++;
          MaterialScore += evRookMaterialValue;
        };

        break;
      case
          sBro: {
          BroNum++;
          MaterialScore -= evRookMaterialValue;
        };

        break;
      case
          sWbi: {
          WbiNum++;
          MaterialScore += evBishopMaterialValue;
        };

        break;
      case
          sBbi: {
          BbiNum++;
          MaterialScore -= evBishopMaterialValue;
        };

        break;
      case
          sWkn: {
          WknNum++;
          MaterialScore += evKnightMaterialValue;
        };

        break;
      case
          sBkn: {
          BknNum++;
          MaterialScore -= evKnightMaterialValue;
        };

        break;
      case
          sWki: {
          WkiPos = i;
        };

        break;
      case
          sBki: {
          BkiPos = i;
        };

        break;
      case
          sWqu: {
          WquNum++;
          MaterialScore += evQueenMaterialValue;
        };

        break;
      case
          sBqu: {
          BquNum++;
          MaterialScore -= evQueenMaterialValue;
        };

        break;
    };
  };

  //set first ply initializers
  enp_sq[0] = GameEnpassant[GMI];

  castle_data[0] = Game_castle_data[GMI];

  Zkey_tab[0] = Game_Zkey_tab[GMI];

  retract_tab[0] = GameRetract[GMI];


  //make new ply
  if (GameWtm[GMI] == true) {
    //set check evasion status
    if (isAttByBla(WkiPos)) {
      CheckStatus[0] = true;
    } else {
      CheckStatus[0] = false;
    };

    max_offs[1] = FULL_WHI_PLY(1);

    #ifdef DEBUG_VERSION
    //cout<<"in GetMove() after whi ply generation - showing few Tree levels:"<<endl; getTreeLevels(0, 3);
    #endif

    //if no moves in the se_ply print error message
    if (max_offs[1] == 0) {
      if (CheckStatus[0] == true) {
        cout << "telluser Black mates" << endl;
				return 0;
      } else {
        cout << "telluser Stalemate" << endl;
        return 0;
      };
    };

    string moveList = "";

    for (int i = 0; i < max_offs[1]; i++) {
      gl_move = MOVES_TREE[(1 << 8) | i];
      gl_fs = ((gl_move << 26) >> 26);
      gl_ts = ((gl_move << 20) >> 26);
      gl_fp = ((gl_move << 16) >> 28);
      gl_tp = ((gl_move << 12) >> 28);
      //gl_cp = ((gl_move<<8)>>28);
      moveList += (" " + printMove(gl_move));

      //if coordinates match
      if ((gl_fs == from_square_num) && (gl_ts == to_square_num)) {
        if (gl_fp != gl_tp) {
          //promotion
          if (gl_tp == promo_piece_type) {
            #ifdef DEBUG_VERSION
	          //cout<<"in GetMove() after function execution and affecting global move: gl_move == "<<printMove(gl_move)<<endl;
            #endif
            return gl_move;
          };

          //enpassant
          if ((gl_tp == sWpa) || (gl_tp == sBpa)) {
            #ifdef DEBUG_VERSION
	          //cout<<"in GetMove() after function execution and affecting global move: gl_move == "<<printMove(gl_move)<<endl;
            #endif
						return gl_move;
          };
        } else {
        	#ifdef DEBUG_VERSION
	        //cout<<"in GetMove() after function execution and affecting global move: gl_move == "<<printMove(gl_move)<<endl;
          #endif
          return gl_move;
        };
      };
    };

    #ifdef DEBUG_VERSION
    cout << "telluser No move " << move
         << " at legal moves list. Legal moves are:" << moveList << endl;
    #endif

    return 0;
  //else if black to move
  } else {
    //set check evasion status
    if (isAttByWhi(BkiPos)) {
      CheckStatus[0] = true;
    } else {
      CheckStatus[0] = false;
    };

    //make new ply
    max_offs[1] = FULL_BLA_PLY(1);

		#ifdef DEBUG_VERSION
    //cout<<"in GetMove() after bla ply generation - showing few tree levels:"<<endl; getTreeLevels(0, 3);
    #endif

    //if no moves in the se_ply print error message
    if (max_offs[1] == 0) {
      if (CheckStatus[0] == true) {
        cout << "telluser White mates" << endl;
        return 0;
      } else {
        cout << "telluser Stalemate" << endl;
        return 0;
      };
    };

    string moveList = "";

    for (int i = 0; i < max_offs[1]; i++) {
      gl_move = MOVES_TREE[(1 << 8) | i];
      gl_fs = ((gl_move << 26) >> 26);
      gl_ts = ((gl_move << 20) >> 26);
      gl_fp = ((gl_move << 16) >> 28);
      gl_tp = ((gl_move << 12) >> 28);
      //gl_cp = ((gl_move<<8)>>28);
      moveList += (" " + printMove(gl_move));

      //if coordinates match
      if ((gl_fs == from_square_num) && (gl_ts == to_square_num)) {
        if (gl_fp != gl_tp) {
          //promotion
          if (gl_tp == promo_piece_type) {
            #ifdef DEBUG_VERSION
	          //cout<<"in GetMove() after function execution and affecting global move: gl_move == "<<printMove(gl_move)<<endl;
            #endif
						return gl_move;
          };

          //enpassant
          if ((gl_tp == sWpa) || (gl_tp == sBpa)) {
            #ifdef DEBUG_VERSION
	          //cout<<"in GetMove() after function execution and affecting global move: gl_move == "<<printMove(gl_move)<<endl;
            #endif
						return gl_move;
          };
        } else {
        	#ifdef DEBUG_VERSION
	        //cout<<"in GetMove() after function execution and affecting global move: gl_move == "<<printMove(gl_move)<<endl;
          #endif
          return gl_move;
        };
      };
    };

		#ifdef DEBUG_VERSION
    cout << "telluser No move " << move
         << " at legal moves list. Legal moves are:" << moveList << endl;
    #endif

    return 0;
  };
};


//winboard session
void makeWinboardSession()
{
	int ping_status = 0;
  string ws_tempstring;
  int ws_r, ws_j;
  
  //Winboard line in - extern variable
  lin = "";
	
  while (true) {
    // use loop to get winboard commands from standard input.
    // set streams properly for winboard protocol,
    // use logging functions for debugging
    //log("---wb session loop start---");
    cin.rdbuf()->in_avail();
    cin >> lin;

    if (lin == "xboard") {
      ping_status++;
      cout << endl;
      ping_status--;
      #ifdef DEBUG_VERSION
      cout << "command XBOARD received in winboard session, responding by endl..." << endl;
      #endif
    } else
      if (lin == "protover") {
        /* protover N
         Beginning in protocol version 2 (in which N=2), this command will be sent
         immediately after the "xboard" command. If you receive some other command
         immediately after "xboard" (such as "new"), you can assume that protocol version 1
          is in use. The "protover" command is the only new command that xboard always sends
         in version 2. All other new commands to the engine are sent only if the engine first
         enables them with the "feature" command. Protocol versions will always be simple
         integers so that they can easily be compared. Your engine should reply to the protover
         command by sending the "feature" command (see below) with the list of non-default
         feature settings that you require, if any. Your engine should never refuse to run due
         to receiving a higher protocol version number than it is expecting!
         New protocol versions will always be compatible with older ones by default;
         the larger version number is simply a hint that additional "feature" command
         options added in later protocol versions may be accepted.
         */
        ping_status++;
        cin.rdbuf()->in_avail();
        cin >> lin;

        /* feature FEATURE1=VALUE1 FEATURE2=VALUE2 ...
        Beginning with version 2, the protocol includes the "feature" command,
        which lets your engine control certain optional protocol features.
        Feature settings are written as FEATURE=VALUE, where FEATURE is a name from
        the list below and VALUE is the value to be assigned. Features can take string,
        integer, or boolean values; the type of value is listed for each feature.
        String values are written in double quotes (for example, feature
        myname="Miracle Chess 0.9"), integers are written in decimal,
        and boolean values are written as 0 for false, 1 for true.
        Any number of features can be set in one feature command,
        or multiple feature commands can be given.
        Your engine should send one or more feature commands immediately
        after receiving the "protover" command, since xboard needs to know the
        values of some features before sending further commands to the engine.
        Because engines that predate protocol version 2 do not send "feature",
        xboard uses a timeout mechanism: when it first starts your engine, it sends
        "xboard" and "protover N", then listens for feature commands for two seconds
        before sending any other commands. To end this timeout and avoid the wait, set
        the feature "done=1" at the end of your last feature command. To increase the
        timeout, if needed, set the feature "done=0" before your first feature command
        and "done=1" at the end. If needed, it is okay for your engine to set done=0 soon
        as it starts, even before it receives the xboard and protover commands. This can
        be useful if your engine takes a long time to initialize itself. It should be
        harmless even if you are talking to a (version 1) user interface that does not
        understand the "feature" command, since such interfaces generally ignore commands
        from the engine that they do not understand.
        The feature command is designed to let the protocol change without breaking
        engines that were written for older protocol versions.
        When a new feature is added to the protocol, its default value is
        always chosen to be compatible with older versions of the protocol that
        did not have the feature. Any feature that your engine does not set in a
        "feature" command retains its default value, so as the protocol changes,
               you do not have to change your engine to keep up with it unless you want
        to take advantage of a new feature. Because some features are improvements
        to the protocol, while others are meant to cater to engines that do not
        implement all the protocol features, the recommended setting for a feature
        is not always the same as the default setting. The listing below gives
        both default and recommended settings for most features.
        You may want to code your engine so as to be able to work with multiple
        versions of the engine protocol. Protocol version 1 does not send the protover
        command and does not implement the feature command; if you send a feature
        command in protocol version 1, it will have no effect and there will be no response.
        In protocol version 2 or later, each feature F that you set generates the response
        "accepted F" if the feature is implemented, or "rejected F" if it is not.
        Thus an engine author can request any feature without having to keep track
        of which protocol version it was introduced in; you need only check whether
        the feature is accepted or rejected. This mechanism also makes it possible for
        a user interface author to implement a subset of a protocol version by rejecting
        some features that are defined in that version; however, you should realize that
        engine authors are likely to code for xboard and may not be prepared to have a
        feature that they depend on be rejected.
        Here are the features that are currently defined.
        ping (boolean, default 0, recommended 1)
        If ping=1, xboard may use the protocol's new "ping" command;
        if ping=0, xboard will not use the command.
        setboard (boolean, default 0, recommended 1)
        If setboard=1, xboard will use the protocol's new "setboard" command
        to set up positions; if setboard=0, it will use the older "edit" command.
        playother (boolean, default 0, recommended 1)
        If playother=1, xboard will use the protocol's new "playother" command
        when appropriate; if playother=0, it will not use the command.
        san (boolean, default 0)
        If san=1, xboard will send moves to the engine in standard algebraic notation (SAN);
        for example, Nf3. If san=0, xboard will send moves in coordinate notation;
        for example, g1f3. See MOVE in section 8 above for more details of both kinds of notation.
        usermove (boolean, default 0)
        If usermove=1, xboard will send moves to the engine with the command "usermove MOVE";
        if usermove=0, xboard will send just the move, with no command name.
        time (boolean, default 1, recommended 1)
        If time=1, xboard will send the "time" and "otim" commands to update the engine's clocks;
        if time=0, it will not.
        draw (boolean, default 1, recommended 1) If draw=1, xboard will send the "draw"
        command if the engine's opponent offers a draw; if draw=0, xboard will not inform
        the engine about draw offers. Note that if draw=1,
        you may receive a draw offer while you are on move; if this will cause you to
        move immediately, you should set draw=0.
        sigint (boolean, default 1)
        If sigint=1, xboard may send SIGINT (the interrupt signal) to the engine as section
        7 above; if sigint=0, it will not.
        sigterm (boolean, default 1)
        If sigterm=1, xboard may send SIGTERM (the termination signal) to the engine as
        section 7 above; if sigterm=0, it will not.
        reuse (boolean, default 1, recommended 1)
        If reuse=1, xboard may reuse your engine for multiple games.
        If reuse=0 (or if the user has set the -xreuse option on xboard's command line),
        xboard will kill the engine process after every game and start a fresh process for
        the next game.
        analyze (boolean, default 1, recommended 1)
        If analyze=0, xboard will not try to use the "analyze" command;
        it will pop up an error message if the user asks for analysis mode.
        If analyze=1, xboard will try to use the command if the user asks for analysis mode.
        myname (string, default determined from engine filename)
        This feature lets you set the name that xboard will use for your engine
        in window banners, in the PGN tags of saved game files, and when sending
        the "name" command to another engine.
        variants (string, see text below)
        This feature indicates which chess variants your engine accepts.
        It should be a comma-separated list of variant names. See the table under the
        "variant" command in section 8 above. If you do not set this feature, xboard will
        assume by default that your engine supports all variants. (However, the -zippyVariants
        command-line option still limits which variants will be accepted in Zippy mode.)
        It is recommended that you set this feature to the correct value for your engine
        (just "normal" in most cases) rather than leaving the default in place, so
        that the user will get an appropriate error message if he tries to play a
        variant that your engine does not support.
        colors (boolean, default 1, recommended 0)
        If colors=1, xboard uses the obsolete "white" and "black" commands in a
        stylized way that works with most older chess engines that require the commands.
        See the "Idioms" section below for details. If colors=0, xboard does not use
        the "white" and "black" commands at all.
        ics (boolean, default 0)
        If ics=1, xboard will use the protocol's new "ics" command to inform the engine
        of whether or not it is playing on a chess server; if ics=0, it will not.
        name (boolean, see text below)
        If name=1, xboard will use the protocol's "name" command to inform the engine
        of the opponent's name; if name=0, it will not. By default, name=1 if the engine
        is playing on a chess server; name=0 if not.
        pause (boolean, default 0)
        If pause=1, xboard may use the protocol's new "pause" command;
        if pause=0, xboard assumes that the engine does not support this command.
        done (integer, no default)
        If you set done=1 during the initial two-second timeout after xboard sends you
        the "xboard" command, the timeout will end and xboard will not look for any
        more feature commands before starting normal operation.
        If you set done=0, the initial timeout is increased to one hour;
             in this case, you must set done=1 before xboard will enter normal operation.
              */
        if (lin == "2") {
          cout << "feature"
               << " myname=\"SFC 1.0 BETA\"" //double quotes needed
               << " name=0"
               << " ics=0"
               << " setboard=1"
               << " playother=1"
               << " san=0"
               << " usermove=1"
               << " time=1"
               << " draw=1"
               << " sigint=0"
               << " sigterm=0"
               << " reuse=0"
               << " analyze=1"
               << " variants=\"normal\""
               << " colors=0"
               << " pause=0"
               << " ping=1"
               << " done=1" << endl;
        } else {
          cout << "features disabled due to OLD winboard protocol version!" << endl;
        };
        #ifdef DEBUG_VERSION
        cout << "command FEATURE proccessed in winboard session..." << endl;
        #endif

        ping_status--;
      } else
        if (lin == "new") {
          /*Reset the board to the standard chess starting position.
          Set White on move. Leave force mode and set the engine to play Black.
          Associate the engine's clock with Black and the opponent's clock with White.
          Reset clocks and time controls to the start of a new game. Stop clocks.
          Do not ponder on this move, even if pondering is on.
          Remove any search depth limit previously set by the sd command.*/

					#ifdef DEBUG_VERSION
          cout << "command NEW handled in winboard session..." << endl;
          #endif

					ping_status++;
          GamePVlen[GMI] = 0;
          GameVal[GMI] = EMPTY_VAL;

          for (ws_j = 0; ws_j < 64; ws_j++) {
            GamePV[GMI][ws_j] = 0; //[GMI][board]
          };

          GMI = 0;

          GameLen = 0;

          setGameMaterial("rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR");

          setGameCastleStatus("KQkq");

          setGameEnpassantStatus("-");

          setGameSideToMove("w");

          clearHash();

          setGameZkey(64);

          GameRetract[GMI] = 0;

          //B[64] = 14;
          ws_j = 0;

          while (ws_j < 64) {
            B[ws_j] = GameBoard[0][ws_j];
            ws_j++;
          };

          enpassant = GameEnpassant[GMI];

          castle_dat = Game_castle_data[GMI];

          Zkey = Game_Zkey_tab[GMI];

          retract = GameRetract[GMI];

          FORCE_MODE = false;

          ENGINE_PLAYS_WHITE = false;

          restoreTimeSettings = true;

          THINKING = false;

          SD_LIMIT = 255;

          ping_status--;
        } else
          if (lin == "nopost") {
            /* nopost
             Turn off thinking/pondering OUTPUT.
             */
            #ifdef DEBUG_VERSION
            cout << "command NOPOST handled in winboard session..." << endl;
            #endif
						ping_status++;
            SHOW_WB_THINKING_OUTPUT = false;
            ping_status--;
          } else
            if (lin == "post") {
              /* post
               Turn on thinking/pondering OUTPUT. See Thinking Output section.
               */
              #ifdef DEBUG_VERSION
              cout << "command POST handled in winboard session..." << endl;
              #endif
              ping_status++;
              SHOW_WB_THINKING_OUTPUT = true;
              ping_status--;
            } else
              if (lin == "force") {
                /* force
                  Set the engine to play neither color ("force mode"). Stop clocks.
                 The engine should check that moves received in force mode are legal
                 and made in the proper turn, but should not think, ponder, or make
								 moves of its own.
                 */

								#ifdef DEBUG_VERSION
                cout << "command FORCE handled in winboard session..." << endl;
                #endif

								ping_status++;
                FORCE_MODE = true; //clocks are stopped automatically if force mode
                THINKING = false;
                PONDERING = false;
                ping_status--;
              } else
                if (lin == "setboard") {
                  /* setboard FEN
                   The setboard command is the new way to set up positions,
                   beginning in protocol version 2. It is not used unless it has been selected
                   with the feature command. Here FEN is a position in Forsythe-Edwards Notation,
                   as defined in the PGN standard.
                   Illegal positions: Note that either setboard or edit can be used to send
                   an illegal position to the engine. The user can create any position with
                   xboard's Edit Position command (even, say, an empty board, or a board with
                   64 white kings and no black ones).
                   If your engine receives a position that it considers illegal,
                   I suggest that you send the response "tellusererror Illegal position",
                   and then respond to any attempted move with "Illegal move" until the next new,
                   edit, or setboard command.
                    */
                  ping_status++;
                  cin.rdbuf()->in_avail();
                  cin >> lin;

									#ifdef DEBUG_VERSION
                  cout << "command SETBOARD "<<lin<<" handled in winboard session..." << endl;
                  #endif

                  if (lin == "accepted") {
                  	;
                  } else {
                    THINKING = false;
                    FORCE_MODE = true;
                    GMI = 0;
                    GameLen = 0;
                    GamePVlen[GMI] = 0;
                    GameVal[GMI] = EMPTY_VAL;

                    for (ws_j = 0; ws_j < 64; ws_j++) {
                      GamePV[GMI][ws_j] = 0; //[GMI][board]
                    };

                    //what happens with next GameVal[GMI]??
										if (setGameMaterial(lin) == false) {
                      cout << "telluser Illegal position (material)." << endl;
                    };

                    cin.rdbuf()->in_avail();
                    cin >> lin;

                    if (setGameSideToMove(lin) == false) {
                      cout << "telluser Illegal position (side to move)." << endl;
                    };

                    cin.rdbuf()->in_avail();
                    cin >> lin;

                    if (setGameCastleStatus(lin) == false) {
                      cout << "telluser Illegal position (castle status)." << endl;
                    };

                    cin.rdbuf()->in_avail();
                    cin >> lin;

                    if (setGameEnpassantStatus(lin) == false) {
                      cout << "telluser Illegal position (enpassant status)." << endl;
                    };

                    clearHash();

                    setGameZkey(64);

                    GameRetract[GMI] = 0;

                    ws_j = 0;

                    while (ws_j < 64) {
                      B[ws_j] = GameBoard[0][ws_j];
                      ws_j++;
                    };

                    enpassant = GameEnpassant[GMI];

                    castle_dat = Game_castle_data[GMI];

                    Zkey = Game_Zkey_tab[GMI];

                    retract = GameRetract[GMI];

                    if (GameWtm[0] == false) {ENGINE_PLAYS_WHITE = false;}
                    else {ENGINE_PLAYS_WHITE = true;};

 										SD_LIMIT = 255;

 										getTimeSettings();

                  };

                  ping_status--;
                } else
                  if (lin == "analyze") {
                    /* analyze
                    Enter analyze mode.
                    xboard supports analyzing fresh games, edited positions, and games from files.
                    However, all of these look the same from the chess engine's perspective.
                    Basically, the engine just has to respond to the "analyze" command.
                    Beginning in protocol version 2, if your engine does not support analyze mode,
                    it should use the feature command to set analyze=0.
                    The older method of printing the error message "Error (unknown command): analyze"
                    in response to the "analyze" command will also work, however.
                    To enter analyze mode, xboard sends the command sequence "post", "analyze".
                    Analyze mode in your engine should be similar to force mode,
                    except that your engine thinks about what move it would make next if it were
                          on move.

                    Your engine should accept the following commands while in analyze mode:
                    - any legal move, as in force mode
                    - undo   Back up one move and analyze previous position.
                    - new   Reset position to start of game but stay in analyze mode.
                    - setboard if you have set feature setboard=1; otherwise edit.
                    - Exiting edit mode returns to analyze mode.
                    - exit   Leave analyze mode.
                    .   Send a search status update (optional); see below.
                    bk   Show book moves from this position, if any; see above.
                    hint   Show the predicted move from this position, if any; see above.
                    If the user selects "Periodic Updates", xboard will send the string ".\n"
                    to the chess engine periodically during analyze mode, unless the last PV
                          received began with a '(' character.
                    The chess engine should respond to ".\n" with a line like this:
                    stat01: time nodes ply mvleft mvtot mvname
                    Where: time Elapsed search time in centiseconds (ie: 567 = 5.67 seconds).
                        - nodes Nodes searched so far.
                        - ply Search depth so far.
                          - mvleft Number of moves left to consider at this depth.
                          - mvtot Total number of moves to consider.
                          - mvname Move currently being considered (SAN or coordinate notation). Optional;
                     added in protocol version 2.

                          Examples:

                          stat01: 1234 30000 7 5 30
                        stat01: 1234 30000 7 5 30 Nf3
                    Meaning: After 12.34 seconds, I've searched 7 ply/30000 nodes,
                    there are a total of 30 legal moves, and I have 5 more moves to search before
                    going to depth 8. In the second example, of the 30 legal moves,
                    the one I am currently searching is Nf3.
                    Implementation of the "." command is optional.
                    If the engine does not respond to the "." command with a "stat01..." line,
                    xboard will stop sending "." commands. If the engine does not implement this command,
                    the analysis window will use a shortened format to display the engine info.
                    To give the user some extra information, the chess engine can output the strings
                        "++\n" and "--\n", to indicate that the current search is failing high or low,
                    respectively. You don't have to send anything else to say
                    "Okay, I'm not failing high/low anymore." xboard will figure this out itself.
                    */

										#ifdef DEBUG_VERSION
                    cout << "command ANALYZE handled in winboard session..." << endl;
                    #endif

										ping_status++;
                    cin.rdbuf()->in_avail();
                    cin >> lin;

                    if (lin == "accepted") {
                      ;
                    } else {
                      if (GameWtm[GMI] == true) {
                        ANALYZE_MODE = true; //clocks are stopped automatically if analyze mode
                        //printExtras("in analyze before whites move:");
                        THINKING = true;
                        ENGINE_PLAYS_WHITE = true;
                        gl_move = IncrementalSearch();//find best move from PV
                      } else {
                        ANALYZE_MODE = true;
                        //printExtras("in analyze before blacks move");
                        THINKING = true;
                        ENGINE_PLAYS_WHITE = false;
                        gl_move = IncrementalSearch();
                      };
                    };

                    ping_status--;
                  } else
                    if (lin == "variant") {
                      ;
                      /*variant VARNAME
                       If the game is not standard chess, but a variant,
                       this command is sent after "new"
                       and before the first move or "edit" command.
                       Currently defined variant names are:
                       wildcastle Shuffle chess where king can castle from d file
                       nocastle Shuffle chess with no castling at all
                       fischerandom Fischer Random (not supported yet)
                       bughouse Bughouse, ICC/FICS rules
                       crazyhouse Crazyhouse, ICC/FICS rules
                       losers Win by losing all pieces or getting mated (ICC)
                       suicide Win by losing all pieces including king, or by having fewer pieces when one player has no legal moves (FICS)
                       giveaway  Win by losing all pieces including king, or by having no legal moves (ICC)
                       twokings Weird ICC wild 9
                       kriegspiel Kriegspiel (engines not supported)
                       atomic Atomic
                       3check Win by giving check 3 times
                       unknown Unknown variant (not supported)*/
                      #ifdef DEBUG_VERSION
                      cout << "command VARIANT handled in winboard session..." << endl;
                      #endif

											ping_status++;
                      cin.rdbuf()->in_avail();
                      cin >> lin;
                      ping_status--;
                    } else
                      if (lin == "level") {
                        /* level moves time inc
                              Set time controls. See the Time Control section below.
                        Time control:
                        Xboard supports three styles of time control:
                            conventional chess clocks,
                            the ICS-style incremental clock,
                            and an exact number of seconds per move.
                        In conventional clock mode, every time control period is the same.
                        That is, if the time control is 40 moves in 5 minutes,
                        then after each side has made 40 moves, they each get an
                        additional 5 minutes, and so on, ad infinitum.
                        The command to set a conventional time control looks like this:
                          level 40 5 0
                          level 40 0:30 0
                        The 40 means that there are 40 moves per time control.
                        The 5 means there are 5 minutes in the control.
                        In the second example, the 0:30 means there are 30 seconds.
                        The final 0 means that we are in conventional clock mode.
                        The command to set an incremental time control looks like this:
                          level 0 2 12
                        Here the 0 means "play the whole game in this time control period",
                        the 2 means "base=2 minutes", and the 12 means "inc=12 seconds".
                        As in conventional clock mode, the second argument to level can be in minutes and seconds.
                        At the start of the game, each player's clock is set to base minutes.
                        Immediately after a player makes a move, inc seconds are added to his clock.
                        A player's clock counts down while it is his turn.
                        Your flag can be called whenever your clock is zero or negative.
                        (Your clock can go negative and then become positive again because of the increment.)
                        A special rule on some ICS implementations:
                        if you ask for a game with base=0, the clocks really start at 10 seconds instead of 0.
                        xboard itself does not know about this rule, so it passes the 0 on to the engine instead
                        of changing it to 0:10.
                        ICS also has time odds games.
                        With time odds, each player has his own (base, inc) pair,
                        but otherwise things work the same as in normal games.
                        The Zippy xboard accepts time odds games but ignores the fact that the opponent's
                        parameters are different; this is perhaps not quite the right thing to do,
                        but gnuchess doesn't understand time odds. Time odds games are always unrated.
                        The command to set an exact number of seconds per move looks like this:
                          st 30
                        This means that each move must be made in at most 30 seconds.
                        Time not used on one move does not accumulate for use on later moves.
                        */

												#ifdef DEBUG_VERSION
                        cout << "command LEVEL handled in winboard session..." << endl;
                        #endif

												ping_status++;
                        cin.rdbuf()->in_avail();
                        cin >> lin;
                        WhiMovesLeft = 0;
                        unsigned int decimal = 0;

                        //1) translate moves left string into decimal number
                        //WhiMovesLeft, BlaMovesLeft
                        while (decimal < lin.length()) {
                          switch (lin.at(decimal)) {
                            case '0'
                                : {
                                WhiMovesLeft *= 10;
                              };

                              break;
                            case '1'
                                : {
                                WhiMovesLeft *= 10;
                                WhiMovesLeft += 1;
                              };

                              break;
                            case '2'
                                : {
                                WhiMovesLeft *= 10;
                                WhiMovesLeft += 2;
                              };

                              break;
                            case '3'
                                : {
                                WhiMovesLeft *= 10;
                                WhiMovesLeft += 3;
                              };

                              break;
                            case '4'
                                : {
                                WhiMovesLeft *= 10;
                                WhiMovesLeft += 4;
                              };

                              break;
                            case '5'
                                : {
                                WhiMovesLeft *= 10;
                                WhiMovesLeft += 5;
                              };

                              break;
                            case '6'
                                : {
                                WhiMovesLeft *= 10;
                                WhiMovesLeft += 6;
                              };

                              break;
                            case '7'
                                : {
                                WhiMovesLeft *= 10;
                                WhiMovesLeft += 7;
                              };

                              break;
                            case '8'
                                : {
                                WhiMovesLeft *= 10;
                                WhiMovesLeft += 8;
                              };

                              break;
                            case '9'
                                : {
                                WhiMovesLeft *= 10;
                                WhiMovesLeft += 9;
                              };

                              break;
                          };

                          decimal++;
                        };

                        BlaMovesLeft = WhiMovesLeft;

                        //2) translate time left string into decimal number
                        //WhiTimeLeft, BlaTimeLeft
                        cin.rdbuf()->in_avail();

                        cin >> lin;

                        //time in centiseconds
                        WhiTimeLeft = 0;

                        decimal = 0;

                        bool converted_from_minutes = false;

                        double timepart = 0;

                        //handle i.e. "5" or "0:30"
                        while (decimal < lin.length()) {
                          switch (lin.at(decimal)) {
                            case '0'
                                : {
                                WhiTimeLeft *= 10;
                              };

                              break;
                            case '1'
                                : {
                                WhiTimeLeft *= 10;
                                WhiTimeLeft += 1;
                              };

                              break;
                            case '2'
                                : {
                                WhiTimeLeft *= 10;
                                WhiTimeLeft += 2;
                              };

                              break;
                            case '3'
                                : {
                                WhiTimeLeft *= 10;
                                WhiTimeLeft += 3;
                              };

                              break;
                            case '4'
                                : {
                                WhiTimeLeft *= 10;
                                WhiTimeLeft += 4;
                              };

                              break;
                            case '5'
                                : {
                                WhiTimeLeft *= 10;
                                WhiTimeLeft += 5;
                              };

                              break;
                            case '6'
                                : {
                                WhiTimeLeft *= 10;
                                WhiTimeLeft += 6;
                              };

                              break;
                            case '7'
                                : {
                                WhiTimeLeft *= 10;
                                WhiTimeLeft += 7;
                              };

                              break;
                            case '8'
                                : {
                                WhiTimeLeft *= 10;
                                WhiTimeLeft += 8;
                              };

                              break;
                            case '9'
                                : {
                                WhiTimeLeft *= 10;
                                WhiTimeLeft += 9;
                              };

                              break;
                            case ':'
                                : {
                                timepart = WhiTimeLeft * 60;
                                WhiTimeLeft = 0;
                                converted_from_minutes = true;
                              };

                              break;
                          };

                          decimal++;
                        };

                        WhiTimeLeft += timepart;

                        //convert into minutes if not done by ":" mark
                        if (converted_from_minutes == false) {
                          WhiTimeLeft *= 60;
                        };

                        WhiTimeLeft *= 100; //convert into centiseconds

                        BlaTimeLeft = WhiTimeLeft;

                        //3) translate time increment string into decimal number
                        //TimeIncrement
                        cin.rdbuf()->in_avail();

                        cin >> lin;

                        TimeIncrement = 0;

                        decimal = 0;

                        //translate string into decimal number
                        while (decimal < (lin.length())) {
                          switch (lin.at(decimal)) {
                            case '0'
                                : {
                                TimeIncrement *= 10;
                              };

                              break;
                            case '1'
                                : {
                                TimeIncrement *= 10;
                                TimeIncrement += 1;
                              };

                              break;
                            case '2'
                                : {
                                TimeIncrement *= 10;
                                TimeIncrement += 2;
                              };

                              break;
                            case '3'
                                : {
                                TimeIncrement *= 10;
                                TimeIncrement += 3;
                              };

                              break;
                            case '4'
                                : {
                                TimeIncrement *= 10;
                                TimeIncrement += 4;
                              };

                              break;
                            case '5'
                                : {
                                TimeIncrement *= 10;
                                TimeIncrement += 5;
                              };

                              break;
                            case '6'
                                : {
                                TimeIncrement *= 10;
                                TimeIncrement += 6;
                              };

                              break;
                            case '7'
                                : {
                                TimeIncrement *= 10;
                                TimeIncrement += 7;
                              };

                              break;
                            case '8'
                                : {
                                TimeIncrement *= 10;
                                TimeIncrement += 8;
                              };

                              break;
                            case '9'
                                : {
                                TimeIncrement *= 10;
                                TimeIncrement += 9;
                              };

                              break;
                          };

                          decimal++;
                        };

                        //in centiseconds
                        TimeIncrement *= 100;

                        //set clock category
                        setClockExact(false);

                        //set clock base
                        setClockBase(BlaMovesLeft);

                        //save time settings and set restore flag
                        saveInitialTimeSettings();

                        restoreTimeSettings = true;

                        #ifdef DEBUG_VERSION
												cout << "Time set by ''level'' command: WhiMovesLeft:" << WhiMovesLeft
                             << " BlaMovesLeft:" << BlaMovesLeft
                             << " WhiTimeLeft (cs):" << WhiTimeLeft
                             << " BlaTimeLeft (cs):" << BlaTimeLeft
                             << " TimeIncrement (cs):" << TimeIncrement << endl;
                        #endif

                        ping_status--;
                      } else
                        if (lin == "quit") {
                          /* quit
                            The chess engine should immediately exit. This command is used when xboard
                           is itself exiting, and also between games if the -xreuse command line option
                           is given (or -xreuse2 for the second engine). See also Signals above.
                               */
                          #ifdef DEBUG_VERSION
                          cout << "command QUIT handled in winboard session, terminating..." << endl;
                          #endif

                          exit(0);

                        } else
                          if (lin == "random") {
                            #ifdef DEBUG_VERSION
                            cout << "command RANDOM handled in winboard session..." << endl;
                            #endif
														ping_status++;
                            /* random
                             This command is specific to GNU Chess 4.
                             You can either ignore it completely (that is, treat it as a no-op)
                             or implement it as GNU Chess does.
                             The command toggles "random" mode (that is, it sets random = !random).
                             In random mode, the engine adds a small random value to its evaluation function
                             to vary its play. The "new" command sets random mode off.
                             */
                            //cout<<"Error (unknown command):"<<lin<<endl;
                            ping_status--;
                          } else
                            if (lin == "go") {
                              /* go
                                    Leave force mode and set the engine to play the color that is on move.
                              Associate the engine's clock with the color that is on move,
                              the opponent's clock with the color that is not on move.
                              Start the engine's clock.
                              Start thinking and eventually make a move.
                                    */

                              #ifdef DEBUG_VERSION
                              cout << "command GO handled in winboard session..." << endl;
                              #endif

															ping_status++;

                              if (GameWtm[GMI] == true) {
                                //////////////////////////////////
                                //printExtras("in go before whites move:");
                                //////////////////////////////////
                                FORCE_MODE = false;
                                THINKING = true;
                                ENGINE_PLAYS_WHITE = true;
                                //manage time
                                getTimeSettings(); //should work after new command
                                setMoveStartTime();
                                setWhiTimePerMove();
                                // set properly CHILD() arguments before move
                                gl_move = IncrementalSearch();//find best move from PV
                                ws_r = 0;

                                while (ws_r < 64) {
                                  B[ws_r] = GameBoard[GMI][ws_r];
                                  ws_r++;
                                };

                                WpaNum = 0;

                                BpaNum = 0;

                                WroNum = 0;

                                BroNum = 0;

                                WbiNum = 0;

                                BbiNum = 0;

                                WknNum = 0;

                                BknNum = 0;

                                WquNum = 0;

                                BquNum = 0;

                                MaterialScore = 0;

                                for (ws_r = 0; ws_r < 64; ws_r++) {
                                  switch (B[ws_r]) {
                                    case
                                        sWpa: {
                                        WpaNum++;
                                        MaterialScore += evPawnMaterialValue;
                                      };

                                      break;
                                    case
                                        sBpa: {
                                        BpaNum++;
                                        MaterialScore -= evPawnMaterialValue;
                                      };

                                      break;
                                    case
                                        sWro: {
                                        WroNum++;
                                        MaterialScore += evRookMaterialValue;
                                      };

                                      break;
                                    case
                                        sBro: {
                                        BroNum++;
                                        MaterialScore -= evRookMaterialValue;
                                      };

                                      break;
                                    case
                                        sWbi: {
                                        WbiNum++;
                                        MaterialScore += evBishopMaterialValue;
                                      };

                                      break;
                                    case
                                        sBbi: {
                                        BbiNum++;
                                        MaterialScore -= evBishopMaterialValue;
                                      };

                                      break;
                                    case
                                        sWkn: {
                                        WknNum++;
                                        MaterialScore += evKnightMaterialValue;
                                      };

                                      break;
                                    case
                                        sBkn: {
                                        BknNum++;
                                        MaterialScore -= evKnightMaterialValue;
                                      };

                                      break;
                                    case
                                        sWki: {
                                        WkiPos = ws_r;
                                      };

                                      break;
                                    case
                                        sBki: {
                                        BkiPos = ws_r;
                                      };

                                      break;
                                    case
                                        sWqu: {
                                        WquNum++;
                                        MaterialScore += evQueenMaterialValue;
                                      };

                                      break;
                                    case
                                        sBqu: {
                                        BquNum++;
                                        MaterialScore -= evQueenMaterialValue;
                                      };

                                      break;
                                  };
                                };

                                //////////////////////////////////////
                                //set global king area boards
                                for (ev_v = 0; ev_v < 64; ev_v++) {
                                  WkiArea[ev_v] = 0;
                                  BkiArea[ev_v] = 0;
                                };

                                WkiArea[WkiPos] = 1;

                                BkiArea[BkiPos] = 1;

                                for (ev_v = 0; ev_v < 8; ev_v++) {
                                  WkiArea[Vec[ev_v][WkiPos][0]] = 1;
                                  BkiArea[Vec[ev_v][BkiPos][0]] = 1;
                                };

                                //////////////////////////////////////
                                enpassant = GameEnpassant[GMI];

                                castle_dat = Game_castle_data[GMI];

                                Zkey = Game_Zkey_tab[GMI];

                                retract = GameRetract[GMI];

                                CHILD(1); //uses gl_move

                                GMI++;

                                GameLen++;

                                GameEnpassant[GMI] = enpassant;

                                Game_castle_data[GMI] = castle_dat;

                                Game_Zkey_tab[GMI] = Zkey;

                                GameRetract[GMI] = retract;

                                //handle 3-fold draw
                                if (retract < 99) {
                                  ws_r = 0;
                                  ws_j = GMI - 1;

                                  while (ws_j > 0) {
                                    if (GameRetract[ws_j] == 0) {
                                      break;
                                    };

                                    if (Zkey == Game_Zkey_tab[ws_j]) {
                                      ws_r++;

                                      if (ws_r > 1) {
                                        GameVal[GMI] = DRAW_SCORE;
                                        cout << "telluser 3-fold repetition draw!" << endl;
                                        break;
                                      };
                                    };

                                    ws_j--;
                                  };
                                }
                                //handle 50-moves rule
                                else {
                                  GameVal[GMI] = DRAW_SCORE;
                                  cout << "telluser 50-moves rule draw!" << endl;
                                };

                                GameMoves[GMI] = gl_move; //set game move

                                ws_r = 0;

                                while (ws_r < 64) {
                                  GameBoard[GMI][ws_r] = B[ws_r];
                                  ws_r++;
                                }; //set game board

                                //////////////////////////////////
                                //printExtras("in go after whites move:");
                                //////////////////////////////////
                                cout << "move " << printMove(GameMoves[GMI]) << endl;    //send command to winboard

                                //update time variables
                                updateTimeVariables();

                              } else {
                                //////////////////////////////////
                                //printExtras("in go before blacks move");
                                //////////////////////////////////
                                FORCE_MODE = false;
                                THINKING = true;
                                ENGINE_PLAYS_WHITE = false;
                                //manage time
                                getTimeSettings(); //should work after new command
                                setMoveStartTime();
                                setBlaTimePerMove();
                                gl_move = IncrementalSearch();//make best move from PV
                                // set properly CHILD() arguments
                                ws_r = 0;

                                while (ws_r < 64) {
                                  B[ws_r] = GameBoard[GMI][ws_r];
                                  ws_r++;
                                };

                                WpaNum = 0;

                                BpaNum = 0;

                                WroNum = 0;

                                BroNum = 0;

                                WbiNum = 0;

                                BbiNum = 0;

                                WknNum = 0;

                                BknNum = 0;

                                WquNum = 0;

                                BquNum = 0;

                                MaterialScore = 0;

                                for (ws_r = 0; ws_r < 64; ws_r++) {
                                  switch (B[ws_r]) {
                                    case
                                        sWpa: {
                                        WpaNum++;
                                        MaterialScore += evPawnMaterialValue;
                                      };

                                      break;
                                    case
                                        sBpa: {
                                        BpaNum++;
                                        MaterialScore -= evPawnMaterialValue;
                                      };

                                      break;
                                    case
                                        sWro: {
                                        WroNum++;
                                        MaterialScore += evRookMaterialValue;
                                      };

                                      break;
                                    case
                                        sBro: {
                                        BroNum++;
                                        MaterialScore -= evRookMaterialValue;
                                      };

                                      break;
                                    case
                                        sWbi: {
                                        WbiNum++;
                                        MaterialScore += evBishopMaterialValue;
                                      };

                                      break;
                                    case
                                        sBbi: {
                                        BbiNum++;
                                        MaterialScore -= evBishopMaterialValue;
                                      };

                                      break;
                                    case
                                        sWkn: {
                                        WknNum++;
                                        MaterialScore += evKnightMaterialValue;
                                      };

                                      break;
                                    case
                                        sBkn: {
                                        BknNum++;
                                        MaterialScore -= evKnightMaterialValue;
                                      };

                                      break;
                                    case
                                        sWki: {
                                        WkiPos = ws_r;
                                      };

                                      break;
                                    case
                                        sBki: {
                                        BkiPos = ws_r;
                                      };

                                      break;
                                    case
                                        sWqu: {
                                        WquNum++;
                                        MaterialScore += evQueenMaterialValue;
                                      };

                                      break;
                                    case
                                        sBqu: {
                                        BquNum++;
                                        MaterialScore -= evQueenMaterialValue;
                                      };

                                      break;
                                  };
                                };

                                //////////////////////////////////////
                                //set global king area boards
                                for (ev_v = 0; ev_v < 64; ev_v++) {
                                  WkiArea[ev_v] = 0;
                                  BkiArea[ev_v] = 0;
                                };

                                WkiArea[WkiPos] = 1;

                                BkiArea[BkiPos] = 1;

                                for (ev_v = 0; ev_v < 8; ev_v++) {
                                  WkiArea[Vec[ev_v][WkiPos][0]] = 1;
                                  BkiArea[Vec[ev_v][BkiPos][0]] = 1;
                                };

                                //////////////////////////////////////
                                enpassant = GameEnpassant[GMI];

                                castle_dat = Game_castle_data[GMI];

                                Zkey = Game_Zkey_tab[GMI];

                                retract = GameRetract[GMI];

                                CHILD(1);

                                GMI++;

                                GameLen++;

                                GameEnpassant[GMI] = enpassant;

                                Game_castle_data[GMI] = castle_dat;

                                Game_Zkey_tab[GMI] = Zkey;

                                GameRetract[GMI] = retract;

                                //handle 3-fold draw
                                if (retract < 99) {
                                  ws_r = 0;
                                  ws_j = GMI - 1;

                                  while (ws_j > 0) {
                                    if (GameRetract[ws_j] == 0) {
                                      break;
                                    };

                                    if (Zkey == Game_Zkey_tab[ws_j]) {
                                      ws_r++;

                                      if (ws_r > 1) {
                                        GameVal[GMI] = DRAW_SCORE;
                                        cout << "telluser 3-fold repetition draw!" << endl;
                                        break;
                                      };
                                    };

                                    ws_j--;
                                  };
                                }
                                //handle 50-moves rule
                                else {
                                  GameVal[GMI] = DRAW_SCORE;
                                  cout << "telluser 50-moves rule draw!" << endl;
                                };

                                GameMoves[GMI] = gl_move;

                                ws_r = 0;

                                while (ws_r < 64) {
                                  GameBoard[GMI][ws_r] = B[ws_r];
                                  ws_r++;
                                };

                                ////////////////
                                //printExtras("in go after blacks move:");
                                ///////////////
                                cout << "move " << printMove(GameMoves[GMI]) << endl;

                                //update time variables
                                updateTimeVariables();
                              };

                              ping_status--;
                            } else
                              if (lin == "playother") {
                                /* playother
                                This command is new in protocol version 2.
                                It is not sent unless you enable it with the feature command.
                                Leave force mode and set the engine to play the color that is
                                      not on move.
                                Associate the opponent's clock with the color that is on move,
                                the engine's clock with the color that is not on move.
                                Start the opponent's clock.
                                If pondering is enabled, the engine should begin pondering.
                                If the engine later receives a move, it should start thinking
                                      and eventually reply.
                                */

                                #ifdef DEBUG_VERSION
                                cout << "command PLAYOTHER handled in winboard session..." << endl;
                                #endif

                                ping_status++;
                                cin.rdbuf()->in_avail();
                                cin >> lin;

                                if (lin == "accepted") {
																	;
                                } else {
                                  //exchange White and Black times left
                                  double tmpWhiTimeLeft = WhiTimeLeft;
                                  double tmpBlaTimeLeft = BlaTimeLeft;
                                  BlaTimeLeft = tmpWhiTimeLeft;
                                  WhiTimeLeft = tmpBlaTimeLeft;
                                  //exchange White and Black moves left
                                  double tmpWhiMovesLeft = WhiMovesLeft;
                                  double tmpBlaMovesLeft = BlaMovesLeft;
                                  BlaMovesLeft = tmpWhiMovesLeft;
                                  WhiMovesLeft = tmpBlaMovesLeft;
                                  /*
                                  //time management

                                  static int WhiTimePerMove = 0; //time devoted to single white move
                                  static int BlaTimePerMove = 0; //time devoted to single black move
                                  static int ClockBase = 0; //if base is zero- time is based on whole game, else: based on number of moves
                                  static bool ClockExact = false; //exact time per move - st command
                                  static bool restoreTimeSettings = false; //setup true when new or setboard command, false if otim or time comand
                                  static int storedTimeIncrement = 0;
                                  static int storedWhiTimeLeft = 0;
                                  static int storedWhiMovesLeft = 0;
                                  static int storedBlaTimeLeft = 0;
                                  static int storedBlaMovesLeft = 0;
                                  static int storedClockBase = 0;
                                  */
                                  //unfreeze
                                  FORCE_MODE = false;
                                  THINKING = true;

                                  //reassociate engine
                                  if (ENGINE_PLAYS_WHITE == false) {
                                    ENGINE_PLAYS_WHITE = true; //opponent plays black
                                    getTimeSettings();
                                    setMoveStartTime();
                                    setWhiTimePerMove();
                                  } else {
                                    ENGINE_PLAYS_WHITE = false;//opponent plays white
                                    getTimeSettings();
                                    setMoveStartTime();
                                    setBlaTimePerMove();
                                  };
                                };

                                ping_status--;
                              } else
                                if (lin == "white") {
                                	#ifdef DEBUG_VERSION
                                  cout << "command WHITE handled in winboard session..." << endl;
                                  #endif
                                  ping_status++;
                                  /* white
                                   This command is obsolete as of protocol version 2,
                                   but is still sent in some situations to accommodate
                                   older engines unless you disable it with the feature command.
                                   Set White on move. Set the engine to play Black. Stop clocks.
                                        */
                                  //cout<<"Error (unknown command):"<<lin<<endl;
                                  ping_status--;
                                } else
                                  if (lin == "black") {
                                  	#ifdef DEBUG_VERSION
                                    cout << "command BLACK handled in winboard session..." << endl;
                                    #endif
                                    ping_status++;
                                    /* black
                                          This command is obsolete as of protocol version 2,
                                     but is still sent in some situations to accommodate
                                     older engines unless you disable it with the feature command.
                                     Set Black on move. Set the engine to play White. Stop clocks.
                                     */
                                    //cout<<"Error (unknown command):"<<lin<<endl;
                                    ping_status--;
                                  } else
                                    if (lin == "st") {
                                      /* st TIME
                                      Set time controls. See the Time Control section below.
                                       The commands "level" and "st" are not used together.
                                       */
                                      #ifdef DEBUG_VERSION
                                      cout << "command ST handled in winboard session..." << endl;
                                      #endif
                                      ping_status++;
                                      cin.rdbuf()->in_avail();
                                      cin >> lin;
                                      // handle tc time info/////////////////////////////////////////
                                      WhiTimeLeft = 0;
                                      unsigned int decimal = 0;
                                      double timepart = 0;

                                      //handle i.e. "5" or "0:30"
                                      //translate string into decimal number
                                      while (decimal < lin.length()) {
                                        switch (lin.at(decimal)) {
                                          case '0'
                                              : {
                                              WhiTimeLeft *= 10;
                                            };

                                            break;
                                          case '1'
                                              : {
                                              WhiTimeLeft *= 10;
                                              WhiTimeLeft += 1;
                                            };

                                            break;
                                          case '2'
                                              : {
                                              WhiTimeLeft *= 10;
                                              WhiTimeLeft += 2;
                                            };

                                            break;
                                          case '3'
                                              : {
                                              WhiTimeLeft *= 10;
                                              WhiTimeLeft += 3;
                                            };

                                            break;
                                          case '4'
                                              : {
                                              WhiTimeLeft *= 10;
                                              WhiTimeLeft += 4;
                                            };

                                            break;
                                          case '5'
                                              : {
                                              WhiTimeLeft *= 10;
                                              WhiTimeLeft += 5;
                                            };

                                            break;
                                          case '6'
                                              : {
                                              WhiTimeLeft *= 10;
                                              WhiTimeLeft += 6;
                                            };

                                            break;
                                          case '7'
                                              : {
                                              WhiTimeLeft *= 10;
                                              WhiTimeLeft += 7;
                                            };

                                            break;
                                          case '8'
                                              : {
                                              WhiTimeLeft *= 10;
                                              WhiTimeLeft += 8;
                                            };

                                            break;
                                          case '9'
                                              : {
                                              WhiTimeLeft *= 10;
                                              WhiTimeLeft += 9;
                                            };

                                            break;
                                          case ':'
                                              : {
                                              timepart = WhiTimeLeft * 60;
                                              WhiTimeLeft = 0;
                                            };

                                            break;
                                        };

                                        decimal++;
                                      };

                                      WhiTimeLeft += timepart;

                                      //time in centiseconds
                                      WhiTimeLeft *= 100;

                                      BlaTimeLeft = WhiTimeLeft;

                                      //set time control per one move
                                      WhiMovesLeft = 1;

                                      BlaMovesLeft = 1;

                                      //set time increment
                                      TimeIncrement = 0;

                                      //set clock base (one move)
                                      setClockBase(1);

                                      //set clock work - exact time per move
                                      setClockExact(true);

                                      ping_status--;
                                    } else
                                      if (lin == "sd") {
                                        /* sd DEPTH
                                        The engine should limit its thinking to DEPTH ply.
                                        */
                                        #ifdef DEBUG_VERSION
                      									cout << "command SD handled in winboard session..." << endl;
                      									#endif
                                        ping_status++;
                                        cin.rdbuf()->in_avail();
                                        cin >> lin;
                                        #ifdef DEBUG_VERSION
                                        log("wb: sd " + lin);
                                        #endif

                                        //translate string into decimal number
                                        unsigned int decimal = 0;

                                        while (decimal < lin.length()) {
                                          switch (lin.at(decimal)) {
                                            case '0'
                                                : {
                                                SD_LIMIT *= 10;
                                              };

                                              break;
                                            case '1'
                                                : {
                                                SD_LIMIT *= 10;
                                                SD_LIMIT += 1;
                                              };

                                              break;
                                            case '2'
                                                : {
                                                SD_LIMIT *= 10;
                                                SD_LIMIT += 2;
                                              };

                                              break;
                                            case '3'
                                                : {
                                                SD_LIMIT *= 10;
                                                SD_LIMIT += 3;
                                              };

                                              break;
                                            case '4'
                                                : {
                                                SD_LIMIT *= 10;
                                                SD_LIMIT += 4;
                                              };

                                              break;
                                            case '5'
                                                : {
                                                SD_LIMIT *= 10;
                                                SD_LIMIT += 5;
                                              };

                                              break;
                                            case '6'
                                                : {
                                                SD_LIMIT *= 10;
                                                SD_LIMIT += 6;
                                              };

                                              break;
                                            case '7'
                                                : {
                                                SD_LIMIT *= 10;
                                                SD_LIMIT += 7;
                                              };

                                              break;
                                            case '8'
                                                : {
                                                SD_LIMIT *= 10;
                                                SD_LIMIT += 8;
                                              };

                                              break;
                                            case '9'
                                                : {
                                                SD_LIMIT *= 10;
                                                SD_LIMIT += 9;
                                              };

                                              break;
                                          };

                                          decimal++;
                                        };

                                        //cout<<"telluser SD_LIMIT set to "<<SD_LIMIT<<endl;
                                        ping_status--;
                                      } else
                                        if (lin == "time") {
                                          /* time N
                                                Set a clock that always belongs to the engine.
                                          N is a number in centiseconds (units of 1/100 second).
                                          Even if the engine changes to playing the opposite color,
                                          this clock remains with the engine.
                                          */
                                          #ifdef DEBUG_VERSION
                      										cout << "command TIME handled in winboard session..." << endl;
                      										#endif

                                          ping_status++;
                                          cin.rdbuf()->in_avail();
                                          cin >> lin;

                                          if (lin == "accepted") {
                                          	;
                                          } else {
                                            //translate string into decimal number
                                            if (ENGINE_PLAYS_WHITE == true) {
                                              WhiTimeLeft = 0;
                                              unsigned int decimal = 0;

                                              while (decimal < lin.length()) {
                                                switch (lin.at(decimal)) {
                                                  case '0'
                                                      : {
                                                      WhiTimeLeft *= 10;
                                                    };

                                                    break;
                                                  case '1'
                                                      : {
                                                      WhiTimeLeft *= 10;
                                                      WhiTimeLeft += 1;
                                                    };

                                                    break;
                                                  case '2'
                                                      : {
                                                      WhiTimeLeft *= 10;
                                                      WhiTimeLeft += 2;
                                                    };

                                                    break;
                                                  case '3'
                                                      : {
                                                      WhiTimeLeft *= 10;
                                                      WhiTimeLeft += 3;
                                                    };

                                                    break;
                                                  case '4'
                                                      : {
                                                      WhiTimeLeft *= 10;
                                                      WhiTimeLeft += 4;
                                                    };

                                                    break;
                                                  case '5'
                                                      : {
                                                      WhiTimeLeft *= 10;
                                                      WhiTimeLeft += 5;
                                                    };

                                                    break;
                                                  case '6'
                                                      : {
                                                      WhiTimeLeft *= 10;
                                                      WhiTimeLeft += 6;
                                                    };

                                                    break;
                                                  case '7'
                                                      : {
                                                      WhiTimeLeft *= 10;
                                                      WhiTimeLeft += 7;
                                                    };

                                                    break;
                                                  case '8'
                                                      : {
                                                      WhiTimeLeft *= 10;
                                                      WhiTimeLeft += 8;
                                                    };

                                                    break;
                                                  case '9'
                                                      : {
                                                      WhiTimeLeft *= 10;
                                                      WhiTimeLeft += 9;
                                                    };

                                                    break;
                                                };

                                                decimal++;
                                              };
                                            } else {
                                              BlaTimeLeft = 0;
                                              unsigned int decimal = 0;

                                              while (decimal < lin.length()) {
                                                switch (lin.at(decimal)) {
                                                  case '0'
                                                      : {
                                                      BlaTimeLeft *= 10;
                                                    };

                                                    break;
                                                  case '1'
                                                      : {
                                                      BlaTimeLeft *= 10;
                                                      BlaTimeLeft += 1;
                                                    };

                                                    break;
                                                  case '2'
                                                      : {
                                                      BlaTimeLeft *= 10;
                                                      BlaTimeLeft += 2;
                                                    };

                                                    break;
                                                  case '3'
                                                      : {
                                                      BlaTimeLeft *= 10;
                                                      BlaTimeLeft += 3;
                                                    };

                                                    break;
                                                  case '4'
                                                      : {
                                                      BlaTimeLeft *= 10;
                                                      BlaTimeLeft += 4;
                                                    };

                                                    break;
                                                  case '5'
                                                      : {
                                                      BlaTimeLeft *= 10;
                                                      BlaTimeLeft += 5;
                                                    };

                                                    break;
                                                  case '6'
                                                      : {
                                                      BlaTimeLeft *= 10;
                                                      BlaTimeLeft += 6;
                                                    };

                                                    break;
                                                  case '7'
                                                      : {
                                                      BlaTimeLeft *= 10;
                                                      BlaTimeLeft += 7;
                                                    };

                                                    break;
                                                  case '8'
                                                      : {
                                                      BlaTimeLeft *= 10;
                                                      BlaTimeLeft += 8;
                                                    };

                                                    break;
                                                  case '9'
                                                      : {
                                                      BlaTimeLeft *= 10;
                                                      BlaTimeLeft += 9;
                                                    };

                                                    break;
                                                };

                                                decimal++;
                                              };
                                            };
                                          };

                                          ping_status--;
                                        } else
                                          if (lin == "otim") {
                                            /* otim N
                                            Set a clock that always belongs to the opponent.
                                            N is a number in centiseconds (units of 1/100 second).
                                            Even if the opponent changes to playing the opposite color,
                                            this clock remains with the opponent.

                                            If needed for purposes of board display in force mode
                                            (where the engine is not participating in the game) the time clock
                                            should be associated with the last color that the engine was set to play,
                                            the otim clock with the opposite color.
                                            Beginning in protocol version 2, if you can't handle the time and otim commands,
                                            you can use the "feature" command to disable them;
                                            see below. The following techniques from older protocol versions also work:
                                            You can ignore the time and otim commands (that is, treat them as no-ops),
                                            or send back "Error (unknown command): time" the first time you see "time".
                                            */

																						#ifdef DEBUG_VERSION
                                            cout << "command OTIM handled in winboard session..." << endl;
                      											#endif

																						ping_status++;
                                            cin.rdbuf()->in_avail();
                                            cin >> lin;

                                            if (lin == "accepted") {
                                            	;
                                            } else {
                                              //translate string into decimal number
                                              if (ENGINE_PLAYS_WHITE == false) {
                                                WhiTimeLeft = 0;
                                                unsigned int decimal = 0;

                                                while (decimal < lin.length()) {
                                                  switch (lin.at(decimal)) {
                                                    case '0'
                                                        : {
                                                        WhiTimeLeft *= 10;
                                                      };

                                                      break;
                                                    case '1'
                                                        : {
                                                        WhiTimeLeft *= 10;
                                                        WhiTimeLeft += 1;
                                                      };

                                                      break;
                                                    case '2'
                                                        : {
                                                        WhiTimeLeft *= 10;
                                                        WhiTimeLeft += 2;
                                                      };

                                                      break;
                                                    case '3'
                                                        : {
                                                        WhiTimeLeft *= 10;
                                                        WhiTimeLeft += 3;
                                                      };

                                                      break;
                                                    case '4'
                                                        : {
                                                        WhiTimeLeft *= 10;
                                                        WhiTimeLeft += 4;
                                                      };

                                                      break;
                                                    case '5'
                                                        : {
                                                        WhiTimeLeft *= 10;
                                                        WhiTimeLeft += 5;
                                                      };

                                                      break;
                                                    case '6'
                                                        : {
                                                        WhiTimeLeft *= 10;
                                                        WhiTimeLeft += 6;
                                                      };

                                                      break;
                                                    case '7'
                                                        : {
                                                        WhiTimeLeft *= 10;
                                                        WhiTimeLeft += 7;
                                                      };

                                                      break;
                                                    case '8'
                                                        : {
                                                        WhiTimeLeft *= 10;
                                                        WhiTimeLeft += 8;
                                                      };

                                                      break;
                                                    case '9'
                                                        : {
                                                        WhiTimeLeft *= 10;
                                                        WhiTimeLeft += 9;
                                                      };

                                                      break;
                                                  };

                                                  decimal++;
                                                };
                                              } else {
                                                BlaTimeLeft = 0;
                                                unsigned int decimal = 0;

                                                while (decimal < lin.length()) {
                                                  switch (lin.at(decimal)) {
                                                    case '0'
                                                        : {
                                                        BlaTimeLeft *= 10;
                                                      };

                                                      break;
                                                    case '1'
                                                        : {
                                                        BlaTimeLeft *= 10;
                                                        BlaTimeLeft += 1;
                                                      };

                                                      break;
                                                    case '2'
                                                        : {
                                                        BlaTimeLeft *= 10;
                                                        BlaTimeLeft += 2;
                                                      };

                                                      break;
                                                    case '3'
                                                        : {
                                                        BlaTimeLeft *= 10;
                                                        BlaTimeLeft += 3;
                                                      };

                                                      break;
                                                    case '4'
                                                        : {
                                                        BlaTimeLeft *= 10;
                                                        BlaTimeLeft += 4;
                                                      };

                                                      break;
                                                    case '5'
                                                        : {
                                                        BlaTimeLeft *= 10;
                                                        BlaTimeLeft += 5;
                                                      };

                                                      break;
                                                    case '6'
                                                        : {
                                                        BlaTimeLeft *= 10;
                                                        BlaTimeLeft += 6;
                                                      };

                                                      break;
                                                    case '7'
                                                        : {
                                                        BlaTimeLeft *= 10;
                                                        BlaTimeLeft += 7;
                                                      };

                                                      break;
                                                    case '8'
                                                        : {
                                                        BlaTimeLeft *= 10;
                                                        BlaTimeLeft += 8;
                                                      };

                                                      break;
                                                    case '9'
                                                        : {
                                                        BlaTimeLeft *= 10;
                                                        BlaTimeLeft += 9;
                                                      };

                                                      break;
                                                  };

                                                  decimal++;
                                                };
                                              };
                                            };

                                            ping_status--;
                                          } else
                                            if (lin == "usermove") {
                                              /* usermove MOVE
                                              By default, moves are sent to the engine without a command name;
                                              the notation is just sent as a line by itself.
                                              Beginning in protocol version 2, you can use the feature command
                                              to cause the command name "usermove" to be sent before the move.
                                              Example: "usermove e2e4".
                                              MOVE
                                              See below for the syntax of moves.

                                              If the move is illegal, print an error message;
                                              see the section "Commands from the engine to xboard".

                                                    If the move is legal and in turn, make it.

                                                    If not in force mode, stop the opponent's clock, start the engine's clock,
                                              start thinking, and eventually make a move.

                                              When xboard sends your engine a move, it normally sends coordinate algebraic notation.
                                              Examples:
                                                   Normal moves: e2e4
                                                   Pawn promotion: e7e8q
                                                   Castling: e1g1, e1c1, e8g8, e8c8
                                                   Bughouse/crazyhouse drop: P@h3
                                                   ICS Wild 0/1 castling: d1f1, d1b1, d8f8, d8b8
                                                   FischerRandom castling: O-O, O-O-O (oh, not zero)
                                                    Beginning in protocol version 2, you can use the feature command
                                              to select SAN (standard algebraic notation) instead; for example,
                                              e4, Nf3, exd5, Bxf7+, Qxf7#, e8=Q, O-O, or P@h3.
                                              Note that the last form, P@h3, is a extension to the PGN standard's
                                              definition of SAN, which does not support bughouse or crazyhouse.

                                              Xboard doesn't reliably detect illegal moves,
                                              because it does not keep track of castling unavailability
                                              due to king or rook moves, or en passant availability.
                                              If xboard sends an illegal move, send back an error message
                                              so that xboard can retract it and inform the user;
                                              see the section "Commands from the engine to xboard".
                                              */


                                              #ifdef DEBUG_VERSION
                                              cout << "command USERMOVE handled in winboard session..." << endl;
                      											  #endif

                                              ping_status++;
                                              cin.rdbuf()->in_avail();
                                              cin >> lin;

                                              if (lin == "accepted") {
                                                ;
                                              } else {
                                                //If the move is legal and in turn, make it.
                                                //If not in force mode, stop the opponent's clock, start the engine's clock,
                                                //start thinking, and eventually make a move.
                                                //cout<<"string received:"<<lin<<". User moved:"<<printMove(ws_move)<<endl;

																								if (THINKING==false) {

																									if (GameWtm[GMI] == true) {
	                                                  gl_move = GetMove(lin);
	                                                  //cout<<"Getmove() found: "<<printMove(gl_move)<<" and move is: "<<lin<<endl;
	                                                  //update global position data, GMI++
	                                                  //printExtras("in usermove before whites move:");
	                                                  // set properly CHILD() arguments before move
	                                                  ws_r = 0;

	                                                  while (ws_r < 64) {
	                                                    B[ws_r] = GameBoard[GMI][ws_r];
	                                                    ws_r++;
	                                                  };

	                                                  WpaNum = 0;

	                                                  BpaNum = 0;

	                                                  WroNum = 0;

	                                                  BroNum = 0;

	                                                  WbiNum = 0;

	                                                  BbiNum = 0;

	                                                  WknNum = 0;

	                                                  BknNum = 0;

	                                                  WquNum = 0;

	                                                  BquNum = 0;

	                                                  MaterialScore = 0;

	                                                  for (ws_r = 0; ws_r < 64; ws_r++) {
	                                                    switch (B[ws_r]) {
	                                                      case
	                                                          sWpa: {
	                                                          WpaNum++;
	                                                          MaterialScore += evPawnMaterialValue;
	                                                        };

	                                                        break;
	                                                      case
	                                                          sBpa: {
	                                                          BpaNum++;
	                                                          MaterialScore -= evPawnMaterialValue;
	                                                        };

	                                                        break;
	                                                      case
	                                                          sWro: {
	                                                          WroNum++;
	                                                          MaterialScore += evRookMaterialValue;
	                                                        };

	                                                        break;
	                                                      case
	                                                          sBro: {
	                                                          BroNum++;
	                                                          MaterialScore -= evRookMaterialValue;
	                                                        };

	                                                        break;
	                                                      case
	                                                          sWbi: {
	                                                          WbiNum++;
	                                                          MaterialScore += evBishopMaterialValue;
	                                                        };

	                                                        break;
	                                                      case
	                                                          sBbi: {
	                                                          BbiNum++;
	                                                          MaterialScore -= evBishopMaterialValue;
	                                                        };

	                                                        break;
	                                                      case
	                                                          sWkn: {
	                                                          WknNum++;
	                                                          MaterialScore += evKnightMaterialValue;
	                                                        };

	                                                        break;
	                                                      case
	                                                          sBkn: {
	                                                          BknNum++;
	                                                          MaterialScore -= evKnightMaterialValue;
	                                                        };

	                                                        break;
	                                                      case
	                                                          sWki: {
	                                                          WkiPos = ws_r;
	                                                        };

	                                                        break;
	                                                      case
	                                                          sBki: {
	                                                          BkiPos = ws_r;
	                                                        };

	                                                        break;
	                                                      case
	                                                          sWqu: {
	                                                          WquNum++;
	                                                          MaterialScore += evQueenMaterialValue;
	                                                        };

	                                                        break;
	                                                      case
	                                                          sBqu: {
	                                                          BquNum++;
	                                                          MaterialScore -= evQueenMaterialValue;
	                                                        };

	                                                        break;
	                                                    };
	                                                  };

	                                                  //////////////////////////////////////
	                                                  //set global king area boards
	                                                  for (ev_v = 0; ev_v < 64; ev_v++) {
	                                                    WkiArea[ev_v] = 0;
	                                                    BkiArea[ev_v] = 0;
	                                                  };

	                                                  WkiArea[WkiPos] = 1;

	                                                  BkiArea[BkiPos] = 1;

	                                                  for (ev_v = 0; ev_v < 8; ev_v++) {
	                                                    WkiArea[Vec[ev_v][WkiPos][0]] = 1;
	                                                    BkiArea[Vec[ev_v][BkiPos][0]] = 1;
	                                                  };

	                                                  //////////////////////////////////////
	                                                  enpassant = GameEnpassant[GMI];

	                                                  castle_dat = Game_castle_data[GMI];

	                                                  Zkey = Game_Zkey_tab[GMI];

	                                                  retract = GameRetract[GMI];

	                                                  CHILD(1);    //uses gl_move

	                                                  GMI++;

	                                                  GameLen++;

	                                                  GameEnpassant[GMI] = enpassant;

	                                                  Game_castle_data[GMI] = castle_dat;

	                                                  Game_Zkey_tab[GMI] = Zkey;

	                                                  GameRetract[GMI] = retract;

	                                                  //handle 3-fold draw
	                                                  if (retract < 99) {
	                                                    ws_r = 0;
	                                                    ws_j = GMI - 1;

	                                                    while (ws_j > 0) {
	                                                      if (GameRetract[ws_j] == 0) {
	                                                        break;
	                                                      };

	                                                      if (Zkey == Game_Zkey_tab[ws_j]) {
	                                                        ws_r++;

	                                                        if (ws_r > 1) {
	                                                          GameVal[GMI] = DRAW_SCORE;
	                                                          cout << "telluser 3-fold repetition draw!" << endl;
	                                                          break;
	                                                        };
	                                                      };

	                                                      ws_j--;
	                                                    };
	                                                  }
	                                                  //handle 50-moves rule
	                                                  else {
	                                                    GameVal[GMI] = DRAW_SCORE;
	                                                    cout << "telluser 50-moves rule draw!" << endl;
	                                                  };

	                                                  GameMoves[GMI] = gl_move; //set game move

	                                                  ws_r = 0;

	                                                  while (ws_r < 64) {
	                                                    GameBoard[GMI][ws_r] = B[ws_r];
	                                                    ws_r++;
	                                                  }; //set game board


	                                                  //printExtras("in usermove after whites move:");
	                                                  //if analyze mode
	                                                  if (ANALYZE_MODE == true) {
	                                                    THINKING = true;
	                                                    MoveStartTime = myTimer.getElapsedTimeInCentiseconds();
	                                                    gl_move = IncrementalSearch();//find best move from PV
	                                                  };

	                                                  // respond immediately to usermove if not in force mode and analyze mode
	                                                  if ((FORCE_MODE == false) && (ANALYZE_MODE == false)) {

	                                                    THINKING = true;

	                                                    //Wait();

																											MoveStartTime = myTimer.getElapsedTimeInCentiseconds();
	                                                    gl_move = IncrementalSearch();//find best move from PV
	                                                    //update global position data, GMI++
	                                                    // set properly CHILD() arguments before move
	                                                    ws_r = 0;

	                                                    while (ws_r < 64) {
	                                                      B[ws_r] = GameBoard[GMI][ws_r];
	                                                      ws_r++;
	                                                    };

	                                                    WpaNum = 0;

	                                                    BpaNum = 0;

	                                                    WroNum = 0;

	                                                    BroNum = 0;

	                                                    WbiNum = 0;

	                                                    BbiNum = 0;

	                                                    WknNum = 0;

	                                                    BknNum = 0;

	                                                    WquNum = 0;

	                                                    BquNum = 0;

	                                                    MaterialScore = 0;

	                                                    for (ws_r = 0; ws_r < 64; ws_r++) {
	                                                      switch (B[ws_r]) {
	                                                        case
	                                                            sWpa: {
	                                                            WpaNum++;
	                                                            MaterialScore += evPawnMaterialValue;
	                                                          };

	                                                          break;
	                                                        case
	                                                            sBpa: {
	                                                            BpaNum++;
	                                                            MaterialScore -= evPawnMaterialValue;
	                                                          };

	                                                          break;
	                                                        case
	                                                            sWro: {
	                                                            WroNum++;
	                                                            MaterialScore += evRookMaterialValue;
	                                                          };

	                                                          break;
	                                                        case
	                                                            sBro: {
	                                                            BroNum++;
	                                                            MaterialScore -= evRookMaterialValue;
	                                                          };

	                                                          break;
	                                                        case
	                                                            sWbi: {
	                                                            WbiNum++;
	                                                            MaterialScore += evBishopMaterialValue;
	                                                          };

	                                                          break;
	                                                        case
	                                                            sBbi: {
	                                                            BbiNum++;
	                                                            MaterialScore -= evBishopMaterialValue;
	                                                          };

	                                                          break;
	                                                        case
	                                                            sWkn: {
	                                                            WknNum++;
	                                                            MaterialScore += evKnightMaterialValue;
	                                                          };

	                                                          break;
	                                                        case
	                                                            sBkn: {
	                                                            BknNum++;
	                                                            MaterialScore -= evKnightMaterialValue;
	                                                          };

	                                                          break;
	                                                        case
	                                                            sWki: {
	                                                            WkiPos = ws_r;
	                                                          };

	                                                          break;
	                                                        case
	                                                            sBki: {
	                                                            BkiPos = ws_r;
	                                                          };

	                                                          break;
	                                                        case
	                                                            sWqu: {
	                                                            WquNum++;
	                                                            MaterialScore += evQueenMaterialValue;
	                                                          };

	                                                          break;
	                                                        case
	                                                            sBqu: {
	                                                            BquNum++;
	                                                            MaterialScore -= evQueenMaterialValue;
	                                                          };

	                                                          break;
	                                                      };
	                                                    };

	                                                    //////////////////////////////////////
	                                                    //set global king area boards
	                                                    for (ev_v = 0; ev_v < 64; ev_v++) {
	                                                      WkiArea[ev_v] = 0;
	                                                      BkiArea[ev_v] = 0;
	                                                    };

	                                                    WkiArea[WkiPos] = 1;

	                                                    BkiArea[BkiPos] = 1;

	                                                    for (ev_v = 0; ev_v < 8; ev_v++) {
	                                                      WkiArea[Vec[ev_v][WkiPos][0]] = 1;
	                                                      BkiArea[Vec[ev_v][BkiPos][0]] = 1;
	                                                    };

	                                                    //////////////////////////////////////
	                                                    enpassant = GameEnpassant[GMI];

	                                                    castle_dat = Game_castle_data[GMI];

	                                                    Zkey = Game_Zkey_tab[GMI];

	                                                    retract = GameRetract[GMI];

	                                                    CHILD(1);    //uses gl_move

	                                                    GMI++;

	                                                    GameLen++;

	                                                    GameEnpassant[GMI] = enpassant;

	                                                    Game_castle_data[GMI] = castle_dat;

	                                                    Game_Zkey_tab[GMI] = Zkey;

	                                                    GameRetract[GMI] = retract;

	                                                    //handle 3-fold draw
	                                                    if (retract < 99) {
	                                                      ws_r = 0;
	                                                      ws_j = GMI - 1;

	                                                      while (ws_j > 0) {
	                                                        if (GameRetract[ws_j] == 0) {
	                                                          break;
	                                                        };

	                                                        if (Zkey == Game_Zkey_tab[ws_j]) {
	                                                          ws_r++;

	                                                          if (ws_r > 1) {
	                                                            GameVal[GMI] = DRAW_SCORE;
	                                                            cout << "telluser 3-fold repetition draw!" << endl;
	                                                            break;
	                                                          };
	                                                        };

	                                                        ws_j--;
	                                                      };
	                                                    }
	                                                    //handle 50-moves rule
	                                                    else {
	                                                      GameVal[GMI] = DRAW_SCORE;
	                                                      cout << "telluser 50-moves rule draw!" << endl;
	                                                    };

	                                                    GameMoves[GMI] = gl_move; //set game move

	                                                    ws_r = 0;

	                                                    while (ws_r < 64) {
	                                                      GameBoard[GMI][ws_r] = B[ws_r];
	                                                      ws_r++;
	                                                    }; //set game board

	                                                    //printExtras("in usermove after blacks move:");
	                                                    cout << "move " << printMove(gl_move) << endl;    //send command to winboard
	                                                  };
	                                                //ELSE IF BLACK TO MOVE
	                                                } else {
	                                                  gl_move = GetMove(lin);
	                                                  //cout<<"Getmove() found: "<<printMove(gl_move)<<" and move is: "<<lin<<endl;
	                                                  //update global position data, GMI++
	                                                  //printExtras("in usermove before blacks move:");
	                                                  // set properly CHILD() arguments before move
	                                                  ws_r = 0;

	                                                  while (ws_r < 64) {
	                                                    B[ws_r] = GameBoard[GMI][ws_r];
	                                                    ws_r++;
	                                                  };

	                                                  WpaNum = 0;

	                                                  BpaNum = 0;

	                                                  WroNum = 0;

	                                                  BroNum = 0;

	                                                  WbiNum = 0;

	                                                  BbiNum = 0;

	                                                  WknNum = 0;

	                                                  BknNum = 0;

	                                                  WquNum = 0;

	                                                  BquNum = 0;

	                                                  MaterialScore = 0;

	                                                  for (ws_r = 0; ws_r < 64; ws_r++) {
	                                                    switch (B[ws_r]) {
	                                                      case
	                                                          sWpa: {
	                                                          WpaNum++;
	                                                          MaterialScore += evPawnMaterialValue;
	                                                        };

	                                                        break;
	                                                      case
	                                                          sBpa: {
	                                                          BpaNum++;
	                                                          MaterialScore -= evPawnMaterialValue;
	                                                        };

	                                                        break;
	                                                      case
	                                                          sWro: {
	                                                          WroNum++;
	                                                          MaterialScore += evRookMaterialValue;
	                                                        };

	                                                        break;
	                                                      case
	                                                          sBro: {
	                                                          BroNum++;
	                                                          MaterialScore -= evRookMaterialValue;
	                                                        };

	                                                        break;
	                                                      case
	                                                          sWbi: {
	                                                          WbiNum++;
	                                                          MaterialScore += evBishopMaterialValue;
	                                                        };

	                                                        break;
	                                                      case
	                                                          sBbi: {
	                                                          BbiNum++;
	                                                          MaterialScore -= evBishopMaterialValue;
	                                                        };

	                                                        break;
	                                                      case
	                                                          sWkn: {
	                                                          WknNum++;
	                                                          MaterialScore += evKnightMaterialValue;
	                                                        };

	                                                        break;
	                                                      case
	                                                          sBkn: {
	                                                          BknNum++;
	                                                          MaterialScore -= evKnightMaterialValue;
	                                                        };

	                                                        break;
	                                                      case
	                                                          sWki: {
	                                                          WkiPos = ws_r;
	                                                        };

	                                                        break;
	                                                      case
	                                                          sBki: {
	                                                          BkiPos = ws_r;
	                                                        };

	                                                        break;
	                                                      case
	                                                          sWqu: {
	                                                          WquNum++;
	                                                          MaterialScore += evQueenMaterialValue;
	                                                        };

	                                                        break;
	                                                      case
	                                                          sBqu: {
	                                                          BquNum++;
	                                                          MaterialScore -= evQueenMaterialValue;
	                                                        };

	                                                        break;
	                                                    };
	                                                  };

	                                                  //////////////////////////////////////
	                                                  //set global king area boards
	                                                  for (ev_v = 0; ev_v < 64; ev_v++) {
	                                                    WkiArea[ev_v] = 0;
	                                                    BkiArea[ev_v] = 0;
	                                                  };

	                                                  WkiArea[WkiPos] = 1;

	                                                  BkiArea[BkiPos] = 1;

	                                                  for (ev_v = 0; ev_v < 8; ev_v++) {
	                                                    WkiArea[Vec[ev_v][WkiPos][0]] = 1;
	                                                    BkiArea[Vec[ev_v][BkiPos][0]] = 1;
	                                                  };

	                                                  //////////////////////////////////////
	                                                  enpassant = GameEnpassant[GMI];

	                                                  castle_dat = Game_castle_data[GMI];

	                                                  Zkey = Game_Zkey_tab[GMI];

	                                                  retract = GameRetract[GMI];

	                                                  CHILD(1);    //uses gl_move

	                                                  GMI++;

	                                                  GameLen++;

	                                                  GameEnpassant[GMI] = enpassant;

	                                                  Game_castle_data[GMI] = castle_dat;

	                                                  Game_Zkey_tab[GMI] = Zkey;

	                                                  GameRetract[GMI] = retract;

	                                                  //handle 3-fold draw
	                                                  if (retract < 99) {
	                                                    ws_r = 0;
	                                                    ws_j = GMI - 1;

	                                                    while (ws_j > 0) {
	                                                      if (GameRetract[ws_j] == 0) {
	                                                        break;
	                                                      };

	                                                      if (Zkey == Game_Zkey_tab[ws_j]) {
	                                                        ws_r++;

	                                                        if (ws_r > 1) {
	                                                          GameVal[GMI] = DRAW_SCORE;
	                                                          cout << "telluser 3-fold repetition draw!" << endl;
	                                                          break;
	                                                        };
	                                                      };

	                                                      ws_j--;
	                                                    };
	                                                  }
	                                                  //handle 50-moves rule
	                                                  else {
	                                                    GameVal[GMI] = DRAW_SCORE;
	                                                    cout << "telluser 50-moves rule draw!" << endl;
	                                                  };

	                                                  GameMoves[GMI] = gl_move; //set game move

	                                                  ws_r = 0;

	                                                  while (ws_r < 64) {
	                                                    GameBoard[GMI][ws_r] = B[ws_r];
	                                                    ws_r++;
	                                                  }; //set game board

	                                                  //printExtras("in usermove after whites move:");
	                                                  //if analyze mode
	                                                  if (ANALYZE_MODE == true) {
	                                                    THINKING = true;
	                                                    MoveStartTime = myTimer.getElapsedTimeInCentiseconds();
	                                                    gl_move = IncrementalSearch();//search best move from PV infinitly
	                                                  };

	                                                  // respond immediately
	                                                  if ((FORCE_MODE == false) && (ANALYZE_MODE == false)) {

																											//Wait();

																											THINKING = true;
	                                                    MoveStartTime = myTimer.getElapsedTimeInCentiseconds();
	                                                    gl_move = IncrementalSearch();//make best move from PV
	                                                    //update global position data, GMI++
	                                                    // set properly CHILD() arguments before move
	                                                    ws_r = 0;

	                                                    while (ws_r < 64) {
	                                                      B[ws_r] = GameBoard[GMI][ws_r];
	                                                      ws_r++;
	                                                    };

	                                                    WpaNum = 0;

	                                                    BpaNum = 0;

	                                                    WroNum = 0;

	                                                    BroNum = 0;

	                                                    WbiNum = 0;

	                                                    BbiNum = 0;

	                                                    WknNum = 0;

	                                                    BknNum = 0;

	                                                    WquNum = 0;

	                                                    BquNum = 0;

	                                                    MaterialScore = 0;

	                                                    for (ws_r = 0; ws_r < 64; ws_r++) {
	                                                      switch (B[ws_r]) {
	                                                        case
	                                                            sWpa: {
	                                                            WpaNum++;
	                                                            MaterialScore += evPawnMaterialValue;
	                                                          };

	                                                          break;
	                                                        case
	                                                            sBpa: {
	                                                            BpaNum++;
	                                                            MaterialScore -= evPawnMaterialValue;
	                                                          };

	                                                          break;
	                                                        case
	                                                            sWro: {
	                                                            WroNum++;
	                                                            MaterialScore += evRookMaterialValue;
	                                                          };

	                                                          break;
	                                                        case
	                                                            sBro: {
	                                                            BroNum++;
	                                                            MaterialScore -= evRookMaterialValue;
	                                                          };

	                                                          break;
	                                                        case
	                                                            sWbi: {
	                                                            WbiNum++;
	                                                            MaterialScore += evBishopMaterialValue;
	                                                          };

	                                                          break;
	                                                        case
	                                                            sBbi: {
	                                                            BbiNum++;
	                                                            MaterialScore -= evBishopMaterialValue;
	                                                          };

	                                                          break;
	                                                        case
	                                                            sWkn: {
	                                                            WknNum++;
	                                                            MaterialScore += evKnightMaterialValue;
	                                                          };

	                                                          break;
	                                                        case
	                                                            sBkn: {
	                                                            BknNum++;
	                                                            MaterialScore -= evKnightMaterialValue;
	                                                          };

	                                                          break;
	                                                        case
	                                                            sWki: {
	                                                            WkiPos = ws_r;
	                                                          };

	                                                          break;
	                                                        case
	                                                            sBki: {
	                                                            BkiPos = ws_r;
	                                                          };

	                                                          break;
	                                                        case
	                                                            sWqu: {
	                                                            WquNum++;
	                                                            MaterialScore += evQueenMaterialValue;
	                                                          };

	                                                          break;
	                                                        case
	                                                            sBqu: {
	                                                            BquNum++;
	                                                            MaterialScore -= evQueenMaterialValue;
	                                                          };

	                                                          break;
	                                                      };
	                                                    };

	                                                    //////////////////////////////////////
	                                                    //set global king area boards
	                                                    for (ev_v = 0; ev_v < 64; ev_v++) {
	                                                      WkiArea[ev_v] = 0;
	                                                      BkiArea[ev_v] = 0;
	                                                    };

	                                                    WkiArea[WkiPos] = 1;

	                                                    BkiArea[BkiPos] = 1;

	                                                    for (ev_v = 0; ev_v < 8; ev_v++) {
	                                                      WkiArea[Vec[ev_v][WkiPos][0]] = 1;
	                                                      BkiArea[Vec[ev_v][BkiPos][0]] = 1;
	                                                    };

	                                                    //////////////////////////////////////
	                                                    enpassant = GameEnpassant[GMI];

	                                                    castle_dat = Game_castle_data[GMI];

	                                                    Zkey = Game_Zkey_tab[GMI];

	                                                    retract = GameRetract[GMI];

	                                                    CHILD(1);    //uses gl_move

	                                                    GMI++;

	                                                    GameLen++;

	                                                    GameEnpassant[GMI] = enpassant;

	                                                    Game_castle_data[GMI] = castle_dat;

	                                                    Game_Zkey_tab[GMI] = Zkey;

	                                                    GameRetract[GMI] = retract;

	                                                    //handle 3-fold draw
	                                                    if (retract < 99) {
	                                                      ws_r = 0;
	                                                      ws_j = GMI - 1;

	                                                      while (ws_j > 0) {
	                                                        if (GameRetract[ws_j] == 0) {
	                                                          break;
	                                                        };

	                                                        if (Zkey == Game_Zkey_tab[ws_j]) {
	                                                          ws_r++;

	                                                          if (ws_r > 1) {
	                                                            GameVal[GMI] = DRAW_SCORE;
	                                                            cout << "telluser 3-fold repetition draw!" << endl;
	                                                            break;
	                                                          };
	                                                        };

	                                                        ws_j--;
	                                                      };
	                                                    }
	                                                    //handle 50-moves rule
	                                                    else {
	                                                      GameVal[GMI] = DRAW_SCORE;
	                                                      cout << "telluser 50-moves rule draw!" << endl;
	                                                    };

	                                                    GameMoves[GMI] = gl_move; //set game move

	                                                    ws_r = 0;

	                                                    while (ws_r < 64) {
	                                                      GameBoard[GMI][ws_r] = B[ws_r];
	                                                      ws_r++;
	                                                    }; //set game board

	                                                    //printExtras("in usermove after whites move:");
	                                                    cout << "move " << printMove(gl_move) << endl;    //send command to winboard
	                                                  };

																									};

																								};

                                              };

                                              ping_status--;

                                            } else
                                              if (lin == "?") {
                                              	ping_status++;
                                                ;
                                                ping_status--;
                                                /* ?
                                                Move now. If your engine is thinking, it should move immediately;
                                                 otherwise, the command should be ignored (treated as a no-op).
                                                 It is permissible for your engine to always ignore the ? command.
                                                 The only bad consequence is that xboard's Move Now menu command
                                                will do nothing.
                                                 It is also permissible for your engine to move immediately if
                                                it gets
                                                 any command while thinking, as long as it processes the command right after moving,
                                                 but it's preferable if you don't do this.
                                                 For example, xboard may send post, nopost, easy, hard, force, quit,
                                                 or other commands while the engine is on move.
                                                 */
                                                 #ifdef DEBUG_VERSION
                                                 cout << "command ? handled in winboard session, doing nothing..." << endl;
                      											     #endif
                                              } else
                                                if (lin == "draw") {
                                                  ping_status++;
                                                  /* draw
                                                   The engine's opponent offers the engine a draw.
                                                   To accept the draw, send "offer draw".
                                                   To decline, ignore the offer (that is, send nothing).
                                                   If you're playing on ICS, it's possible for the draw offer to have been withdrawn
                                                   by the time you accept it, so don't assume the game is over because you accept
                                                   a draw offer. Continue playing until xboard tells you the game is over.
                                                   See also "offer draw" below.
                                                   */
                                                  //cout<<"Error (unknown command):"<<lin<<endl;
                                                  ping_status--;
                                                  #ifdef DEBUG_VERSION
                                                  cout << "command draw handled in winboard session, doing nothing..." << endl;
                      											      #endif
                                                } else
                                                  if (lin == "result") {
                                                    /* result RESULT {COMMENT}
                                                     After the end of each game, xboard will send you a result command.
                                                     You can use this command to trigger learning.
                                                     RESULT is either 1-0, 0-1, 1/2-1/2, or *,
                                                     indicating whether white won, black won, the game was a draw,
                                                    or the game was unfinished.
                                                     The COMMENT string is purely a human-readable comment;
                                                     its content is unspecified and subject to change.
                                                     In ICS mode, it is passed through from ICS uninterpreted.
                                                      Example: result 1-0 {White mates}
                                                      Here are some notes on interpreting the "result" command.
                                                     Some apply only to playing on ICS ("Zippy" mode).
                                                     If you won but did not just play a mate, your opponent must have resigned or forfeited.
                                                     If you lost but were not just mated, you probably forfeited on time,
                                                     or perhaps the operator resigned manually.
                                                     If there was a draw for some nonobvious reason, perhaps your opponent
                                                     called your flag when he had insufficient mating material (or vice versa),
                                                     or perhaps the operator agreed to a draw manually.
                                                     You will get a result command even if you already know the game ended
                                                    - for example, after you just checkmated your opponent.
                                                     In fact, if you send the "RESULT {COMMENT}" command (discussed below),
                                                     you will simply get the same thing fed back to you with "result" tacked
                                                    in front.
                                                     You might not always get a "result *" command, however.
                                                     In particular, you won't get one in local chess engine mode when the user
                                                     stops playing by selecting Reset, Edit Game, Exit or the like.
                                                     */
                                                    ping_status++;
                                                    cin.rdbuf()->in_avail();
                                                    cin >> lin;

                                                    if (lin == "1-0") {
                                                      cout << "telluser White mates" << endl;
                                                    } else
                                                      if (lin == "0-1") {
                                                        cout << "telluser Black mates" << endl;
                                                      } else
                                                        if (lin == "1/2-1/2") {
                                                          cout << "telluser Draw" << endl;
                                                        } else
                                                          if (lin == "*") {
                                                            cout << "telluser Game unfinished" << endl;
                                                          };

                                                    cin.rdbuf()->in_avail();

                                                    cin >> lin;

                                                    cout << "telluser Comment:" << lin << endl;

                                                    ping_status--;

                                                    #ifdef DEBUG_VERSION
                                                    cout << "command RESULT handled in winboard session..." << endl;
                      											        #endif

																									} else
                                                    if (lin == "edit") {
                                                      /* edit (OLD)
                                                      The edit command is the old way to set up positions.
                                                      For compatibility with old engines, it is still used by default,
                                                      but new engines may prefer to use the feature command (see below)
                                                      to cause xboard to use setboard instead.
                                                      The edit command puts the chess engine into a special mode,
                                                      where it accepts the following subcommands:
                                                         c change current piece color, initially white
                                                         Pa4 (for example) place pawn of current color on a4
                                                         xa4 (for example) empty the square a4 (not used by xboard)
                                                         # clear board
                                                         . leave edit mode
                                                      See the Idioms section below for additional subcommands used in ChessBase's
                                                      implementation of the protocol.
                                                      The edit command does not change the side to move.
                                                      To set up a black-on-move position, xboard uses the following command sequence:
                                                            new
                                                            force
                                                         a2a3
                                                         edit
                                                         <edit commands>
                                                         .
                                                      This sequence is used to avoid the "black" command,
                                                      which is now considered obsolete and which many engines never did implement
                                                      as specified in this document.
                                                      After an edit command is complete, if a king and a rook are on their home squares,
                                                      castling is assumed to be available to them.
                                                      En passant capture is assumed to be illegal on the current move regardless
                                                      of the positions of the pawns. The clock for the 50 move rule starts at zero,
                                                      and for purposes of the draw by repetition rule, no prior positions are deemed
                                                      to have occurred.
                                                      */
                                                      ping_status++;
                                                      //cout<<"Error (unknown command):"<<lin<<endl;
                                                      ping_status--;
                                                      #ifdef DEBUG_VERSION
                                                      cout << "command EDIT handled in winboard session, doing nothing..." << endl;
                      											          #endif
                                                    } else
                                                      if (lin == "hint") {
                                                        ;
                                                        /* hint
                                                        If the user asks for a hint, xboard sends your engine the command "hint".
                                                        Your engine should respond with "Hint: xxx", where xxx is a suggested move.
                                                        If there is no move to suggest, you can ignore the hint command
                                                        (that is, treat it as a no-op).
                                                        */
                                                        ping_status++;
                                                        cout << "Hint: " << printMove(GamePV[GMI][0]) << endl;
                                                        ping_status--;
                                                        #ifdef DEBUG_VERSION
                                                        cout << "command HINT handled in winboard session, responding..." << endl;
                      											            #endif
                                                      } else
                                                        if (lin == "bk") {
                                                          ;
                                                          /* bk
                                                           If the user selects "Book" from the xboard menu, xboard will
                                                           send your engine the command "bk". You can send any text you like as the response,
                                                           as long as each line begins with a blank space or tab (\t) character,
                                                           and you send an empty line at the end.
                                                           The text pops up in a modal information dialog.
                                                           */
                                                          ping_status++;
                                                          //cout<<"Error (unknown command):"<<lin<<endl;
                                                          ping_status--;
                                                          #ifdef DEBUG_VERSION
                                                          cout << "command BK handled in winboard session..." << endl;
                      											              #endif
                                                        } else
                                                          if (lin == "undo") {
                                                            /* undo
                                                             If the user asks to back up one move, xboard will send you the "undo" command.
                                                             xboard will not send this command without putting you in "force" mode first,
                                                             so you don't have to worry about what should happen if the user asks to
                                                             undo a move your engine made.
                                                             (GNU Chess 4 actually switches to playing the opposite color in
                                                                  this case.)
                                                             */
                                                            ping_status++;

																														if (GMI > 0) {

																															if (GameWtm[GMI] == true) {

	                                                              gl_move = GameMoves[GMI];

	                                                              ws_r = 0;

							                                                  while (ws_r < 64) {
							                                                    B[ws_r] = GameBoard[GMI][ws_r];
							                                                    ws_r++;
							                                                  };

							                                                  WpaNum = 0;

							                                                  BpaNum = 0;

							                                                  WroNum = 0;

							                                                  BroNum = 0;

							                                                  WbiNum = 0;

							                                                  BbiNum = 0;

							                                                  WknNum = 0;

							                                                  BknNum = 0;

							                                                  WquNum = 0;

							                                                  BquNum = 0;

							                                                  MaterialScore = 0;

							                                                  for (ws_r = 0; ws_r < 64; ws_r++) {
							                                                    switch (B[ws_r]) {
							                                                      case
							                                                          sWpa: {
							                                                          WpaNum++;
							                                                          MaterialScore += evPawnMaterialValue;
							                                                        };

							                                                        break;
							                                                      case
							                                                          sBpa: {
							                                                          BpaNum++;
							                                                          MaterialScore -= evPawnMaterialValue;
							                                                        };

							                                                        break;
							                                                      case
							                                                          sWro: {
							                                                          WroNum++;
							                                                          MaterialScore += evRookMaterialValue;
							                                                        };

							                                                        break;
							                                                      case
							                                                          sBro: {
							                                                          BroNum++;
							                                                          MaterialScore -= evRookMaterialValue;
							                                                        };

							                                                        break;
							                                                      case
							                                                          sWbi: {
							                                                          WbiNum++;
							                                                          MaterialScore += evBishopMaterialValue;
							                                                        };

							                                                        break;
							                                                      case
							                                                          sBbi: {
							                                                          BbiNum++;
							                                                          MaterialScore -= evBishopMaterialValue;
							                                                        };

							                                                        break;
							                                                      case
							                                                          sWkn: {
							                                                          WknNum++;
							                                                          MaterialScore += evKnightMaterialValue;
							                                                        };

							                                                        break;
							                                                      case
							                                                          sBkn: {
							                                                          BknNum++;
							                                                          MaterialScore -= evKnightMaterialValue;
							                                                        };

							                                                        break;
							                                                      case
							                                                          sWki: {
							                                                          WkiPos = ws_r;
							                                                        };

							                                                        break;
							                                                      case
							                                                          sBki: {
							                                                          BkiPos = ws_r;
							                                                        };

							                                                        break;
							                                                      case
							                                                          sWqu: {
							                                                          WquNum++;
							                                                          MaterialScore += evQueenMaterialValue;
							                                                        };

							                                                        break;
							                                                      case
							                                                          sBqu: {
							                                                          BquNum++;
							                                                          MaterialScore -= evQueenMaterialValue;
							                                                        };

							                                                        break;
							                                                    };
							                                                  };

							                                                  //////////////////////////////////////
							                                                  //set global king area boards
							                                                  for (ev_v = 0; ev_v < 64; ev_v++) {
							                                                    WkiArea[ev_v] = 0;
							                                                    BkiArea[ev_v] = 0;
							                                                  };

							                                                  WkiArea[WkiPos] = 1;

							                                                  BkiArea[BkiPos] = 1;

							                                                  for (ev_v = 0; ev_v < 8; ev_v++) {
							                                                    WkiArea[Vec[ev_v][WkiPos][0]] = 1;
							                                                    BkiArea[Vec[ev_v][BkiPos][0]] = 1;
							                                                  };

							                                                  //////////////////////////////////////
							                                                  enpassant = GameEnpassant[GMI];

							                                                  castle_dat = Game_castle_data[GMI];

							                                                  Zkey = Game_Zkey_tab[GMI];

							                                                  retract = GameRetract[GMI];

							                                                  cout<<"retracting game move: "<<printMove(GameMoves[GMI])<<endl;

							                                                  PARENT(1);

							                                                  GMI--;

							                                                  GameLen--;

							                                                  GameEnpassant[GMI] = enpassant;

							                                                  Game_castle_data[GMI] = castle_dat;

							                                                  Game_Zkey_tab[GMI] = Zkey;

							                                                  GameRetract[GMI] = retract;

							                                                  ws_r = 0;

							                                                  while (ws_r < 64) {
							                                                    GameBoard[GMI][ws_r] = B[ws_r];
							                                                    ws_r++;
							                                                  }; //set game board
							                                                }

																															//btm
																															else {

																															  gl_move = GameMoves[GMI];

																																ws_r = 0;

							                                                  while (ws_r < 64) {
							                                                    B[ws_r] = GameBoard[GMI][ws_r];
							                                                    ws_r++;
							                                                  };

							                                                  WpaNum = 0;

							                                                  BpaNum = 0;

							                                                  WroNum = 0;

							                                                  BroNum = 0;

							                                                  WbiNum = 0;

							                                                  BbiNum = 0;

							                                                  WknNum = 0;

							                                                  BknNum = 0;

							                                                  WquNum = 0;

							                                                  BquNum = 0;

							                                                  MaterialScore = 0;

							                                                  for (ws_r = 0; ws_r < 64; ws_r++) {
							                                                    switch (B[ws_r]) {
							                                                      case
							                                                          sWpa: {
							                                                          WpaNum++;
							                                                          MaterialScore += evPawnMaterialValue;
							                                                        };

							                                                        break;
							                                                      case
							                                                          sBpa: {
							                                                          BpaNum++;
							                                                          MaterialScore -= evPawnMaterialValue;
							                                                        };

							                                                        break;
							                                                      case
							                                                          sWro: {
							                                                          WroNum++;
							                                                          MaterialScore += evRookMaterialValue;
							                                                        };

							                                                        break;
							                                                      case
							                                                          sBro: {
							                                                          BroNum++;
							                                                          MaterialScore -= evRookMaterialValue;
							                                                        };

							                                                        break;
							                                                      case
							                                                          sWbi: {
							                                                          WbiNum++;
							                                                          MaterialScore += evBishopMaterialValue;
							                                                        };

							                                                        break;
							                                                      case
							                                                          sBbi: {
							                                                          BbiNum++;
							                                                          MaterialScore -= evBishopMaterialValue;
							                                                        };

							                                                        break;
							                                                      case
							                                                          sWkn: {
							                                                          WknNum++;
							                                                          MaterialScore += evKnightMaterialValue;
							                                                        };

							                                                        break;
							                                                      case
							                                                          sBkn: {
							                                                          BknNum++;
							                                                          MaterialScore -= evKnightMaterialValue;
							                                                        };

							                                                        break;
							                                                      case
							                                                          sWki: {
							                                                          WkiPos = ws_r;
							                                                        };

							                                                        break;
							                                                      case
							                                                          sBki: {
							                                                          BkiPos = ws_r;
							                                                        };

							                                                        break;
							                                                      case
							                                                          sWqu: {
							                                                          WquNum++;
							                                                          MaterialScore += evQueenMaterialValue;
							                                                        };

							                                                        break;
							                                                      case
							                                                          sBqu: {
							                                                          BquNum++;
							                                                          MaterialScore -= evQueenMaterialValue;
							                                                        };

							                                                        break;
							                                                    };
							                                                  };

							                                                  //////////////////////////////////////
							                                                  //set global king area boards
							                                                  for (ev_v = 0; ev_v < 64; ev_v++) {
							                                                    WkiArea[ev_v] = 0;
							                                                    BkiArea[ev_v] = 0;
							                                                  };

							                                                  WkiArea[WkiPos] = 1;

							                                                  BkiArea[BkiPos] = 1;

							                                                  for (ev_v = 0; ev_v < 8; ev_v++) {
							                                                    WkiArea[Vec[ev_v][WkiPos][0]] = 1;
							                                                    BkiArea[Vec[ev_v][BkiPos][0]] = 1;
							                                                  };

							                                                  //////////////////////////////////////
							                                                  enpassant = GameEnpassant[GMI];

							                                                  castle_dat = Game_castle_data[GMI];

							                                                  Zkey = Game_Zkey_tab[GMI];

							                                                  retract = GameRetract[GMI];

							                                                  cout<<"retracting game move: "<<printMove(GameMoves[GMI])<<endl;

							                                                  PARENT(1);    //uses gl_move

							                                                  GMI--;

							                                                  GameLen--;

							                                                  GameEnpassant[GMI] = enpassant;

							                                                  Game_castle_data[GMI] = castle_dat;

							                                                  Game_Zkey_tab[GMI] = Zkey;

							                                                  GameRetract[GMI] = retract;

							                                                  ws_r = 0;

							                                                  while (ws_r < 64) {
							                                                    GameBoard[GMI][ws_r] = B[ws_r];
							                                                    ws_r++;
							                                                  }; //set game board

																															};
							                                              };

                                                            ping_status--;
                                                            #ifdef DEBUG_VERSION
                                                            cout << "command UNDO handled in winboard session..." << endl;
                      											                #endif
                                                          } else
                                                            if (lin == "remove") {
                                                              ;
                                                              /* remove
                                                              If the user asks to retract a move, xboard will send you the "remove" command.
                                                              It sends this command only when the user is on move.
                                                              Your engine should undo the last two moves (one for each player)
                                                              and continue playing the same color.
                                                              */
                                                              ping_status++;

                                                              ;
                                                              ping_status--;
                                                              #ifdef DEBUG_VERSION
                                                              cout << "command REMOVE handled in winboard session..." << endl;
                      											                  #endif
                                                            } else
                                                              if (lin == "hard") {
                                                                ;
                                                                /* hard
                                                                Turn on pondering (thinking on the opponent's time, also known
                                                                      as "permanent brain").
                                                                xboard will not make any assumption about what your
                                                                      default is for pondering
                                                                or whether "new" affects this setting.
                                                                */
                                                                ping_status++;
                                                                PONDERING = true;
                                                                ping_status--;
                                                                #ifdef DEBUG_VERSION
                                                                cout << "command HARD handled in winboard session..." << endl;
                      											                    #endif
                                                              } else
                                                                if (lin == "easy") {
                                                                  ;
                                                                  /* easy
                                                                   Turn off pondering.
                                                                   */
                                                                  ping_status++;
                                                                  PONDERING = false;
                                                                  ping_status--;
                                                                  #ifdef DEBUG_VERSION
                                                                  cout << "command EASY handled in winboard session..." << endl;
                      											                      #endif
                                                                } else
                                                                  if (lin == "name") {
                                                                    ;
                                                                    /* name X
                                                                     This command informs the engine of its opponent's name.
                                                                     When the engine is playing on a chess server, xboard obtains the opponent's
                                                                     name from the server. When the engine is playing locally against a human user,
                                                                     xboard obtains the user's login name from the local operating system.
                                                                     When the engine is playing locally against another engine, xboard uses either
                                                                     the other engine's filename or the name that the other engine supplied in the
                                                                     myname option to the feature command.
                                                                     By default, xboard uses the name command only when the engine is playing on a chess server.
                                                                     Beginning in protocol version 2, you can change this with the name
                                                                     option to the feature command; see below.
                                                                     */
                                                                    ping_status++;
                                                                    //cout<<"Error (unknown command):"<<lin<<endl;
                                                                    ping_status--;
                                                                    #ifdef DEBUG_VERSION
                                                    								cout << "command NAME handled in winboard session..." << endl;
                      											        								#endif
                                                                  } else
                                                                    if (lin == "rating") {
                                                                      ;
                                                                      /* rating
                                                                      In ICS mode, xboard obtains the ICS opponent's rating from the "Creating:" message
                                                                      that appears before each game. (This message may not appear on servers
                                                                      using outdated versions of the FICS code.) In Zippy mode, it sends these ratings
                                                                      on to the chess engine using the "rating" command. The chess engine's own rating
                                                                      comes first, and if either opponent is not rated, his rating is given as 0.
                                                                      In the future this command may also be used in other modes, if ratings are known.
                                                                      Example: rating 2600 1500
                                                                      */
                                                                      ping_status++;
                                                                      //cout<<"Error (unknown command):"<<lin<<endl;
                                                                      ping_status--;
                                                                      #ifdef DEBUG_VERSION
                                                                      cout << "command RATING handled in winboard session..." << endl;
                      											                          #endif
                                                                    } else
                                                                      if (lin == "ics") {
                                                                        /* ics HOSTNAME
                                                                        If HOSTNAME is "-", the engine is playing against a local opponent;
                                                                        otherwise, the engine is playing on an Internet Chess Server (ICS) with
                                                                        the given hostname. This command is new in protocol version 2 and is not sent
                                                                        unless the engine has enabled it with the "feature" command. Example: "ics freechess.org"
                                                                        */
                                                                        ping_status++;
                                                                        //cout<<"Error (unknown command):"<<lin<<endl;
                                                                        ping_status--;
                                                                        #ifdef DEBUG_VERSION
                                                    										cout << "command ICS handled in winboard session..." << endl;
                      											        										#endif
                                                                      } else
                                                                        if (lin == "computer") {
                                                                          ;
                                                                          /* computer
                                                                           The opponent is also a computer chess engine. Some engines alter their
                                                                           playing style when they receive this command.
                                                                           */
                                                                          ping_status++;
                                                                          //cout<<"Error (unknown command):"<<lin<<endl;
                                                                          ping_status--;
                                                                          #ifdef DEBUG_VERSION
                                                                          cout << "command COMPUTER handled in winboard session..." << endl;
                      											                              #endif
                                                                        } else
                                                                          if (lin == "pause") {
                                                                            ;
                                                                            /* pause
                                                                            */
                                                                            ping_status++;
                                                                            //cout<<"Error (unknown command):"<<lin<<endl;
                                                                            ping_status--;
                                                                            #ifdef DEBUG_VERSION
                                                    												cout << "command PAUSE handled in winboard session..." << endl;
                      											        												#endif
                                                                          } else
                                                                            if (lin == "resume") {
                                                                              ;
                                                                              /* resume
                                                                              These commands are new in protocol version 2 and will not be sent unless
                                                                              feature pause=1 is set. At this writing, xboard actually does not use the
                                                                              commands at all, but it or other interfaces may use them in the future.)
                                                                              The "pause" command puts the engine into a special state where it does not
                                                                              think, ponder, or otherwise consume significant CPU time.
                                                                              The current thinking or pondering (if any) is suspended and both player's
                                                                              clocks are stopped. The only command that the interface may send to the engine
                                                                              while it is in the paused state is "resume".
                                                                              The paused thinking or pondering (if any) resumes from exactly where it left off,
                                                                              and the clock of the player on move resumes running from where it stopped.
                                                                              */
                                                                              ping_status++;
                                                                              //cout<<"Error (unknown command):"<<lin<<endl;
                                                                              ping_status--;
                                                                              #ifdef DEBUG_VERSION
                                                    													cout << "command RESUME handled in winboard session..." << endl;
                      											        													#endif
                                                                            } else
                                                                              if (lin == "ping") {
                                                                                /* ping N
                                                                                In this command, N is a decimal number.
                                                                                When you receive the command, reply by sending the string pong N,
                                                                                where N is the same number you received.

                                                                                Important: You must not reply to a "ping" command until you have
                                                                                      finished executing all commands that you received before it.

                                                                                Pondering does not count; if you receive a ping while pondering,
                                                                                you should reply immediately and continue pondering.
                                                                                Because of the way xboard uses the ping command, if you implement the other commands
                                                                                in this protocol, you should never see a "ping" command when it is
                                                                                       your move;
                                                                                however, if you do, you must not send the "pong" reply to xboard
                                                                                      until after you send
                                                                                your move. For example, xboard may send "?"
                                                                                      immediately followed by "ping".
                                                                                If you implement the "?" command, you will have moved by the time you see
                                                                                the subsequent ping command. Similarly, xboard may send a sequence like
                                                                                "force", "new", "ping". You must not send the pong response until after you have
                                                                                finished executing the "new" command and are ready for the new game to start.
                                                                                      The ping command is new in protocol version 2 and will not be sent unless you
                                                                                enable it with the "feature" command. Its purpose is to allow several race conditions
                                                                                that could occur in previous versions of the protocol to be fixed,
                                                                                so it is highly recommended that you implement it.
                                                                                It is especially important in simple engines that do not ponder
                                                                                      and do not poll for input
                                                                                while thinking, but it is needed in all engines.
                                                                                */
                                                                                cin.rdbuf()->in_avail();
                                                                                cin >> lin;

                                                                                if (lin == "accepted") {
                                                                                  ;
                                                                                } else {
                                                                                  if (ping_status == 0) {
                                                                                    cout << "pong " << lin << endl;
                                                                                  };

                                                                                  //else {cout<<"telluser ping_status=="<<ping_status<<endl;};
                                                                                };
                                                                                #ifdef DEBUG_VERSION
                                                                                cout << "command PING handled in winboard session..." << endl;
                      											        														#endif
                                                                              }
                                                                              else {
																																							  #ifdef DEBUG_VERSION
                                                                                cout << "command "<<lin<<" UNHANDLED in winboard session!!!" << endl;
                      											        														#endif
																																							};
  };
};

#ifdef DEBUG_VERSION
// function used beyond wiboard session to initialize engine
void NO_WINBOARDselfInitialize()
{
  SHOW_WB_THINKING_OUTPUT = true;

  if (setGameMaterial("rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR")) {
    if (setGameSideToMove("w")) {
      if (setGameCastleStatus("KQkq")) {
        if (setGameEnpassantStatus("-")) {
          clearHash();
          setGameZkey(64);
          GameRetract[GMI] = 0;
          printGameBoard(0);
        };
      };
    };
  };
};
#endif //debug_version



