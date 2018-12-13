#ifndef SERVER_CORE_HPP
#define SERVER_CORE_HPP

#include <map>
#include <string>
#include <thread>
#include <mutex>
#include <SFML/Network.hpp>
#include "Core.hpp"

#define TCP_MAX_LENGTH 20000

class Player;

class ServerCore: public Core {
public:
    ServerCore(unsigned short port);
    virtual ~ServerCore();
    
    void startAcceptingNewPlayers();
    void stopAcceptingNewPlayers();
protected:
    virtual void lowRateUpdate(float updateTime) override;
    virtual void highRateUpdate(float updateTime) override;
private:
    unsigned short m_port;
    
    // std::mutex m_mutex;
    
    std::map<std::string, Player*> m_players;
    bool m_isAcceptingNewPlayers;
    std::thread* m_newPlayersListenerThread;
    sf::TcpListener* m_tcpListener;
    void newPlayersListener();
    
    sf::UdpSocket* m_udpSocket;
    
    bool addPlayer(Player* player);
    void removePlayer(const std::string& name);
    
    bool udpSend(const json& r, const std::string& ip, unsigned short port);
    bool udpReceive(json& r, std::string& ip, unsigned short& port);
};

#endif