#include "Shared.h"

void PlayerMove::Write(OutputMemoryStream& inStream) const
{
	inStream.Write(playerType);
	inStream.Write(row);
	inStream.Write(col);
}

void PlayerMove::Read(InputMemoryStream& inStream)
{
	inStream.Read(playerType);
	inStream.Read(row);
	inStream.Read(col);
}