#include "Shared.h"

void GameBoard::Write(OutputMemoryStream& inStream) const
{
	inStream.Write(turn);
	for (int i = 0; i < SIZE; i++) {
		for (int j = 0; j < SIZE; j++) {
			inStream.Write(cells[i][j]->state); 
		}
	}
}

void GameBoard::Read(InputMemoryStream& inStream)
{
	inStream.Read(turn);
	for (int i = 0; i < SIZE; i++) {
		for (int j = 0; j < SIZE; j++) {
			inStream.Read(cells[i][j]->state);
		}
	}
}