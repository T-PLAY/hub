
/*
Polhemus Tracker Terminal version 1.0.3 -- Terminal Interface to Polhemus Trackers: Fastrak, Patriot, and Liberty
Copyright  ©  2009-2011  Polhemus, Inc.

This file is part of Tracker Terminal.

Tracker Terminal is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

Tracker Terminal is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with Tracker Terminal.  If not, see <http://www.gnu.org/licenses/>.

*************************************************************************


*/

/*
version 1.0.1 -- September 2009
added support for the keypad keys

version 1.0.2 -- March 2010
added support for 64-bit environments

version 1.0.3 -- December 2011
added support for Fastrak 3
changed over to gtk+-3.0
*/

#include "main2.h"

#define CNX_FILE PACKAGE_LOCAL_STATE_DIR_PITERM "/PiTerm_params.cnx"
#define LOGO_FILE PACKAGE_DATA_ROOT_DIR_PITERM "/PiTermIcon.png"

#define BUFFER_SIZE 1000

int main(int argc, char* argv[])
{

    // first set up the user interface
    // Using GTK+ 2.0 in v1.0.2
    // Using GTK+ 3.0 in v1.0.3

    GtkWidget *scrWin, *grd1, *grd2, *grd6, *grd4, *grd3, *fr1, *fr2;
    GtkWidget *connect, *disconnect, *usb, *rs232, *capOn, *capOff, *lab1, *lab2, *cmd, *textview;
    GtkWidget *about, *close, *grd5, *clear;

    CAP_STRUCT cs;
    CNX_STRUCT cnxStruct;

    PingPong pong;
    if (pong.InitPingPong(BUFFER_SIZE) < 0) {
        fprintf(stderr, "Memory Allocation Error setting up buffers\n");
        return -1;
    }

    int keepLooping = 0;

    cs.fCap = NULL;
    cs.filename = NULL;

    gtk_init(&argc, &argv);

    int useDefault = true;
    CNX_PARAMS cp;

    // check for an existing parameter file, if it exists use that data,
    // otherwise load defaults
    if (g_file_test(CNX_FILE, G_FILE_TEST_EXISTS)) {
        FILE* f = fopen(CNX_FILE, "r+");
        int br = fread(&cp, sizeof(CNX_PARAMS), 1, f);
        if (br == 1) {
            useDefault = FALSE;
            cnxStruct.cnxType = cp.cnxType;
            cnxStruct.trackerType = cp.tracker;
        }
        fclose(f);
    }

    if (useDefault) {
        cnxStruct.cnxType = USB_CNX;
        cnxStruct.trackerType = TRKR_LIB_HS;
        g_strlcpy(cp.port, "/dev/ttyS0", 50);
    }

    // The communication with the tracker
    cnxStruct.pTrak = new PiTracker;
    if (!cnxStruct.pTrak) {
        printf("Memory Allocation Error creating tracker communications module\n");
        return -3;
    }

    // set up read_write structs to read data in worker thread,
    // use timeout func to write the data to the textview

    // create structs to be used in Read thread and the write timeout fxn
    pthread_t thread_id;
    READ_WRITE_STRUCT readStruct = { &pong, keepLooping, &thread_id, cnxStruct.pTrak };
    READ_WRITE_STRUCT writeStruct = { &pong, keepLooping, &thread_id, NULL }; // will add textview after it's created

    g_object_set(gtk_settings_get_default(), "gtk-button_images", TRUE, NULL);
    // Setup main window
    cs.win = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    gtk_window_set_title(GTK_WINDOW(cs.win), "Polhemus Tracker Terminal");
    gtk_container_set_border_width(GTK_CONTAINER(cs.win), 10);
    gtk_window_set_position(GTK_WINDOW(cs.win), GTK_WIN_POS_CENTER);
    gtk_window_set_default_icon_from_file(LOGO_FILE, NULL);
    g_signal_connect(G_OBJECT(cs.win), "destroy", G_CALLBACK(gtk_main_quit), NULL);

    // Connect and disconnect buttons
    grd1 = gtk_grid_new();
    gtk_grid_set_row_spacing(GTK_GRID(grd1), 5);
    gtk_orientable_set_orientation(GTK_ORIENTABLE(grd1), GTK_ORIENTATION_VERTICAL);
    connect = gtk_button_new_from_stock(GTK_STOCK_CONNECT);
    disconnect = gtk_button_new_from_stock(GTK_STOCK_DISCONNECT);
    gtk_widget_set_sensitive(disconnect, FALSE);
    gtk_container_add(GTK_CONTAINER(grd1), connect);
    gtk_container_add(GTK_CONTAINER(grd1), disconnect);
    g_object_set_data(G_OBJECT(connect), "disconnect", (gpointer)disconnect); // to enable disconnect button
    g_object_set_data(G_OBJECT(connect), "readstruct", (gpointer)&readStruct); // to start read thread
    g_object_set_data(G_OBJECT(connect), "writestruct", (gpointer)&writeStruct); // to start timout write fxn
    g_object_set_data(G_OBJECT(disconnect), "connect", (gpointer)connect); // to enable connect button
    g_object_set_data(G_OBJECT(disconnect), "readstruct", (gpointer)&readStruct); // to kill read thread and stop timeout fxn
    g_signal_connect(G_OBJECT(connect), "clicked", G_CALLBACK(OnConnect), (gpointer)&cnxStruct);
    g_signal_connect(G_OBJECT(disconnect), "clicked", G_CALLBACK(OnDisconnect), (gpointer)&cnxStruct);

    // USB or RS232 Connection Interface
    fr1 = gtk_frame_new("Connection Type");
    grd4 = gtk_grid_new();
    gtk_grid_set_column_spacing(GTK_GRID(grd4), 10);
    usb = gtk_radio_button_new_with_mnemonic(NULL, "_USB");

    // filechooser to indicate rs232 port
    cnxStruct.portChooser = gtk_file_chooser_button_new("Choose the RS232 Port", GTK_FILE_CHOOSER_ACTION_OPEN);
    gtk_file_chooser_set_filename(GTK_FILE_CHOOSER(cnxStruct.portChooser), cp.port);
    gtk_widget_set_sensitive(cnxStruct.portChooser, FALSE);
    rs232 = gtk_radio_button_new_with_mnemonic_from_widget(GTK_RADIO_BUTTON(usb), "_RS232");
    gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(cnxStruct.cnxType == USB_CNX ? usb : rs232), TRUE);
    gtk_grid_attach(GTK_GRID(grd4), usb, 0, 0, 1, 1);
    gtk_grid_attach(GTK_GRID(grd4), rs232, 1, 0, 1, 1);
    gtk_grid_attach(GTK_GRID(grd4), cnxStruct.portChooser, 0, 1, 2, 1);
    gtk_container_add(GTK_CONTAINER(fr1), grd4);
    g_signal_connect(G_OBJECT(usb), "toggled", G_CALLBACK(OnCnxType), (gpointer)&cnxStruct);

    // Data Capture Interface
    fr2 = gtk_frame_new("Capture Data");
    grd2 = gtk_grid_new();
    gtk_grid_set_column_spacing(GTK_GRID(grd2), 50);
    capOn = gtk_radio_button_new_with_mnemonic(NULL, "O_n");
    capOff = gtk_radio_button_new_with_mnemonic_from_widget(GTK_RADIO_BUTTON(capOn), "O_ff");
    gtk_grid_attach(GTK_GRID(grd2), capOn, 0, 0, 1, 1);
    gtk_grid_attach(GTK_GRID(grd2), capOff, 1, 0, 1, 1);
    gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(capOff), TRUE);
    cs.label = gtk_label_new("Capture File: None");
    cs.browse = gtk_button_new_with_label("Browse...");
    gtk_grid_attach(GTK_GRID(grd2), cs.label, 0, 1, 2, 1);
    gtk_grid_attach(GTK_GRID(grd2), cs.browse, 2, 1, 1, 1);
    gtk_container_add(GTK_CONTAINER(fr2), grd2);
    g_signal_connect(G_OBJECT(capOn), "toggled", G_CALLBACK(OnCapture), &cs);
    g_object_set_data(G_OBJECT(cs.win), "capOff", (gpointer)capOff);
    g_signal_connect(G_OBJECT(cs.browse), "clicked", G_CALLBACK(OnCaptureBrowse), (gpointer)&cs);

    // pack it together and put on top of the window
    grd3 = gtk_grid_new();
    gtk_grid_set_column_spacing(GTK_GRID(grd3), 50);
    gtk_container_add(GTK_CONTAINER(grd3), grd1);
    gtk_container_add(GTK_CONTAINER(grd3), fr1);
    gtk_container_add(GTK_CONTAINER(grd3), fr2);

    // This widget is for the tracker response
    scrWin = gtk_scrolled_window_new(NULL, NULL);
    gtk_scrolled_window_set_policy(GTK_SCROLLED_WINDOW(scrWin), GTK_POLICY_AUTOMATIC, GTK_POLICY_AUTOMATIC);
    gtk_widget_set_size_request(scrWin, 700, 500);
    textview = gtk_text_view_new();
    writeStruct.pParam = textview;
    gtk_text_view_set_editable(GTK_TEXT_VIEW(textview), FALSE); // read only
    gtk_container_add(GTK_CONTAINER(scrWin), textview);
    g_object_set_data(G_OBJECT(textview), "capFile", (gpointer)&cs.fCap);

    // Add some labels
    lab1 = gtk_label_new("Tracker Output:");
    lab2 = gtk_label_new("Enter Tracker Command:");
    gtk_misc_set_alignment(GTK_MISC(lab1), 0, 0.5); // set to left
    gtk_misc_set_alignment(GTK_MISC(lab2), 0, 0.5);

    // a text entry to receive the commands to the tracker
    cmd = gtk_entry_new();
    g_object_set_data(G_OBJECT(connect), "entry", (gpointer)cmd); // so that we can set the cursor here on connect

    // bottom buttons
    about = gtk_button_new_from_stock(GTK_STOCK_ABOUT);
    close = gtk_button_new_from_stock(GTK_STOCK_CLOSE);
    clear = gtk_button_new_from_stock(GTK_STOCK_CLEAR);

    // connection status label
    cnxStruct.cnxLabel = GTK_LABEL(gtk_label_new("No Connection"));

    // add buttons and label to bottom of window
    grd5 = gtk_grid_new();
    GtkWidget* spacer; // need this to push 3 buttons over to right side
    spacer = gtk_alignment_new(0.0, 0.0, 1.0, 1.0);
    gtk_widget_set_size_request(spacer, 350, 1);

    gtk_grid_attach_next_to(GTK_GRID(grd5), close, NULL, GTK_POS_LEFT, 1, 1);
    gtk_grid_attach_next_to(GTK_GRID(grd5), clear, NULL, GTK_POS_LEFT, 1, 1);
    gtk_grid_attach_next_to(GTK_GRID(grd5), about, NULL, GTK_POS_LEFT, 1, 1);
    gtk_grid_attach_next_to(GTK_GRID(grd5), spacer, NULL, GTK_POS_LEFT, 1, 1);
    gtk_grid_attach_next_to(GTK_GRID(grd5), GTK_WIDGET(cnxStruct.cnxLabel), NULL, GTK_POS_LEFT, 1, 1);
    // Setup some callbacks for signals and events
    gtk_widget_set_events(cmd, GDK_KEY_PRESS);
    g_signal_connect(G_OBJECT(close), "clicked", G_CALLBACK(gtk_main_quit), NULL);
    g_signal_connect(G_OBJECT(about), "clicked", G_CALLBACK(OnAbout), (gpointer)cs.win);
    g_signal_connect(G_OBJECT(cmd), "key-press-event", G_CALLBACK(CommandEntered), &cnxStruct);
    g_signal_connect(G_OBJECT(clear), "clicked", G_CALLBACK(OnClear), (gpointer)textview);

    // Put it all together on the window
    // vb3=gtk_vbox_new(FALSE,5);
    // gtk_box_pack_start(GTK_BOX(vb3),grd3,FALSE,TRUE,0);
    // gtk_box_pack_start(GTK_BOX(vb3),lab1,FALSE,TRUE,0);
    // gtk_box_pack_start(GTK_BOX(vb3),scrWin,TRUE,TRUE,0);
    // gtk_box_pack_start(GTK_BOX(vb3),lab2,FALSE,TRUE,0);
    // gtk_box_pack_start(GTK_BOX(vb3),cmd,FALSE,TRUE,0);
    // gtk_box_pack_start(GTK_BOX(vb3),grd5,FALSE,TRUE,0);
    grd6 = gtk_grid_new();
    gtk_orientable_set_orientation(GTK_ORIENTABLE(grd6), GTK_ORIENTATION_VERTICAL);
    gtk_container_add(GTK_CONTAINER(grd6), grd3);
    gtk_container_add(GTK_CONTAINER(grd6), lab1);
    gtk_container_add(GTK_CONTAINER(grd6), scrWin);
    gtk_container_add(GTK_CONTAINER(grd6), lab2);
    gtk_container_add(GTK_CONTAINER(grd6), cmd);
    gtk_container_add(GTK_CONTAINER(grd6), grd5);
    gtk_container_add(GTK_CONTAINER(cs.win), grd6);
    gtk_widget_show_all(cs.win);

    gtk_main();

    // closing down, time to clean up
    // close capture file if open
    if (cs.fCap)
        fclose(cs.fCap);

    // clean up thread if still connected
    if (keepLooping) {
        keepLooping = 0;
        pthread_join(thread_id, NULL); // wait on the thread to finish
        cnxStruct.pTrak->CloseTrk();
    }

    // save connection parameters for next time
    FILE* f = fopen(CNX_FILE, "w");
    if (f) {
        cp.cnxType = cnxStruct.cnxType;
        cp.tracker = cnxStruct.trackerType;
        fwrite(&cp, sizeof(CNX_PARAMS), 1, f);
        fclose(f);
    }

    delete cnxStruct.pTrak;
    return 0;
}

