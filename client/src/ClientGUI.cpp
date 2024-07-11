#include "../include/ClientGUI.h"
#include <iostream>
#include <sys/socket.h>
#include <sys/types.h>
#include <netdb.h>
#include <netinet/in.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <errno.h>

#define PORT "2222" // the port client will be connecting to

#define MAXDATASIZE 100 // max number of bytes we can get at once

ClientGUI::ClientGUI()
: m_SendButton("Send"),
  m_MainBox(Gtk::ORIENTATION_VERTICAL),
  m_ClientListBox(Gtk::ORIENTATION_VERTICAL),
  m_TextBox(Gtk::ORIENTATION_VERTICAL)
{

   /*----------GUI COMPONENTS-----------*/
   // Create the MenuBar
    auto menu_bar = Gtk::manage(new Gtk::MenuBar());
    m_MainBox.pack_start(*menu_bar, Gtk::PACK_SHRINK);
    
    //file
    auto file_menu = Gtk::manage(new Gtk::Menu());
    auto file_item = Gtk::manage(new Gtk::MenuItem("File", true));
    menu_bar->append(*file_item);

    //quit
    auto quit_menu = Gtk::manage(new Gtk::Menu());
    auto quit_item = Gtk::manage(new Gtk::MenuItem("Quit", true));
    menu_bar->append(*quit_item);

    // Create a Toolbar
    // auto toolbar = Gtk::manage(new Gtk::Toolbar());
    m_ToolBarBox.set_orientation(Gtk::ORIENTATION_HORIZONTAL);

    //toolbar items
    auto option_button = Gtk::manage(new Gtk::ToolButton());
    option_button->set_label("Options");
    option_button->set_margin_end(10);
    m_ToolBarBox.pack_start(*option_button, Gtk::PACK_SHRINK);

    auto search_label = Gtk::manage(new Gtk::Label());
    search_label->set_text("Search");
    search_label->set_margin_end(10);
    m_ToolBarBox.pack_start(*search_label, Gtk::PACK_SHRINK);
  
    auto search_entry = Gtk::manage(new Gtk::Entry());
    m_ToolBarBox.pack_start(*search_entry, Gtk::PACK_SHRINK);

    m_ToolBarBox.set_margin_top(5);
    //chatDisplay (TextView)
    // m_ChatDisplay.set_size_request(1200,-1);
    m_ChatDisplay.set_halign(Gtk::ALIGN_FILL);
    m_ChatDisplay.set_valign(Gtk::ALIGN_FILL);
    m_ChatDisplay.set_hexpand(true);
    m_ChatDisplay.set_vexpand(true);

    //Create a TextBuffer(chatBuffer)
    m_ChatBuffer = Gtk::TextBuffer::create();
    //Set buffer to text view
    m_ChatDisplay.set_buffer(m_ChatBuffer);
    m_ChatDisplay.set_wrap_mode(Gtk::WRAP_WORD); //set word wrap mode

    //Message Entry Area
    Gtk::Box *m_MessageEntryBox = Gtk::manage(new Gtk::Box());
    m_MessageEntryBox->set_orientation(Gtk::ORIENTATION_HORIZONTAL);
    m_MessageEntryBox->pack_start(m_MessageEntry, Gtk::PACK_EXPAND_WIDGET);

    //add send message button
    m_SendButton.set_margin_start(10);
    m_MessageEntryBox->pack_end(m_SendButton, Gtk::PACK_SHRINK);
    m_SendButton.signal_clicked().connect(sigc::mem_fun(*this, &ClientGUI::on_send_button_clicked));

    //chatbox (TextBox)
    m_TextBox.pack_start(m_ToolBarBox, Gtk::PACK_SHRINK);
    m_TextBox.pack_start(m_ScrolledWindow, Gtk::PACK_SHRINK);
    m_ScrolledWindow.set_hexpand(true);
    m_ScrolledWindow.set_vexpand(true);
    m_ScrolledWindow.set_margin_bottom(10);
    m_ScrolledWindow.set_margin_top(10);
    m_ScrolledWindow.add(m_ChatDisplay);

    //Only show the scrollbars when necessary
    m_ScrolledWindow.set_policy(Gtk::POLICY_AUTOMATIC, Gtk::POLICY_AUTOMATIC);
    m_ScrolledWindow.set_size_request(1500,880);
    //Add messageEntry to textbox
    m_TextBox.pack_start(*m_MessageEntryBox, Gtk::PACK_SHRINK);
    
    //for client list box
    m_ClientListBox.pack_start(*InfoBox, Gtk::PACK_SHRINK);
    InfoBox->set_orientation(Gtk::ORIENTATION_HORIZONTAL);
    InfoBox->pack_start(*Online,Gtk::PACK_SHRINK);
    InfoBox->pack_start(*OnlineNum, Gtk::PACK_SHRINK);
    Online->set_text("Online: ");
    OnlineNum->set_text("1");

    //create the tree Model
    m_ClientTreeModel = Gtk::ListStore::create(m_Columns);
    m_ClientTreeView.set_model(m_ClientTreeModel);
    m_ClientListBox.pack_start(m_ClientTreeView);

    //Append a column(for users) without title
    m_ClientTreeView.append_column("",m_Columns.col_name);
    add_userName("Roshan");
    add_userName("Nishan");
    add_userName("Prajwal");

    //hide the column headers
    m_ClientTreeView.set_headers_visible(false);

    // Add TextBox and ClientListBox to the grid
    m_MainGrid.attach(m_TextBox, 0,0,1,1);
    m_TextBox.set_margin_start(10);
    // m_TextBox.set_margin_end(10);
    m_ClientListBox.set_size_request(300,-1);
    m_ClientListBox.set_margin_end(10);
    m_ClientListBox.set_vexpand(true);
    m_MainGrid.attach_next_to(m_ClientListBox, m_TextBox, Gtk::POS_RIGHT,1,1);

    //Add grid to Main_Box
    m_MainGrid.set_column_spacing(10);
    m_MainBox.pack_start(m_MainGrid, Gtk::PACK_SHRINK);

    //Add mainbox to window 
    pack_start(m_MainBox);

    show_all_children();

    /*--------------------------------*/

    //Stream Client Logic 
    

};

ClientGUI::~ClientGUI(){

}

//add online users in treeView
void ClientGUI::add_userName(const Glib::ustring &userName){
    Gtk::TreeModel::Row row = *(m_ClientTreeModel->append());
    row[m_Columns.col_name] = userName;
}

//update the text buffer with message
void ClientGUI::update_chat_display(const Glib::ustring& message){
    m_ChatBuffer->insert(m_ChatBuffer->end(), message + "\n");
};

//action for send button clicked
void ClientGUI::on_send_button_clicked(){
    Glib::ustring message = m_MessageEntry.get_text();
    update_chat_display(message);
}
