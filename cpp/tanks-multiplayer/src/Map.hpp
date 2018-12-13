#ifndef MAP_HPP
#define MAP_HPP

#include <vector>
#include <string>
#include <map>
#include "sys/IDrawable.hpp"
#include "sys/ISerializable.hpp"

class Block;
class Renderer;

class Map : public IDrawable, public ISerializable {
public:
    Map(const Map& r);
    // Map(float width, float height);
    Map(const std::string& fileName);
    Map(const json& r);
    virtual ~Map();
    
    float getWidth() const { return m_width; }
    float getHeight() const { return m_height; }
    const std::vector<Block*>& getBlocks() const { return m_blocks; }
    const std::vector< std::pair<float, float> >& getSpawns() const { return m_spawns; }
    const std::string& getFileName() const { return m_fileName; }
    
    void addBlock(Block* block) { m_blocks.push_back(block); }
    void addSpawn(const std::pair<float, float>& spawn) { m_spawns.push_back(spawn); }
    void setBlocks(const std::vector<Block*>& blocks) { m_blocks = blocks; }
    void setSpawns(const std::vector< std::pair<float, float> >& spawns) { m_spawns = spawns; }
    
    void clear();
    
    virtual void draw(Renderer* renderer) override;
    
    static void freeLoaded();
    
    virtual json serialize() const override;
    virtual bool deserialize(const json& r) override;
protected:
private:
    float m_width;
    float m_height;
    
    std::vector<Block*> m_blocks;
    std::vector< std::pair<float, float> > m_spawns;
    
    static std::map< std::string, Map* > m_loaded;
    std::string m_fileName;
    
    void loadFromFile(const std::string& filePath);
    void copy(const Map& r);
};

#endif