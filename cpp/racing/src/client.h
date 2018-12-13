#ifndef CLIENT_H
#define CLIENT_H

#include <string>
#include <thread>
#include <netlink/socket.h>
#include <netlink/socket_group.h>

#define MAX_MESSAGE_SIZE 32768

class Client
{
public:
    Client(const std::string& hostName, unsigned int hostPort);
    ~Client();
    
    bool connect();
    void disconnect();
    
    bool isConnected() const { return m_connected; }
    std::string getHostName() const { return m_hostName; }
    unsigned getHostPort() const { return m_hostPort; }
    
    bool sendMessage(const std::string& text);
    unsigned int getInboxSize() const { return m_receivedMessages.size(); }
    std::vector<std::string> getInbox() const { return m_receivedMessages; }
    void clearInbox() { m_receivedMessages.clear(); }
protected:
private:
    class OnRead: public NL::SocketGroupCmd {
        public: OnRead(Client* client) : m_client(client) {} virtual ~OnRead() {} void exec(NL::Socket* socket, NL::SocketGroup* group, void* reference);
        private: Client* m_client;
    }; friend OnRead;
    class OnDisconnect: public NL::SocketGroupCmd {
        public: OnDisconnect(Client* client) : m_client(client) {} virtual ~OnDisconnect() {}  void exec(NL::Socket* socket, NL::SocketGroup* group, void* reference);
        private: Client* m_client;
    }; friend OnDisconnect;
    
    std::vector<std::string> m_receivedMessages;
    
    void listener();
    bool m_isListening;
    void startListening();
    void stopListening();
    std::thread* m_listeningThread;

    bool m_connected;
    const std::string m_hostName;
    const unsigned int m_hostPort;
    
    NL::SocketGroup* m_socketGroup;
    OnRead* m_readCmd;
    OnDisconnect* m_disconnectCmd;
};

#endif