#include "client.h"

#include <iostream>
#include <chrono>

Client::Client(const std::string& hostName, unsigned int hostPort)
: m_hostName(hostName), m_hostPort(hostPort)
{
    try {
        m_connected = false;
        m_isListening = false;
        
        NL::init();
        
        m_socketGroup = new NL::SocketGroup;       
        m_readCmd = new OnRead(this);
        m_disconnectCmd = new OnDisconnect(this);
        
        m_socketGroup->setCmdOnRead(m_readCmd);
        m_socketGroup->setCmdOnDisconnect(m_disconnectCmd);
    } catch (NL::Exception e) {
        throw std::runtime_error("Couldn't initialize client. " + std::string(e.what()));
    }
}

Client::~Client() {
    stopListening();
    
    delete m_socketGroup;
    
    delete m_readCmd;
    delete m_disconnectCmd;
}

bool Client::connect() {
    try {
        if (m_connected)
            return true;
        
        for (unsigned int i = 0; i < m_socketGroup->size(); ++i)
            m_socketGroup->remove(i);
            
        m_socketGroup->add(new NL::Socket(m_hostName, m_hostPort));
        
        startListening();
            
        m_connected = true;
    } catch (NL::Exception e) {
        m_connected = false;
        throw std::runtime_error("Couldn't connect to server. " + std::string(e.what()));
    } catch (const std::exception& e) {
        m_connected = false;
        throw std::runtime_error("Couldn't connect to server. " + std::string(e.what()));
    } catch (...) {
        m_connected = false;
        throw std::runtime_error("Couldn't connect to server.");
    }
    return m_connected;
}

void Client::disconnect() {
    try {
        if (!m_connected)
            return;
        
        for (unsigned int i = 0; i < m_socketGroup->size(); ++i)
            m_socketGroup->remove(i);
        
        stopListening();
    } catch (NL::Exception e) {
        throw std::runtime_error("An error occured while disconnecting. " + std::string(e.what()));
    } catch (...) {
        throw std::runtime_error("An error occured while disconnecting.");
    }
    m_connected = false;   
}

bool Client::sendMessage(const std::string& text) {
    try {
        if (!m_connected)
            throw std::runtime_error("Cannot send message when client isn't connected!");
        stopListening();
        
        if (text.size() > MAX_MESSAGE_SIZE+1)
            throw std::logic_error("Message length is too large.");
        m_socketGroup->get(0)->send(text.c_str(), text.size()+1);
        
        startListening();
    } catch (NL::Exception e) {
        m_connected = false;
        std::cerr << "Error! Sending message failed. " << e.what() << std::endl;
    } catch (const std::exception& e) {
        m_connected = false;
        std::cerr << "Error! Sending message failed. " << e.what() << std::endl;
    }
    return m_connected;
}

void Client::OnRead::exec(NL::Socket* socket, NL::SocketGroup* group, void* reference) {
    try {
        char buffer[MAX_MESSAGE_SIZE];
        buffer[MAX_MESSAGE_SIZE-1] = '\0';
        socket->read(buffer, MAX_MESSAGE_SIZE-1);
        m_client->m_receivedMessages.push_back(std::string(buffer));
    } catch (...) {
        std::cerr << "Error! Couldn't receive message!" << std::endl;
    }
}

void Client::OnDisconnect::exec(NL::Socket* socket, NL::SocketGroup* group, void* reference) {
    m_client->disconnect();
}

void Client::listener() {
    try {
        while (m_isListening)
            m_socketGroup->listen(10);
            std::this_thread::sleep_for(std::chrono::milliseconds(10));
    } catch (...) {
        m_isListening = false;
    }
}

void Client::startListening() {
    if (!m_isListening) {
        m_isListening = true;
        m_listeningThread = new std::thread(&Client::listener, this);
    }
}

void Client::stopListening() {
    if (m_isListening) {
        m_isListening = false;
        m_listeningThread->join();
        delete m_listeningThread;
    }
}
