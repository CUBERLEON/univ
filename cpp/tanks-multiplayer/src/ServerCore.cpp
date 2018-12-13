#include "ServerCore.hpp"

#include "World.hpp"
#include "Map.hpp"
#include "Player.hpp"
#include "Tank.hpp"
#include "Input.hpp"
#include "sys/Debug.hpp"

#include <json/json.hpp>
using json = nlohmann::json;

ServerCore::ServerCore(unsigned short port)
: m_port(port), m_isAcceptingNewPlayers(false), m_newPlayersListenerThread(nullptr), m_tcpListener(new sf::TcpListener), m_udpSocket(new sf::UdpSocket)
{
    try {
        m_udpSocket->bind(m_port);
        m_udpSocket->setBlocking(false);
        // m_tcpListener->setBlocking(false);
        
        startAcceptingNewPlayers();
        
        setWorld(new World(new Map(std::string("default"))));
    } catch (const std::exception& e) {
        m_udpSocket->unbind();
        stopAcceptingNewPlayers();
        Debug::error("Couldn't initialize server core");
        throw std::runtime_error(e.what());
    }
}

ServerCore::~ServerCore()
{
    m_udpSocket->unbind();
    stopAcceptingNewPlayers();
    
    for (auto i = m_players.begin(); i != m_players.end();) {
        Player* tmp = i->second;
        i = m_players.erase(i);
        delete tmp;
    }
}

void ServerCore::startAcceptingNewPlayers() {
    if (m_isAcceptingNewPlayers)
        return;
   
   if (m_tcpListener->listen(m_port) != sf::Socket::Done) {
        throw std::runtime_error("Cannot listen to port " + std::to_string(m_port));
        exit(1);
    }
    // m_tcpListener->setBlocking(false);
    // Debug::info("blocking " + std::to_string(m_tcpListener->isBlocking()));
    m_isAcceptingNewPlayers = true;
    m_newPlayersListenerThread = new std::thread(&ServerCore::newPlayersListener, this);
}

void ServerCore::stopAcceptingNewPlayers() {
    if (!m_isAcceptingNewPlayers)
        return;
    
    m_isAcceptingNewPlayers = false;
    m_tcpListener->close();
    m_newPlayersListenerThread->join();
    
    delete m_newPlayersListenerThread;
    delete m_tcpListener;
    m_newPlayersListenerThread = nullptr;
    m_tcpListener = nullptr;
}

void ServerCore::lowRateUpdate(float updateTime) {
    for (auto i = m_players.cbegin(); i != m_players.cend(); ++i) {
        if (!i->second->isConnected()) {
            Debug::info("Player '" + i->first + "' disconnected");
            removePlayer(i->second->getName());
        }
    }
    
    for (auto i = m_players.begin(); i != m_players.end(); ++i) {
        Input* input = i->second->getInput();
        Tank* tank = i->second->getTank();
        if (input->getKeyboardKeyState(Input::Keyboard::A))
            tank->rotate(updateTime*1.5);
        if (input->getKeyboardKeyState(Input::Keyboard::D))
            tank->rotate(-updateTime*1.5);
        if (input->getKeyboardKeyState(Input::Keyboard::W))
            tank->move(rotate<float, float>({0., 1.}, tank->getRot()) * -updateTime*20);
        if (input->getKeyboardKeyState(Input::Keyboard::S))
            tank->move(rotate<float, float>({0., 1.}, tank->getRot()) * updateTime*20);
    }
    
    for (auto i = m_players.begin(); i != m_players.end(); ++i) {
        json r;
        r["type"] = "world";
        r["world"] = m_world->serialize();
        udpSend(r, i->second->getIp(), i->second->getPort());   
    }
}

void ServerCore::highRateUpdate(float updateTime) {
    json r;
    std::string ip;
    unsigned short port;
    if (udpReceive(r, ip, port)) {
        for (auto i = m_players.begin(); i != m_players.end(); ++i) {
            if (i->second->getIp() == ip && i->second->getPort() == port)
                i->second->process(r);
        }
    }
}

void ServerCore::newPlayersListener() {
    srand(time(0));
    while (m_isAcceptingNewPlayers) {
        try {
            sf::TcpSocket* socket = new sf::TcpSocket;
            if (m_tcpListener->accept(*socket) != sf::Socket::Done)
                continue;
                
            char data[TCP_MAX_LENGTH];
            std::size_t received;
            if (socket->receive(data, TCP_MAX_LENGTH, received) != sf::Socket::Done) {
                delete socket;
                throw std::runtime_error("Couldn't receive player's info");
            }   
            
            json playerInfo = json::parse(data);
            if (playerInfo["type"] != "connection") {
                delete socket;
                throw std::runtime_error("Incorrect msg type");
            }
            Player* player = new Player(playerInfo["name"], socket);
            
            if (!addPlayer(player)) {
                delete player;
                throw std::runtime_error("Couldn't add player");
            }
        } catch (const std::exception& r) {
            Debug::error(r.what());
        }
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
    }
}

bool ServerCore::addPlayer(Player* player) {
    if (!m_players.count(player->getName())) {
        if (player->send({{"type", "connection"}, {"status", 1}, {"msg", "Success"}})) {
            
            Tank* tank = new Tank(player->getName(), 10, 5, new Polygon("tank_1"));
            tank->setPos({ rand()%(int)m_world->getMap()->getWidth(), rand()%(int)m_world->getMap()->getHeight() });
            m_world->addTank(tank);
            
            player->setTank(tank);
            m_players[player->getName()] = player;
            
            Debug::info("Player '" + player->getName() + "' connected");
            
            return true;
        }
    } else
        player->send({{"type", "connection"}, {"status", 0}, {"msg", "Player with such name already exists"}});
    return false;
}

void ServerCore::removePlayer(const std::string& name) {
    m_players.erase(m_players.find(name));
    m_world->removeTank(name);
}

bool ServerCore::udpSend(const json& r, const std::string& ip, unsigned short port) {
    std::string data = r.dump();
    if (data.size() + 1 > sf::UdpSocket::MaxDatagramSize)
        return false;
        
    sf::Socket::Status res = m_udpSocket->send(data.c_str(), data.size() + 1, sf::IpAddress(ip), port);
    
    return res == sf::Socket::Done;
}

bool ServerCore::udpReceive(json& r, std::string& ip, unsigned short& port) {
    r.clear();
    sf::Socket::Status res;
    try {
        char data[sf::UdpSocket::MaxDatagramSize];
        std::size_t received;
        sf::IpAddress sender;
        res = m_udpSocket->receive(data, sf::UdpSocket::MaxDatagramSize, received, sender, port);
        
        if (res == sf::Socket::Done) {
            ip = sender.toString();
            r = json::parse(data);
        }
    } catch (const std::exception& e) {
        Debug::error(e.what());
        return false;
    }
    return res == sf::Socket::Done;
}