void OnAbout(GtkWidget* w, GtkWindow* win)
{

    const gchar* author[] = { "James C. Farr", NULL };
    const gchar* copyright = "Polhemus Tracker Terminal Copyright © Polhemus 2009-2012";

    gtk_show_about_dialog(win, "authors", author, "comments", "Terminal Interface for Polhemus Trackers",
        "copyright", copyright, "version", PACKAGE_VERSION, "program-name",
        "Polhemus Tracker Terminal", "website", "http://www.polhemus.com",
        "license", licText, "wrap-license", TRUE, NULL);
}

// The user wants to clear the tracker output window
void OnClear(GtkWidget* w, GtkTextView* tv)
{

    GtkTextBuffer* buffer;

    buffer = gtk_text_view_get_buffer(tv);
    gtk_text_buffer_set_text(buffer, "", -1);
}

void OnCapture(GtkToggleButton* button, LPCAP_STRUCT pcs)
{

    gboolean capOn = gtk_toggle_button_get_active(button);
    GtkWidget* capOff = GTK_WIDGET(g_object_get_data(G_OBJECT(pcs->win), "capOff"));

    if (capOn) {
        if (!pcs->filename) { // need a new file
            if (Browse4CaptureFile(pcs) < 0) {
                gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(capOff), TRUE);
                return;
            }
        }

        else { // already a file indicated just open it
            if (OpenCaptureFile(pcs) != 0) {
                if (Browse4CaptureFile(pcs) < 0) {
                    gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(capOff), TRUE);
                    return;
                }
            }
        }
    }

    // if stopping capture close the file
    else if (pcs->fCap) {
        fclose(pcs->fCap);
        pcs->fCap = NULL;
    }
}
void OnCaptureBrowse(GtkWidget* w, LPCAP_STRUCT pcs)
{

    Browse4CaptureFile(pcs);
}

