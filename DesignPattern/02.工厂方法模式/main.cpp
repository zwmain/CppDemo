#include <iostream>
#include <vector>
#include <string>

class Block {
public:
    Block() { }
    virtual ~Block() { }
    virtual void draw() = 0;
};

class CircleBlock : public Block {
public:
    CircleBlock() { }
    ~CircleBlock() { }
    void draw() override
    {
        std::cout << "Circle Block" << std::endl;
    }
};

class SquareBlock : public Block {
public:
    SquareBlock() { }
    ~SquareBlock() { }
    void draw() override
    {
        std::cout << "Square Block" << std::endl;
    }
};

class BlockFactory {
public:
    BlockFactory() { }
    virtual ~BlockFactory() { }
    virtual Block* createBlock() = 0;
};

class CircleBlockFactory : public BlockFactory {
public:
    CircleBlockFactory() { }
    ~CircleBlockFactory() { }
    Block* createBlock() override
    {
        return new CircleBlock();
    }
};

class SquareBlockFactory : public BlockFactory {
public:
    SquareBlockFactory() { }
    ~SquareBlockFactory() { }
    Block* createBlock() override
    {
        return new SquareBlock();
    }
};

class BlockFactorySystem {
public:
    BlockFactorySystem() { }
    ~BlockFactorySystem()
    {
        for (auto* b : mBlockArr) {
            delete b;
        }
        mBlockArr.clear();
    }
    void produceBlocks(const std::string& key, int quantity)
    {
        std::unique_ptr<BlockFactory> factory;
        if (key == "Circle") {
            factory = std::make_unique<CircleBlockFactory>();
        } else if (key == "Square") {
            factory = std::make_unique<SquareBlockFactory>();
        } else {
            return;
        }
        for (int i = 0; i < quantity; ++i) {
            Block* block = factory->createBlock();
            block->draw();
            mBlockArr.push_back(block);
        }
    }

private:
    std::vector<Block*> mBlockArr;
};

int main()
{
    std::string key;
    int num = 0;
    std::cin >> key >> num;
    BlockFactorySystem sys;
    sys.produceBlocks(key, num);
    return 0;
}
