#include "Shared.h"

int main()
{
    SocketUtil::StaticInit();
    SocketAddressPtr servAddr = IPAddressFactory::CreateIPv4FromString("127.0.0.1:2000");

    SocketAddress* inAddress = new SocketAddress(servAddr->mSockAddr);

    TCPSocketPtr clientSocket = SocketUtil::CreateTCPSocket(SocketAddressFamily::INET);

    clientSocket->Connect(*inAddress);

#pragma region Player
    CellState playerType;
#pragma endregion

    int32_t id = -1;

#pragma region  Receive
    void* temp = std::malloc(kMaxPacketSize);
    if (nullptr != temp)
    {
        char* temporaryBuffer = static_cast<char*>(temp);

        int receivedByteCount = clientSocket->Receive(temporaryBuffer, kMaxPacketSize);

        if (receivedByteCount > 0)
        {
            InputMemoryStream stream(temporaryBuffer, static_cast<uint32_t>(receivedByteCount));
            stream.Read(id);
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
#pragma endregion

    std::cout << std::endl<< "id = " << id << std::endl;
    switch (id)
    {
    case 0:
        playerType = CROSS;
        break;
    case 1:
        playerType = ZERO;
        break;
    default:
        playerType = EMPTY;
        break;
    }


    PlayerMove pm(playerType);

    while (true) {
        char obuf[50];

        std::cin >> obuf;


        if (playerType == EMPTY) {
            continue;
        }

        if (strlen(obuf) != 2) {
            continue;
        }

        int col;
        switch (obuf[0])
        {
        case 'A':
        case 'a':
            col = 0;
            break;
        case 'B':
        case 'b':
            col = 1;
            break;
        case 'C':
        case 'c':
            col = 2;
            break;
        default:
            continue;
            break;
        }
        int row;
        switch (obuf[1])
        {
        case '1':
            row = 0;
            break;
        case '2':
            row = 1;
            break;
        case '3':
            row = 2;
            break;
        default:
            continue;
            break;
        }
        pm.col = col;
        pm.row = row;

        OutputMemoryStream stream;
        pm.Write(stream);
        const char* buffer = stream.GetBufferPtr();
        uint32_t sz = stream.GetLength();

        //clientSocket->Send(buffer, stream.GetLength());
        std::cout << "Sended" << std::endl;
    }

    system("pause");

    SocketUtil::CleanUp();
}