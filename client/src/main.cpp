#include "LoginWindow.h"
#include "ClientGUI.h"
#include <gtkmm.h>

int main(int argc, char *argv[]) {
    auto app = Gtk::Application::create(argc, argv, "org.gtkmm.example");

    LoginWindow login_window;

    return app->run(login_window);

    return 0;
}
