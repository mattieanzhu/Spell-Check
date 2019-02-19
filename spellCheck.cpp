#include <iostream>
#include <fstream>
#include <set>
#include <string>
#include <vector>
#include <chrono>

std::vector<std::string> spellCheck(std::string check, std::set<std::string> dictionary);
bool charCheck(std::string check, std::string dict);

int main(int argc, char **argv){
    std::set<std::string> dictionary;
    std::set<std::string> check;
    std::ifstream checkFile;
    std::ifstream dictionaryFile;
    std::ofstream outputFile;
    std::string checkWord;
    std::string dictionaryWord;
    checkFile.open(argv[1]);
    while(!checkFile.eof()){
        while(checkFile >> checkWord){
            for(int i=0; i<checkWord.size(); i++){
                if(ispunct(checkWord[i])){
                    checkWord.erase(i--, 1);
                }
            }
            std::transform(checkWord.begin(), checkWord.end(), checkWord.begin(), ::tolower);
            check.insert(checkWord);
        }
    }
    checkFile.close();
    dictionaryFile.open(argv[2]);
    while(!dictionaryFile.eof()){
        while(dictionaryFile >> dictionaryWord){
            dictionary.insert(dictionaryWord);
        }
    }
    dictionaryFile.close();
    outputFile.open("output.txt");
    auto begin = std::chrono::high_resolution_clock::now();
    std::set<std::string>::iterator checkIt;
    for(checkIt = check.begin(); checkIt != check.end(); checkIt++){
        bool match = false;
        std::set<std::string>::iterator dictionaryIt;
        for(dictionaryIt = dictionary.begin(); dictionaryIt != dictionary.end(); dictionaryIt++){
            if(*checkIt == *dictionaryIt){
                match = true;
                break;
            }
        }
        if(match == false){
            outputFile << *checkIt << ": ";
            for (int i=0; i<spellCheck(*checkIt, dictionary).size(); i++){
                outputFile << spellCheck(*checkIt, dictionary)[i] << "  ";
            }
            outputFile << std::endl;
        }
                
    }
    auto end = std::chrono::high_resolution_clock::now();
    std::cout << std::chrono::duration_cast<std::chrono::milliseconds>(end-begin).count() << std::endl;
    outputFile.close();
}

std::vector<std::string> spellCheck(std::string check, std::set<std::string> dictionary){
    std::vector<std::string> correctWords;
    std::set<std::string>::iterator dictionaryIt;
    for(dictionaryIt = dictionary.begin(); dictionaryIt != dictionary.end(); dictionaryIt++){
        if(charCheck(check, *dictionaryIt)==true){
            correctWords.push_back(*dictionaryIt);
        }
    }
    return correctWords;
}
  
bool charCheck(std::string check, std::string dict){
    if(dict.size()-check.size() == 1){
        bool match = true;
        bool firstCheck = true;
        int key;
        int i = 0;
        int nonMatch = 0;
        while(i<check.size() && nonMatch == 0){
            if(check[i] != dict[i]){
                nonMatch++;
                key = i;
                firstCheck = false;
            }
            i++;
        }
        if(firstCheck == true){
            return true;
        }
        for(int j=key; j<check.size(); j++){
            if(check[j] != dict[j+1]){
                match = false;
                break;
            }
        }
        return match;
    }
    else if(check.size()-dict.size() == 1){
        bool match = true;
        bool firstCheck = true;
        int key;
        int i = 0;
        int nonMatch = 0;
        while(i<check.size()-1 && nonMatch == 0){
            if(check[i] != dict[i]){
                nonMatch++;
                key = i;
                firstCheck = false;
            }
            i++;
        }
        if(firstCheck == true){
            return true;
        }
        for(int j=key+1; j<check.size(); j++){
            if(check[j] != dict[j-1]){
                match = false;
                break;
            }
        }
        return match;
    }

    else if(check.size() == dict.size()){
        int nonMatch = 0;
        int swap;
        for(int i=0; i<check.size(); i++){
            if(check[i] != dict[i]){
                nonMatch++;
                if(nonMatch == 1){
                    swap = i;
                }
            }
        }
        if(nonMatch == 1){
            return true;
        }
        else if(nonMatch == 2){
            if(check[swap] == dict[swap+1] && check[swap+1] == dict[swap]){
                return true;
            }
        }
        else{
            return false;
        }
    }
    return false;
}
