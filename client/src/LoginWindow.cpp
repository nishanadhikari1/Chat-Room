#include "../include/LoginWindow.h"
#include "../include/ClientGUI.h"
#include <iostream>
#include <gtkmm/cssprovider.h>
#include <gtkmm/stylecontext.h>
#include <gdkmm/screen.h>
#include <iomanip>
#include <regex>
#include <string>

LoginWindow::LoginWindow()
    : Gtk::Box(Gtk::ORIENTATION_VERTICAL),
      m_VBox(Gtk::ORIENTATION_VERTICAL),
      m_SubBox(Gtk::ORIENTATION_VERTICAL),
      m_ButtonBox(Gtk::ORIENTATION_VERTICAL) // Initialize button box
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

    // Configure and add the VBox
    m_VBox.set_halign(Gtk::ALIGN_CENTER); 
    m_VBox.set_valign(Gtk::ALIGN_CENTER); 
    m_VBox.set_size_request(400, 500);

    // Add title
    m_TitleLabel.set_text("Connect & Chat");
    m_TitleLabel.set_name("title_label");
    m_TitleLabel.set_margin_bottom(20);

    m_VBox.pack_start(m_TitleLabel, Gtk::PACK_SHRINK); 

    // Configure the sub-box
    m_SubBox.set_border_width(0);
    m_SubBox.set_name("sub_box");
    m_SubBox.set_size_request(400, 300); 
    m_SubBox.set_halign(Gtk::ALIGN_CENTER); 
    m_SubBox.set_valign(Gtk::ALIGN_CENTER); 
    m_SubBox.set_spacing(10); // 

    // Add elements to the sub-box
    m_LabelUsername.set_text("Username:");
    m_EntryUsername.set_placeholder_text("Enter username");
    m_SubBox.pack_start(m_LabelUsername, Gtk::PACK_SHRINK); 
    m_SubBox.pack_start(m_EntryUsername, Gtk::PACK_SHRINK); 

    m_LabelIPAddress.set_text("IP Address:");
    m_EntryIPAddress.set_placeholder_text("Enter IP address");
    m_SubBox.pack_start(m_LabelIPAddress, Gtk::PACK_SHRINK);
    m_SubBox.pack_start(m_EntryIPAddress, Gtk::PACK_SHRINK);

    m_LabelPort.set_text("Port:");
    m_EntryPort.set_placeholder_text("Enter port");
    m_SubBox.pack_start(m_LabelPort, Gtk::PACK_SHRINK);
    m_SubBox.pack_start(m_EntryPort, Gtk::PACK_SHRINK);

    // Add the color button
    m_LabelColor.set_text("Pick Color");
    m_SubBox.pack_start(m_LabelColor, Gtk::PACK_SHRINK);
    m_ColorButton.set_title("Select Color");
    m_ColorButton.set_name("color_button");
    m_ColorButton.set_rgba(Gdk::RGBA("255,255,255")); // Set default color to white

    // Initialize and configure the button box
    m_ButtonBox.set_halign(Gtk::ALIGN_CENTER);
    m_ButtonBox.set_valign(Gtk::ALIGN_CENTER); 
    m_ButtonBox.set_spacing(6); 

    //Configure Connect Button
    m_ButtonConnect.set_label("Connect");
    m_ButtonConnect.set_name("button_connect");
    m_ButtonConnect.set_size_request(100, 30); 
    m_ButtonConnect.set_margin_top(1); 

    // Set text alignment for entry boxes
    m_EntryUsername.set_alignment(Gtk::ALIGN_CENTER); 
    m_EntryIPAddress.set_alignment(Gtk::ALIGN_CENTER); 
    m_EntryPort.set_alignment(Gtk::ALIGN_CENTER); 

    // CSS classes for input boxes
    m_EntryUsername.get_style_context()->add_class("entry");
    m_EntryIPAddress.get_style_context()->add_class("entry");
    m_EntryPort.get_style_context()->add_class("entry");

    m_ButtonConnect.signal_clicked().connect(sigc::mem_fun(*this, &LoginWindow::on_button_connect_clicked));

    m_ButtonBox.pack_start(m_ColorButton, Gtk::PACK_SHRINK);
    m_ButtonBox.pack_start(m_ButtonConnect, Gtk::PACK_SHRINK); 

    // Add the button box to the sub-box
    m_SubBox.pack_start(m_ButtonBox, Gtk::PACK_SHRINK); 

    // Add the sub-box to the VBox
    m_VBox.pack_start(m_SubBox, Gtk::PACK_EXPAND_WIDGET); 

    // Finally, add the VBox to the window
    pack_start(m_VBox, Gtk::PACK_EXPAND_WIDGET);

    show_all_children();
}

LoginWindow::~LoginWindow()
{
}

void LoginWindow::showErrorDialog(Gtk::Window& parent, const std::string& message) {
    Gtk::MessageDialog dialog(parent, message, false, Gtk::MESSAGE_ERROR, Gtk::BUTTONS_OK, true);
    dialog.run();
    dialog.set_name("error_dialog");
}

void LoginWindow::on_button_connect_clicked()
{
    std::string username = m_EntryUsername.get_text();
    std::string server_ip = m_EntryIPAddress.get_text();
    std::string port_str = m_EntryPort.get_text();
    std::string color = get_Color();

    // Validate username
    if (username.empty()) {
        // Display error message
        showErrorDialog(*dynamic_cast<Gtk::Window*>(get_toplevel()), "Username cannot be empty");
        return;
    }

    // Validate IP address (use a regular expression or IP validation library)
    if (!isValidIPAddress(server_ip)) {
        showErrorDialog(*dynamic_cast<Gtk::Window*>(get_toplevel()), "Invalid IP address");
        return;
    }

    // Validate port number (convert to integer and check range)
    int port;
    try {
        port = std::stoi(port_str);
        if (port < 0 || port > 65535) {
            throw std::out_of_range("Invalid port number");
        }
    } catch (const std::invalid_argument& e) {
        showErrorDialog(*dynamic_cast<Gtk::Window*>(get_toplevel()), "Invalid port number");
        return;
    } catch (const std::out_of_range& e) {
        showErrorDialog(*dynamic_cast<Gtk::Window*>(get_toplevel()), "Port number out of range");
        return;
    }
    m_signal_login_success.emit(username, server_ip, port, color);
}

sigc::signal<void, const std::string&, const std::string&, int, const std::string&> LoginWindow::signal_login_success()
{
    return m_signal_login_success;
}

bool LoginWindow::isValidIPAddress(const std::string& ip) {
    std::regex pattern(R"((\b(?:(?:25[0-5]|2[0-4][0-9]|[01]?[0-9][0-9]?)\.){3}(?:25[0-5]|2[0-4][0-9]|[01]?[0-9][0-9]?)\b))");
    return std::regex_match(ip, pattern);
}

std::string LoginWindow::rgba_to_hex(const Gdk::RGBA& color) const {
    std::stringstream ss;
    ss << "#" 
       << std::setw(2) << std::setfill('0') << std::hex << static_cast<int>(color.get_red() * 255)
       << std::setw(2) << std::setfill('0') << std::hex << static_cast<int>(color.get_green() * 255)
       << std::setw(2) << std::setfill('0') << std::hex << static_cast<int>(color.get_blue() * 255);
    return ss.str();
}

std::string LoginWindow::get_Color() const {
    Gdk::RGBA color = m_ColorButton.get_rgba();
    return rgba_to_hex(color);
}
