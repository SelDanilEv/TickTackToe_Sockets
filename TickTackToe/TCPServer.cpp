#include "Shared.h"
#include <cstdlib>

GameBoard* mainGameBoard = new GameBoard();

DWORD WINAPI SendBoard()
{
    SocketUtil::StaticInit();

    SocketAddress* inAddress = new SocketAddress(2000, true);

    UDPSocketPtr serverSocket = SocketUtil::CreateUDPSocketForAll(SocketAddressFamily::INET);

    for (int i = 0; i < 3; i++)
    {
        OutputMemoryStream stream;
        mainGameBoard->Write(stream);
        const char* buffer = stream.GetBufferPtr();
        uint32_t sz = stream.GetLength();

        serverSocket->SendTo(buffer, stream.GetLength(), *inAddress);

        std::cout << "Send ";
        for (int i = 0; i < sz; i++) {
            std::cout << (int)buffer[i];
        }
        std::cout << std::endl;
        Sleep(10);
    }
    return 0;
}

TickTackToeServer::TickTackToeServer()
{
}

TickTackToeServer::~TickTackToeServer()
{
}

void startSendBoard() {
#pragma region SendBoard
    DWORD th1 = NULL;

    HANDLE hTh1 = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)SendBoard, NULL, 0, &th1);

    ResumeThread(hTh1);
#pragma endregion
}

void TickTackToeServer::DoTCPLoop()
{
    SocketUtil::StaticInit();

    const int GOOD_SEGMENT_SIZE = 5000;

    TCPSocketPtr listenSocket = SocketUtil::CreateTCPSocket(SocketAddressFamily::INET);

    std::string addr;
    std::cout << "Create a server : ";
    std::cin >> addr;

    SocketAddressPtr servAddr = IPAddressFactory::CreateIPv4FromString(addr);  //set here server address

    SocketAddress* inAddress = new SocketAddress(servAddr->mSockAddr);

    if (listenSocket->Bind(*inAddress) != NO_ERROR)
    {
        return;
    }
    vector<TCPSocketPtr> readBlockSockets;
    listenSocket->Listen(32);
    readBlockSockets.push_back(listenSocket);
    vector<TCPSocketPtr> readableSockets;
    while (gIsGameRunning)
    {
        if (SocketUtil::Select(&readBlockSockets, &readableSockets,
            nullptr, nullptr,
            nullptr, nullptr))
        {
            for (const TCPSocketPtr& socket : readableSockets)
            {
                if (socket == listenSocket)
                {
                    SocketAddress newClientAddress;
                    auto newSocket = listenSocket->Accept(newClientAddress);
                    readBlockSockets.push_back(newSocket);
                    ProcessNewClient(newSocket, newClientAddress);
                }
                else
                {
                    char segment[GOOD_SEGMENT_SIZE];
                    int dataReceived = socket->Receive(segment, GOOD_SEGMENT_SIZE);
                    if (dataReceived > 0)
                    {
                        ProcessDataFromClient(socket, segment,
                            dataReceived);
                    } 
                    else
                    {
                        readBlockSockets.erase(std::remove(readBlockSockets.begin(), readBlockSockets.end(), socket));
                    }
                }
            }
        }
    }
}


int32_t PlayerCounter = 0;
void TickTackToeServer::ProcessNewClient(TCPSocketPtr socket, SocketAddress address)
{
    OutputMemoryStream stream;
    stream.Write(PlayerCounter++);
    const char* buffer = stream.GetBufferPtr();
    uint32_t sz = stream.GetLength();

    socket->Send(buffer, stream.GetLength());
    std::cout << "New client" << std::endl;
    startSendBoard();
}

int isGameFinished() {
    if (mainGameBoard->cells[0][0]->state == mainGameBoard->cells[1][0]->state && mainGameBoard->cells[1][0]->state == mainGameBoard->cells[2][0]->state && mainGameBoard->cells[2][0]->state != EMPTY)
        return 1;
    else if (mainGameBoard->cells[0][1]->state == mainGameBoard->cells[1][1]->state && mainGameBoard->cells[1][1]->state == mainGameBoard->cells[2][1]->state && mainGameBoard->cells[2][1]->state != EMPTY)
        return 1;
    else if (mainGameBoard->cells[0][2]->state == mainGameBoard->cells[1][2]->state && mainGameBoard->cells[1][2]->state == mainGameBoard->cells[2][2]->state && mainGameBoard->cells[2][2]->state != EMPTY)
        return 1;
    else if (mainGameBoard->cells[0][0]->state == mainGameBoard->cells[0][1]->state && mainGameBoard->cells[0][1]->state == mainGameBoard->cells[0][2]->state && mainGameBoard->cells[0][2]->state != EMPTY)
        return 1;
    else if (mainGameBoard->cells[1][0]->state == mainGameBoard->cells[1][1]->state && mainGameBoard->cells[1][1]->state == mainGameBoard->cells[1][2]->state && mainGameBoard->cells[1][2]->state != EMPTY)
        return 1;
    else if (mainGameBoard->cells[2][0]->state == mainGameBoard->cells[2][1]->state && mainGameBoard->cells[2][1]->state == mainGameBoard->cells[2][2]->state && mainGameBoard->cells[2][2]->state != EMPTY)
        return 1;
    else if (mainGameBoard->cells[0][0]->state == mainGameBoard->cells[1][1]->state && mainGameBoard->cells[1][1]->state == mainGameBoard->cells[2][2]->state && mainGameBoard->cells[2][2]->state != EMPTY)
        return 1;
    else if (mainGameBoard->cells[2][0]->state == mainGameBoard->cells[1][1]->state && mainGameBoard->cells[1][1]->state == mainGameBoard->cells[0][2]->state && mainGameBoard->cells[2][0]->state != EMPTY)
        return 1;

    for (int i = 0; i < mainGameBoard->SIZE; i++) {
        for (int j = 0; j < mainGameBoard->SIZE; j++) {
            if (mainGameBoard->cells[i][j]->state == EMPTY) {
                return 0;
            }
        }
    }

    return 2;
}

void TickTackToeServer::ProcessDataFromClient(TCPSocketPtr socket, char* data, int dataLen)
{
    PlayerMove* pM = new PlayerMove();

    InputMemoryStream inputMS(data, dataLen);

    pM->Read(inputMS);

    if (mainGameBoard->turn != pM->playerType) {
        return;
    }

    if (mainGameBoard->cells[pM->col][pM->row]->state != EMPTY) {
        return;
    }

    mainGameBoard->cells[pM->col][pM->row]->state = pM->playerType;

    std::cout << pM->playerType << pM->col << pM->row << std::endl;


    mainGameBoard->isGameOver = true;
    switch (isGameFinished())
    {
    case 1:
        mainGameBoard->message += "Winner ";
        mainGameBoard->message += mainGameBoard->GetCharFromCell(mainGameBoard->turn);
        break;
    case 2:
        mainGameBoard->message += "Draw";
        break;
    default:
        mainGameBoard->isGameOver = false;
        break;
    }

    mainGameBoard->ChangeTurn();

    startSendBoard();
}