// set up a dialog to designate the capture file.
int Browse4CaptureFile(LPCAP_STRUCT pcs)
{

    GtkWidget* dlg;
    gchar* txt;
    gint resp;

    dlg = gtk_file_chooser_dialog_new("Specify File to write Captured Data", NULL, GTK_FILE_CHOOSER_ACTION_SAVE,
        GTK_STOCK_SAVE, GTK_RESPONSE_ACCEPT, GTK_STOCK_CANCEL, GTK_RESPONSE_CANCEL, NULL);

    resp = gtk_dialog_run(GTK_DIALOG(dlg));
    if (resp == GTK_RESPONSE_CANCEL) {
        gtk_widget_destroy(dlg);
        return -1;
    }

    pcs->filename = gtk_file_chooser_get_filename(GTK_FILE_CHOOSER(dlg));
    gchar* slash = g_strrstr(pcs->filename, "/");
    slash++;
    txt = g_strconcat("Capture File: ", slash, NULL);
    gtk_label_set_text(GTK_LABEL(pcs->label), txt);
    g_free(txt);
    gtk_widget_destroy(dlg);

    // open the file
    OpenCaptureFile(pcs);
    return 0;
}

// open the file and set the indicators correctly
int OpenCaptureFile(LPCAP_STRUCT pcs)
{

    GtkToggleButton* capOff;
    GtkWidget* dlg;
    int rv = 0;

    pcs->fCap = fopen(pcs->filename, "a");
    if (pcs->fCap == NULL) {
        dlg = gtk_message_dialog_new(GTK_WINDOW(pcs->win), GTK_DIALOG_MODAL, GTK_MESSAGE_WARNING, GTK_BUTTONS_OK,
            "Unable to open file %s for writing.  Permissions?", pcs->filename);
        gtk_dialog_run(GTK_DIALOG(dlg));
        gtk_widget_destroy(dlg);
        g_free(pcs->filename);
        pcs->filename = NULL;
        gtk_label_set_text(GTK_LABEL(pcs->label), "Capture File:  None");
        capOff = GTK_TOGGLE_BUTTON(g_object_get_data(G_OBJECT(pcs->win), "capOff"));
        gtk_toggle_button_set_active(capOff, TRUE);
        rv = -1;
    }

    return rv;
}

