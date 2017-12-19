#include <gtk/gtk.h>
#include <string.h>
#include <stdlib.h>

const int BUFFER_SIZE_FGETS = 1024;
const int IMP_OBJ_BUFFER_SIZE = 2048;


typedef struct {
    GtkWidget *ent_date;
    GtkWidget *ent_lib;
    GtkWidget *ent_mont;
    GtkWidget *treeview;

} app_widgets;

enum
{
    DATE_COLUMN,
    LIBELLE_COLUMN,
    MONTANT_COLUMN,
    N_COLUMN
};


void on_window_main_destroy()
{
    gtk_main_quit();
}

void on_button_test_load_clicked(GtkButton *button, gpointer *user_data) {

    gchar* nomFichier = "data.txt";
    FILE *fichier = NULL;
    gchar* buffer = NULL;
    char *tok;
    char col[3][1000];

    buffer = malloc (IMP_OBJ_BUFFER_SIZE);
    if (buffer == NULL)
    {
        printf("Error: Out of Memory");
        return 1;
    }


    GtkTreeIter iter;
    GtkListStore *store = GTK_LIST_STORE(user_data);


    fichier = fopen(nomFichier, "r");
    if (fichier != NULL) {
        while (fgets(buffer, BUFFER_SIZE_FGETS, fichier) != NULL) {
            tok = strtok(buffer,",");
            strcpy(col[0],tok);
            tok = strtok(NULL,",");
            strcpy(col[1],tok);
            tok = strtok(NULL,",");
            tok[strcspn(tok, "\n")] = 0; //supprimer le \n
            strcpy(col[2],tok);

            gtk_list_store_append(store, &iter);  // Acquire an iterator
            gtk_list_store_set(store, &iter,
                   DATE_COLUMN, col[0],
                   LIBELLE_COLUMN, col[1],
                   MONTANT_COLUMN, col[2],
                   -1);
        }

    }
}


void on_treeview_selection1_changed (GtkTreeSelection *treeselection, app_widgets *app_wid) {

    gchar value[3];
    GtkTreeIter iter;
    GtkTreePath *path;
    //GtkTreeView *treeview = (GtkTreeView *)app_wid->treeview;
    GtkListStore *store = GTK_LIST_STORE(gtk_tree_view_get_model(app_wid->treeview));

    gtk_tree_view_get_cursor (app_wid->treeview, &path, NULL);
    gtk_tree_model_get_iter (GTK_TREE_MODEL(store), &iter, path);
    gtk_tree_path_free (path);

    if (gtk_tree_selection_get_selected(treeselection, &store, &iter))
    {
        gtk_tree_model_get(GTK_TREE_MODEL(store), &iter, DATE_COLUMN, &value[0], LIBELLE_COLUMN, &value[1],MONTANT_COLUMN, &value[2], -1);

    }

    gtk_entry_set_text(GTK_ENTRY(app_wid->ent_date), value[0]);
    gtk_entry_set_text(GTK_ENTRY(app_wid->ent_lib), value[1]);
    gtk_entry_set_text(GTK_ENTRY(app_wid->ent_mont), value[2]);

    g_free(value);

}


int main(int argc, char *argv[])
{
    GtkBuilder *builder;
    //GtkWidget *window;
    app_widgets *widgets = g_slice_new(app_widgets);


    gtk_init(&argc, &argv);

    builder = gtk_builder_new();
    gtk_builder_add_from_file (builder, "test8.glade", NULL);

    widgets->ent_date  = GTK_WIDGET(gtk_builder_get_object(builder, "entry_date"));
    widgets->ent_lib = GTK_WIDGET(gtk_builder_get_object(builder, "entry_lib"));
    widgets->ent_mont = GTK_WIDGET(gtk_builder_get_object(builder, "entry_mont"));
    widgets->treeview = GTK_TREE_VIEW(gtk_builder_get_object(builder, "treeview1"));

    gtk_builder_connect_signals(builder, NULL);

    gtk_widget_show(GTK_WIDGET(gtk_builder_get_object(builder, "window1")));

    g_object_unref(builder);

    gtk_main();

    g_slice_free(app_widgets, widgets);
    return 0;
}



