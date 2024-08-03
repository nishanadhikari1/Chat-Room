#include "../include/ClientGUI.h"
#include <iostream>
#include <sys/socket.h>
#include <sys/types.h>
#include <sys/select.h>
#include <netdb.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <errno.h>
#include <sstream>
#include <gtkmm/cssprovider.h>
#include <gtkmm/stylecontext.h>

#define MAX_LEN 200 // max number of bytes at once

ClientGUI::ClientGUI()
: m_SendButton("Send"),
  m_MainBox(Gtk::ORIENTATION_VERTICAL),
  m_ClientListBox(Gtk::ORIENTATION_VERTICAL),
  m_TextBox(Gtk::ORIENTATION_VERTICAL),
  is_running(false),
  exitflag(false)
{
    // Load the CSS file
    auto cssProvider = Gtk::CssProvider::create();
    try
    {
        cssProvider->load_from_path("../client/src/ClientGUI.css");
    }
    catch (const Gtk::CssProviderError& e)
    {
        std::cerr << "Error loading CSS file: " << e.what() << std::endl;
    }

    // Get the default screen and add the CSS provider
    auto screen = Gdk::Screen::get_default();
    auto styleContext = Gtk::StyleContext::create();
    styleContext->add_provider_for_screen(screen, cssProvider, GTK_STYLE_PROVIDER_PRIORITY_USER);

    /*----------GUI COMPONENTS-----------*/
    set_name("client_gui");

    // Create a Toolbar
    m_ToolBarBox.set_orientation(Gtk::ORIENTATION_HORIZONTAL);
    m_ToolBarBox.set_name("toolbar");
    m_ToolBarBox.set_margin_top(5);

    // Toolbar items
    disconnect_button.set_label("Disconnect");
    disconnect_button.set_name("disconnect_button");
    disconnect_button.set_margin_end(10);
    m_ToolBarBox.pack_start(disconnect_button, Gtk::PACK_SHRINK);
    disconnect_button.signal_clicked().connect(sigc::mem_fun(*this, &ClientGUI::on_disconnect_button_clicked));

    m_ChatDisplay.set_halign(Gtk::ALIGN_FILL);
    m_ChatDisplay.set_valign(Gtk::ALIGN_FILL);
    m_ChatDisplay.set_hexpand(true);
    m_ChatDisplay.set_vexpand(true);
    m_ChatDisplay.set_editable(false);
    m_ChatDisplay.set_cursor_visible(false);
    m_ChatDisplay.set_name("chat_display");

    // Create a TextBuffer(chatBuffer)
    m_ChatBuffer = Gtk::TextBuffer::create();
    m_ChatDisplay.set_buffer(m_ChatBuffer);
    m_ChatDisplay.set_wrap_mode(Gtk::WRAP_WORD); // set word wrap mode

    // Message Entry Area
    Gtk::Box *m_MessageEntryBox = Gtk::manage(new Gtk::Box());
    m_MessageEntryBox->set_orientation(Gtk::ORIENTATION_HORIZONTAL);
    m_MessageEntryBox->pack_start(m_MessageEntry, Gtk::PACK_EXPAND_WIDGET);
    m_MessageEntry.set_name("message_entry");

    // Add send message button
    m_SendButton.set_margin_start(10);
    m_SendButton.set_name("send_button");
    m_MessageEntryBox->pack_end(m_SendButton, Gtk::PACK_SHRINK);
    m_SendButton.signal_clicked().connect(sigc::mem_fun(*this, &ClientGUI::on_send_button_clicked));
    m_MessageEntry.signal_key_press_event().connect(sigc::mem_fun(*this, &ClientGUI::on_message_entry_key_press), false);

    // Chatbox (TextBox)
    m_TextBox.pack_start(m_ToolBarBox, Gtk::PACK_SHRINK);
    m_TextBox.pack_start(m_ScrolledWindow, Gtk::PACK_SHRINK);
    m_ScrolledWindow.set_hexpand(true);
    m_ScrolledWindow.set_vexpand(true);
    m_ScrolledWindow.set_margin_bottom(10);
    m_ScrolledWindow.set_margin_top(10);
    m_ScrolledWindow.add(m_ChatDisplay);
    m_ScrolledWindow.set_name("scrolled_window");

    // Only show the scrollbars when necessary
    m_ScrolledWindow.set_policy(Gtk::POLICY_AUTOMATIC, Gtk::POLICY_AUTOMATIC);
    m_ScrolledWindow.set_size_request(1500, 850);
    // Add messageEntry to textbox
    m_TextBox.pack_start(*m_MessageEntryBox, Gtk::PACK_SHRINK);
    m_TextBox.set_name("text_box");

    // For client list box
    m_ClientListBox.pack_start(*InfoBox, Gtk::PACK_SHRINK);
    m_ClientListBox.set_name("client_list_box");
    InfoBox->set_orientation(Gtk::ORIENTATION_HORIZONTAL);
    InfoBox->pack_start(*OnlineUsersCount, Gtk::PACK_SHRINK);
    InfoBox->set_name("info_box");

    // Set the name of OnlineUsersCount for CSS styling
    OnlineUsersCount->set_name("online_users_count");

    // Create the tree Model
    m_ClientTreeModel = Gtk::ListStore::create(m_Columns);
    m_ClientTreeView.set_model(m_ClientTreeModel);
    m_ClientListBox.pack_start(m_ClientTreeView);
    m_ClientTreeView.set_name("client_tree_view");

    // Append a column (for users) without title
    m_ClientTreeView.append_column("", m_Columns.col_name);

    // Hide the column headers
    m_ClientTreeView.set_headers_visible(false);

    // Add TextBox and ClientListBox to the grid
    m_MainGrid.attach(m_TextBox, 0, 0, 1, 1);
    m_TextBox.set_margin_start(10);
    m_ClientListBox.set_size_request(300, -1);
    m_ClientListBox.set_margin_end(10);
    m_ClientListBox.set_vexpand(true);
    m_MainGrid.attach_next_to(m_ClientListBox, m_TextBox, Gtk::POS_RIGHT, 1, 1);
    m_MainGrid.set_name("main_grid");

    // Add grid to Main_Box
    m_MainGrid.set_column_spacing(10);
    m_MainBox.pack_start(m_MainGrid, Gtk::PACK_SHRINK);

    // Add mainbox to window
    pack_start(m_MainBox);
    m_MainBox.set_name("main_box");

    show_all_children();
}

