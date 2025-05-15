/*******************************************************************************
 * Size: 13 px
 * Bpp: 1
 * Opts: --bpp 1 --size 13 --no-compress --font Helvetica_Oblique.ttf --symbols AaBbCcDdEeFfGgHhIiJjKkLlMmNnOoPpQqRrSsTtUuVvWwXxYyZz --format lvgl -o helvetica_oblique_13.c
 ******************************************************************************/

#ifdef LV_LVGL_H_INCLUDE_SIMPLE
#include "lvgl.h"
#else
#include "lvgl/lvgl.h"
#endif

#ifndef HELVETICA_OBLIQUE_13
#define HELVETICA_OBLIQUE_13 1
#endif

#if HELVETICA_OBLIQUE_13

/*-----------------
 *    BITMAPS
 *----------------*/

/*Store the image of the glyphs*/
static LV_ATTRIBUTE_LARGE_CONST const uint8_t glyph_bitmap[] = {
    /* U+0041 "A" */
    0x6, 0x3, 0x2, 0xc1, 0x61, 0x31, 0xf8, 0x84,
    0x82, 0xc1, 0x0,

    /* U+0042 "B" */
    0x3f, 0x21, 0x61, 0x43, 0x7e, 0x43, 0xc3, 0xc2,
    0xfc,

    /* U+0043 "C" */
    0xf, 0x18, 0xd8, 0x28, 0x4, 0x6, 0x3, 0x4,
    0x84, 0x3c, 0x0,

    /* U+0044 "D" */
    0x3f, 0x10, 0xd8, 0x6c, 0x14, 0x1a, 0x9, 0x5,
    0x84, 0xfc, 0x0,

    /* U+0045 "E" */
    0x3f, 0x90, 0x8, 0xc, 0x7, 0xf2, 0x1, 0x1,
    0x80, 0xfe, 0x0,

    /* U+0046 "F" */
    0x3f, 0x10, 0x8, 0xc, 0x7, 0xe2, 0x1, 0x1,
    0x80, 0xc0, 0x0,

    /* U+0047 "G" */
    0xf, 0x18, 0x58, 0x28, 0x4, 0x7e, 0x7, 0x6,
    0x86, 0x3d, 0x0,

    /* U+0048 "H" */
    0x20, 0x90, 0x58, 0x2c, 0x37, 0xf2, 0x9, 0x5,
    0x86, 0x83, 0x0,

    /* U+0049 "I" */
    0x22, 0x26, 0x64, 0x44, 0xc0,

    /* U+004A "J" */
    0x2, 0x2, 0x2, 0x6, 0x6, 0x4, 0x44, 0x4c,
    0x78,

    /* U+004B "K" */
    0x21, 0x88, 0xc6, 0x41, 0x60, 0x78, 0x13, 0x4,
    0x43, 0x18, 0x82, 0x0,

    /* U+004C "L" */
    0x20, 0x86, 0x10, 0x41, 0x4, 0x30, 0xfc,

    /* U+004D "M" */
    0x30, 0x66, 0x1d, 0xc3, 0x28, 0xa5, 0x14, 0xa5,
    0xb7, 0xa6, 0x64, 0x88, 0x80,

    /* U+004E "N" */
    0x30, 0x98, 0x5e, 0x29, 0x34, 0x92, 0x69, 0x15,
    0x8e, 0x83, 0x0,

    /* U+004F "O" */
    0xf, 0xc, 0x26, 0xd, 0x3, 0xc0, 0xf0, 0x2c,
    0x19, 0xc, 0x3c, 0x0,

    /* U+0050 "P" */
    0x3f, 0x10, 0xc8, 0x4c, 0x27, 0xe2, 0x1, 0x1,
    0x80, 0x80, 0x0,

    /* U+0051 "Q" */
    0xf, 0xc, 0x26, 0xd, 0x3, 0xc0, 0xf0, 0x2c,
    0x59, 0x1c, 0x3d, 0x0,

    /* U+0052 "R" */
    0x3f, 0x10, 0xc8, 0x2c, 0x27, 0xe2, 0x19, 0x5,
    0x86, 0xc3, 0x0,

    /* U+0053 "S" */
    0x1e, 0x10, 0x98, 0x4c, 0x3, 0xe0, 0x1a, 0xd,
    0x84, 0x7c, 0x0,

    /* U+0054 "T" */
    0xff, 0x10, 0x10, 0x10, 0x30, 0x20, 0x20, 0x20,
    0x60,

    /* U+0055 "U" */
    0x41, 0x41, 0xc1, 0xc3, 0x82, 0x82, 0x82, 0x84,
    0x78,

    /* U+0056 "V" */
    0xc1, 0x43, 0x42, 0x44, 0x4c, 0x48, 0x78, 0x70,
    0x20,

    /* U+0057 "W" */
    0xc6, 0x3c, 0x62, 0xca, 0x2c, 0xa4, 0xd2, 0x4d,
    0x28, 0x62, 0x86, 0x30, 0x43, 0x0,

    /* U+0058 "X" */
    0x10, 0xc6, 0x60, 0x90, 0x38, 0x4, 0x3, 0x81,
    0x20, 0x8c, 0x41, 0x0,

    /* U+0059 "Y" */
    0xc1, 0x21, 0x19, 0x5, 0x3, 0x80, 0x80, 0xc0,
    0x40, 0x20, 0x0,

    /* U+005A "Z" */
    0x1f, 0xc0, 0x60, 0x30, 0x18, 0xc, 0x6, 0x3,
    0x1, 0x80, 0xff, 0x0,

    /* U+0061 "a" */
    0x3d, 0x10, 0x5f, 0x86, 0x2f, 0xc0,

    /* U+0062 "b" */
    0x20, 0x81, 0x73, 0x34, 0x68, 0xa1, 0x66, 0xf0,

    /* U+0063 "c" */
    0x3d, 0x1c, 0x20, 0x86, 0x27, 0x0,

    /* U+0064 "d" */
    0x2, 0x4, 0xea, 0x3c, 0x50, 0xa1, 0x46, 0x7c,

    /* U+0065 "e" */
    0x3c, 0xc9, 0x1f, 0xe8, 0x19, 0x9e, 0x0,

    /* U+0066 "f" */
    0x32, 0x74, 0x44, 0x4c, 0x80,

    /* U+0067 "g" */
    0x1d, 0x22, 0x62, 0x42, 0x42, 0x46, 0x3c, 0x4,
    0xcc, 0x78,

    /* U+0068 "h" */
    0x20, 0xc1, 0x73, 0x24, 0x48, 0xb1, 0x42, 0x88,

    /* U+0069 "i" */
    0x21, 0x24, 0xb4, 0x80,

    /* U+006A "j" */
    0x8, 0x4, 0x21, 0x8, 0xc4, 0x21, 0x9, 0x80,

    /* U+006B "k" */
    0x20, 0xc1, 0x12, 0xc7, 0xe, 0x24, 0x4c, 0x88,

    /* U+006C "l" */
    0x25, 0x24, 0xb4, 0x80,

    /* U+006D "m" */
    0x5d, 0xd9, 0x94, 0x45, 0x11, 0xcc, 0xe2, 0x28,
    0x88,

    /* U+006E "n" */
    0x5c, 0xc9, 0x12, 0x2c, 0x50, 0xa3, 0x0,

    /* U+006F "o" */
    0x3c, 0x8b, 0x1c, 0x28, 0x51, 0x1c, 0x0,

    /* U+0070 "p" */
    0x2e, 0x32, 0x23, 0x62, 0x42, 0x66, 0x7c, 0x40,
    0xc0, 0x80,

    /* U+0071 "q" */
    0x3a, 0x8f, 0x14, 0x28, 0x51, 0x9f, 0x4, 0x8,
    0x10,

    /* U+0072 "r" */
    0x5b, 0x10, 0x8c, 0x42, 0x0,

    /* U+0073 "s" */
    0x39, 0x14, 0xe, 0xa, 0x2f, 0x0,

    /* U+0074 "t" */
    0x22, 0xf4, 0x44, 0x4c, 0xc0,

    /* U+0075 "u" */
    0x46, 0x89, 0x16, 0x28, 0x59, 0x9e, 0x0,

    /* U+0076 "v" */
    0x8e, 0x29, 0x24, 0xa2, 0x8c, 0x0,

    /* U+0077 "w" */
    0x99, 0x4c, 0xaa, 0x95, 0x4c, 0xc6, 0x63, 0x20,

    /* U+0078 "x" */
    0x22, 0x34, 0x18, 0x18, 0x38, 0x6c, 0xc4,

    /* U+0079 "y" */
    0x23, 0x22, 0x24, 0x24, 0x28, 0x28, 0x30, 0x10,
    0x20, 0xc0,

    /* U+007A "z" */
    0x3e, 0xc, 0x30, 0xc3, 0xc, 0x3f, 0x0
};


