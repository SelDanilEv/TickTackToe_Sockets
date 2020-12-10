#include "Shared.h"

void PlayerMove::Write(OutputMemoryStream& inStream) const
{
	inStream.Write(playerType);
	inStream.Write(col);
	inStream.Write(row);
}

void PlayerMove::Read(InputMemoryStream& inStream)
{
	inStream.Read(playerType);
	inStream.Read(col);
	inStream.Read(row);
}