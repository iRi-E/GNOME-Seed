#!/usr/bin/env seed
// Returns: 0
// STDIN:
// STDOUT:JSON Win!
// STDERR:

Gtk = imports.gi.Gtk;
GLib = imports.gi.GLib;

Gtk.init(Seed.argv);
window = new Gtk.Window({title: "JSON Win!"});
window.show_all();

print(window.title);

