#!/usr/local/bin/seed
Seed.import_namespace("Gtk")
Seed.import_namespace("Gdk");

Gtk.init(null, null);

w = new Gtk.Window();
group = new Gtk.AccelGroup();
group.connect(Gdk.keyval_from_name("q"), 
	      0, 
	      0, 
	      function(){Gtk.main_quit()});
w.add_accel_group(group);

label = new Gtk.Label({label: "Press Q to quit"});
w.add(label);
w.show_all();

Gtk.main();