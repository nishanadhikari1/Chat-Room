#include "../include/MainWindow.h"
#include <gtkmm.h>


int main(int argc, char *argv[]) {
    auto app = Gtk::Application::create(argc, argv, "org.gtkmm.example");

    MainWindow login_window;

    return app->run(login_window);

    return 0;
}
