#pragma once
#include <string>
#include <map>
#include <fstream>
#include <sstream>
#include <vector>

class HuffmanTree {

private:
	struct node{
		char c='\0';
		unsigned int freq=0;

		node* l = nullptr;
		node* r = nullptr;

		bool operator<(const node &mag) const { return (this->freq < mag.freq); }
	};

	node* start = nullptr;
	std::vector<node> minHeap;

	std::string pathSource, pathDest;
	std::fstream fSource, fDest;
	std::map<char, std::string> codes;

	void makeMinHeap(bool comp);
	void makeTree(std::vector<node> temp);
	void code(node* r, std::string s);

public:
	HuffmanTree(std::string f1, std::string f2);

	void compress();
	void decompress();
};