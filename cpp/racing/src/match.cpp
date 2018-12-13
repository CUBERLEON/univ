#include <iostream>
#include <vector>
#include "track.h"
#include "client.h"

int main(int argc, char** argv) {
    try {
        if (argc < 2)
            throw std::invalid_argument("There are no passed arguments!");
 
        json data = json::parse(argv[1]);
        int port = (argc >= 3) ? std::atoi(argv[2]) : 5000;
               
        std::vector<Cockroach> c;
        std::vector<Obstacle> o;
        
        for (unsigned int i = 0; i < data["cockroaches"].size(); ++i)
            c.push_back(Cockroach(data["cockroaches"][i]));
        for (unsigned int i = 0; i < data["obstacles"].size(); ++i)
            o.push_back(Obstacle(data["obstacles"][i]));
        
        Track t(data["length"], data["width"]);
        
        t.setCockroaches(c);
        t.setObstacles(o);
        
        t.run();
        
        Client client("127.0.0.1", port);
        client.connect();
        client.sendMessage(t.serialize().dump());
        client.disconnect();
    } catch (const std::exception& e) {
        std::cerr << e.what() << std::endl;
        exit(1);
    }
    
    return 0;
}