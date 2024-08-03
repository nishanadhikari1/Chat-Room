#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <gtkmm.h>
#include "LoginWindow.h"
#include "ClientGUI.h"

class MainWindow : public Gtk::Window
{
public:
    MainWindow();

protected:
    // Signal handlers
    void on_login_success(const std::string& username, const std::string& server_ip, int port, const std::string&);
    void on_client_disconnected();

    // Member widgets
    Gtk::Stack m_Stack;
    LoginWindow m_LoginWindow;
    ClientGUI m_ClientGUI;
};

#endif // MAINWINDOW_H