/*---------------------
 *  GLYPH DESCRIPTION
 *--------------------*/

static const lv_font_fmt_txt_glyph_dsc_t glyph_dsc[] = {
    {.bitmap_index = 0, .adv_w = 0, .box_w = 0, .box_h = 0, .ofs_x = 0, .ofs_y = 0} /* id = 0 reserved */,
    {.bitmap_index = 0, .adv_w = 139, .box_w = 9, .box_h = 9, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 11, .adv_w = 139, .box_w = 8, .box_h = 9, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 20, .adv_w = 150, .box_w = 9, .box_h = 9, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 31, .adv_w = 150, .box_w = 9, .box_h = 9, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 42, .adv_w = 139, .box_w = 9, .box_h = 9, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 53, .adv_w = 127, .box_w = 9, .box_h = 9, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 64, .adv_w = 162, .box_w = 9, .box_h = 9, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 75, .adv_w = 150, .box_w = 9, .box_h = 9, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 86, .adv_w = 58, .box_w = 4, .box_h = 9, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 91, .adv_w = 104, .box_w = 8, .box_h = 9, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 100, .adv_w = 139, .box_w = 10, .box_h = 9, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 112, .adv_w = 116, .box_w = 6, .box_h = 9, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 119, .adv_w = 173, .box_w = 11, .box_h = 9, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 132, .adv_w = 150, .box_w = 9, .box_h = 9, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 143, .adv_w = 162, .box_w = 10, .box_h = 9, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 155, .adv_w = 139, .box_w = 9, .box_h = 9, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 166, .adv_w = 162, .box_w = 10, .box_h = 9, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 178, .adv_w = 150, .box_w = 9, .box_h = 9, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 189, .adv_w = 139, .box_w = 9, .box_h = 9, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 200, .adv_w = 127, .box_w = 8, .box_h = 9, .ofs_x = 2, .ofs_y = 0},
    {.bitmap_index = 209, .adv_w = 150, .box_w = 8, .box_h = 9, .ofs_x = 2, .ofs_y = 0},
    {.bitmap_index = 218, .adv_w = 139, .box_w = 8, .box_h = 9, .ofs_x = 2, .ofs_y = 0},
    {.bitmap_index = 227, .adv_w = 196, .box_w = 12, .box_h = 9, .ofs_x = 2, .ofs_y = 0},
    {.bitmap_index = 241, .adv_w = 139, .box_w = 10, .box_h = 9, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 253, .adv_w = 139, .box_w = 9, .box_h = 9, .ofs_x = 2, .ofs_y = 0},
    {.bitmap_index = 264, .adv_w = 127, .box_w = 10, .box_h = 9, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 276, .adv_w = 116, .box_w = 6, .box_h = 7, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 282, .adv_w = 116, .box_w = 7, .box_h = 9, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 290, .adv_w = 104, .box_w = 6, .box_h = 7, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 296, .adv_w = 116, .box_w = 7, .box_h = 9, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 304, .adv_w = 116, .box_w = 7, .box_h = 7, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 311, .adv_w = 58, .box_w = 4, .box_h = 9, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 316, .adv_w = 116, .box_w = 8, .box_h = 10, .ofs_x = 0, .ofs_y = -3},
    {.bitmap_index = 326, .adv_w = 116, .box_w = 7, .box_h = 9, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 334, .adv_w = 46, .box_w = 3, .box_h = 9, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 338, .adv_w = 46, .box_w = 5, .box_h = 12, .ofs_x = -1, .ofs_y = -3},
    {.bitmap_index = 346, .adv_w = 104, .box_w = 7, .box_h = 9, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 354, .adv_w = 46, .box_w = 3, .box_h = 9, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 358, .adv_w = 173, .box_w = 10, .box_h = 7, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 367, .adv_w = 116, .box_w = 7, .box_h = 7, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 374, .adv_w = 116, .box_w = 7, .box_h = 7, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 381, .adv_w = 116, .box_w = 8, .box_h = 10, .ofs_x = 0, .ofs_y = -3},
    {.bitmap_index = 391, .adv_w = 116, .box_w = 7, .box_h = 10, .ofs_x = 1, .ofs_y = -3},
    {.bitmap_index = 400, .adv_w = 69, .box_w = 5, .box_h = 7, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 405, .adv_w = 104, .box_w = 6, .box_h = 7, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 411, .adv_w = 58, .box_w = 4, .box_h = 9, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 416, .adv_w = 116, .box_w = 7, .box_h = 7, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 423, .adv_w = 104, .box_w = 6, .box_h = 7, .ofs_x = 2, .ofs_y = 0},
    {.bitmap_index = 429, .adv_w = 150, .box_w = 9, .box_h = 7, .ofs_x = 2, .ofs_y = 0},
    {.bitmap_index = 437, .adv_w = 104, .box_w = 8, .box_h = 7, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 444, .adv_w = 104, .box_w = 8, .box_h = 10, .ofs_x = 0, .ofs_y = -3},
    {.bitmap_index = 454, .adv_w = 104, .box_w = 7, .box_h = 7, .ofs_x = 0, .ofs_y = 0}
};

