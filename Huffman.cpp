#include <iostream>
#include "HuffmanTree.h"

bool checkExtension(std::string s,std::string ext) {
    return (s.substr(s.find_last_of(".") + 1) == ext);
}

int main(int argc, char* argv[]) {
    if (argc < 2 ) {
        std::cout << "Argomenti insufficienti";
        return 1;
    }
    else {
        std::string a1, a2;
        bool compress;

        if (argc == 2) {
            a1 = argv[1];
            if (checkExtension(a1,"txt")) {
                a2 = a1.substr(0, a1.find_last_of(".") + 1) + "james";
                compress = true;
            }
            else if (checkExtension(a1,"james")) {
                a2 = a1.substr(0, a1.find_last_of(".") + 1) + "txt";
                compress = false;
            }
            else {
                std::cout << "Estensione del file sbagliata";
                return 1;
            }
        }
        else {
            a1 = argv[1];
            a2 = argv[2];

            if (!((checkExtension(a1, "txt") && checkExtension(a2, "james")) || (checkExtension(a1, "james") && checkExtension(a2, "txt")))) {
                std::cout << "Estensione dei file sbagliata";
                return 1;
            }
            compress = checkExtension(a1, "txt");
        }

        HuffmanTree huf(a1,a2);

        if (compress) { huf.compress(); }
        else { huf.decompress(); }

        std::cout << "File " << ((compress) ? "" : "de") << "compresso in " << a2;
    }
}