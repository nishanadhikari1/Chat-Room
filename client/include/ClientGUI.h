//header file
#ifndef CLIENTGUI_H
#define CLIENTGUI_H

#include <gtkmm.h>
#include <thread>
#include <vector>
#include <string>
#include <mutex>

class ClientGUI : public Gtk::Box {
public:
        ClientGUI();
        virtual ~ClientGUI();
        void Connect_to_server(const std::string &ip, const std::string &port, const std::string &username);
        void Send_messages(const std::string &message);
        void Recv_messages();

protected:
    int client_sockfd;
    std::string username;
    std::thread thread_send, thread_recv;
    std::mutex chat_mutex; //for chat buffer I think

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
    Gtk::Label *Online = Gtk::manage(new Gtk::Label());
    Gtk::Label *OnlineNum = Gtk::manage(new Gtk::Label());

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
    void add_userName(const Glib::ustring &userName);

    void on_send_button_clicked();
    void on_connect_button_clicked();
    void update_chat_display(const Glib::ustring& message);

};

#endif //CLIENTGUI_H