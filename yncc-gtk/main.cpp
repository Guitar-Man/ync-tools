/* 
 * Copyright (C) 2018, Alexandre Courtois
 * 
 * This file is part of YNCC-GTK.
 *
 * YNCC-GTK is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * YNCC-GTK is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with YNCC-GTK.  If not, see <http://www.gnu.org/licenses/>.
 * 
 */

#include <gtkmm.h>
#include "yncc-gtk-window.h"
#include <locale.h>
#include <libintl.h>


int main(int argc, char *argv[]) {
    setlocale(LC_ALL, "");
    textdomain("yncc-gtk");
    
    auto app = Gtk::Application::create(argc, argv, "yncc-gtk");
    app->run(*yncc_gtk::Yncc_Gtk_Window().getGtkWindowPtr());
    return 0;
}
