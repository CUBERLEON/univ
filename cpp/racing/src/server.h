#ifndef SERVER_H
#define SERVER_H

#include <string>
#include <thread>
#include <algorithm>
#include <netlink/socket.h>
#include <netlink/socket_group.h>

#define MAX_MESSAGE_SIZE 32768

class Server
{
public:
    Server(unsigned int port);
    ~Server();
    
    bool start();
    void stop();
    
    bool isRunning() const { return m_running; }
    unsigned getPort() const { return m_port; }
    unsigned getConnectionsCnt() const { return std::max(0, (int)m_socketGroup->size()-1); }
    
    bool sendMessage(const std::string& text, unsigned index);
    unsigned int getInboxSize() const { return m_receivedMessages.size(); }
    std::vector<std::string> getInbox() const { return m_receivedMessages; }
    void clearInbox() { m_receivedMessages.clear(); }
protected:
private:
    class OnAccept: public NL::SocketGroupCmd {
        public: OnAccept(Server* client) : m_server(client) {} virtual ~OnAccept() {} void exec(NL::Socket* socket, NL::SocketGroup* group, void* reference);
        private: Server* m_server;
    }; friend OnAccept;
    class OnRead: public NL::SocketGroupCmd {
        public: OnRead(Server* client) : m_server(client) {} virtual ~OnRead() {} void exec(NL::Socket* socket, NL::SocketGroup* group, void* reference);
        private: Server* m_server;
    }; friend OnRead;
    class OnDisconnect: public NL::SocketGroupCmd {
        public: OnDisconnect(Server* client) : m_server(client) {} virtual ~OnDisconnect() {}  void exec(NL::Socket* socket, NL::SocketGroup* group, void* reference);
        private: Server* m_server;
    }; friend OnDisconnect;
    
    std::vector<std::string> m_receivedMessages;

    void listener();
    void startListening();
    void stopListening();
    bool m_isListening;
    std::thread* m_listeningThread;

    bool m_running;
    const unsigned int m_port;
    
    NL::SocketGroup* m_socketGroup;
    OnAccept* m_acceptCmd;
    OnRead* m_readCmd;
    OnDisconnect* m_disconnectCmd;
};

#endif