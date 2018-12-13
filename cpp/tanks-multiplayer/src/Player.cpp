#include "Player.hpp"

#include "Input.hpp"
#include "World.hpp"
#include "Tank.hpp"
#include "sys/Debug.hpp"

Player::Player(const std::string& name, sf::TcpSocket* socket)
: m_name(name), m_input(new Input()), m_tank(nullptr),
  m_ip(socket->getRemoteAddress().toString()), m_port(socket->getRemotePort()), m_tcpSocket(socket), m_isConnected(true), m_tcpCommunicatorThread(nullptr), m_isTcpCommunicatorRunning(false)
{
    m_isTcpCommunicatorRunning = true;
    m_tcpCommunicatorThread = new std::thread(&Player::tcpCommunicator, this);
}

Player::~Player()
{
    m_tcpSocket->disconnect();
    m_isTcpCommunicatorRunning = false;
    m_tcpCommunicatorThread->join();
    delete m_tcpCommunicatorThread;
    delete m_input; 
    delete m_tcpSocket;
}

Input* Player::getInput() const {
    return m_input;
}

Tank* Player::getTank() const {
    return m_tank;
}

void Player::setTank(Tank* tank) {
    m_tank = tank;
}

void Player::process(const json& r) {
    try {
        if (r["type"] == "input") {
            m_input->deserialize(r["input"]);
        }
    } catch (const std::exception& e) {
        Debug::error(e.what());
    }
}

bool Player::send(const json& r) {
    std::string data = r.dump();
    if (data.size() + 1 > TCP_MAX_LENGTH)
        return false;
    sf::Socket::Status res = m_tcpSocket->send(data.c_str(), data.size() + 1);
    
    if (res == sf::Socket::Disconnected)
        m_isConnected = false;
    
    return res == sf::Socket::Done;
}

bool Player::receive(json& r) {
    r.clear();
    sf::Socket::Status res;
    try {
        char data[TCP_MAX_LENGTH];
        std::size_t received;
        res = m_tcpSocket->receive(data, TCP_MAX_LENGTH, received);
        
        if (res == sf::Socket::Disconnected)
            m_isConnected = false;
        if (res == sf::Socket::Done)
            r = json::parse(data);
    } catch (const std::exception& e) {
        Debug::error(e.what());
        return false;
    }
    return res == sf::Socket::Done;
}

void Player::tcpCommunicator() {
    while (m_isTcpCommunicatorRunning) {
        send({{"type", "test"}});
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
    }
}