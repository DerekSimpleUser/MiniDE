#include <gtk/gtk.h>
#include <stdlib.h>

// Funciones de control del sistema
static void on_shutdown(GtkButton *btn, gpointer user_data) {
    system("systemctl poweroff");
}

static void on_reboot(GtkButton *btn, gpointer user_data) {
    system("systemctl reboot");
}

static void on_logout(GtkButton *btn, gpointer user_data) {
    system("loginctl terminate-user $USER");
}

// Lanzador de aplicaciones con GtkDropDown
static void on_launch_app(GtkDropDown *dropdown, gpointer user_data) {
    int index = gtk_drop_down_get_selected(dropdown);
    if (index < 0) return;

    const char *apps[] = {"xterm", "firefox", "gedit", "nautilus"};
    if (index < 4) {
        gchar *cmd = g_strdup_printf("%s &", apps[index]);
        system(cmd);
        g_free(cmd);
    }
}

// Función principal de la interfaz
static void activate(GtkApplication *app, gpointer user_data) {
    GtkWidget *window = gtk_application_window_new(app);
    gtk_window_set_title(GTK_WINDOW(window), "MiniDE Extended");
    gtk_window_set_default_size(GTK_WINDOW(window), 1024, 600);

    GtkWidget *main_box = gtk_box_new(GTK_ORIENTATION_VERTICAL, 10);

    // Barra superior moderna
    GtkWidget *header = gtk_header_bar_new();
    gtk_header_bar_set_title_widget(GTK_HEADER_BAR(header),
        gtk_label_new("MiniDE Extended"));
    gtk_window_set_titlebar(GTK_WINDOW(window), header);

    // Lista de apps para el lanzador
    GtkStringList *app_list = gtk_string_list_new((const char*[]) {
        "xterm", "firefox", "gedit", "nautilus", NULL
    });

    GtkWidget *dropdown = gtk_drop_down_new(G_LIST_MODEL(app_list), NULL);
    gtk_drop_down_set_selected(GTK_DROP_DOWN(dropdown), 0);
    g_signal_connect(dropdown, "notify::selected", G_CALLBACK(on_launch_app), NULL);

    gtk_box_append(GTK_BOX(main_box), dropdown);

    // Botones de control
    GtkWidget *btn_shutdown = gtk_button_new_with_label("Apagar");
    GtkWidget *btn_reboot   = gtk_button_new_with_label("Reiniciar");
    GtkWidget *btn_logout   = gtk_button_new_with_label("Cerrar Sesión");

    g_signal_connect(btn_shutdown, "clicked", G_CALLBACK(on_shutdown), NULL);
    g_signal_connect(btn_reboot, "clicked", G_CALLBACK(on_reboot), NULL);
    g_signal_connect(btn_logout, "clicked", G_CALLBACK(on_logout), NULL);

    gtk_box_append(GTK_BOX(main_box), btn_shutdown);
    gtk_box_append(GTK_BOX(main_box), btn_reboot);
    gtk_box_append(GTK_BOX(main_box), btn_logout);

    gtk_window_set_child(GTK_WINDOW(window), main_box);
    gtk_window_present(GTK_WINDOW(window));
}

int main(int argc, char *argv[]) {
    GtkApplication *app = gtk_application_new("org.minide.extended", G_APPLICATION_DEFAULT_FLAGS);
    g_signal_connect(app, "activate", G_CALLBACK(activate), NULL);

    int status = g_application_run(G_APPLICATION(app), argc, argv);
    g_object_unref(app);
    return status;
}
