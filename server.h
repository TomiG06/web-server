#pragma once

#include <iostream>

using namespace std;

/*  Server Class Prototype */

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
        bool read_multi;
        bool res_is_set;

        ssize_t fsize();        //File Size
        void set_response();    //Sets response string
        char* read_ifile();     //Reads the contents of file

    public:
        Server(string fname, bool read_multi);
        void Init(uint16_t port);
};

