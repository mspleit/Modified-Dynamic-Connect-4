// Modified dynamic Connect-4 game-playing agent
//

#include "stdafx.h"

#include <iostream>
#include <sstream>
#include <string>
#include <fstream>
#include <math.h>
#include <list>
#include <vector>
#include <ctime>

using namespace std;

//static const int MAX_DEPTH = 6;
static const int ALPHABETA = 1; //Set to 1 to use ALPHABETA, anything else to just use MINMAX
static const int EVALUATOR = 2; //Set to 1 for simple +1,0,-1 evaluator, set to 2 for the better heuristic, 3 is for developmental work
static const int PRINT = 0; //Set to 1 to print percentage of initial move list complete
int MAX_DEPTH=2;

int GetBoard();
int DisplayBoard();
int EvalBoard(string board, int player, int &state);
int MakeBoardCurrent(string best_move);

int GetPlayers();
int GetMove();
int PlayMove(char move[4]);
int CompPlayMove(string parent_board, string &target_board, char* move);

int GenerateMoves(string parent_board, std::vector<string> &child_boards, char player_symbol);
int GetMobility(string board, int i, int j);

string MiniMax(string board, int player);
int MinMove(string board, int player, int depth);
int MaxMove(string board, int player, int depth);

int BoardState(string board, int player);
int DeclareWinner(int state);

float MaxV(string board, int player, int depth, float alpha, float beta);
float MinV(string board, int player, int depth, float alpha, float beta);

float max(float a, float b)
{
	if (a>b)
		return a;
	else
		return b;
}

float min(float a, float b)
{
	if (a<b)
		return a;
	else
		return b;
}

static const int INF = 9999999;
char board_filename[256];
char rootboard[49];
int player[3], currentplayer;
int state;
unsigned long states;
int WIN_FLAG;
clock_t startTime, endTime;
clock_t clockTicksTaken;
double timeInSeconds;
int STOP_FLAG=0;

//int MAX_DEPTH, ALPHABETA, EVALUATOR, PRINT;

int _tmain(int argc, _TCHAR* argv[])
{
	char move[256];
	int score;
	int state=-1;

	//char board_filename[50];
	cout  << "ECSE 526 - Assignment #1\n";
	cout << "Enter filename for initial board setup:\n";
	scanf("%s", board_filename);
	//strcpy(board_filename, "boardc.txt");
//	cout<<"Enter Max Search Depth:\n";
//	scanf("%d", &MAX_DEPTH);
//	cout<<"Enter [0,1] for MiniMax,AlphaBeta:\n";
//	scanf("%d", &ALPHABETA);
//	cout<<"Enter [1,2] for Simple,Personal Evaluator:\n";
//	scanf("%d", &EVALUATOR);
//	cout<<"Enter [0,1] for Printing % Complete:\n";
//	scanf("%d", &PRINT);

	GetBoard();
	GetPlayers();
	currentplayer=1; //1 for white, 2 for black
	string best_move ("                                                  ");
	score=EvalBoard(rootboard, currentplayer, state);
	cout << "\nThe score is: " << score << "\n";
	while(state == -1)
	{
		if (player[currentplayer]==1) // Computer player
		{	
			STOP_FLAG=0;
			startTime=clock();
			states=0;

			best_move = MiniMax(rootboard, currentplayer);

			cout << "\nStates considered: " << states << "\n";
			MakeBoardCurrent(best_move);
			state=BoardState(rootboard, currentplayer);
		}
		else // Human player
		{	
			cout << "Player" << currentplayer << " move: ";
			scanf("%s", move);
			if (strcmp(move, "draw")==0)
			{
				state=0;
			}
			else
			{ 
			    PlayMove(move);				
			}
		}
		
		score=EvalBoard(rootboard, 1, state);
		cout << "\nThe score is: " << score << "\n";
		DisplayBoard();

		if (state!=-1)
		{
			DeclareWinner(state);
			char key;
			std::cin >> key;
			return 0;
		}
		else 
			state=-1;

		//Switch the current player
		if (currentplayer==1)
			currentplayer=2;
		else
			currentplayer=1;
	}
	
	return 0;
}

