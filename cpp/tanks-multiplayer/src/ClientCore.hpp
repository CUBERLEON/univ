#ifndef CLIENT_CORE_HPP
#define CLIENT_CORE_HPP

#include <string>
#include <thread>
#include "Core.hpp"
#include <SFML/Network.hpp>

#define TCP_MAX_LENGTH 20000

class ClientCore: public Core {
public:
    ClientCore(const std::string& ip, unsigned short port);
    virtual ~ClientCore();
    
    void connect(const std::string& name);
    void disconnect();
protected:
    virtual void lowRateUpdate(float updateTime) override;
    virtual void highRateUpdate(float updateTime) override;
private:
    bool m_isConnected;
    
    std::string m_serverIp;
    unsigned short m_serverPort;
    unsigned short m_localPort;
    
    sf::TcpSocket* m_tcpSocket;
    sf::UdpSocket* m_udpSocket;
    
    std::thread* m_tcpCommunicatorThread;
    bool m_isTcpCommunicatorRunning;
    void startTcpCommunicator();
    void stopTcpCommunicator();
    void tcpCommunicator();
    
    bool send(const json& r);
    bool receive(json& r);
    bool udpSend(const json& r);
    bool udpReceive(json& r);
};

#endif