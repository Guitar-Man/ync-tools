#include <gtkmm.h>
#include "yncc-gtk-window.h"
#include <locale.h>
#include <libintl.h>


int main(int argc, char *argv[]) {
    setlocale(LC_ALL, "");
    //bindtextdomain("yncc-gtk", getenv("PWD"));
    textdomain("yncc-gtk");
    
    auto app = Gtk::Application::create(argc, argv, "yncc-gtk");
    app->run(*yncc_gtk::Yncc_Gtk_Window().getGtkWindowPtr());
    return 0;
}
