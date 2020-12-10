#include "Shared.h"

int main()
{
    TCPServer server;

    server.DoTCPLoop();
}

//int main()
//{
//    SocketUtil::StaticInit();
//    SocketAddressPtr servAddr = IPAddressFactory::CreateIPv4FromString("10.0.2.15:2000");
//
//    SocketAddress* inAddress = new SocketAddress(servAddr->mSockAddr);
//
//    TCPSocketPtr clientSocket = SocketUtil::CreateTCPSocket(SocketAddressFamily::INET);
//    int ooo = clientSocket->Connect(*inAddress);
//
//    if (ooo < 0) {
//        return 0;
//    }
//
//    TestClass* test = new TestClass(1, 1, "name");
//
//    SendTestClass(clientSocket, test);
//
//    system("pause");
//
//    SocketUtil::CleanUp();
//}