ClientGUI::~ClientGUI(){
    disconnect_from_server();
}

//Start Client 
void ClientGUI::Start_Client(){
    if((client_socket=socket(AF_INET,SOCK_STREAM,0))==-1){
        perror("socket: ");
        exit(-1);
    }
    std::cout<<"Created Socket successfully"<<std::endl;

    client.sin_family=AF_INET; //ipv4
    client.sin_port = htons(port); //Port number of server
    if((inet_pton(AF_INET, server_ip.c_str(), &(client.sin_addr)))<=0){
        perror("inet pton: ");
        close(client_socket);
        exit(-1);
    }
    memset(&client.sin_zero, 0, sizeof(client.sin_zero));

    Connect_to_server();

    send(client_socket, username.c_str(), username.length(), 0); //send the name of client to server after connecting 

    //start receive thread 
    thread_recv = std::thread(&ClientGUI::Recv_messages, this, client_socket);

    is_running = true;
}

void ClientGUI::Connect_to_server(){
    if((connect(client_socket, (struct sockaddr*)&client, sizeof(struct sockaddr_in)))==-1){
        perror("connect: ");
        exit(-1);
    }
}

//send messages 
void ClientGUI::Send_messages(int client_socket, const std::string& message){
    send(client_socket,message.c_str(),message.length(),0);
}