// set the type of connection and set the buttons properly
void OnCnxType(GtkToggleButton* button, LPCNX_STRUCT pcs)
{

    if (gtk_toggle_button_get_active(button)) {
        pcs->cnxType = USB_CNX;
        gtk_widget_set_sensitive(pcs->portChooser, FALSE);
    } else {
        pcs->cnxType = RS232_CNX;
        gtk_widget_set_sensitive(pcs->portChooser, TRUE);
    }
}

// connect to the tracker and start the read thread and write timeout
void OnConnect(GtkWidget* w, LPCNX_STRUCT pcs)
{

    int cnxSuccess;
    gchar* str;
    gchar* port;
    GtkWidget* msg;

    if (pcs->cnxType == USB_CNX) {
        do {
            cnxSuccess = pcs->pTrak->UsbConnect(usbTrkParams[pcs->trackerType].vid, usbTrkParams[pcs->trackerType].pid,
                usbTrkParams[pcs->trackerType].writeEp, usbTrkParams[pcs->trackerType].readEp);

            if (cnxSuccess != 0) { // failure
                if (QueryUser4TrackerType(pcs->trackerType) == -1) // user cancelled out
                    return;
            }

        } while (cnxSuccess != 0);

        // set label
        str = g_strconcat("Connected to ", trackerNames[pcs->trackerType], " over USB", NULL);
        gtk_label_set_text(pcs->cnxLabel, str);
        g_free(str);
    }

    else { // rs232
        port = gtk_file_chooser_get_filename(GTK_FILE_CHOOSER(pcs->portChooser));
        cnxSuccess = pcs->pTrak->Rs232Connect(port);
        if (cnxSuccess != 0) {
            msg = gtk_message_dialog_new(NULL, GTK_DIALOG_MODAL, GTK_MESSAGE_WARNING, GTK_BUTTONS_OK,
                "Unable to connect to RS232 port %s.", port);
            gtk_dialog_run(GTK_DIALOG(msg));
            gtk_widget_destroy(msg);
            g_free(port);
            return;
        }

        // set label
        str = g_strconcat("Connected over RS232 at port ", port, NULL);
        gtk_label_set_text(pcs->cnxLabel, str);
        g_free(port);
        g_free(str);
    }

    // disable connect, enable disconnect
    GtkWidget* disconnect = GTK_WIDGET(g_object_get_data(G_OBJECT(w), "disconnect"));
    gtk_widget_set_sensitive(w, FALSE);
    gtk_widget_set_sensitive(disconnect, TRUE);

    // start read thread

    LPREAD_WRITE_STRUCT prs = (LPREAD_WRITE_STRUCT)g_object_get_data(G_OBJECT(w), "readstruct");
    prs->keepLooping = 1;
    pthread_create(prs->pthread, NULL, ReadTrackerThread, prs);

    // start the write thread
    gpointer p = g_object_get_data(G_OBJECT(w), "writestruct");
    g_timeout_add(10, (GSourceFunc)Write2Display, p);

    // give the cmd entry the focus
    GtkWidget* entry = GTK_WIDGET(g_object_get_data(G_OBJECT(w), "entry"));
    gtk_widget_grab_focus(entry);
}

