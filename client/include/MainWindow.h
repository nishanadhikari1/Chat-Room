#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <gtkmm.h>
#include "LoginWindow.h"
#include "ClientGUI.h"

class MainWindow : public Gtk::Window
{
public:
    MainWindow();
    virtual ~MainWindow();

protected:
    // Signal handlers
    void on_login_success(const std::string& username, const std::string& server_ip, int port);
    void on_client_disconnected();

    // bool on_window_delete_event(GdkEventAny* event); 

    // Member widgets
    Gtk::Stack m_Stack;
    LoginWindow m_LoginWindow;
    ClientGUI m_ClientGUI;
    // ClientGUI m_ClientGUI;
};

#endif // MAINWINDOW_H
