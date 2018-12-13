#ifndef PLAYER_HPP
#define PLAYER_HPP

#include <string>
#include <thread>
#include <SFML/Network.hpp>
#include <json/json.hpp>
using json = nlohmann::json;

#define TCP_MAX_LENGTH 20000

class Input;
class World;
class Tank;

class Player {
public:
    Player(const std::string& name, sf::TcpSocket* socket);
    ~Player();
    
    const std::string& getName() const { return m_name; }
    Input* getInput() const;
    Tank* getTank() const;
    void setTank(Tank* tank);
    
    const std::string& getIp() const { return m_ip; }
    unsigned short getPort() const { return m_port; }
    bool isConnected() const { return m_isConnected; }
    
    void process(const json& r);
    
    bool send(const json& r);
    bool receive(json& r);
protected:
private:
    std::string m_name;
    Input* m_input;
    Tank* m_tank;

    std::string m_ip;
    unsigned short m_port;
    
    sf::TcpSocket* m_tcpSocket;
    
    bool m_isConnected;
    
    std::thread* m_tcpCommunicatorThread;
    bool m_isTcpCommunicatorRunning;
    void tcpCommunicator();
};

#endif