void OnDisconnect(GtkWidget* w, LPCNX_STRUCT pcs)
{

    gtk_label_set_text(pcs->cnxLabel, "No Connection");
    GtkWidget* connect = GTK_WIDGET(g_object_get_data(G_OBJECT(w), "connect"));
    gtk_widget_set_sensitive(w, FALSE);
    gtk_widget_set_sensitive(connect, TRUE);

    LPREAD_WRITE_STRUCT prs = (LPREAD_WRITE_STRUCT)g_object_get_data(G_OBJECT(w), "readstruct");
    prs->keepLooping = 0;
    pthread_join(*prs->pthread, NULL); // wait for the read thread to complete
    pcs->pTrak->CloseTrk(); // close down the tracker connection
}

// This function gets called when a usb connection fails
// It presents a dialog to the user to indicate the kind of tracker
// which they are trying to connect.
int QueryUser4TrackerType(int& trk)
{

    GtkWidget *dlg, *rb0, *rb, *label, *content;
    int rv = -1;
    int trk_type = TRKR_LIB_HS;
    GdkColor red;

    dlg = gtk_dialog_new_with_buttons("Select Tracker Type", NULL, GTK_DIALOG_MODAL, GTK_STOCK_OK, GTK_RESPONSE_ACCEPT,
        GTK_STOCK_CANCEL, GTK_RESPONSE_REJECT, NULL);

    gtk_container_set_border_width(GTK_CONTAINER(dlg), 30);
    content = gtk_dialog_get_content_area(GTK_DIALOG(dlg));

    label = gtk_label_new("Unable to connect to tracker!");
    gdk_color_parse("#ff0000", &red);
    gtk_widget_modify_fg(label, GTK_STATE_NORMAL, &red);

    // add a radio button for each tracker
    //  gtk_box_pack_start(GTK_BOX(GTK_DIALOG(dlg)->vbox),label,TRUE,TRUE,5);
    gtk_box_pack_start(GTK_BOX(content), label, TRUE, TRUE, 5);
    rb0 = gtk_radio_button_new_with_label(NULL, trackerNames[0]);
    //  gtk_box_pack_start(GTK_BOX(GTK_DIALOG(dlg)->vbox),rb0,TRUE,TRUE,5);
    gtk_box_pack_start(GTK_BOX(content), rb0, TRUE, TRUE, 5);
    g_object_set_data(G_OBJECT(rb0), "index", (gpointer)0);
    g_signal_connect(G_OBJECT(rb0), "toggled", G_CALLBACK(TrackerSelect), (gpointer)&trk_type);

    for (long i = 1; i < NUM_SUPP_TRKS; i++) {
        rb = gtk_radio_button_new_with_label_from_widget(GTK_RADIO_BUTTON(rb0), trackerNames[i]);
        gtk_box_pack_start(GTK_BOX(content), rb, TRUE, TRUE, 5);
        g_object_set_data(G_OBJECT(rb), "index", (gpointer)i);
        g_signal_connect(G_OBJECT(rb), "toggled", G_CALLBACK(TrackerSelect), (gpointer)&trk_type);
    }
    gtk_widget_show_all(dlg);

    int resp = gtk_dialog_run(GTK_DIALOG(dlg));

    if (resp == GTK_RESPONSE_ACCEPT) {
        trk = trk_type;
        rv = 0;
    }

    gtk_widget_destroy(dlg);
    return rv;
}