int DeclareWinner(int state)
{
	if (state==+INF)
		cout << "O (WHITE) WINS!!!\n";
	else if (state==-INF)
		cout << "X (BLACK) WINS!!!\n";
	else
		cout << "DRAW.\n";
	return 0;
}
int MakeBoardCurrent(string best_move)
{	int i,j;
	for (i=0; i<7; i++)
	{	
		for (j=0; j<7; j++)
		{
			rootboard[i+j*7]=best_move[i+j*7];
		}
	}
	return 0;
}

float MaxV(string board, int player, int depth, float alpha, float beta)
{
	int state;
	int score;
	endTime = clock();
	clockTicksTaken = endTime - startTime;
	timeInSeconds = clockTicksTaken / (double) CLOCKS_PER_SEC;
	if (timeInSeconds>18)
		STOP_FLAG=1;
	
	if (depth > MAX_DEPTH || STOP_FLAG==1)
	{	return EvalBoard(board, player, state)/pow(1.10,depth);
	}
    int pos_value = EvalBoard(board, player, state); //Get a value for the current board
	
	if (state != -1)
	{
		return pos_value/pow(1.10,depth);
    }
	std::vector<string> child_boards;
    if (player==1)
		GenerateMoves(board, child_boards, 'O');
	else
		GenerateMoves(board, child_boards, 'X');
	states+=(int)child_boards.size();
	while(!child_boards.empty())
	{
        score = MinV(child_boards.back(), player, depth+1, alpha, beta);	
		child_boards.pop_back();
		alpha = max(alpha, score);
        if (alpha >= beta)
			return alpha/pow(1.10,depth);
    }
	return alpha/pow(1.10,depth);

}
float MinV(string board, int player, int depth, float alpha, float beta)
{
	int state;
	int score;
	endTime = clock();
	clockTicksTaken = endTime - startTime;
	timeInSeconds = clockTicksTaken / (double) CLOCKS_PER_SEC;
	if (timeInSeconds>18)
		STOP_FLAG=1;
	
	if (depth > MAX_DEPTH || STOP_FLAG==1)
        return EvalBoard(board, player, state)/pow(1.10,depth);

    int pos_value = EvalBoard(board, player, state); //Get a value for the current board
	
	if (state != -1)
	{
  		return pos_value/pow(1.10,depth);
    }
	std::vector<string> child_boards;
    if (player==1)
		GenerateMoves(board, child_boards, 'X');
	else
		GenerateMoves(board, child_boards, 'O');
	states+=(int)child_boards.size();
	while(!child_boards.empty())
	{
		score=MaxV(child_boards.back(), player, depth+1, alpha, beta);
		if (score>=1)
			score=score;
		child_boards.pop_back();
		beta = min(beta, score);
        if (beta <= alpha)
			return beta/pow(1.10,depth);
    }
	return beta/pow(1.10,depth);
}

