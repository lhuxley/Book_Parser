# Book_Parser

A multithreading c++ program to parse through a specified .txt file and find some intersting factoids, like most common word, or number of unique words. This program is focused on doing this process as efficiently with respect to time as possible, and is not very memory effecient. If you wish to do this task but are more limited with space than time, this may not be the program for you. The prototype.cpp file included is a non-multithreaded version, which is less efficient. It is exponentially less efficient for huge files.

Note: to compile this program, pthread flag must be included
Example: g++ -std=c++11 -pthread main.cpp -o main

You must upload your own .txt file to the same working directory as to where this program runs.