void TrackerSelect(GtkToggleButton* button, int* pIndex)
{

    if (gtk_toggle_button_get_active(button))
        *pIndex = (int)(long)g_object_get_data(G_OBJECT(button), "index");
}

// collect the key value typed and send it to the tracker
gboolean CommandEntered(GtkEntry* w, GdkEventKey* key, LPCNX_STRUCT pcs)
{

    int keyval = key->keyval;
    GtkWidget* dlg;
    gchar* txt;
    gboolean rv = FALSE;

    static int clearEntry = 0;

    if (pcs->pTrak->GetCnxType() == NO_CNX) {
        dlg = gtk_message_dialog_new(NULL, GTK_DIALOG_MODAL, GTK_MESSAGE_ERROR, GTK_BUTTONS_OK,
            "No Connection with a Polhemus Tracker detected.");
        gtk_dialog_run(GTK_DIALOG(dlg));
        gtk_widget_destroy(dlg);
        return FALSE;
    }

    if (clearEntry) {
        gtk_entry_set_text(w, "");
        clearEntry = 0;
    }

    // if keypad is used, convert to corresponding key
    if ((keyval >= GDK_KEY_KP_Space) && (keyval < GDK_KEY_KP_Equal)) {
        if (keyval == GDK_KEY_KP_Multiply)
            keyval = GDK_KEY_asterisk;
        else
            keyval -= 0x80;
    }

    // these are non-alpha num keys that are valid to the tracker
    int validKey = (keyval == GDK_KEY_asterisk) || (keyval == GDK_KEY_comma) || (keyval == GDK_KEY_period) || (keyval == GDK_KEY_at);

    if ((key->type == GDK_KEY_PRESS) && ((g_ascii_isalnum(keyval)) || (keyval == GDK_KEY_Return) || validKey)) {
        if (key->state & GDK_CONTROL_MASK) { // a ctrl key
            keyval = g_ascii_tolower(keyval);
            txt = g_strdup_printf("^%c", keyval & 0xff);
            gtk_entry_set_text(w, txt);
            keyval -= 0x60;
            gtk_editable_set_position(GTK_EDITABLE(w), -1); // set cursor to end
            g_free(txt);
            rv = TRUE; // ^V causes the clipboard to be pasted to the entry. return TRUE to prevent it.
        }

        pcs->pTrak->WriteTrkData(&keyval, 1); // send the command to the tracker

        // if 'p' or return clear the command entry
        if ((keyval == 'p') || (keyval == 'P') || (keyval == GDK_KEY_Return))
            clearEntry = 1;
    }

    return rv;
}

