#include "huffman_tree.h"


heapNode::heapNode(char c, int freq)
{
	this->c = c;
	this->freq = freq;
	this->left = nullptr;
    this->right = nullptr;
}


/*
Preconditions: file_name is the name of (and possibly path to) a text file
Postconditions: Reads the contents of file_name and constructs a
				huffman tree based on the character frequencies of the file contents
*/
huffman_tree::huffman_tree(const std::string &file_name)
{
	//read file, create frequency table, create huffman tree
	ifstream input(file_name);
	char c;
	map <char, int> freqtable;
	map <char, int>:: iterator it;
	

	 while (input.get(c))
     {
		 
		 it = freqtable.find(c);

		 //if character is present in map, increase it's frequency
		 if(it != freqtable.end())
		 {
			 it->second = it->second+1;			 
		 }
		 //else add character and 1 freqency in the map
		 else
		 {
			 freqtable.insert(it, pair<char, int>(c,1));
		 }
		 
     }
	 
    //create heap nodes from the map and add it to priority queue
	it = freqtable.begin();
	

    for(int i = 0; i < freqtable.size(); i++)
    {
        heapNode* temp = new heapNode(it->first, it->second);
        heap.push(temp);
        it++;
    }

    

	while(heap.size() > 1)
    {
        heapNode* h1 = heap.top();
        heap.pop();
        heapNode* h2 = heap.top();
        heap.pop();

        auto temp = new heapNode('0', h1->freq+h2->freq);
        temp->left = h1;
        temp->right = h2;
        heap.push(temp);
    }

	//create map of character codes

	//if only one character, insert it in map, else DFS tree and create codes
	if(heap.top()->left == nullptr && heap.top()->right == nullptr)
	{
		codeMap.insert(pair<char, string>(heap.top()->c, "0"));
	}

	else
     makeCodes(heap.top(), "");


}

huffman_tree::~huffman_tree()
{
	//delete memory by  traversing the tree
	
	heapNode* root = heap.top();
	if(root == nullptr)
	return;

	stack<heapNode*> s;
	s.push(root);

	heapNode* node = nullptr;
	while(!s.empty())
	{
		node = s.top();
		s.pop();

		if(node->left != nullptr)
		s.push(node->left);

		if(node->right != nullptr)
		s.push(node->right);

		delete node;

	}

	root = nullptr;

}

/*
Preconditions: Character is a character with an ASCII value
				between 0 and 127 (inclusive).
Postconditions: Returns the Huffman code for character if character is in the tree
				and an empty string otherwise.
*/
std::string huffman_tree::get_character_code(char character) const 
{
	string str;
    try 
	{
        str =codeMap.at(character);
		
    }
    catch (const std::out_of_range& oor)
    {
		
        str = "";
    }

	//return string code for the character given
	return str;

	
}

//stores character codes for all characters in the tree in a map
void huffman_tree::makeCodes(heapNode* node, std::string s)
{
    
	if(node->left != nullptr)	
	 makeCodes(node->left, s+"0");

	if(node->right != nullptr)
	  makeCodes(node->right, s+"1");

    //if leaf is reached, insert		
    if (node->left == nullptr && node->right == nullptr)
	{         
		 codeMap.insert(pair<char, string>(node->c, s));
	}

	else
	 return;
	
		
}


/*
Preconditions: file_name is the name of (and possibly path to) a text file
Postconditions: Returns the Huffman encoding for the contents of file_name
				if file name exists and an empty string otherwise.
				If the file contains letters not present in the huffman_tree,
				return an empty string
*/
std::string huffman_tree::encode(const std::string &file_name) const 
{
	
	int count = 0;
	string str = "";
	string code = "";
	ifstream input(file_name);

	if(!input)
	return "";

	char c;
    if (input.good())
	{
		
		while (input.get(c))
		{
					
		   count++;
		   code = get_character_code(c);
		   
		   if(code != "")
		   {
                str = str+code;
		   }

		   else
		   {
			   return "";
		   }		   
           
		}


		if(count == 1)
		return "0";

			return str;
		
		
	}

	else
	return "";


}

/*
Preconditions: string_to_decode is a string containing Huffman-encoded text
Postconditions: Returns the plaintext represented by the string if the string
				is a valid Huffman encoding and an empty string otherwise
*/
std::string huffman_tree::decode(const std::string &string_to_decode) const 
{
	
    string str = ""; 
    heapNode* node = heap.top();

	if(string_to_decode.length() == 1)
	{
		string s(1, node->c);
		return s;
	}

    //traverse string and traverse tree based on the characters encountered during traversing string
    for (int i=0;i<string_to_decode.length();i++) 
    { 
		if(string_to_decode[i] != '1' && string_to_decode[i] != '0')
		{
			return "";
		}

		//if character is 0 traverse left, if 1 traverse right
        if (string_to_decode[i] == '0') 
           node= node->left; 
        else 
           node = node->right; 
  
        // if leaf node is reached, add character to our decode string and 
		//set current pointer to root of the heap to traverse from top again
        if (node->left==nullptr and node->right==nullptr) 
        { 
            str = str + node->c; 
            node = heap.top(); 
        } 
    } 
    
    return str; 
	
}
