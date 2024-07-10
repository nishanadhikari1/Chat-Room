#ifndef SERVER_H
#define SERVER_H

#include <vector>
#include <thread>
#include <string>
#include <mutex>
#include <netinet/in.h>

#define MAX_LEN 200
#define NUM_COLORS 6

struct Terminal
{
    int id;
    std::string name;
    int socket;
    std::thread th;
};

class Server
{
public:
    Server(int port);
    ~Server();
    void start();

private:
    int server_socket;
    int seed;
    int port;
    std::vector<Terminal> clients;
    std::string def_col;
    std::string colors[NUM_COLORS];
    std::mutex cout_mtx, clients_mtx;

    void accept_clients();
    void handle_client(int client_socket, int id);
    void set_name(int id, const char* name);
    void shared_print(const std::string& str, bool endLine = true);
    void broadcast_message(const std::string& message, int sender_id);
    void broadcast_message(int num, int sender_id);
    void end_connection(int id);
    std::string color(int code);
};

#endif
