#include <iostream>
#include <sstream>
#include <string>
#include <vector>
#include <unordered_map>

class Node
{
    public:
        std::vector<Vertice> vertices;
};

class Vertice
{
    int weight;
    int n1;
    int n2;
    public:
        Vertice(int n1, int n2, int weight)
        {
            this->n1 = n1;
            this->n2 = n2;
            this->weight = weight;
        }
};

int main()
{

    int people;
    int pairs;

    std::cin >> people;
    std::cin >> pairs;

    std::unordered_map<int, Node> graph;

    std::string line;
    while (std::getline(std::cin, line))
    {
        std::stringstream ss(line);

        int firstNode, secondNode, weight;
        ss >> firstNode >> secondNode >> weight;

        Node n1 = Node();
        Node n2 = Node();

        n1.vertices.push_back(Vertice(firstNode, secondNode, weight));
        n2.vertices.push_back(Vertice(firstNode, secondNode, weight));

        graph.insert({firstNode, n1});
        graph.insert({secondNode, n2});
    }

    return 0;
}

int mst()
{
    
}