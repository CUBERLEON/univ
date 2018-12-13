#include <iostream>
#include <thread>
#include <chrono>
#include <string>
#include <vector>
#include <cstdio>
#include <sstream>
#include <json/json.hpp>
#include "server.h"
#include "utils.h"
#include "cockroach.h"
#include "obstacle.h"

using json=nlohmann::json;

void init();
void process_command(std::string text);
void results_listener();

Server* server;

std::vector<Cockroach> m_cockroaches;
std::vector<Obstacle> m_obstacles;
unsigned m_length, m_width;

int main() {
    try {
        init();
        
        server->start();
        std::thread results_listener_thread(results_listener);

        std::string text;
        while (true) {
            std::cout << ">";
            while (!std::getline(std::cin, text) || text == "");
            
            process_command(text);
            
            std::this_thread::sleep_for(std::chrono::milliseconds(50));
        }
    } catch (std::exception& e) {
        std::cout << e.what() << std::endl;
        exit(1);
    }    
    
    return 0; 
}

void process_command(std::string text) {
    try {
        std::stringstream s(text);
        std::string command, tmp;
        std::vector<std::string> args;
        
        s >> command;
        while (s >> tmp) args.push_back(tmp);
        
        if (command == "exit") {
            server->stop();
            exit(0);
        } else if (command == "run") {
            json data;
            data["length"] = m_length;
            data["width"] = m_width;
            
            for (unsigned i = 0; i < m_cockroaches.size(); ++i)
                data["cockroaches"][i] = m_cockroaches[i].serialize();
            for (unsigned i = 0; i < m_obstacles.size(); ++i)
                data["obstacles"][i] = m_obstacles[i].serialize();
            
            std::string s = data.dump();
            #ifdef _WIN32
                replaceStringInPlace(s, "\"", "\"\"\"");
                system((std::string(".\\match.exe ") + "\"" + s + "\" 5000").c_str());
            #else
                system((std::string("./match ") + "\'" + s + "\' 5000").c_str());
            #endif
        } else if (command == "add") {
            if (args.size() == 0)
                throw std::invalid_argument("There are no parameters. Read 'help' instructions how to use 'add' command.");
            for (unsigned i = 0; i < args.size(); ++i) {
                if (args[i] == "-c") {
                    std::string name, strategy_name;
                    float max_speed, accel, endurance, mass;
                    
                    std::cout << "Name: ";
                    std::cin >> name;
                    std::cout << "Max speed: ";
                    std::cin >> max_speed;
                    std::cout << "Acceleration: ";
                    std::cin >> accel;
                    std::cout << "Endurance: ";
                    std::cin >> endurance;
                    std::cout << "Mass: ";
                    std::cin >> mass;
                    std::cout << "Strategy name: ";
                    std::cin >> strategy_name;
                    
                    m_cockroaches.push_back(Cockroach(name, max_speed, accel, endurance, mass, strategy_name == "" ? "default_strategy" : strategy_name));
                } else if (args[i] == "-o") {
                    std::pair<float, float> pos;
                    float radius, mass;
                    
                    std::cout << "Position (x, y): ";
                    std::cin >> pos.first >> pos.second;
                    std::cout << "Radius: ";
                    std::cin >> radius;
                    std::cout << "Mass: ";
                    std::cin >> mass;
                    
                    m_obstacles.push_back(Obstacle(pos, radius, mass));
                } else
                    throw std::invalid_argument("'" + args[i] + "' is unknown parameter.");
            }
        } else if (command == "set") {
            if (args.size() == 0)
                throw std::invalid_argument("There are no parameters. Read 'help' instructions how to use 'set' command.");
            for (unsigned i = 0; i < args.size(); ++i) {
                if (args[i] == "-l" || args[i] == "-w") {
                    if (i + 1 >= args.size())
                        throw std::invalid_argument("Set value wasn't specified for '" + args[i] + "' parameter.");
                    unsigned val;
                    std::stringstream stream(args[i+1]);
                    if (stream >> val) {
                        if (args[i] == "-l") m_length = val;
                        if (args[i] == "-w") m_width = val;
                    } else {
                        throw std::invalid_argument("Invalid set value for '" + args[i+1] + "' parameter.");
                    }
                    ++i;
                } else
                    throw std::invalid_argument("'" + args[i] + "' is unknown parameter.");
            }
        } else if (command == "get") {
            if (args.size() == 0)
                throw std::invalid_argument("There are no parameters. Read 'help' instructions how to use 'get' command.");
            for (unsigned i = 0; i < args.size(); ++i) {
                if (args[i] == "-l") {
                    std::cout << "Length: " << m_length << std::endl;
                } else if (args[i] == "-w") {
                    std::cout << "Width: " << m_width << std::endl;
                } else if (args[i] == "-c") {
                    std::cout << "-----------COCKROACHES-----------" << std::endl;
                    for (unsigned j = 0; j < m_cockroaches.size(); ++j) {
                        std::cout << "#" << j << std::endl;
                        std::cout << "  Name: " << m_cockroaches[j].getName() << std::endl;
                        std::cout << "  Max speed: " << m_cockroaches[j].getMaxSpeed() << std::endl;
                        std::cout << "  Acceleration: " << m_cockroaches[j].getAccel() << std::endl;
                        std::cout << "  Endurance: " << m_cockroaches[j].getEndurance() << std::endl;
                        std::cout << "  Mass: " << m_cockroaches[j].getMass() << std::endl;
                        std::cout << "  Strategy name: " << m_cockroaches[j].getStrategyName() << std::endl;
                    }
                    std::cout << "---------------------------------" << std::endl;
                } else if (args[i] == "-o") {
                    std::cout << "------------OBSTACLES------------" << std::endl;
                    for (unsigned j = 0; j < m_obstacles.size(); ++j) {
                        std::cout << "#" << j << std::endl;
                        std::cout << "  Position (x, y): (" << m_obstacles[j].getPosX() << ", " << m_obstacles[j].getPosY() << ")" <<  std::endl;
                        std::cout << "  Radius: " << m_obstacles[j].getRadius() << std::endl;
                        std::cout << "  Mass: " << m_obstacles[j].getMass() << std::endl;
                    }
                    std::cout << "---------------------------------" << std::endl;
                } else
                    throw std::invalid_argument("'" + args[i] + "' is unknown parameter.");
            }
        } else if (command == "test") {
            m_cockroaches.push_back(Cockroach("t1", 1.5, 0.05, 30, 8));
            m_cockroaches.push_back(Cockroach("t2", 2, 0.06, 25, 7, "my_strategy"));
            m_cockroaches.push_back(Cockroach("t3", 1.45, 0.04, 40, 11));
            m_cockroaches.push_back(Cockroach("t4", 1.8, 0.09, 23, 6));
            
            m_obstacles.push_back(Obstacle({10, 20}, 7, 0.1));
            m_obstacles.push_back(Obstacle({50, 15}, 15, 0.1));
            m_obstacles.push_back(Obstacle({70, 10}, 13, 0.1));
            m_obstacles.push_back(Obstacle({90, 5}, 7, 0.1));
            m_obstacles.push_back(Obstacle({60, 13}, 9, 0.1));
            m_obstacles.push_back(Obstacle({75, 17}, 11, 0.1));
        } else if (command == "help") {
            std::cout << "Help is currently not available!" << std::endl;
        } else {
            std::cout << "'" << command << "' is unknown command. Please type 'help' for usage instructions." << std::endl;
        }
    } catch (const std::exception& e) {
        std::cout << e.what() << std::endl;
    }
}

void init() {
    m_length = 150, m_width = 30;
    server = new Server(5000);
}

void results_listener() {
    while (true) {
        for (unsigned i = 0; i < server->getInboxSize(); ++i) {
            try {
                json data = json::parse(server->getInbox()[i]);
                if (data["run_finished"]) {
                    std::cout << "----------RACE FINISHED----------" << std::endl;
                    std::cout << "Results:" << std::endl;
                    for (unsigned i = 0; i < data["standings"].size(); ++i) {
                        std::string name = data["world"]["cockroaches"][(int)data["standings"][i]]["name"];
                        std::cout << i+1 << ((i == 0) ? "st" : (i == 1) ? "nd" : (i == 2) ? "rd" : "th") << " - " << name << std::endl; 
                    }
                    std::cout << "---------------------------------" << std::endl;
                }
            } catch (std::exception& e) {
                std::cerr << "Error! " << e.what() << std::endl;
            }
        }
        server->clearInbox();
        
        std::this_thread::sleep_for(std::chrono::milliseconds(50));
    }
}