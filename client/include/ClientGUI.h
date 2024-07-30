//header file
#ifndef CLIENTGUI_H
#define CLIENTGUI_H

#include <gtkmm.h>
#include <thread>
#include <vector>
#include <string>
#include <mutex>
#include <netinet/in.h>
#include <atomic>

class ClientGUI : public Gtk::Box {
public:
        ClientGUI();
        virtual ~ClientGUI();
        void Start_Client();
        void Connect_to_server();
        void disconnect_from_server();
        void Send_messages(int client_socket, const std::string& message);
        void Recv_messages(int client_socket);
        void setUserName(const std::string& userName);
        void setServerIP(const std::string& serverIP);
        void setPort(int Port);
        sigc::signal<void> signal_disconnected; //signal emit on disconnect button clicked

private:
    int client_socket;
    std::string username;
    std::string server_ip;
    int port;
    std::thread thread_recv;
    std::atomic<bool> is_running;
    std::atomic_bool stop_recv_thread = false;

    struct sockaddr_in client;  //ipv4 scokaddr struct for client
    bool exitflag;
    // std::mutex chat_mutex; //for chat buffer

protected:
    //Gui components
    Gtk::Box m_MainBox;
    Gtk::MenuBar m_Menu;
    Gtk::Grid m_MainGrid; //two sections
    
    //TextBox Items
    Gtk::Box m_TextBox;
    Gtk::Box m_ToolBarBox;
    Gtk::TextView m_ChatDisplay;
    Gtk::Entry m_MessageEntry;
    Gtk::Button m_SendButton;
    Glib::RefPtr<Gtk::TextBuffer> m_ChatBuffer;
    Gtk::ScrolledWindow m_ScrolledWindow;

    //ClientListBox Items
    Gtk::Box m_ClientListBox;
    Gtk::Box *InfoBox = Gtk::manage(new Gtk::Box());
    Gtk::Label *OnlineUsersCount = Gtk::manage(new Gtk::Label());

    //Tree model columns
    class ModelColumns : public Gtk::TreeModel::ColumnRecord{
        public:
            ModelColumns(){
                add(col_name);
            }
            Gtk::TreeModelColumn<Glib::ustring> col_name;
    };
    ModelColumns m_Columns;
    Glib::RefPtr<Gtk::ListStore> m_ClientTreeModel;
    Gtk::TreeView m_ClientTreeView;

    //function to add add item(userName) to liststore(ClientTreeModel)
    void add_userNameToList(const Glib::ustring &userName);
    void update_client_list(const std::string& client_list);

    void on_send_button_clicked();
    bool on_message_entry_key_press(GdkEventKey* key_event); //handler for enter key for message entry
    void on_connect_button_clicked();
    void update_chat_display(const Glib::ustring& message);
    void update_online_count(int num);
    void on_disconnect_button_clicked();
    int count_online_users(const Glib::RefPtr<Gtk::ListStore>& user_list_store);

};

#endif //CLIENTGUI