#pragma once
class TickTackToeServer
{
public:
	TickTackToeServer();
	~TickTackToeServer();

	void DoTCPLoop();
private:
	bool gIsGameRunning = true;
	void ProcessNewClient(TCPSocketPtr socket, SocketAddress address);
	void ProcessDataFromClient(TCPSocketPtr socket, char* data, int dataLen);
};