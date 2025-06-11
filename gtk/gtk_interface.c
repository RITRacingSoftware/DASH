#include <gtk/gtk.h>
#include <stdint.h>
#include <stdio.h>
#include <sys/time.h>
#include <stdlib.h>

#include "gtk_interface.h"
#include "config.h"

typedef struct gtk_globals_s {
    GtkWidget *drawing_area;
    cairo_surface_t *surface;
} gtk_globals;

uint64_t millis_offset = 0;
static gtk_globals fd;

uint16_t screen_data[TFT_SCREEN_PIXELS];
static int surface_stride;
static unsigned char *surface_data;


// Arduino functions from the user code
void setup();
void loop();

unsigned int millis() {
    struct timespec tv;
    clock_gettime(CLOCK_MONOTONIC, &tv);
    uint64_t ms = (tv.tv_sec * 1000ULL + tv.tv_nsec / 1000000);
    return ms - millis_offset;
}

void update_screen_data() {
    uint16_t max = 0;
    for (int i=0; i < TFT_SCREEN_PIXELS; i++) {
        if (screen_data[i] > max) max = screen_data[i];
    }
    printf("Updating screen %d\n", max);
    if (max == 0) return;
    cairo_surface_flush(fd.surface);
    for (int i=0; i < TFT_SCREEN_HEIGHT; i++) memcpy(surface_data + i*surface_stride, screen_data, surface_stride);
    cairo_surface_mark_dirty(fd.surface);
    gtk_widget_queue_draw(fd.drawing_area);
}

gboolean redraw_callback(GtkWidget *widget, cairo_t *cr, gpointer data_pointer) {
    printf("redrawing\n");
    gtk_globals *fd = (gtk_globals*)(data_pointer);
    cairo_set_source_surface(cr, fd->surface, 0, 0);
    cairo_paint(cr);
    return FALSE;
}

static gboolean configure_callback(GtkWidget *widget, GdkEventConfigure *event, gpointer data_pointer) {
    int da_width = gtk_widget_get_allocated_width(widget);
    int da_height = gtk_widget_get_allocated_height(widget);
    printf("drawingarea size is (%d, %d)\n", da_width, da_height);
    gtk_globals *fd = (gtk_globals*)(data_pointer);
    if (fd->surface) cairo_surface_destroy(fd->surface);
    //fd->surface = gdk_window_create_similar_surface(gtk_widget_get_window(widget), CAIRO_CONTENT_COLOR, da_width, da_height);
    //fd->surface = cairo_image_surface_create(CAIRO_FORMAT_RGB16_565, da_width, da_height);
    surface_stride = cairo_format_stride_for_width(CAIRO_FORMAT_RGB16_565, da_width);
    surface_data = malloc(surface_stride * da_height);
    fd->surface = cairo_image_surface_create_for_data(surface_data, CAIRO_FORMAT_RGB16_565, da_width, da_height, surface_stride);
    return TRUE;
}

static gboolean timeout_callback(gpointer data_pointer) {
    loop();
    return TRUE;
}

static void activate (GtkApplication *app, gpointer user_data) {
    printf("activating\n");
    GtkWidget *window;
    GtkWidget *event_box;
    gtk_globals *fd = (gtk_globals*)user_data;

    window = gtk_application_window_new (app);
    printf("window is %p\n", window);
    event_box = gtk_event_box_new();
    gtk_container_add(GTK_CONTAINER(window), event_box);

    // gtk_widget_add_events(event_box, GDK_BUTTON_PRESS_MASK);
    // gtk_widget_add_events(event_box, GDK_SCROLL_MASK);
    // gtk_widget_add_events(event_box, GDK_POINTER_MOTION_MASK);
    // gtk_widget_add_events(event_box, GDK_BUTTON_RELEASE_MASK);

    GtkWidget *drawing_area = gtk_drawing_area_new();
    gtk_widget_set_size_request(drawing_area, TFT_SCREEN_WIDTH, TFT_SCREEN_HEIGHT);
    fd->drawing_area = drawing_area;
    g_signal_connect(G_OBJECT(drawing_area), "draw", G_CALLBACK(redraw_callback), fd);
    g_signal_connect(G_OBJECT(drawing_area), "configure-event", G_CALLBACK(configure_callback), fd);
    gtk_container_add(GTK_CONTAINER(event_box), drawing_area);

    gtk_widget_show_all(window);

    g_timeout_add(10, timeout_callback, fd);
}

int main(int argc, char **argv) {
    GtkApplication *app;
    int status;

    struct timespec tv;
    clock_gettime(CLOCK_MONOTONIC, &tv);
    uint64_t millis_offset = (tv.tv_sec * 1000ULL + tv.tv_nsec / 1000000);

    setup();

    app = gtk_application_new("org.gtk.example", G_APPLICATION_FLAGS_NONE);
    g_signal_connect(app, "activate", G_CALLBACK (activate), &fd);
    status = g_application_run(G_APPLICATION (app), argc, argv);
    g_object_unref (app);
}
