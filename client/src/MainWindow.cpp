#include "../include/MainWindow.h"
#include <iostream>
#include <future>

MainWindow::MainWindow()
    : m_LoginWindow(),
    m_ClientGUI()
{
    set_title("Chat Application");
    set_default_size(1920, 1080);

    // Add stack to the main window
    add(m_Stack);

    // Add LoginWindow and ClientGUI to the stack
    m_Stack.add(m_LoginWindow, "login");
    m_Stack.add(m_ClientGUI, "client");

    // Connect the login success signal to the slot
    m_LoginWindow.signal_login_success().connect(sigc::mem_fun(*this, &MainWindow::on_login_success));

     // Connect the delete event to the handler
    // signal_delete_event().connect(sigc::mem_fun(*this, &MainWindow::on_window_delete_event),false);

    // Show the login window initially
    m_Stack.set_visible_child("login");

    show_all_children();
}

MainWindow::~MainWindow()
{
    
}

void MainWindow::on_login_success(const std::string& username, const std::string& server_ip, int port)
{
    m_ClientGUI.setUserName(username);
    m_ClientGUI.setServerIP(server_ip);
    m_ClientGUI.setPort(port);

    //set ClientGUI to visible
    m_Stack.set_visible_child("client"); 
    //start ClientGUI
    m_ClientGUI.Start_Client();
    std::cout<<"started client network"<<std::endl;
}

// bool MainWindow::on_window_delete_event(GdkEventAny* event)
// {
//     std::cout << "Main window is closing, disconnecting the client" << std::endl;
//      std::thread disconnect_thread([this]() {
//         m_ClientGUI.disconnect_from_server(); // Disconnect the client
//      });
//         disconnect_thread.detach(); // Detach the disconnection thread to avoid blocking
    
//     return false; 
// }
