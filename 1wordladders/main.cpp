#include <iostream>
#include <vector>
#include <string>
#include <queue>
#include <unordered_map>
#include <array>

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
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(NULL);

    int wordCount, instructionsCount;

    if (!(std::cin >> wordCount >> instructionsCount))
        return 0;

    std::vector<std::string> words(wordCount);
    std::unordered_map<std::string, int> wordToId;

    std::vector<std::array<int, 26>> fullWordFreq(wordCount, {0});
    std::vector<std::array<int, 26>> lastFourFreq(wordCount, {0});

    for (int i = 0; i < wordCount; ++i)
    {
        std::cin >> words[i];
        wordToId[words[i]] = i;

        for (char c : words[i])
        {
            fullWordFreq[i][c - 'a']++;
        }

        if (words[i].size() >= 4)
        {
            for (size_t k = words[i].size() - 4; k < words[i].size(); ++k)
            {
                lastFourFreq[i][words[i][k] - 'a']++;
            }
        }
    }

    std::vector<std::vector<int>> adj(wordCount);

    // Build Graph
    for (int i = 0; i < wordCount; ++i)
    {
        for (int j = 0; j < wordCount; ++j)
        {
            bool isConnected = true;

            for (int c = 0; c < 26; ++c)
            {
                if (lastFourFreq[i][c] > fullWordFreq[j][c])
                {
                    isConnected = false;
                    break;
                }
            }

            if (isConnected)
            {
                adj[i].push_back(j);
            }
        }
    }

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

    return 0;
}