string MiniMax(string board, int player)
{
	int best_val = -INF, index = 0;
	int score, state;
	float numstates, percent_complete;
    std::vector<string> child_boards;
	std::vector<string> best_moves;
	int val;
    WIN_FLAG=0;
	MAX_DEPTH=1;
	
	while (STOP_FLAG==0 && MAX_DEPTH<5)
	{	child_boards.clear();
		MAX_DEPTH+=1;
	if (player==1)
		GenerateMoves(board, child_boards, 'O');
	else
		GenerateMoves(board, child_boards, 'X');

	states+=(int)child_boards.size();
	numstates=states;
	percent_complete=0;
	float alpha=-INF;
	float beta=INF;
    int pos_value;
	
	while(!child_boards.empty() && STOP_FLAG==0) 
	{
		if (ALPHABETA==1)
		{	
			int pos_value = EvalBoard(child_boards.back(), player, state); //Get a value for the current board
			if (state != -1)
			{	STOP_FLAG=1;
  				best_moves.clear();
				best_moves.push_back(child_boards.back()); // Save that move from the list into a "best moves" list	
				return best_moves[index];
			}
			val = MinV(child_boards.back(), player, 2, alpha, beta); //Find the MinMove
			alpha=max(alpha, val);
			if (alpha >= beta)
			{
				if(index > 0) { //If we get here and have more than 1 best move, 
					index = rand() % index; // randomly pick the mofo
				}
				return best_moves[index];
			}
		}
		else
		{	val = MinMove(child_boards.back(), player, 2);
		} //Find the MinMove

        if (val > best_val) //If the MinMove is greater than the best board value so far
		{ 
            best_val = val; // Set the best val equal to the MinMove
            index = 0; // No need to store the list of previous "equal" best moves
			best_moves.clear();
            best_moves.push_back(child_boards.back()); // Save that move from the list into a "best moves" list
        } 
		else if(val == best_val) // If this move is equivalent to the best move found already
		{ 
          	best_moves.push_back(child_boards.back()); //Save it to the array and increase our index by 1
			index++;
			
        }
        child_boards.pop_back(); // Delete the move we just processed
		if (PRINT==1)
		{
			percent_complete+=1;
			cout << (int)(percent_complete/numstates*100) <<" ";
		}
		if (best_val==INF)
			child_boards.clear();

		endTime = clock();
		clockTicksTaken = endTime - startTime;
		timeInSeconds = clockTicksTaken / (double) CLOCKS_PER_SEC;
		if (timeInSeconds>18)
			STOP_FLAG=1;
	}
	}
	if(index > 0) { //If we get here and have more than 1 best move, 
        index = rand() % index; // randomly pick the mofo
    }
    return best_moves[index];
}

// finds best move for 'min player'
int MinMove(string board, int player, int depth)
{
	int state;
	if (depth > MAX_DEPTH)
        return EvalBoard(board, player, state);
    int pos_value = EvalBoard(board, player, state); //Get a value for the current board
//	int state = BoardState(board, player); //Get the current win/loss board state

    if(state != -1) //If the game is over (ie. not -1), return the end-state
	{ 
        return pos_value; //INF for win, -INF for loss, 0 for draw
    }
    int best_val = +INF;
    std::vector<string> child_boards;
    if (player==1)
		GenerateMoves(board, child_boards, 'X');
	else
		GenerateMoves(board, child_boards, 'O');
	states+=(int)child_boards.size();
	
    while(!child_boards.empty()) {
        int val = MaxMove(child_boards.back(), player, depth+1);
        if(val < best_val) {
            best_val = val;
        }
        child_boards.pop_back();
    }
    return best_val-1;
}

// finds best move for 'max player'
int MaxMove(string board, int player, int depth)
{
	if (depth > MAX_DEPTH)
        return EvalBoard(board,player, state);
    int pos_value = EvalBoard(board, player, state); //Get a value for the current board
//	int state = BoardState(board, player); //Get the current win/loss board state
	
	if (state != -1)
	{
        return pos_value;
    }
    int best_val = -INF;
    std::vector<string> child_boards;
    if (player==1)
		GenerateMoves(board, child_boards, 'O');
	else
		GenerateMoves(board, child_boards, 'X');
	states+=(int)child_boards.size();
    while(!child_boards.empty()) {
        int val = MinMove(child_boards.back(), player, depth+1);
        if(val > best_val) {
            best_val = val;
        }
        child_boards.pop_back();
    }
    return best_val-1;
}
// evaluates the current board position
int BoardState(string board, int player)
{
    int i,j;
	int state=-1; //1 for O is winner, 2 for X is winner, 3 for draw
	for (i=0;i<6;i++)
	{
		for (j=0;j<6;j++)
		{
			if (board[i+j*7]==board[i+1+j*7] && board[i+j*7]==board[i+(j+1)*7] && board[i+j*7]==board[i+1+(j+1)*7] && board[i+j*7]=='O')
				state=1;
			if (board[i+j*7]==board[i+1+j*7] && board[i+j*7]==board[i+(j+1)*7] && board[i+j*7]==board[i+1+(j+1)*7] && board[i+j*7]=='X')
				state=2;
		}
	}
	if (state==1)
		return +INF;
	else if (state==2)
		return -INF;
	else
		return -1;
}

