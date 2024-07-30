#include "Server.h"
#include <iostream>
#include <algorithm>
#include <cstring>
#include <unistd.h>
#include <arpa/inet.h>
#include <ifaddrs.h>

Server::Server(int port) : port(port), seed(0), def_col("\033[0m")
{
    colors[0] = "\033[31m";
    colors[1] = "\033[32m";
    colors[2] = "\033[33m";
    colors[3] = "\033[34m";
    colors[4] = "\033[35m";
    colors[5] = "\033[36m";
}

Server::~Server()
{
    close(server_socket);
}

void Server::start()
{
    if ((server_socket = socket(AF_INET, SOCK_STREAM, 0)) == -1)
    {
        perror("socket: ");
        exit(-1);
    }


    struct sockaddr_in server;
    server.sin_family = AF_INET;
    server.sin_port = htons(port);
    server.sin_addr.s_addr = INADDR_ANY;
    bzero(&server.sin_zero, 0);

    if ((bind(server_socket, (struct sockaddr*)&server, sizeof(struct sockaddr_in))) == -1)
    {
        perror("bind error: ");
        exit(-1);
    }

    if ((listen(server_socket, 8)) == -1)
    {
        perror("listen error: ");
        exit(-1);
    }

    std::cout << colors[NUM_COLORS - 1] << "\n\t  ====== Welcome to the chat-room ======   " << std::endl << def_col;

    accept_clients();

    for (auto& client : clients)
    {
        if (client.th.joinable())
            client.th.join();
    }
}

void Server::accept_clients()
{
    struct sockaddr_in client;
    unsigned int len = sizeof(sockaddr_in);

    while (true)
    {
        int client_socket;
        if ((client_socket = accept(server_socket, (struct sockaddr*)&client, &len)) == -1)
        {
            perror("accept error: ");
            continue; // Don't exit on accept error
        }
        seed++;
        std::thread t(&Server::handle_client, this, client_socket, seed);
        {
            std::lock_guard<std::mutex> guard(clients_mtx);
            clients.push_back({seed, "Anonymous", client_socket, std::move(t)});
        }
    }
}

void Server::handle_client(int client_socket, int id)
{
    char name[MAX_LEN], str[MAX_LEN];
    memset(name, 0, sizeof(name)); // Clear buffer before using it
    memset(str, 0, sizeof(str)); // Clear buffer before using it
    int bytes_received = recv(client_socket, name, (sizeof(name)-1), 0);
    if (bytes_received <= 0)
    {
        close(client_socket);
        return;
    }
    set_name(id, name);

    std::string welcome_message = std::string(name) + " has joined";
    // broadcast_message("#NULL", id);
    // broadcast_message(id, id);
    broadcast_message(welcome_message, id);
    shared_print(color(id) + welcome_message + def_col);

    //broadcast the updated clientList
    broadcast_client_list();

    while (true)
    {
        bytes_received = recv(client_socket, str, sizeof(str), 0);
        if (bytes_received == 0){
            std::string left_message = std::string(name) + " has left";
            // broadcast_message("#NULL", id);
            // broadcast_message(id, id);
            broadcast_message(left_message, id);
            shared_print(color(id) + left_message + def_col);
            end_connection(id);
            //update client list
            clients.erase(std::next(clients.begin(),id));
            //boradcast clientlist
            broadcast_client_list();
            break;
        }
        if(bytes_received<0){
            perror("recieve error: ");
        }

        // broadcast_message(std::string(name), id);
        // broadcast_message(id, id);

        str[bytes_received] = '\0'; // Null terminate
        std::string sent_message = std::string(name) + ": "+ std::string(str);
        broadcast_message(sent_message, id);
        shared_print(color(id) + name + " : " + def_col + str);
    }
}

void Server::set_name(int id, const char* name)
{
    std::lock_guard<std::mutex> guard(clients_mtx);
    for (auto& client : clients)
    {
        if (client.id == id)
        {
            client.name = std::string(name);
            break;
        }
    }
}

void Server::shared_print(const std::string& str, bool endLine)
{
    std::lock_guard<std::mutex> guard(cout_mtx);
    std::cout << str;
    if (endLine)
        std::cout << std::endl;
}

void Server::broadcast_message(const std::string& message, int sender_id)
{
    char temp[MAX_LEN];
    strncpy(temp, message.c_str(), sizeof(temp));
    temp[sizeof(temp) - 1] = '\0'; // Ensure null-termination
    std::lock_guard<std::mutex> guard(clients_mtx);
    for (const auto& client : clients)
    {
        if (client.id != sender_id)
        {
            send(client.socket, temp, sizeof(temp), 0);
        }
    }
}

void Server::broadcast_message(int num, int sender_id)
{
    std::lock_guard<std::mutex> guard(clients_mtx);
    for (const auto& client : clients)
    {
        if (client.id != sender_id)
        {
            send(client.socket, &num, sizeof(num), 0);
        }
    }
}

void Server::end_connection(int id)
{
    std::lock_guard<std::mutex> guard(clients_mtx);
    auto it = std::remove_if(clients.begin(), clients.end(), [id](const client& client) {
        return client.id == id;
    });
    if (it != clients.end())
    {
        it->th.detach();
        close(it->socket);
        clients.erase(it, clients.end());
    }
    // broadcast_client_list();
}

std::string Server::color(int code)
{
    return colors[code % NUM_COLORS];
}

std::string Server::get_client_list(){
    std::lock_guard<std::mutex> guard(clients_mtx);
    std::string client_list = "#USERLIST"; //special identifier for client list
    for (const auto& client : clients)
    {
        client_list += "\n" + client.name;
    }
    return client_list;
}

void Server::broadcast_client_list()
{
    std::string client_list = get_client_list();
    broadcast_message(client_list, -1); //-1 will never match any client id
    
}




