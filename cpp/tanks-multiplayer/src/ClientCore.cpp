#include "ClientCore.hpp"

#include "Renderer.hpp"
#include "Input.hpp"
#include "sys/Debug.hpp"
#include "World.hpp"
#include "Map.hpp"

ClientCore::ClientCore(const std::string& ip, unsigned short port)
: m_isConnected(false), m_serverIp(ip), m_serverPort(port), m_localPort(0),
  m_tcpSocket(new sf::TcpSocket()), m_udpSocket(new sf::UdpSocket()), m_tcpCommunicatorThread(nullptr), m_isTcpCommunicatorRunning(false)
{
    m_udpSocket->setBlocking(false);
}

ClientCore::~ClientCore()
{
    m_tcpSocket->disconnect();
    m_udpSocket->unbind();
    stopTcpCommunicator();
    delete m_tcpSocket;
    delete m_udpSocket;
}

void ClientCore::lowRateUpdate(float updateTime) {
    udpSend({{"type", "input"}, {"input", getRenderer()->getInput()->serialize()}});
}

void ClientCore::highRateUpdate(float updateTime) {
    if (!m_isConnected) {
        m_udpSocket->unbind();
        stopTcpCommunicator();
        throw std::runtime_error("Lost connection");
    }
    
    json r;
    if (udpReceive(r)) {
        try {
            if (r["type"] == "world")
                setWorld(r["world"]);
        }
        catch (const std::exception& e) {
            Debug::error(e.what());
        }
    }
}

void ClientCore::connect(const std::string& name) {
    sf::Socket::Status status = m_tcpSocket->connect(sf::IpAddress(m_serverIp), m_serverPort, sf::seconds(3));
    if (status != sf::Socket::Done) {
        throw std::runtime_error("Couldn't connect to server " + m_serverIp + ":" + std::to_string(m_serverPort));
        exit(1);
    }
    m_localPort = m_tcpSocket->getLocalPort();
    m_udpSocket->bind(m_localPort);
    
    send({{"type", "connection"}, {"name", name}});
    json respose;
    while (receive(respose) && respose["type"] != "connection");
    
    if (respose["status"].get<int>())
        Debug::info("Connection is established");
    else
        throw std::runtime_error(respose["msg"].get<std::string>());
    
    startTcpCommunicator();
    
    m_isConnected = true;
}

void ClientCore::disconnect() {
    m_isConnected = false;
    m_tcpSocket->disconnect();
    m_udpSocket->unbind();
    stopTcpCommunicator();
}

void ClientCore::startTcpCommunicator() {
    if (m_isTcpCommunicatorRunning)
        return;
    
    m_isTcpCommunicatorRunning = true;
    m_tcpCommunicatorThread = new std::thread(&ClientCore::tcpCommunicator, this);
}

void ClientCore::stopTcpCommunicator() {
    if (m_tcpCommunicatorThread) {
        m_isTcpCommunicatorRunning = false;
        m_tcpCommunicatorThread->join();
        delete m_tcpCommunicatorThread;
        m_tcpCommunicatorThread = nullptr;
    }
}

void ClientCore::tcpCommunicator() {
    json r;
    while (m_isTcpCommunicatorRunning) {
        if (receive(r)) {
            
        }
        std::this_thread::sleep_for(std::chrono::milliseconds(10));
    }
    m_isTcpCommunicatorRunning = false;
}

bool ClientCore::send(const json& r) {
    std::string data = r.dump();
    if (data.size() + 1 > TCP_MAX_LENGTH)
        return false;
    sf::Socket::Status res = m_tcpSocket->send(data.c_str(), data.size() + 1);
    
    if (res == sf::Socket::Disconnected)
        m_isConnected = false;
        
    return res == sf::Socket::Done;
}

bool ClientCore::receive(json& r) {
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

bool ClientCore::udpSend(const json& r) {
    std::string data = r.dump();
    if (data.size() + 1 > sf::UdpSocket::MaxDatagramSize)
        return false;
    sf::Socket::Status res = m_udpSocket->send(data.c_str(), data.size() + 1, sf::IpAddress(m_serverIp), m_serverPort);
    
    if (res == sf::Socket::Disconnected)
        m_isConnected = false;
        
    return res == sf::Socket::Done;
}

bool ClientCore::udpReceive(json& r) {
    r.clear();
    sf::Socket::Status res;
    try {
        char data[sf::UdpSocket::MaxDatagramSize];
        std::size_t received;
        sf::IpAddress sender;
        unsigned short port;
        res = m_udpSocket->receive(data, sf::UdpSocket::MaxDatagramSize, received, sender, port);
        
        if (sender.toString() != m_serverIp || port != m_serverPort)
            return false;
        
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