int GetMobility(string board, int i, int j)
{//Note that i,j are 1-7 in this case so adjusted by -1 for indexing
	int opponent_sum=0, max_move=0;
	int a=i+j*7;
	//Note that board[i-1+(j-1)*7] is the 'current' cell
	
	max_move=1;
	return max_move;
}

int GenerateMoves(string parent_board, std::vector<string> &child_boards, char player_symbol)
{
	int i, j;
	int n,e,s,w;
	int max_move;
	char move[4];
	string target_board ("                                                  ");
	for (i=1; i<8; i++)
	{
		for (j=1; j<8; j++)
		{
			if (parent_board[i-1+(j-1)*7]==player_symbol)
			{
				max_move=GetMobility(parent_board, i, j);
				
				for (n=1; (n<=max_move) && (j-n>=1); n++)			//Moves to the north
				{
					if (parent_board[i-1+(j-1-n)*7]==' ')
					{	move[0]=i+'0';
						move[1]=j+'0';
						move[2]='N';
						move[3]=n+'0';
						CompPlayMove(parent_board, target_board, move);
						child_boards.push_back(target_board);
					}
					else
						break;
				}
				for (e=1; (e<=max_move) && ((i+e)<8); e++)				//Moves to the east
				{
					if (parent_board[i-1+e+(j-1)*7]==' ')
					{	move[0]=i+'0';
						move[1]=j+'0';
						move[2]='E';
						move[3]=e+'0';
						CompPlayMove(parent_board, target_board, move);
						child_boards.push_back(target_board);
					}
					else
						break;
				}
				for (s=1; (s<=max_move) && (j+s<8); s++)				//Moves to the south
				{
					if (parent_board[i-1+(j-1+s)*7]==' ')
					{	move[0]=i+'0';
						move[1]=j+'0';
						move[2]='S';
						move[3]=s+'0';
						CompPlayMove(parent_board, target_board, move);
						child_boards.push_back(target_board);
					}
					else
						break;
				}
				for (w=1; (w<=max_move) && (i-w>=1); w++)				//Moves to the west
				{
					if (parent_board[i-1-w+(j-1)*7]==' ')
					{	move[0]=i+'0';
						move[1]=j+'0';
						move[2]='W';
						move[3]=w+'0';
						CompPlayMove(parent_board, target_board, move);
						child_boards.push_back(target_board);
					}
					else
						break;
				}
			}
		}
	}
	
	return 0;
}

