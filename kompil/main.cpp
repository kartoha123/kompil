//
//  main.cpp
//  kompil
//
//  Created by Елизавета on 10.12.15.
//  Copyright (c) 2015 Елизавета. All rights reserved.
//

#include <iostream>
#include "../lexanallis.h"
#include "../sintaxanalis.h"

int main(int argc, const char * argv[]) {
    reading("/Users/elizaveta/Documents/Универ/Новая папка/языки/lexanalis/lexanalis/newfile.txt");
    for (int a=0; a<result.size(); a++) {
        cout<<result[a].ValuaLexem<<' '<<result[a].TypeLexem<<' '<<result[a].NumberOfStr<<' '<<'\n';
        
    }
cout << "\nParsing tree\n";
Node* node = new Node(result);
    if (!error_flag) {
        cout << "\nPrinting tree\n";
        node->print();
    }
 
    
    
}

