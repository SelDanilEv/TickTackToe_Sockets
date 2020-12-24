#include "Shared.h"


#pragma region Player
CellState playerType;
#pragma endregion
GameBoard* GB = new GameBoard();


DWORD WINAPI TakeBoard()
{
    UDPSocketPtr serverSocket = SocketUtil::CreateUDPSocket(SocketAddressFamily::INET);

    //SocketAddressPtr servAddr = IPAddressFactory::CreateIPv4FromString("127.0.0.1:2000");

    SocketAddress* inAddress = new SocketAddress((int)2000);

    //while (serverSocket->Bind(*inAddress) != 0) {
    //    std::cout << "Try bind";
    //    Sleep(100);
    //}

    //serverSocket->SetNonBlockingMode(true);

    serverSocket->Bind(*inAddress);

    while (true)
    {
        char segment[5000];
        int dataReceived = serverSocket->ReceiveFrom(segment, 5000, *inAddress);
        if (dataReceived <= 0)
        {
            Sleep(100);
            continue;
        }

        InputMemoryStream inputMS(segment, dataReceived);

        GB->Read(inputMS);

        system("cls");

        std::string output = "You play as ";
        output += GB->GetCharFromCell(playerType);
        output += "\n   A  B  C\n";
        for (int i = 0; i < GB->SIZE; i++) {
            output += std::to_string(i + 1);
            output += "  ";
            for (int j = 0; j < GB->SIZE; j++) {
                output += GB->GetCharFromCell(GB->cells[i][j]->state);
                output += "  ";
            }
            output += '\n';
        }
        output += "\n\t Current turn => ";
        output += GB->GetCharFromCell(GB->turn);
        output += "\n\n";
        output += GB->message;
        output += '\n';
        std::cout << output;
    }

    std::cout << "Finish take" << std::endl;
    return 0;
}


void makeClient() {
    SocketUtil::StaticInit();

#pragma region TakeBoard
    DWORD th1 = NULL;

    HANDLE hTh1 = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)TakeBoard, NULL, 0, &th1);

    ResumeThread(hTh1);
#pragma endregion

    std::string addr;
    std::cout << "Join to server : ";
    std::cin >> addr;

    SocketAddressPtr servAddr = IPAddressFactory::CreateIPv4FromString(addr);  //set here server address

    SocketAddress* inAddress = new SocketAddress(servAddr->mSockAddr);

    TCPSocketPtr clientSocket = SocketUtil::CreateTCPSocket(SocketAddressFamily::INET);

    clientSocket->Connect(*inAddress);



    int32_t id = -1;

#pragma region  ReceivePlayerType
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

    std::cout << std::endl << "id = " << id << std::endl;
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

        if (GB->isGameOver) {
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

        clientSocket->Send(buffer, sz);

        std::cout << "Sended" << std::endl;
    }

    system("pause");

    SocketUtil::CleanUp();
}

int main()
{
    int i = 0;
    std::cout << "Type '0' to create server, or '1' to create client\n";
    std::cin >> i;
    if (i == 1) {
        makeClient();
    }
    else {
        TickTackToeServer server;
        server.DoTCPLoop();
    }
}