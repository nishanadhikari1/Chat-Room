#ifndef LOGINWINDOW_H
#define LOGINWINDOW_H

#include <gtkmm.h>
#include "ClientGUI.h"
class LoginWindow : public Gtk::Box {
public:
    LoginWindow();
    virtual ~LoginWindow();

    // Signal to indicate login success
    sigc::signal<void, const std::string&, const std::string&, int> signal_login_success();

protected:
    // Signal handlers
    void on_button_connect_clicked();

    // Child widgets
    Gtk::Box m_VBox;
    Gtk::Label m_TitleLabel;
    Gtk::Entry m_EntryUsername;
    Gtk::Entry m_EntryIPAddress;
    Gtk::Entry m_EntryPort;
    Gtk::Box m_ButtonBox;
    Gtk::Button m_ButtonConnect;
    Gtk::Label m_LabelUsername;
    Gtk::Label m_LabelIPAddress;
    Gtk::Label m_LabelPort;

    // Sub-division box
    Gtk::Box m_SubBox; // This will hold the username, IP address, port, and connect button

    // Signal
    sigc::signal<void, const std::string&, const std::string&, int > m_signal_login_success;
};

#endif // LOGINWINDOW_H