/*---------------------
 *  CHARACTER MAPPING
 *--------------------*/



/*Collect the unicode lists and glyph_id offsets*/
static const lv_font_fmt_txt_cmap_t cmaps[] =
{
    {
        .range_start = 65, .range_length = 26, .glyph_id_start = 1,
        .unicode_list = NULL, .glyph_id_ofs_list = NULL, .list_length = 0, .type = LV_FONT_FMT_TXT_CMAP_FORMAT0_TINY
    },
    {
        .range_start = 97, .range_length = 26, .glyph_id_start = 27,
        .unicode_list = NULL, .glyph_id_ofs_list = NULL, .list_length = 0, .type = LV_FONT_FMT_TXT_CMAP_FORMAT0_TINY
    }
};

/*-----------------
 *    KERNING
 *----------------*/


/*Pair left and right glyphs for kerning*/
static const uint8_t kern_pair_glyph_ids[] =
{
    1, 20,
    1, 22,
    1, 23,
    1, 25,
    1, 48,
    1, 49,
    1, 51,
    6, 1,
    12, 20,
    12, 22,
    12, 23,
    12, 25,
    12, 51,
    16, 1,
    18, 20,
    18, 22,
    18, 23,
    18, 25,
    20, 1,
    20, 15,
    20, 27,
    20, 29,
    20, 31,
    20, 35,
    20, 41,
    20, 44,
    20, 45,
    20, 47,
    20, 49,
    20, 51,
    22, 1,
    22, 27,
    22, 31,
    22, 35,
    22, 41,
    22, 44,
    22, 47,
    22, 51,
    23, 1,
    23, 27,
    23, 31,
    23, 41,
    23, 44,
    23, 47,
    23, 51,
    25, 1,
    25, 27,
    25, 31,
    25, 35,
    25, 41,
    25, 42,
    25, 43,
    25, 47,
    25, 48,
    32, 32
};