int EvalBoard(string board, int player, int &state)
{	
	int score=0;
	int i,j, wi=0, bi=0;
	int whites[6][2];
	int blacks[6][2];
	state = -1;

	if (EVALUATOR==1)
	{
		for (i=0;i<6;i++)
		{
			for (j=0;j<6;j++)
			{
				if (board[i+j*7]==board[i+1+j*7] && board[i+j*7]==board[i+(j+1)*7] && board[i+j*7]==board[i+1+(j+1)*7] && board[i+j*7]=='O')
				{	
					if (player==1) 
					{	score=INF;
						state=INF;
					}
					else
					{	score=-INF;
						state=-INF;
					}
				}	
				if (board[i+j*7]==board[i+1+j*7] && board[i+j*7]==board[i+(j+1)*7] && board[i+j*7]==board[i+1+(j+1)*7] && board[i+j*7]=='X')
				{
					if (player==1) 
					{	score=-INF;
						state=-INF;
					}
					else
					{
						score=INF;
						state=INF;
					}
				}
			}
		}
		return score;
	}
	else if (EVALUATOR==2)
	{	
		score=0;
		int whitescore=0, blackscore=0;
		int ww=0, bb=0;
		int runvo=0, runvx=0, runho=0, runhx=0, rundo=0, rundx=0;
		for (i=0; i<7; i++)  //Loop through all squares on the board
		{
			runho=runhx=runvo=runvx=0;
			for (j=0; j<7; j++)
			{	
				if (board[i+j*7]=='O')
				{	runvo+=1;
					if (runvx>0)
						runvx=0;
				}
				else if (board[i+j*7]=='X')
				{	runvx+=1;
					if (runvo>0)
						runvo=0;
				}
				if (board[j+i*7]=='O')
				{	runho+=1;
					if (runhx>0)
						runhx=0;
				}
				else if (board[j+i*7]=='X')
				{	runvx+=1;
					if (runvo>0)
						runho=0;
				}
				
				if ((i+3<7)&& (j+3<7))
				{  if (board[i+j*7]=='O' && board[i+1+(j+1)*7]=='O' && board[i+2+(j+2)*7]=='O' && board[i+3+(j+3)*7]=='O' && player==1)
					score=score+500000;
				   else if (board[i+j*7]=='O' && board[i+1+(j+1)*7]=='O' && board[i+2+(j+2)*7]=='O' && board[i+3+(j+3)*7]=='O' && player==2)
					score=score-500000;
					if (board[i+j*7]=='X' && board[i+1+(j+1)*7]=='X' && board[i+2+(j+2)*7]=='X' && board[i+3+(j+3)*7]=='X' && player==2)
					score=score+500000;
				   else if (board[i+j*7]=='X' && board[i+1+(j+1)*7]=='X' && board[i+2+(j+2)*7]=='X' && board[i+3+(j+3)*7]=='X' && player==1)
					score=score-500000;
				}
				if ((i-3>=0) && (j-3>=0))
				{  if (board[i+j*7]=='O' && board[i-1+(j-1)*7]=='O' && board[i-2+(j-2)*7]=='O' && board[i-3+(j-3)*7]=='O' && player==1)
					score=score+500000;
				   else if (board[i+j*7]=='O' && board[i-1+(j-1)*7]=='O' && board[i-2+(j-2)*7]=='O' && board[i-3+(j-3)*7]=='O' && player==2)
					score=score-500000;
				if (board[i+j*7]=='X' && board[i-1+(j-1)*7]=='X' && board[i-2+(j-2)*7]=='X' && board[i-3+(j-3)*7]=='X' && player==2)
					score=score+500000;
				else if (board[i+j*7]=='X' && board[i-1+(j-1)*7]=='X' && board[i-2+(j-2)*7]=='X' && board[i-3+(j-3)*7]=='X' && player==1)
						score=score-500000;
				}

			}
			if (player==1)
				score=score+10000*(runho+runvo-runhx-runvx);
			else
				score=score-10000*(runho+runvo-runhx-runvx);

		    

			if (runvo>=4)
			{
				if (player==1)
				{	score=500000+score;
					state=INF;
				}
				else 
				{	score=score-500000;
					state=-INF;
				}
			}
			if (runvx>=4)
			{
				if (player==1)
				{	score=score-500000;
					state=-INF;
				}
				else 
				{	score=score+500000;
					state=INF;
				}
			}
			if (runho>=4)
			{
				if (player==1)
				{	score=500000+score;
					state=INF;
				}
				else 
				{	score=score-500000;
					state=-INF;
				}
			}
			if (runhx>=4)
			{
				if (player==1)
				{	score=score-500000;
					state=-INF;
				}
				else 
				{	score=score+500000;
					state=INF;
				}
			}
			
		}

		return score;
	}
	else if (EVALUATOR==3)
	{
		int current_o,current_x,down_o,down_x,right_o,right_x,downright_o, downright_x,sumo,sumx;
		current_o=current_x=down_o=down_x=right_o=right_x=downright_o=downright_x=sumo=sumx=0;
		score=0;
		for (i=0;i<7;i++)
		{
			for (j=0;j<7;j++)
			{
				if (board[i+j*7]=='O' && player==1)
				{	score+=(i*i);
				score=score;}

				if (board[i+j*7]=='X' && player==2)
				{	score+=((6-i)*(6-i));
				score=score;}
			}
		}
		if (player==1)
			return score;
		else
			return score;
	}

}