void ClientGUI::Recv_messages(int client_socket){
    char recv_buffer[MAX_LEN]; //recv_buffer for holding messages 
    while(!stop_recv_thread){
        fd_set read_fds;
        FD_ZERO(&read_fds);
        FD_SET(client_socket, &read_fds);

        struct timeval timeout;
        timeout.tv_sec = 1; // 1 second timeout
        timeout.tv_usec = 0;

        int activity = select(client_socket + 1, &read_fds, nullptr, nullptr, &timeout);
        
        if (activity < 0) {
            perror("select error");
            continue; // Handle error as needed
        }

        if (activity == 0) {
            // Timeout occurred, check the stop condition
            continue;
        }

        if (FD_ISSET(client_socket, &read_fds)) {
        memset(recv_buffer, 0, sizeof(recv_buffer)); //clear buffer before each receive
        int bytes_received = recv(client_socket, recv_buffer, (sizeof(recv_buffer)), 0); //client list recv

        if (bytes_received <= 0) {
            if (bytes_received == 0) {
                std::cout << "connection closed" << std::endl;
            } else {
                perror("receive error in client: ");
            }
            is_running = false;
            break;
        }
        recv_buffer[bytes_received] = '\0'; //terminate the C-style string
        std::string message(recv_buffer); //convert to std::string

        if (message.find("#USERLIST") == 0) {
            // clientlist update
            update_client_list(message.substr(9)); // Remove the "#USERLIST" prefix
            int online_users = count_online_users(m_ClientTreeModel);
            update_online_count(online_users);

        }
        else{

        //updaate the chat display with the recieved message
        Glib::ustring messageWithColor = "<span foreground ='"+client_color + "'>" + message + "</span>";
        update_chat_display(Glib::ustring(messageWithColor));
        }
    }
}
}

//add online users in TreeView
void ClientGUI::add_userNameToList(const Glib::ustring &userName){
    Gtk::TreeModel::Row row = *(m_ClientTreeModel->append());
    row[m_Columns.col_name] = userName;
}

//update clientlist in client TreeView
void ClientGUI::update_client_list(const std::string& client_list){
    //clear the existing list
    m_ClientTreeModel->clear();

    //split the list to get users
    std::istringstream ss(client_list); //istringstream treats string like input stream
    std::string userName;

    while (std::getline(ss, userName)) {
        if (!userName.empty()) {
            // Add each username to the TreeView
            add_userNameToList(Glib::ustring(userName));
        }
    }
}

//update the text buffer with message
void ClientGUI::update_chat_display(const Glib::ustring& message){
    m_ChatBuffer->insert_markup(m_ChatBuffer->end(), message + "\n");
};

//action for send button clicked
void ClientGUI::on_send_button_clicked(){
    Glib::ustring message = m_MessageEntry.get_text();
    Glib::ustring messageWithColor = "<span foreground='" +client_color +"'>"+username +": " + message + "</span>"; //pango markup
    update_chat_display("<span foreground='"+ client_color +"'>You: " + message + "</span>");

    scroll_to_bottom();

    Send_messages(client_socket, std::string(messageWithColor));
    m_MessageEntry.set_text("");
}
void ClientGUI::scroll_to_bottom()
{
    auto adj = m_ScrolledWindow.get_vadjustment();
    adj->set_value(adj->get_upper() - adj->get_page_size());
}

void ClientGUI::setUserName(const std::string& userName){
    this->username = userName;
}

void ClientGUI::setServerIP(const std::string& serverIP){
    this->server_ip = serverIP;
}

void ClientGUI::setPort(int Port){
    this->port = Port;
}

void ClientGUI::setClientColor(std::string color){
    this->client_color = color;
}

void ClientGUI::disconnect_from_server() {
    stop_recv_thread = true; // Signal the thread to stop

    if (thread_recv.joinable()) {
        thread_recv.join(); // Wait for the thread to finish
    }

    close(client_socket); // Close the socket
    std::cout << "disconnected" << std::endl;
}

// count online users
int ClientGUI::count_online_users(const Glib::RefPtr<Gtk::ListStore>& user_list_store) {
    int count = 0;
    for (auto i = user_list_store->children().begin(); i != user_list_store->children().end(); ++i) {
        ++count;
    }
    return count;
}

void ClientGUI::update_online_count(int num){
    OnlineUsersCount->set_text("Online: " + std::to_string(num));
}

//handler for message entry enter key
bool ClientGUI::on_message_entry_key_press(GdkEventKey* key_event)
{
    if (key_event->keyval == GDK_KEY_Return || key_event->keyval == GDK_KEY_KP_Enter)
    {
        Glib::ustring message = m_MessageEntry.get_text();
        if (!message.empty())
        {
            on_send_button_clicked();
            m_MessageEntry.set_text(""); // Clear the text entry
        }
        return true; //handled successfully
    }
    return false;
}

void ClientGUI::on_disconnect_button_clicked(){
     disconnect_from_server(); // Disconnect from server

    // Signal to MainWindow to switch back to the login window
    signal_disconnected.emit();
}
