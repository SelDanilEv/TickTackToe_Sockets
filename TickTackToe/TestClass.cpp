#include "Shared.h"

void TestClass::Write(OutputMemoryStream& inStream) const
{
	inStream.Write(mHealth);
	inStream.Write(mMeowCount);
	inStream.Write(mName);
}

void TestClass::Read(InputMemoryStream& inStream)
{
	inStream.Read(mHealth);
	inStream.Read(mMeowCount);
	inStream.Read(mName);
}

void SendTestClass(TCPSocketPtr inSocket, TestClass* inTest)
{
	OutputMemoryStream stream;
	inTest->Write(stream);
	const char* buffer = stream.GetBufferPtr();
	uint32_t sz = stream.GetLength();

	inSocket->Send(&buffer, stream.GetLength());
}

void ReceiveTestClass(TCPSocketPtr inSocket, TestClass* outTest)
{
	void* temp = std::malloc(kMaxPacketSize);
	if (nullptr != temp)
	{
		char* temporaryBuffer = static_cast<char*>(temp);

		int receivedByteCount =  inSocket->Receive(temporaryBuffer, kMaxPacketSize);

		if (receivedByteCount > 0)
		{
			InputMemoryStream stream(temporaryBuffer, static_cast<uint32_t>(receivedByteCount));
			outTest->Read(stream);
		}
		else
		{
			std::free(temporaryBuffer);
		}
	}
	else
	{
		throw std::bad_alloc();
	}
}
