#include <cstdio>
#include <cstdlib>
#include <filesystem>
#include <fstream>
#include <netinet/in.h>
#include <string>
#include <sys/socket.h>
#include <sys/types.h>
#include <thread>
#include <unistd.h>

#include "server.h"

/* Display error message and exit*/
void err(string msg) {
    cout << msg << endl;
    exit(1);
}

static const string basic_head = "HTTP/1.1 200 OK\r\n";

ssize_t Server::fsize() {
    return filesystem::file_size(this->fname);
}

void Server::set_response() {
    if(this->res_is_set) return;

    this->response = "";
    this->response.append(basic_head);
    this->response.append("Content-Type: text/html\r\n");
    this->response.append("Content-Length: " + to_string(this->fsize()));
    this->response.append("\r\n\r\n");
 
    char* body = this->read_ifile();
         
    string sbody(body, body+this->fsize());
       
    this->response = this->response + sbody + "\r\n\0";

    if(!this->read_multi) this->res_is_set = true;

    free(body);
}

Server::Server(string filename, bool read_multi) {
    this->fname = filename;
    this->read_multi = read_multi;

    ifstream f(this->fname);

    if(!f.good()) err("File '"+ this->fname + "' does not exist");

    f.close();

    if(this->read_multi) Server::set_response();
}


char* Server::read_ifile() {
    ifstream ifile(this->fname);

    ssize_t c = 0;
    char* ret = (char*) malloc(this->fsize());

    while(!ifile.eof()) ifile.get(ret[c++]);

    ifile.close();

    return ret;
}

void Server::handle_request(int ns) {
    this->m.lock();

    char buff[4096];

    if(recv(ns, buff, sizeof(buff), 0) == -1) {
        puts("recv");
        return;
    }
    puts(buff);

    Server::set_response();
    cout << this->response;
 
    if(send(ns, this->response.c_str(), this->response.size(), 0) < 0) {
        puts("send");
        return;
    }

    this->m.unlock();
 
    /* Close connection */
    close(ns);

    return;
}

void Server::Init(uint16_t port) {
    struct sockaddr_in server;
    struct sockaddr_in client;

    int namelen;
    int ns;

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

        thread(&Server::handle_request, this, ns).detach();

    }

    /* 
        This command will never be executed
    */
    close(s);
}

