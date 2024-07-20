#include "../include/LoginWindow.h"
#include "../include/ClientGUI.h"
#include <iostream>
#include <gtkmm/cssprovider.h>
#include <gtkmm/stylecontext.h>
#include <gdkmm/screen.h>

LoginWindow::LoginWindow()
    : m_VBox(Gtk::ORIENTATION_VERTICAL),
      m_SubBox(Gtk::ORIENTATION_VERTICAL),
      m_ButtonBox(Gtk::ORIENTATION_HORIZONTAL) // Initialize button box
{
    // Load the CSS file
    auto cssProvider = Gtk::CssProvider::create();
    try
    {
        cssProvider->load_from_path("../client/src/LoginWindow.css");
    }
    catch (const Gtk::CssProviderError& e)
    {
        std::cerr << "Error loading CSS file: " << e.what() << std::endl;
    }

    // Get the default screen and add the CSS provider
    auto screen = Gdk::Screen::get_default();
    auto styleContext = Gtk::StyleContext::create();
    styleContext->add_provider_for_screen(screen, cssProvider, GTK_STYLE_PROVIDER_PRIORITY_USER);

    // Set the CSS ID for this window
    set_name("login_window");

    set_border_width(0);
    // set_default_size(400, 500); // Set a default size for the window

    // Configure and add the VBox
    m_VBox.set_halign(Gtk::ALIGN_CENTER); // Center VBox horizontally
    m_VBox.set_valign(Gtk::ALIGN_CENTER); // Center VBox vertically
    m_VBox.set_size_request(400, 500); // Set size for VBox

    // Add title
    m_TitleLabel.set_text("Connect & Chat");
    m_TitleLabel.set_name("title_label"); // Optional: set CSS class for styling
    m_TitleLabel.set_margin_bottom(20);

    m_VBox.pack_start(m_TitleLabel, Gtk::PACK_SHRINK); // Add title to VBox

    // Configure the sub-box
    m_SubBox.set_border_width(0);
    m_SubBox.set_name("sub_box");
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

    // Initialize and configure the button box
    m_ButtonBox.set_halign(Gtk::ALIGN_CENTER); // Center horizontally
    m_ButtonBox.set_valign(Gtk::ALIGN_CENTER); // Center vertically
    m_ButtonBox.set_spacing(10); // Add spacing inside button box

    m_ButtonConnect.set_label("Connect");
    m_ButtonConnect.set_name("button_connect");
    m_ButtonConnect.set_size_request(100, 30); // Set desired size for the button
    m_ButtonConnect.set_margin_top(10); // Add top margin to the button for spacing

    // Set text alignment for entry boxes
    m_EntryUsername.set_alignment(Gtk::ALIGN_CENTER); // Align text to the start
    m_EntryIPAddress.set_alignment(Gtk::ALIGN_CENTER); // Align text to the start
    m_EntryPort.set_alignment(Gtk::ALIGN_CENTER); // Align text to the start

    // CSS classes for input boxes
    m_EntryUsername.get_style_context()->add_class("entry");
    m_EntryIPAddress.get_style_context()->add_class("entry");
    m_EntryPort.get_style_context()->add_class("entry");

    m_ButtonConnect.signal_clicked().connect(sigc::mem_fun(*this, &LoginWindow::on_button_connect_clicked));

    m_ButtonBox.pack_start(m_ButtonConnect, Gtk::PACK_SHRINK); // Ensure button is packed last

    // Add the button box to the sub-box
    m_SubBox.pack_start(m_ButtonBox, Gtk::PACK_SHRINK); // Ensure button box is packed last

    // Add the sub-box to the VBox
    m_VBox.pack_start(m_SubBox, Gtk::PACK_EXPAND_WIDGET); // Ensure VBox expands

    // Finally, add the VBox to the window
    pack_start(m_VBox, Gtk::PACK_EXPAND_WIDGET);

    show_all_children();
}

LoginWindow::~LoginWindow()
{
}

void LoginWindow::on_button_connect_clicked()
{
    std::string username = m_EntryUsername.get_text();
    std::string ip_address = m_EntryIPAddress.get_text();
    int port = std::stoi(m_EntryPort.get_text());

    std::cout << "Username: " << username << std::endl;
    std::cout << "IP Address: " << ip_address << std::endl;
    std::cout << "Port: " << port << std::endl;
    // You can now use these details to connect to the server
    // Emit the login success signal
    m_signal_login_success.emit();
}

sigc::signal<void> LoginWindow::signal_login_success()
{
    return m_signal_login_success;
}
