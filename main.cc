/*
    Web-Server

    usage: ws [-p: port] [-f: path/to/file]
        
        -p: The port where the server will run
            default port: 8080

        -f: The file which the server will serve 
            (hypothesizes that the file will be served as 'text/html')
        -m: Set response every time a TCP client is accepted
*/


#include <iostream>
#include <string.h>
#include <stdlib.h>

#include "server.h"

using namespace std;

void uknown_arg(char* arg) {
    cout << "Invalid arg " << arg << endl;
    exit(1);
}

int main(int argc, char** argv) {
    
    /* Variables to store flag arguments */
    int port = 8080;
    string fname = "";
    bool read_multi = false;

    for(uint8_t x = 0; x < argc; x++) {
        
        if(argv[x][0] == '-') {
            if(strlen(argv[x]) != 2) uknown_arg(argv[x]);

            switch(argv[x][1]) {
                case 'h': 
                    cout << "usage: ws [-f (input file)] [-p (port)]" << endl;
                    return 0;
                case 'p': 
                    port = atoi(argv[x+1]);
                    break;
                case 'f':
                    fname = argv[x+1];
                    break;
                case 'm':
                    read_multi = true;
                    break;
                default:
                    uknown_arg(argv[x]);
            }
        }
    }

    if(port <= 1024) {
        cout << "Port must be > 1024\nPort given: "<< port << endl;
        return 1;
    }

    if(fname.empty()) {
        cout << "No input file\n";
        return 1;
    }

    /* Instantiate Server */
    Server s(fname, read_multi);

    /* Start Server */
    s.Init(port);

    return 0;
}
