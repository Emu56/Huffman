#include "HuffmanTree.h"

HuffmanTree::HuffmanTree(std::string f1, std::string f2) {
	pathSource = f1;
	pathDest = f2;
}

void HuffmanTree::makeMinHeap(bool comp) {
	std::string validChar = "\t\n !\"#$%&\'()*+,-./0123456789:;<=>\?@ABCDEFGHIJKLMNOPQRSTUVWXYZ[\\]^_`abcdefghijklmnopqrstuvwxyz{|}~";
	std::string as;

	if (comp) {
		fSource.open(pathSource, std::ios::in);
		std::stringstream temp;
		temp << fSource.rdbuf();
		as = temp.str();

		for (char a : validChar) {
			node t;
			t.c = a;
			int fff = std::count(as.begin(), as.end(), a);
			t.freq = fff;
			if (t.freq > 0) { minHeap.insert(minHeap.begin(),t); }
		}
	}
	else {
		fSource.open(pathSource, std::ios::in | std::ios::binary);
		int t = 0;

		char nChar;
		int nFreq;

		fSource.read(reinterpret_cast<char*>(&t), 1);
		for (int i = 0; i < t; i++) {
			fSource.read(&nChar, 1);
			fSource.read(reinterpret_cast<char*>(&nFreq), 4);
			node tmp;
			tmp.c = nChar;
			tmp.freq = nFreq;
			minHeap.insert(minHeap.begin(), tmp);;
		}
	}

	std::sort(minHeap.begin(), minHeap.end());
	fSource.close();
}

void HuffmanTree::makeTree(std::vector<node> temp) {
	while (temp.size() > 1) {
		node t;

		t.l = new node(temp[0]);
		temp.erase(temp.begin());

		t.r = new node(temp[0]);;
		temp.erase(temp.begin());

		t.freq = (t.l->freq) + (t.r->freq);
		temp.insert(temp.begin(), t);;

		std::sort(temp.begin(), temp.end());
	}

	start = new node(temp[0]);
}

void HuffmanTree::code(node* r, std::vector<bool> v) {
	if (r->l == nullptr || r->r == nullptr) {
		codes[r->c] = v;
		return;
	}

	std::vector<bool> vL = v,vR = v;
	vL.push_back(false);
	vR.push_back(true);
	code(r->l,vL);
	code(r->r,vR);
}

void HuffmanTree::compress() {
	makeMinHeap(true);
	makeTree(minHeap);
	code(start,std::vector<bool>());

	char wr = minHeap.size();

	fSource.open(pathSource, std::ios::in);
	fDest.open(pathDest, std::ios::out | std::ios::binary);
	fDest.write(reinterpret_cast<const char*>(&wr), 1);

	while (!minHeap.empty()) {
		node t(minHeap[0]);
		fDest.write(&t.c, 1);
		fDest.write(reinterpret_cast<const char*>(&t.freq), 4);
		minHeap.erase(minHeap.begin());
	}

	char c;

	fSource.get(c);
	short i = 0;
	wr = '\0';
	while (!fSource.eof()) {
		std::vector<bool> temp = codes[c];

		for (auto z : temp) {
			wr <<= 1;
			wr |= z;
			i++;
			if (i == 8) {
				fDest.write(&wr, 1);
				i = 0;
				wr = '\0';
			}
		}
		fSource.get(c);
	}
	
	char unused;
	if(i==0) unused = 0;
	else unused = 8-i;
	if (i != 0) {
		wr <<= unused;
		fDest.write(&wr, 1);
	}
	fDest.write(&unused, 1);

	fSource.close();
	fDest.close();
}

void HuffmanTree::decompress() {
	makeMinHeap(false);
	makeTree(minHeap);

	node* tmpNode = start;

	fSource.open(pathSource, std::ios::in | std::ios::binary);
	fDest.open(pathDest, std::ios::out);

	std::stringstream temp;
	temp << fSource.rdbuf();
	std::string as = temp.str();
	as = as.substr(1 + 5 * minHeap.size());
	char unusedBits=as.back();
	as.pop_back();
	char lastChar = as.back();
	as.pop_back();

	for (char a : as) {
		for (int i = 7; i >= 0; i--) {
			if ((a >> i) & 1) { tmpNode = tmpNode->r; }
			else { tmpNode = tmpNode->l; }
			if (tmpNode->c != '\0') {
				fDest.put(tmpNode->c);
				tmpNode = start;
			}
		}
	}
	for (int i = 7; i >= unusedBits; i--) {
		if ((lastChar >> i) & 1) { tmpNode = tmpNode->r; }
		else { tmpNode = tmpNode->l; }
		if (tmpNode->c != '\0') {
			fDest.put(tmpNode->c);
			tmpNode = start;
		}
	}

	fSource.close();
	fDest.close();
}