#include <iostream>
#include <vector>
#include <string>
#include <queue>
#include <unordered_map>
#include <array>
#include <chrono>

// Breadth-First Search
int bfs(int startId, int endId, const std::vector<std::vector<int>> &adj, int numWords)
{
    if (startId == endId)
        return 0;

    std::queue<int> q;
    std::vector<int> distance(numWords, -1);

    q.push(startId);
    distance[startId] = 0;

    while (!q.empty())
    {
        int current = q.front();
        q.pop();

        for (int neighbor : adj[current])
        {
            if (distance[neighbor] == -1)
            {
                distance[neighbor] = distance[current] + 1;
                if (neighbor == endId)
                {
                    return distance[neighbor];
                }
                q.push(neighbor);
            }
        }
    }
    return -1;
}

int main()
{
    // Optimize standard I/O operations for speed
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(NULL);

    int wordCount, instructionsCount;

    if (!(std::cin >> wordCount >> instructionsCount))
        return 0;

    auto startInput = std::chrono::high_resolution_clock::now();

    std::vector<std::string> words(wordCount);
    std::unordered_map<std::string, int> wordToId; // Maps words to unique integer IDs

    // Arrays to store character frequencies
    std::vector<std::array<int, 26>> fullWordFreq(wordCount, {0});
    std::vector<std::array<int, 26>> lastFourFreq(wordCount, {0});

    // Read words and calculate letter frequencies
    for (int i = 0; i < wordCount; ++i)
    {
        std::cin >> words[i];
        wordToId[words[i]] = i;

        // Count frequency of all characters in the word
        for (char c : words[i])
        {
            fullWordFreq[i][c - 'a']++;
        }

        // Count frequency of the last four characters
        if (words[i].size() >= 4)
        {
            for (size_t k = words[i].size() - 4; k < words[i].size(); ++k)
            {
                lastFourFreq[i][words[i][k] - 'a']++;
            }
        }
    }

    auto endInput = std::chrono::high_resolution_clock::now();
    auto startGraph = std::chrono::high_resolution_clock::now();

    // Adjacency list representation of the graph
    std::vector<std::vector<int>> adj(wordCount);

    // Build Graph
    for (int i = 0; i < wordCount; ++i)
    {
        for (int j = 0; j < wordCount; ++j)
        {
            bool isConnected = true;

            // Check if word j contains all required letters from the last 4 letters of word i
            for (int c = 0; c < 26; ++c)
            {
                if (lastFourFreq[i][c] > fullWordFreq[j][c])
                {
                    isConnected = false;
                    break;
                }
            }

            // Draw a directed edge from i to j if the condition is met
            if (isConnected)
            {
                adj[i].push_back(j);
            }
        }
    }

    auto endGraph = std::chrono::high_resolution_clock::now();
    auto startInstructions = std::chrono::high_resolution_clock::now();

    // Process all search instructions
    for (int i = 0; i < instructionsCount; ++i)
    {
        std::string startWord, endWord;
        std::cin >> startWord >> endWord;

        int startId = wordToId[startWord];
        int endId = wordToId[endWord];

        int result = bfs(startId, endId, adj, wordCount);

        if (result == -1)
        {
            std::cout << "Impossible\n";
        }
        else
        {
            std::cout << result << "\n";
        }
    }

    auto endInstructions = std::chrono::high_resolution_clock::now();

    std::chrono::duration<double> timeInput = endInput - startInput;
    std::chrono::duration<double> timeGraph = endGraph - startGraph;
    std::chrono::duration<double> timeInstructions = endInstructions - startInstructions;
    std::chrono::duration<double> totalTime = endInstructions - startInput;

    std::cerr << "Inläsning tid: " << timeInput.count() << " s\n";
    std::cerr << "Grafbygge tid: " << timeGraph.count() << " s\n";
    std::cerr << "BFS instructioner tid: " << timeInstructions.count() << " s\n";
    std::cerr << "Total tid: " << totalTime.count() << " s\n";

    return 0;
}
