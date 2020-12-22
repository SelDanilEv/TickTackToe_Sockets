class SocketAddress
{
public:
    SocketAddress(uint32_t inAddress, uint16_t inPort)
    {
        GetAsSockAddrIn()->sin_family = AF_INET;
        GetAsSockAddrIn()->sin_addr.S_un.S_addr = htonl(inAddress);
        GetAsSockAddrIn()->sin_port = htons(inPort);
    }

    SocketAddress(uint16_t inPort)//if only port it is broadcast
    {
        GetAsSockAddrIn()->sin_family = AF_INET;
        GetAsSockAddrIn()->sin_addr.S_un.S_addr = INADDR_BROADCAST;
        GetAsSockAddrIn()->sin_port = htons(inPort);
    }

    SocketAddress(int inPort)//if only port it is broadcast
    {
        GetAsSockAddrIn()->sin_family = AF_INET;
        GetAsSockAddrIn()->sin_addr.S_un.S_addr = INADDR_ANY;
        GetAsSockAddrIn()->sin_port = htons(inPort);
    }


    SocketAddress(const sockaddr& inSockAddr)
    {
        memcpy(&mSockAddr, &inSockAddr, sizeof(sockaddr));
    }
    
    SocketAddress(){}
    size_t GetSize() const { return sizeof(sockaddr); }
    // Was private
    sockaddr mSockAddr;
private:
    sockaddr_in* GetAsSockAddrIn()
    {
        return reinterpret_cast<sockaddr_in*>(&mSockAddr);
    }
};

typedef shared_ptr<SocketAddress> SocketAddressPtr;
