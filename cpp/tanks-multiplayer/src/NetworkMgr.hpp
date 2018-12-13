#ifndef NETWORK_MGR_HPP
#define NETWORK_MGR_HPP

class NetworkMgr {
public:
    virtual ~NetworkMgr();
    
    // virtual void sendToPlayer(const std::string& data, const Player* player) = 0;
    // virtual std::string receiveFromPlayer(const Player* player) = 0;
protected:
    NetworkMgr();
private:
};

#endif