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

// Globals
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
std::unordered_map<std::string, int> word_count;
std::vector<std::string> substrings;


void* thread_function(void *arg) {
    // Convert the void pointer to an int pointer, and dereference it to get the value of 'a'
    int a = *(int*)arg;

    // Create a stringstream object to tokenize the substring
    std::string word;
    std::stringstream ss(substrings[a]);

    // Iterate through all the words in the substring
    while (ss >> word) {
        // Lock the mutex before accessing the shared data structures
        pthread_mutex_lock(&mutex);
        ++word_count[word];
        pthread_mutex_unlock(&mutex);
    }
}

void print_info(struct stat sb, int num_threads){
    // Find the most common word in the word_count map
    auto largest = word_count.begin();
    for(auto curr = word_count.begin(); curr != word_count.end(); curr++) {
        if(curr->second > largest->second)
            largest = curr;
    }

    std::cout << "Here's some helpful info about your file: \n";
    
    int numwords = 0;
    for(auto curr = word_count.begin(); curr != word_count.end(); curr++) {
        numwords += curr->second;
    }

    std::cout << "Total number of words: " << numwords << "\n";
    std::cout << "Number of unique words: " <<  word_count.size() << "\n";
    std::cout << "Most common word: " << largest->first << "\n";
    std::cout << "Number of occurences of the word '" << largest->first << "': " << largest->second << "\n\n";

    std::cout << "Here's a little bit of info for the programmer: \n";
    std::cout << "Number of threads created for this file: " << num_threads << "\n";
    std::cout << "Number of bytes in file: " << sb.st_size << "\n";
}

void seperate_into_substrings(std::string file_contents){
    // Get the length of the file contents
    int str_size = file_contents.length();

    // Initialize a variable to keep track of the total length of processed substrings
    int stored_total_length = 0;

    // While there is still unprocessed content in the file
    while (stored_total_length < str_size){
        // Set the minimum length of the substring to 20000 characters
        int index = 20000;

        // If the total processed material is larger than the total length of the txt file, set it so that its now the exact length of the txt file
        if (index + stored_total_length > str_size)
            index = str_size - stored_total_length;

        // Find the next whitespace or newline character to delimit the substring
        while(file_contents[index + stored_total_length] != ' ' && file_contents[index+stored_total_length] != '\n' && stored_total_length + index != str_size){
            index++;
        }
        // Add the substring to the substrings vector
        substrings.push_back(file_contents.substr(stored_total_length, index));

        // Increment the stored_total_length by the length of the substring + 1
        stored_total_length = stored_total_length + index+1;

    }

}



int main(int argc, char *argv[]){
    // Check the number of arguments and usage 
    if (argc < 2) {  
        fprintf(stderr,"USAGE: %s 'filename \n", argv[0]); 
        exit(0); 
    } 

    // Get the file name from the command line arguments
    const char* file_name = argv[1];

    // Open the file
    int fd = open(file_name, O_RDONLY);

    // Get the file status
    struct stat sb;
    fstat(fd, &sb);

    // Memory map the file
    char* mapped_memory = (char*)mmap(nullptr, sb.st_size, PROT_READ, MAP_PRIVATE, fd, 0);
    // Check for mmap failure
    if (mapped_memory == MAP_FAILED) {
        std::cout << "mmap failed! This is often due to an incorrect .txt file in command line arguments \n";
        exit(1);
    }

    // Convert the memory mapped file to a C++ string
    std::string file_contents(mapped_memory, sb.st_size);
    

    // Break the file contents into substrings
    seperate_into_substrings(file_contents);

    // Get the number of threads to be created
    int num_threads = substrings.size();

    // Create the threads array
    pthread_mutex_init(&mutex, NULL);
    pthread_t threads[num_threads];
    // Initialize the mutex


    // Create the threads
    for (int i = 0; i < num_threads; i++) {
        std::unique_ptr<int> thread_num(new int(i));
        pthread_create(&threads[i], NULL, thread_function, (void*)thread_num.get());
        pthread_join(threads[i], NULL);
    }




    // Print out information about the processed file
    print_info(sb, num_threads);

    // Unmap the memory
    munmap(mapped_memory, sb.st_size);

    // Close the file descriptor
    close(fd);

    // Exit the program
    return 0;
    


}
