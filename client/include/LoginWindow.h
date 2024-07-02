#ifndef LOGINWINDOW_H
#define LOGINWINDOW_H

#include <gtkmm.h>

class LoginWindow : public Gtk::Window {
public:
    LoginWindow();
    virtual ~LoginWindow();

protected:
    // Signal handlers
    void on_button_connect_clicked();

    // Child widgets
    Gtk::Box m_VBox;
    Gtk::Entry m_EntryUsername;
    Gtk::Entry m_EntryIPAddress;
    Gtk::Entry m_EntryPort;
    Gtk::Button m_ButtonConnect;
    Gtk::Label m_LabelUsername;
    Gtk::Label m_LabelIPAddress;
    Gtk::Label m_LabelPort;

    // Sub-division box
    Gtk::Box m_SubBox; // This will hold the username, IP address, port, and connect button
};

#endif // LOGINWINDOW_H
