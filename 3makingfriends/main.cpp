#include <iostream>
#include <sstream>
#include <string>

class Tree
{
    Node nodes[];
};

class Node
{
    int value;

public:
    Node(int value)
    {
        this->value = value;
    }
    Vertice vertices[];
};

class Vertice
{
    int weight;
    std::tuple<Node, Node> nodes;
};

int main()
{

    int people;
    int pairs;

    std::cin >> people;
    std::cin >> pairs;

    Tree trees = new Tree[people];

    std::string line;
    while (std::getline(std::cin, line))
    {
        std::stringstream ss(line);

        int firstNode, secondNode, weight;
        ss >> firstNode >> secondNode >> weight;

        Node n1 = Node(firstNode);
        Node n2 = Node(secondNode);
    }

    return 0;
}

int mst(Tree trees[])
{
    for (auto tree : trees)
    {
    }
}