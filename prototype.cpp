#include <fcntl.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <unistd.h>
#include <string>
#include <iostream>
#include <pthread.h>
#include <vector>
#include <unordered_map>
#include <sstream>
#include <memory>
#include <chrono>
#include <fstream>





int main(int argc, char *argv[]){
    // Check the number of arguments and usage 


    auto start = std::chrono::high_resolution_clock::now();

    int numwords = 0;

    std::unordered_map<std::string, int> word_count;
    
    if (argc < 2) {  
        fprintf(stderr,"USAGE: %s 'filename \n", argv[0]); 
        exit(0); 
    } 

    // Get the file name from the command line arguments

    // Open the file
    
    std::ifstream file(argv[1]);
    std::string file_contents((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());

    // Get the file status

    std::stringstream ss(file_contents);
    std::string word;

    while (ss >> word) {
        ++word_count[word];
        numwords++;
    }

    auto largest = word_count.begin();
    for(auto curr = word_count.begin(); curr != word_count.end(); curr++) {
        if(curr->second > largest->second)
            largest = curr;
    }


    // Print out information about the processed file
    std::cout << "Here's some helpful info about your file: \n";

    std::cout << "Total number of words: " << numwords << "\n";
    std::cout << "Number of unique words: " <<  word_count.size() << "\n";
    std::cout << "Most common word: " << largest->first << "\n";
    std::cout << "Number of occurences of the word '" << largest->first << "': " << largest->second << "\n\n";


    auto end = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> elapsed = end - start;
    std::cout << "Time taken for: " << elapsed.count() << " seconds\n";
    // Exit the program
    return 0;
    


}