# ync-tools

Yncc, Yncc-Gtk and Yncapi are small command-line tool, GUI and library to drive Yamaha network players through network.

This is a mainly a hobby project, written to drive my Yamaha CD-N301 through my home network, without using the
official Android app provided by the manufacturer. The code is written in C++, and uses 4 external libraries, namely
libglade to build the GTK GUI, tinyxml2 to decode XML coming from the network player, SFML for network requests, and
libconfig to save and restore some parameters.

There are 3 main modules: Yncapi, Yncc and Yncc-Gtk.

- Yncapi    : This is the library, providing the basic I/O functions to talk to the network player,
            and process the data sent back.
         
- Yncc      : The command-line tool to easily send requests to the player, even about currently unsupported
            features with a direct request command.
         
- Yncc-Gtk  : It's the GUI version of Yncc, which support multiple audio sources, multiple virtual audio players,
            and browsing all files the player can access.
            
As a hobby project, it's pretty hard to test it with multiple devices, so I provide these programs without any
warranty they will work with your device. The only thing I can be sure is : it works like a charm with the Yamaha 
CD-N301 !
