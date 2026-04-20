#include <iostream>
#include <unordered_map>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>

int main()
{
    std::unordered_map<std::string, std::vector<std::string>> wordMap;
    std::ifstream wordFile("words.txt");
    // remove first line of the file
    std::string firstLine;
    std::getline(wordFile, firstLine);

    // split first line by space and get the first number which is the number of words in the file
    std::istringstream iss(firstLine);
    int wordCount, instructionsCount;
    iss >> wordCount >> instructionsCount;

    std::string word;

    for (int i = 0; i < wordCount; ++i)
    {
        std::getline(wordFile, word);

        wordMap[word] = std::vector<std::string>(); // Initialize the vector for the word
    }
    for (const auto &pair : wordMap)
    {
        // take four last letters of the word
        std::string lastFourLetters = pair.first.substr(pair.first.size() - 4);
        // check if the last four letters are in the each word in the map
        for (const auto &innerPair : wordMap)
        {
            if (innerPair.first.find(lastFourLetters) != std::string::npos)
            {
                wordMap[pair.first].push_back(innerPair.first); // Add the word to the vector of the key word
            }
        }
    }

    std::string instruction;

    for (int i = 0; i < instructionsCount; ++i)
    {
        std::getline(wordFile, instruction);
        std::istringstream iss(instruction);
        std::string startWord, endWord;
        iss >> startWord >> endWord;
        std::string result = breadthFirstSearch(wordMap, startWord, endWord);
        std::cout << result << std::endl;
    }
    return 0;
}

std::string breadthFirstSearch(std::unordered_map<std::string, std::vector<std::string>> &wordMap, const std::string &startWord, const std::string &endWord)
{
    std::vector<std::string> queue;
    queue.push_back(startWord);

    std::unordered_map<std::string, bool> visited;
    std::unordered_map<std::string, int> distance;

    visited[startWord] = true;
    distance[startWord] = 0;

    while (!queue.empty())
    {
        std::string currentWord = queue.front();
        queue.erase(queue.begin());
        for (const std::string &neighbor : wordMap[currentWord])
        {
            if (!visited[neighbor])
            {
                visited[neighbor] = true;
                queue.push_back(neighbor);
                distance[neighbor] = distance[currentWord] + 1;
                if (neighbor == endWord)
                {
                    return std::to_string(distance[neighbor]);
                }
            }
        }
    }

    return "Impossible";
}