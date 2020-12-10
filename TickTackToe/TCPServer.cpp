#include "Shared.h"
#include <cstdlib>

TCPServer::TCPServer()
{
}

TCPServer::~TCPServer()
{
}

void TCPServer::DoTCPLoop()
{
    const int GOOD_SEGMENT_SIZE = 5000;

    SocketUtil::StaticInit();

    TCPSocketPtr listenSocket = SocketUtil::CreateTCPSocket(SocketAddressFamily::INET);

    SocketAddressPtr servAddr = IPAddressFactory::CreateIPv4FromString("127.0.0.1:2000");

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
                    // это обычный сокет — обработать данные...
                    char segment[GOOD_SEGMENT_SIZE];
                    int dataReceived = socket->Receive(segment, GOOD_SEGMENT_SIZE);
                    if (dataReceived > 0)
                    {
                        ProcessDataFromClient(socket, segment,
                            dataReceived);
                    }
                }
            }
        }
    }
}


int PlayerCounter = 1;
void TCPServer::ProcessNewClient(TCPSocketPtr socket, SocketAddress address)
{
    OutputMemoryStream stream;
    stream.Write(PlayerCounter++);
    const char* buffer = stream.GetBufferPtr();
    uint32_t sz = stream.GetLength();

    socket->Send(&buffer, stream.GetLength());
    std::cout << "New client" << std::endl;
}

void TCPServer::ProcessDataFromClient(TCPSocketPtr socket, char* data, int dataLen)
{
    GameBoard* board = new GameBoard();

    //TestClass* test = new TestClass();

    InputMemoryStream inputMS(data, dataLen);

    board->Read(inputMS);
    ////ReceiveTestClass(socket, test);

    ////string tmp = std::to_string((int)socket->mSocket);
    ////char const* ptmp = tmp.c_str();
    ////size_t newStrLen = dataLen + strlen(ptmp) + strlen(" - ") + 1;

    ////strcat_s(data, newStrLen, " - ");
    ////strcat_s(data, newStrLen, ptmp);

    //std::cout << "Output: " << test->GetName() << std::endl;

    ////socket->Send(data, strlen(data) + 1);
}