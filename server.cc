#include "server.h"
#include <string>
#include <filesystem>
#include <fstream>
#include <cstdlib>
#include <cstdio>
#include <unistd.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>

void err(string msg) {
    cout << msg << endl;
    exit(1);
}

static const string basic_head = "HTTP/1.1 200 OK\r\n";

ssize_t Server::fsize() {
    return filesystem::file_size(this->fname);
}

void Server::set_response() {
    this->response = "";
    this->response.append(basic_head);
    this->response.append("Content-Type: text/html\r\n");
    this->response.append("Content-Length: " + to_string(this->fsize()));
    this->response.append("\r\n\r\n");
 
    char* body = this->read_ifile();
         
    string sbody(body, body+this->fsize());
       
    this->response = this->response + sbody + "\r\n\0";

    free(body);
}

Server::Server(string filename) {
    this->fname = filename;
}


char* Server::read_ifile() {
    ifstream ifile(this->fname);

    if(!ifile.good()) {
        cout << "File '" << this->fname << "' does not exist\n";
        exit(1);
    }

    ssize_t c = 0;
    char* ret = (char*) malloc(this->fsize());

    while(!ifile.eof()) ifile.get(ret[c++]);

    return ret;
}

void Server::Init(uint16_t port) {
    struct sockaddr_in server;
    struct sockaddr_in client;

    int namelen;
    int ns;

    char buff[4096];

    int s;

    if((s = socket(AF_INET, SOCK_STREAM, 0)) < 0) err("socket");

    server.sin_family = AF_INET;
    server.sin_port   = htons(port);
    server.sin_addr.s_addr = INADDR_ANY;

    if(bind(s, (struct sockaddr*)&server, sizeof(server)) < 0) err("bind");
    if(listen(s, 3)) err("listen");

    namelen = sizeof(client);

    while(1) {

        if((ns = accept(s, (struct sockaddr*) &client, (socklen_t*)&namelen)) == -1) err("accept");

        if(!fork()) {

            if(recv(ns, buff, sizeof(buff), 0) == -1) err("recv");

            puts(buff);

            Server::set_response();

            cout << this->response;

            if(send(ns, this->response.c_str(), this->response.size(), 0) < 0) err("send");

            close(ns);
            exit(0);
        }

        close(ns);

    }
    close(s);
}

