#!/usr/bin/env seed

Gtk = imports.gi.Gtk;
WebKit = imports.gi.WebKit;

Gtk.init(null, null);

TabbedBrowser = imports.TabbedBrowser;
BrowserSettings = imports.BrowserSettings;
//Seed.include("BrowserStatusbar.js");
//Seed.include("BrowserTab.js");
//Seed.include("TabbedBrowser.js");
//Seed.include("BrowserToolbar.js");

// Test for Gtk >= 2.16 (otherwise don't have a progress bar)

BrowserSettings.have_progress_bar = false;

if(Gtk.Entry.prototype.set_progress_fraction)
{
	BrowserSettings.have_progress_bar = true;
}

window = new Gtk.Window({title: "Browser"});
window.resize(800, 600);
window.signal.hide.connect(function () { Gtk.main_quit(); });

TabbedBrowser.browser = new TabbedBrowser.TabbedBrowser();
window.add(TabbedBrowser.browser);
window.show_all();

Gtk.main();
