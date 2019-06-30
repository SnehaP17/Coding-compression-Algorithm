#ifndef _HUFFMAN_TREE_H_
#define _HUFFMAN_TREE_H_

#include <iostream>
#include <fstream>
#include <map>
#include <queue>
#include <string>
#include <stack>
using namespace std;

struct heapNode
{
	char c;
	int freq;
	heapNode* left;
    heapNode* right;
	
	heapNode(char c, int freq);

};

//comparator for priority queue
//reference - https://stackoverflow.com/questions/16111337/declaring-a-priority-queue-in-c-with-a-custom-comparator
struct compare
{
    bool operator()(heapNode* h1, heapNode* h2)
    {
        return (h1->freq > h2->freq);
    }
};

class huffman_tree {
	public:
	    //map to store character codes
        map<char, string> codeMap;

		//queue
		//priority_queue<heapNode*, vector<heapNode*>, decltype(compare)> heap(compare);
		priority_queue<heapNode*, vector<heapNode*>, compare> heap;


		huffman_tree(const std::string &file_name);
		~huffman_tree();
		
		std::string get_character_code(char character) const;
		std::string encode(const std::string &file_name) const;
		std::string decode(const std::string &string_to_decode) const;	

		void makeCodes(heapNode* node, std::string s);
};






#endif