// This is the thread that reads data from the tracker and stores in the
// ping pong buffer
void* ReadTrackerThread(void* pParam)
{

    BYTE buf[BUFFER_SIZE];
    LPREAD_WRITE_STRUCT prs = (LPREAD_WRITE_STRUCT)pParam;
    PiTracker* pTrak = (PiTracker*)prs->pParam;
    int len = 0;
    int bw;

    // first establish comm and clear out any residual trash data
    do {
        pTrak->WriteTrkData((void*)"\r", 1); // send just a cr, should return an short "Invalid Command" response
        usleep(100000);
        len = pTrak->ReadTrkData(buf, BUFFER_SIZE); // keep trying till we get a response
    } while (!len);

    while (prs->keepLooping) {

        len = pTrak->ReadTrkData(buf, BUFFER_SIZE); // read tracker data
        if (len > 0 && len < BUFFER_SIZE) {
            buf[len] = 0; // null terminate
            do {
                bw = prs->pPong->WritePP(buf, len); // write to buffer
                usleep(1000);
            } while (!bw);
        }
        usleep(2000); // rest for 2ms
    }

    return NULL;
}

// This is a timeout fxn that will read the ping pong buffer and output to the display
// set to fire every 10 ms
gboolean Write2Display(gpointer p)
{

    GtkTextBuffer* buffer;
    GtkTextIter iter;
    GtkTextMark* mark;

    LPREAD_WRITE_STRUCT lws = (LPREAD_WRITE_STRUCT)p;
    BYTE buf[BUFFER_SIZE];
    GtkTextView* textview = GTK_TEXT_VIEW(lws->pParam);
    FILE** f = (FILE**)g_object_get_data(G_OBJECT(textview), "capFile");

    int len = lws->pPong->ReadPP(buf);
    while (len) {
        buf[len] = 0;

        // write to display
        buffer = gtk_text_view_get_buffer(textview);
        gtk_text_buffer_get_end_iter(buffer, &iter);
        gtk_text_buffer_insert(buffer, &iter, (const char*)buf, -1);
        if (*f)
            fwrite(buf, 1, len, *f);

        // now scroll it down
        mark = gtk_text_buffer_create_mark(buffer, "mark", &iter, TRUE);
        gtk_text_view_scroll_mark_onscreen(textview, mark);
        gtk_text_buffer_delete_mark(buffer, mark); // clean up
        len = lws->pPong->ReadPP(buf);
    }

    return lws->keepLooping;
}
