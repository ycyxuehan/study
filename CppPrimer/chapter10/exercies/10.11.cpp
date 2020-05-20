#include <iostream>
#include <algorithm>
#include <vector>
#include <string>

bool isShorter(const std::string &cs1, const std::string &cs2)
{
    return cs1.size() < cs2.size();
}

void elimDups(std::vector<std::string> &words)
{
    std::sort(words.begin(), words.end());
    auto end_unique = std::unique(words.begin(), words.end());
    words.erase(end_unique, words.end());    
}

int main()
{
    std::vector<std::string> sv = {"fox", "over", "quick", "red", "the", "slow", "quick", "the", "jumps", "turtle"};
    elimDups(sv);
    std::stable_sort(sv.begin(), sv.end(), isShorter);
    for(const auto &s : sv)
    {
        std::cout<<s<<" ";
    }
    std::cout<<std::endl;
}

