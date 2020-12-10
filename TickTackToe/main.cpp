#include "Shared.h"

//int main()
//{
//    SocketUtil::StaticInit();
//    SocketAddressPtr servAddr = IPAddressFactory::CreateIPv4FromString("127.0.0.1:2000");
//
//    SocketAddress* inAddress = new SocketAddress(servAddr->mSockAddr);
//
//    TCPSocketPtr clientSocket = SocketUtil::CreateTCPSocket(SocketAddressFamily::INET);
//
//    clientSocket->Connect(*inAddress);
//
//    char ibuf[50];
//    int id;
//    clientSocket->Receive(ibuf, strlen(ibuf) + 1);
//    InputMemoryStream inputMS(ibuf, strlen(ibuf) + 1);
//
//    inputMS.Read(id);
//    std::cout << std::endl<< "id = " << id << std::endl;
//
//    system("pause");
//    int i = 0;
//
//    //while (i != 5) {
//
//    //    char obuf[50];
//    //    char ibuf[50];
//
//    //    _itoa_s(++i, obuf, 10);
//
//    //    clientSocket->Send(obuf, strlen(obuf) + 1);
//    //    clientSocket->Receive(ibuf, strlen(ibuf) + 1);
//    //    std::cout << std::endl << ibuf << std::endl;
//    //}
//
//    SocketUtil::CleanUp();
//}