/* Kerning between the respective left and right glyphs
 * 4.4 format which needs to scaled with `kern_scale`*/
static const int8_t kern_pair_values[] =
{
    -15, -15, -8, -15, -4, -4, -4, -11,
    -15, -15, -15, -15, -8, -15, -4, -4,
    -4, -4, -15, -4, -23, -23, -23, -8,
    -23, -8, -23, -8, -11, -11, -15, -15,
    -11, -4, -11, -8, -8, -8, -8, -8,
    -4, -4, -4, -4, -2, -15, -15, -19,
    -8, -19, -15, -19, -11, -11, -4
};

/*Collect the kern pair's data in one place*/
static const lv_font_fmt_txt_kern_pair_t kern_pairs =
{
    .glyph_ids = kern_pair_glyph_ids,
    .values = kern_pair_values,
    .pair_cnt = 55,
    .glyph_ids_size = 0
};

/*--------------------
 *  ALL CUSTOM DATA
 *--------------------*/

#if LVGL_VERSION_MAJOR == 8
/*Store all the custom data of the font*/
static  lv_font_fmt_txt_glyph_cache_t cache;
#endif

#if LVGL_VERSION_MAJOR >= 8
static const lv_font_fmt_txt_dsc_t font_dsc = {
#else
static lv_font_fmt_txt_dsc_t font_dsc = {
#endif
    .glyph_bitmap = glyph_bitmap,
    .glyph_dsc = glyph_dsc,
    .cmaps = cmaps,
    .kern_dsc = &kern_pairs,
    .kern_scale = 16,
    .cmap_num = 2,
    .bpp = 1,
    .kern_classes = 0,
    .bitmap_format = 0,
#if LVGL_VERSION_MAJOR == 8
    .cache = &cache
#endif
};



/*-----------------
 *  PUBLIC FONT
 *----------------*/

/*Initialize a public general font descriptor*/
#if LVGL_VERSION_MAJOR >= 8
const lv_font_t helvetica_oblique_13 = {
#else
lv_font_t helvetica_oblique_13 = {
#endif
    .get_glyph_dsc = lv_font_get_glyph_dsc_fmt_txt,    /*Function pointer to get glyph's data*/
    .get_glyph_bitmap = lv_font_get_bitmap_fmt_txt,    /*Function pointer to get glyph's bitmap*/
    .line_height = 12,          /*The maximum line height required by the font*/
    .base_line = 3,             /*Baseline measured from the bottom of the line*/
#if !(LVGL_VERSION_MAJOR == 6 && LVGL_VERSION_MINOR == 0)
    .subpx = LV_FONT_SUBPX_NONE,
#endif
#if LV_VERSION_CHECK(7, 4, 0) || LVGL_VERSION_MAJOR >= 8
    .underline_position = -2,
    .underline_thickness = 1,
#endif
    .dsc = &font_dsc,          /*The custom font data. Will be accessed by `get_glyph_bitmap/dsc` */
#if LV_VERSION_CHECK(8, 2, 0) || LVGL_VERSION_MAJOR >= 9
    .fallback = NULL,
#endif
    .user_data = NULL,
};



#endif /*#if HELVETICA_OBLIQUE_13*/

