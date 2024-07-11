#include "../include/MainWindow.h"

MainWindow::MainWindow()
    : m_LoginWindow(),
      m_ClientGUI()
{
    set_title("Chat Application");
    set_default_size(1920, 1080);

    // Add stack to the main window
    add(m_Stack);

    // Add LoginWindow and ClientGUI to the stack
    m_Stack.add(m_LoginWindow, "login");
    m_Stack.add(m_ClientGUI, "client");

    // Connect the login success signal to the slot
    m_LoginWindow.signal_login_success().connect(sigc::mem_fun(*this, &MainWindow::on_login_success));

    // Show the login window initially
    m_Stack.set_visible_child("login");

    show_all_children();
}

MainWindow::~MainWindow()
{
}

void MainWindow::on_login_success()
{
    // Switch to the client GUI when login is successful
    m_Stack.set_visible_child("client");
}