int CompPlayMove(string parent_board, string &target_board, char* move)
{
	int i,j,dist;
	char dir,a,b;
	i=move[0]-'0';
	j=move[1]-'0';
	dir=move[2];
	dist=move[3]-'0';
	
	int origi=i;
	int origj=j;
		
	int xadd=0, yadd=0;
	if (dir=='N')
	{  xadd=0; yadd=-1;
	}
	if (dir=='E')
	{  xadd=1; yadd=0;
	}
	if (dir=='S')
	{ xadd=0; yadd=1;
	}
	if (dir=='W')
	{ xadd=-1; yadd=0;
	}
	i+=dist*xadd;
	j+=dist*yadd;
	
	for (a=0; a<7; a++)
	{	
		for (b=0; b<7; b++)
		{
			target_board[a+b*7]=parent_board[a+b*7];
		}
	}

	target_board[i-1+(j-1)*7]=target_board[origi-1+(origj-1)*7];	
	target_board[origi-1+(origj-1)*7]=' ';
		
	return 0;
}

int PlayMove(char move[4])
{
	int i,j,dist;
	char dir;
	i=move[0]-'0';
	j=move[1]-'0';
	dir=move[2];
	dist=move[3]-'0';
	int flag=0;
	int origi=i;
	int origj=j;
	printf("\nMove is: %d %d %c %d\n",i, j, dir, dist);
	//Loop through current position of piece to destination position
	//If it is illegal, NOOP
	//To check legal, must do following:
	//Step 1: Validate i,j is owned by current player
	//Step 2: Check surrounding blocks for sum of opponent pieces
	//Step 3: Ensure dist is < max_moveable given the above sum
	//Step 4: Check for pieces between current location and destination location
	//If it is legal, make the move - assign current position to ' ', destination position to piece value
	int opponent_sum=0, max_move=0;
	if (currentplayer==1) // If the current player is white ie. O
	{	printf("Current player is #1, white\n");
		if (rootboard[i-1+(j-1)*7] != 'O')
		{	printf("\nError: That piece isn't owned by you!\n");
			flag=1; //If the specified piece isn't owned by the current player, flag illegal
		}
		if (i>1 && j>1)
			if (rootboard[i-2+(j-2)*7] == 'X') opponent_sum++;
		if (i>0 && j>0)
			if (rootboard[i-1+(j-2)*7] == 'X') opponent_sum++;
		if (i<8 && j>1)
			if (rootboard[i+(j-2)*7] == 'X') opponent_sum++;
		if (i>1 && j>0)
			if (rootboard[i-2+(j-1)*7] == 'X') opponent_sum++;
		if (i<7 && j>0)
			if (rootboard[i+(j-1)*7] == 'X') opponent_sum++;
		if (i>1 && j<7)
			if (rootboard[i-2+(j)*7] == 'X') opponent_sum++;
		if (i>0 && j<7)
			if (rootboard[i-1+(j)*7] == 'X') opponent_sum++;
		if (i<7 && j<7)
			if (rootboard[i+(j)*7] == 'X') opponent_sum++;
		
		if (opponent_sum>=3) max_move=0;
		if (opponent_sum==2) max_move=1;
		if (opponent_sum==1) max_move=2;
		if (opponent_sum==0) max_move=3;
		if (dist>max_move)
		{
			flag=1;
			printf("move distance error\n");
		}
		
		int xadd=0, yadd=0;
		if (dir=='N')
		{  xadd=0; yadd=-1;
		}
		if (dir=='E')
		{  xadd=1; yadd=0;
		}
		if (dir=='S')
		{ xadd=0; yadd=1;
		}
		if (dir=='W')
		{ xadd=-1; yadd=0;
		}

		while (dist-->0)	//Check as many squares as the distance
		{ 
			i+=xadd;
			j+=yadd;
			if (i>0 && j>0 && i<8 && j<8)
			{  if (rootboard[i-1+(j-1)*7]!=' ')
				{	printf("\nError between source and destination\n");
					flag=1;
				}	
			}
			else
				break;
		}
	}
	else  // If the current player is Black ie. X
	{
		if (rootboard[i-1+(j-1)*7] != 'X') flag=1; //If the specified piece isn't owned by the current player, flag illegal
		
		if (i>1 && j>1)
			if (rootboard[i-2+(j-2)*7] == 'O') {
				opponent_sum++;}
		if (i>0 && j>1)
			if (rootboard[i-1+(j-2)*7] == 'O') {
				opponent_sum++;}
		if (i<7 && j>1)
			if (rootboard[i+(j-2)*7] == 'O') {
				opponent_sum++;}
		if (i>1 && j>0)
			if (rootboard[i-2+(j-1)*7] == 'O') {
				opponent_sum++;}
		if (i<7 && j>0)
			if (rootboard[i+(j-1)*7] == 'O') {
				opponent_sum++;}
		if (i>1 && j<7)
			if (rootboard[i-2+(j)*7] == 'O') {
				opponent_sum++;}
		if (i>0 && j<7)
			if (rootboard[i-1+(j)*7] == 'O') {
				opponent_sum++;}
		if (i<7 && j<7)
			if (rootboard[i+(j)*7] == 'O') {
				opponent_sum++;}
		
		if (opponent_sum>=3) max_move=0;
		if (opponent_sum==2) max_move=1;
		if (opponent_sum==1) max_move=2;
		if (opponent_sum==0) max_move=3;
		if (dist>max_move) 
		{	printf("Max Distance achieved!");
			flag=1;
		}
		
		int xadd=0, yadd=0;
		if (dir=='N')
		{  xadd=0; yadd=-1;
		}
		if (dir=='E')
		{  xadd=1; yadd=0;
		}
		if (dir=='S')
		{ xadd=0; yadd=1;
		}
		if (dir=='W')
		{ xadd=-1; yadd=0;
		}

		while (dist-->0)	//Check as many squares as the distance
		{ 
			i+=xadd;
			j+=yadd;
			if (i>0 && j>0 && i<8 && j<8)
			{  printf("Check for blocking at i,j: %d, %d\n", i,j);
				if (rootboard[i-1+(j-1)*7]!=' ')
				{	printf("Blocking error!");
						flag=1;
				}
			}
			else
				break;
		}
	}

	printf("\nFlag: %d\n", flag);
	printf("\nThat piece is i,j,piece: %d,%d,%c,!\n", i,j,rootboard[i-1+(j-1)*7]);
	if (flag==0)
	{
			rootboard[i-1+(j-1)*7]=rootboard[origi-1+(origj-1)*7];	
			rootboard[origi-1+(origj-1)*7]=' ';
			printf("\nUpdating!\n");
	}

	return 0;
}

