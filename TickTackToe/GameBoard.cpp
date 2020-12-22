#include "Shared.h"

void GameBoard::Write(OutputMemoryStream& inStream) const
{
	inStream.Write(turn);
	for (int i = 0; i < SIZE; i++) {
		for (int j = 0; j < SIZE; j++) {
			inStream.Write(cells[j][i]->state); 
		}
	}
	inStream.Write(message);
	inStream.Write(isGameOver);
}

void GameBoard::Read(InputMemoryStream& inStream)
{
	inStream.Read(turn);
	for (int i = 0; i < SIZE; i++) {
		for (int j = 0; j < SIZE; j++) {
			inStream.Read(cells[i][j]->state);
		}
	}
	inStream.Read(message);
	inStream.Read(isGameOver);
}

void  GameBoard::ChangeTurn()
{	
	switch (turn)
	{
	case CROSS:
		turn = ZERO;
		break;
	case ZERO:
		turn = CROSS;
		break;
	default:
		break;
	}
}