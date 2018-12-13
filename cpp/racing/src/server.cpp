#include "server.h"

#include <iostream>
#include <chrono>

Server::Server(unsigned int port)
: m_port(port)
{
    try {
        m_running = false;
        m_isListening = false;
        
        NL::init();
        
        m_socketGroup = new NL::SocketGroup;        
        m_acceptCmd = new OnAccept(this);
        m_readCmd = new OnRead(this);
        m_disconnectCmd = new OnDisconnect(this);
        
        m_socketGroup->setCmdOnAccept(m_acceptCmd);
        m_socketGroup->setCmdOnRead(m_readCmd);
        m_socketGroup->setCmdOnDisconnect(m_disconnectCmd);
    } catch (NL::Exception e) {
        throw std::runtime_error("Couldn't initialize server. " + std::string(e.what()));
    }
}

Server::~Server() {
    stopListening();
    
    delete m_socketGroup;
    
    delete m_acceptCmd;
    delete m_readCmd;
    delete m_disconnectCmd;
}

bool Server::start() {
    try {
        if (m_running)
            return true;
        
        for (unsigned int i = 0; i < m_socketGroup->size(); ++i)
            m_socketGroup->remove(i);
            
        m_socketGroup->add(new NL::Socket(m_port));
        
        startListening();
            
        m_running = true;
    } catch (NL::Exception e) {
        m_running = false;
        throw std::runtime_error("Couldn't start server. " + std::string(e.what()));
    } catch (const std::exception& e) {
        m_running = false;
        throw std::runtime_error("Couldn't start server. " + std::string(e.what()));
    } catch (...) {
        m_running = false;
        throw std::runtime_error("Couldn't start server.");
    }
    return m_running;
}

void Server::stop() {
    try {
        if (!m_running)
            return;
        
        for (unsigned int i = 0; i < m_socketGroup->size(); ++i)
            m_socketGroup->remove(i);
        
        stopListening();
    } catch (NL::Exception e) {
        throw std::runtime_error("An error occured while stopping server. " + std::string(e.what()));
    } catch (...) {
        throw std::runtime_error("An error occured while stopping server.");
    }
    m_running = false;   
}

bool Server::sendMessage(const std::string& text, unsigned index) {
    try {
        if (!m_running)
            throw std::runtime_error("Cannot send message when server isn't running!");
        stopListening();
        
        if (text.size() > MAX_MESSAGE_SIZE+1)
            throw std::logic_error("Message length is too large.");
        m_socketGroup->get(index)->send(text.c_str(), text.size()+1);
             
        startListening();
    } catch (NL::Exception e) {
        m_running = false;
        std::cerr << "Error! Sending message failed. " << e.what() << std::endl;
    } catch (const std::exception& e) {
        m_running = false;
        std::cerr << "Error! Sending message failed. " << e.what() << std::endl;
    }
    return m_running;
}

void Server::OnAccept::exec(NL::Socket* socket, NL::SocketGroup* group, void* reference) {
    try {
        NL::Socket* newConnection = socket->accept();
        group->add(newConnection);
        // std::cout << "Connection " << newConnection->hostTo() << ":" << newConnection->portTo() << " added..." << std::endl;
    } catch (...) {
        std::cerr << "Error! Couldn't add connection!" << std::endl;
    }
}

void Server::OnRead::exec(NL::Socket* socket, NL::SocketGroup* group, void* reference) {
    try {
        char buffer[MAX_MESSAGE_SIZE];
        buffer[MAX_MESSAGE_SIZE-1] = '\0';
        socket->read(buffer, MAX_MESSAGE_SIZE-1);
        m_server->m_receivedMessages.push_back(std::string(buffer));
        // std::cout << "READ -- Message from " << socket->hostTo() << ":" << socket->portTo() << ". Text received: " << buffer << std::endl;
    } catch (...) {
        std::cerr << "Error! Couldn't receive message!" << std::endl;
    }
}

void Server::OnDisconnect::exec(NL::Socket* socket, NL::SocketGroup* group, void* reference) {
    try {
        group->remove(socket);
        // std::cout << "Client " << socket->hostTo() << " disconnected..." << std::endl;
        delete socket;
    } catch (...) {
        std::cerr << "Error! Couldn't dop connection!" << std::endl;
    }
}

void Server::listener() {
    try {
        while (m_isListening)
            m_socketGroup->listen(10);
            std::this_thread::sleep_for(std::chrono::milliseconds(10));
    } catch (...) {
        m_isListening = false;
    }
}

void Server::startListening() {
    if (!m_isListening) {
        m_isListening = true;
        m_listeningThread = new std::thread(&Server::listener, this);
    }
}

void Server::stopListening() {
    if (m_isListening) {
        m_isListening = false;
        m_listeningThread->join();
        delete m_listeningThread;
    }
}
