#ifndef SERVER_H
#define SERVER_H

#include <stdint.h>
#include <iostream>
#include <sys/socket.h>

using namespace std;

/* 
    Server class
    
    Each Server contains a file
    which contents are sent back
    as a response to TCP clients.
*/


class Server {
    private:
        string response;
        string fname;           //File name

        ssize_t fsize();        //File Size
        void set_response();    //Sets response string
        char* read_ifile();     //Reads the contents of file

    public:
        Server(string fname);
        void Init(uint16_t port);
};

#endif

