#include "../include/LoginWindow.h"
#include "../include/ClientGUI.h"
#include <iostream>

LoginWindow::LoginWindow()
    : m_VBox(Gtk::ORIENTATION_VERTICAL),
      m_SubBox(Gtk::ORIENTATION_VERTICAL)
{

    set_border_width(10);

    // Initialize and add the sub-box to the main VBox
    m_VBox.pack_start(m_SubBox);

    // Set properties for the sub-box
    m_SubBox.set_size_request(400, 300); // Set size to 400x300
    m_SubBox.set_halign(Gtk::ALIGN_CENTER); // Center horizontally
    m_SubBox.set_valign(Gtk::ALIGN_CENTER); // Center vertically
    m_SubBox.set_spacing(10); // Add spacing between elements

    // Add elements to the sub-box
    m_LabelUsername.set_text("Username:");
    m_EntryUsername.set_placeholder_text("Enter username");
    m_SubBox.pack_start(m_LabelUsername, Gtk::PACK_SHRINK); // PACK_SHRINK to keep label size small
    m_SubBox.pack_start(m_EntryUsername, Gtk::PACK_SHRINK); // PACK_SHRINK to keep entry size small

    m_LabelIPAddress.set_text("IP Address:");
    m_EntryIPAddress.set_placeholder_text("Enter IP address");
    m_SubBox.pack_start(m_LabelIPAddress, Gtk::PACK_SHRINK);
    m_SubBox.pack_start(m_EntryIPAddress, Gtk::PACK_SHRINK);

    m_LabelPort.set_text("Port:");
    m_EntryPort.set_placeholder_text("Enter port");
    m_SubBox.pack_start(m_LabelPort, Gtk::PACK_SHRINK);
    m_SubBox.pack_start(m_EntryPort, Gtk::PACK_SHRINK);

    m_SubBox.pack_start(m_ButtonConnect);
    m_ButtonConnect.set_label("Connect");
    m_ButtonConnect.set_margin_top(10); // Add top margin to the button for spacing

    // Connect button signal
    m_ButtonConnect.signal_clicked().connect(sigc::mem_fun(*this, &LoginWindow::on_button_connect_clicked));

    pack_start(m_VBox);

    show_all_children();
}

LoginWindow::~LoginWindow()
{
}

void LoginWindow::on_button_connect_clicked()
{
    std::string username = m_EntryUsername.get_text();
    std::string server_ip = m_EntryIPAddress.get_text();
    int port = std::stoi(m_EntryPort.get_text());

    std::cout << "Username: " << username << std::endl;
    std::cout << "IP Address: " << server_ip << std::endl;
    std::cout << "Port: " << port << std::endl;
    // You can now use these details to connect to the server
    // Emit the login success signal
    m_signal_login_success.emit(username, server_ip, port);
}

sigc::signal<void, const std::string&, const std::string&, int> LoginWindow::signal_login_success()
{
    return m_signal_login_success;
}


