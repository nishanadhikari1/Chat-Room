#ifndef LOGINWINDOW_H
#define LOGINWINDOW_H

#include <gtkmm.h>

class LoginWindow : public Gtk::Box {
public:
    LoginWindow();
    virtual ~LoginWindow();

    sigc::signal<void, const std::string&, const std::string&, int, const std::string&> signal_login_success();

protected:
    // Signal handlers
    void on_button_connect_clicked();
    void showErrorDialog(Gtk::Window& parent, const std::string& message);

    // Member widgets
    Gtk::Box m_VBox;
    Gtk::Box m_SubBox;
    Gtk::Box m_ButtonBox;
    Gtk::Label m_LabelUsername;
    Gtk::Label m_LabelIPAddress;
    Gtk::Label m_LabelPort;
    Gtk::Label m_LabelColor;
    Gtk::Label m_TitleLabel;
    Gtk::Entry m_EntryUsername;
    Gtk::Entry m_EntryIPAddress;
    Gtk::Entry m_EntryPort;
    Gtk::ColorButton m_ColorButton;
    Gtk::Button m_ButtonConnect;

    sigc::signal<void, const std::string&, const std::string&, int, const std::string&> m_signal_login_success;

    // Utility functions
    bool isValidIPAddress(const std::string& ip);
    std::string rgba_to_hex(const Gdk::RGBA& color) const;
    std::string get_Color() const;
};

#endif // LOGINWINDOW_H