int GetPlayers()
{
	printf("Player 1, white (1=computer, 2=human): ");
	scanf("%d", &player[1]);
	printf("Player 2, black (1=computer, 2=human): ");
	scanf("%d", &player[2]);
	
	return 0;
}

int GetBoard()
{	
	ifstream infile;
	int i=0;
	char mychar, comma;
    int row=0, col=0;
	infile.open (board_filename, ifstream::in);
    if (infile.is_open())
    {
		for(row=0; row<7; row++)
		{	for(col=0; col<7; col++)
			{
			  infile.get(mychar);
			  infile.get(comma);
			  printf("%c,", mychar);
			  rootboard[i]=mychar;
			  i++;
		    }
			printf("\n");
		}
		infile.close();
    }
    else
    {
		cout << "Error opening file";
    }
	return 0;
}

int DisplayBoard()
{
	ofstream myfile;
	myfile.open ("output.txt",ios::app);
	
	int i=0;
	int row=0, col=0;
	printf(" ");
	myfile << " ";
	for(col=1; col<8; col++)
	{	printf("%d,", col);
		myfile << col << ",";
	}
	printf("\n");
	myfile << "\n";
	for(row=1; row<8; row++)
	{	printf("%d",row);
		myfile << row;
		for(col=0; col<7; col++)
		{
			printf("%c,", rootboard[i]);
			myfile << rootboard[i]<<",";
			i++;
		}
		printf("%d",row);
		printf("\n");
		myfile << row << "\n";
	}
	printf(" ");
	myfile << " ";
	for(col=1; col<8; col++)
	{	printf("%d,", col);
		myfile << col << ",";
	}
	printf("\n");
	myfile << "\n\n";
	myfile.close();
	return 0;
}

