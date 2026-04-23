#include <iostream>
#include <unordered_map>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>

std::string breadthFirstSearch(std::unordered_map<std::string, std::vector<std::string>> &wordMap, const std::string &startWord, const std::string &endWord)
{
    if (startWord == endWord)
        return "0";

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

int main()
{
    std::unordered_map<std::string, std::vector<std::string>> wordMap;
    std::vector<std::string> words;

    int wordCount, instructionsCount;
    std::cin >> wordCount >> instructionsCount;

    for (int i = 0; i < wordCount; ++i)
    {
        std::string word;
        std::cin >> word;
        words.push_back(word);
        wordMap[word] = std::vector<std::string>();
    }

    // Build Graph
    for (const std::string &w : words)
    {
        for (const std::string &innerW : words)
        {
            std::string temp = innerW;
            bool isConnected = true;

            for (size_t k = w.size() - 4; k < w.size(); ++k)
            {
                size_t pos = temp.find(w[k]);

                if (pos != std::string::npos)
                {
                    temp.erase(pos, 1);
                }
                else
                {
                    isConnected = false;
                    break;
                }
            }

            if (isConnected)
            {
                wordMap[w].push_back(innerW);
            }
        }
    }

    for (int i = 0; i < instructionsCount; ++i)
    {
        std::string startWord, endWord;
        std::cin >> startWord >> endWord;
        std::string result = breadthFirstSearch(wordMap, startWord, endWord);
        std::cout << result << std::endl;
    }

    return 0;
}
