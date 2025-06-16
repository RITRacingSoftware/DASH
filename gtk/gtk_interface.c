#include <gtk/gtk.h>
#include <stdint.h>
#include <stdio.h>
#include <sys/time.h>
#include <stdlib.h>
#include <signal.h>
#include <libavcodec/avcodec.h>
#include <libavutil/opt.h>
#include <libavutil/imgutils.h>

#include <libyuv.h>

#include "gtk_interface.h"
#include "config.h"

#define FAIL(...) do {fprintf(stderr, __VA_ARGS__); done(1);} while (0)

#define TESTING_VIDEO_ENABLE
#define TESTING_VIDEO_BITRATE 10000000

typedef struct gtk_globals_s {
    GtkWidget *drawing_area;
    cairo_surface_t *surface;
    int surface_stride;
    unsigned char *surface_data;
#ifdef TESTING_VIDEO_ENABLE
    AVCodecContext *avctx;
    AVPacket *packet;
    AVFrame *frame;
    FILE *vid_fptr;
    const AVCodec *codec;
#endif
} gtk_globals;

static gtk_globals fd;

uint16_t screen_data[TFT_SCREEN_PIXELS];


// Arduino functions from the user code
void setup();
void loop();

void encode_frame(AVCodecContext *avctx, AVFrame *frame, AVPacket *packet, FILE *vid_fptr);

uint64_t time_usecs() {
    struct timespec t;
    clock_gettime(CLOCK_MONOTONIC, &t);
    return t.tv_sec*1000000ULL + t.tv_nsec/1000;
}

void update_screen_data() {
    gtk_widget_queue_draw(fd.drawing_area);
}

void done(int sig) {
    printf("Closing\n");
#ifdef TESTING_VIDEO_ENABLE
    if (fd.avctx && fd.packet && fd.vid_fptr) {
        encode_frame(fd.avctx, NULL, fd.packet, fd.vid_fptr);
        if (fd.codec->id == AV_CODEC_ID_MPEG1VIDEO || fd.codec->id == AV_CODEC_ID_MPEG2VIDEO) {
            fwrite((const uint8_t[]){ 0, 0, 1, 0xb7 }, 1, 4, fd.vid_fptr);
        }
    }

    if (fd.vid_fptr) fclose(fd.vid_fptr);
    if (fd.avctx) avcodec_free_context(&(fd.avctx));
    if (fd.frame) av_frame_free(&(fd.frame));
    if (fd.packet) av_packet_free(&(fd.packet));
#endif
    exit(sig);
}

int frame_divider = 0;
int nframes = 0;

#ifdef TESTING_VIDEO_ENABLE
void encode_frame(AVCodecContext *avctx, AVFrame *frame, AVPacket *packet, FILE *vid_fptr) {
    int ret;
    ret = avcodec_send_frame(avctx, frame);
    if (ret < 0) FAIL("Error sending packet: %s\n", av_err2str(ret));

    while (1) {
        ret = avcodec_receive_packet(avctx, packet);
        if ((ret == AVERROR(EAGAIN)) || (ret == AVERROR_EOF)) break;
        if (ret < 0) FAIL("Error receiving packet: %s\n", av_err2str(ret));
        fwrite(packet->data, 1, packet->size, vid_fptr);
        av_packet_unref(packet);
    }
}
#endif

gboolean redraw_callback(GtkWidget *widget, cairo_t *cr, gpointer data_pointer) {
    gtk_globals *fd = (gtk_globals*)(data_pointer);
    uint16_t max = 0;
    int ret;
    AVFrame *frame = fd->frame;
    AVPacket *packet = fd->packet;

#ifdef TESTING_VIDEO_ENABLE
    if (frame_divider == 0) {
        // Save a frame to the video
        ret = av_frame_make_writable(frame);
        if (ret < 0) FAIL("Unable to make frame writable: %s\n", av_err2str(ret));
        RGB565ToI420(screen_data, 2*TFT_SCREEN_WIDTH, frame->data[0], frame->linesize[0], frame->data[1], frame->linesize[1], frame->data[2], frame->linesize[2], TFT_SCREEN_WIDTH, TFT_SCREEN_HEIGHT);
        frame->pts = nframes++;
        encode_frame(fd->avctx, frame, packet, fd->vid_fptr);
    }
    frame_divider = (frame_divider + 1) % 2;
#endif

    cairo_surface_flush(fd->surface);
    for (int i=0; i < TFT_SCREEN_HEIGHT; i++) memcpy(fd->surface_data + i*fd->surface_stride, screen_data + i*TFT_SCREEN_WIDTH, 2*TFT_SCREEN_WIDTH);
    cairo_surface_mark_dirty(fd->surface);
    cairo_set_source_surface(cr, fd->surface, 0, 0);
    cairo_pattern_set_filter(cairo_get_source(cr), CAIRO_FILTER_NEAREST);
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
    fd->surface_stride = cairo_format_stride_for_width(CAIRO_FORMAT_RGB16_565, da_width);
    fd->surface_data = malloc(fd->surface_stride * da_height);
    fd->surface = cairo_image_surface_create_for_data(fd->surface_data, CAIRO_FORMAT_RGB16_565, da_width, da_height, fd->surface_stride);
    return TRUE;
}

static gboolean timeout_callback(gpointer data_pointer) {
    //printf("loop\n");
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
    int ret;

    signal(SIGINT, done);

#ifdef TESTING_VIDEO_ENABLE
    AVCodecContext *avctx;
    AVPacket *packet;
    AVFrame *frame;
    // Initialize video output
    const AVCodec *codec = avcodec_find_encoder_by_name("libx264");
    if (!codec) FAIL("Codec not found\n");
    avctx = avcodec_alloc_context3(codec);
    if (!(avctx)) FAIL("Unable to allocate context for encoder\n");
    packet = av_packet_alloc();
    if (!(packet)) FAIL("Unable to allocate packet buffer\n");
    frame = av_frame_alloc();
    if (!(frame)) FAIL("Unable to allocate frame buffer\n");
    // Configure codec
    avctx->bit_rate = TESTING_VIDEO_BITRATE;
    avctx->width = TFT_SCREEN_WIDTH;
    avctx->height = TFT_SCREEN_HEIGHT;
    avctx->time_base = (AVRational){1, 30};
    avctx->framerate = (AVRational){30, 1};
    avctx->gop_size = 10;
    avctx->max_b_frames = 0;
    avctx->pix_fmt = AV_PIX_FMT_YUVJ420P;

    // Configure frame
    frame->format = avctx->pix_fmt;
    frame->width  = avctx->width;
    frame->height = avctx->height;

    if (codec->id == AV_CODEC_ID_H264) {
        av_opt_set(avctx->priv_data, "preset", "slow", 0); 
    }

    ret = avcodec_open2(avctx, codec, NULL);
    if (ret < 0) FAIL("Could not open codec: %s\n", av_err2str(ret));
    
    // Allocate video buffer
    ret = av_frame_get_buffer(frame, 0);
    if (ret < 0) FAIL("Unable to allocate video buffer: %s\n", av_err2str(ret));

    fd.frame = frame;
    fd.packet = packet;
    fd.avctx = avctx;
    fd.codec = codec;
    fd.vid_fptr = fopen("/tmp/output.mp4", "wb");
#endif

    setup();

    app = gtk_application_new("org.gtk.example", G_APPLICATION_FLAGS_NONE);
    g_signal_connect(app, "activate", G_CALLBACK (activate), &fd);
    status = g_application_run(G_APPLICATION (app), argc, argv);
    g_object_unref (app);

    done(0);
}
