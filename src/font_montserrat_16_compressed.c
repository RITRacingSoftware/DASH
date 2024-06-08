/*******************************************************************************
 * Size: 16 px
 * Bpp: 1
 * Opts: --bpp 1 --format lvgl --font Montserrat-Medium.ttf --size 16 --symbols ABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789 :()_-.%? -o font_montserrat_16_compressed.c
 ******************************************************************************/

#ifdef LV_LVGL_H_INCLUDE_SIMPLE
#include "lvgl.h"
#else
#include "lvgl/lvgl.h"
#endif

#ifndef FONT_MONTSERRAT_16_COMPRESSED
#define FONT_MONTSERRAT_16_COMPRESSED 1
#endif

#if FONT_MONTSERRAT_16_COMPRESSED

/*-----------------
 *    BITMAPS
 *----------------*/

/*Store the image of the glyphs*/
static LV_ATTRIBUTE_LARGE_CONST const uint8_t glyph_bitmap[] = {
    /* U+0020 " " */
    0x0,

    /* U+0025 "%" */
    0x70, 0x4d, 0x88, 0x88, 0x88, 0x90, 0x8b, 0xd,
    0xa0, 0x74, 0xe0, 0xd1, 0x9, 0x11, 0x11, 0x11,
    0x12, 0xe,

    /* U+0028 "(" */
    0x36, 0x64, 0xcc, 0xcc, 0xcc, 0xc4, 0x66, 0x30,

    /* U+0029 ")" */
    0xc6, 0x62, 0x33, 0x33, 0x33, 0x32, 0x66, 0xc0,

    /* U+002D "-" */
    0xf0,

    /* U+002E "." */
    0xf0,

    /* U+0030 "0" */
    0x1c, 0x31, 0x98, 0xd8, 0x3c, 0x1e, 0xf, 0x7,
    0x83, 0xc1, 0xb1, 0x98, 0xc7, 0x80,

    /* U+0031 "1" */
    0xf8, 0xc6, 0x31, 0x8c, 0x63, 0x18, 0xc6, 0x30,

    /* U+0032 "2" */
    0x7c, 0xc6, 0x3, 0x3, 0x3, 0x6, 0xe, 0x1c,
    0x38, 0x30, 0x60, 0xff,

    /* U+0033 "3" */
    0x7f, 0x1, 0x81, 0x80, 0x80, 0xc0, 0xf0, 0xc,
    0x3, 0x1, 0x80, 0xd8, 0xc7, 0xc0,

    /* U+0034 "4" */
    0x6, 0x3, 0x80, 0xc0, 0x60, 0x30, 0xc, 0xc6,
    0x33, 0xc, 0xff, 0xc0, 0xc0, 0x30, 0xc,

    /* U+0035 "5" */
    0x3f, 0x30, 0x18, 0xc, 0x6, 0x3, 0xf0, 0xc,
    0x3, 0x1, 0x80, 0xf0, 0xcf, 0xc0,

    /* U+0036 "6" */
    0x1f, 0x18, 0x98, 0x18, 0xc, 0x6, 0xf3, 0x8d,
    0xc3, 0xc1, 0xb0, 0xd8, 0xc3, 0xc0,

    /* U+0037 "7" */
    0xff, 0xe0, 0xb0, 0xc0, 0x40, 0x20, 0x30, 0x10,
    0x18, 0xc, 0x4, 0x6, 0x2, 0x0,

    /* U+0038 "8" */
    0x3e, 0x71, 0xb0, 0x78, 0x3e, 0x39, 0xf1, 0x8d,
    0x83, 0xc1, 0xe0, 0xd8, 0xc7, 0xc0,

    /* U+0039 "9" */
    0x3c, 0x31, 0xb0, 0x58, 0x3c, 0x1b, 0x1c, 0xf6,
    0x3, 0x1, 0x81, 0x91, 0x8f, 0x80,

    /* U+003A ":" */
    0xf0, 0x3, 0xc0,

    /* U+003F "?" */
    0x3e, 0x31, 0xc0, 0x60, 0x30, 0x38, 0x38, 0x38,
    0x18, 0x0, 0x0, 0x3, 0x1, 0x80,

    /* U+0041 "A" */
    0x6, 0x0, 0xe0, 0xf, 0x0, 0x90, 0x19, 0x81,
    0x18, 0x30, 0x82, 0xc, 0x7f, 0xc6, 0x6, 0x40,
    0x6c, 0x3,

    /* U+0042 "B" */
    0xfe, 0x30, 0xec, 0x1b, 0x6, 0xc1, 0xb0, 0xcf,
    0xfb, 0x7, 0xc0, 0xf0, 0x3c, 0x1b, 0xfc,

    /* U+0043 "C" */
    0xf, 0x86, 0x19, 0x81, 0x70, 0xc, 0x1, 0x80,
    0x30, 0x6, 0x0, 0x60, 0xc, 0x8, 0xc3, 0x7,
    0xc0,

    /* U+0044 "D" */
    0xff, 0x18, 0x33, 0x3, 0x60, 0x3c, 0x7, 0x80,
    0xf0, 0x1e, 0x3, 0xc0, 0x78, 0x1b, 0x6, 0x7f,
    0x80,

    /* U+0045 "E" */
    0xff, 0x60, 0x30, 0x18, 0xc, 0x6, 0x3, 0xfd,
    0x80, 0xc0, 0x60, 0x30, 0x1f, 0xf0,

    /* U+0046 "F" */
    0xff, 0xc0, 0xc0, 0xc0, 0xc0, 0xc0, 0xff, 0xc0,
    0xc0, 0xc0, 0xc0, 0xc0,

    /* U+0047 "G" */
    0x1f, 0x8c, 0x36, 0x3, 0x0, 0xc0, 0x30, 0xc,
    0xf, 0x3, 0xc0, 0xd8, 0x33, 0xc, 0x7e,

    /* U+0048 "H" */
    0xc0, 0xf0, 0x3c, 0xf, 0x3, 0xc0, 0xf0, 0x3f,
    0xff, 0x3, 0xc0, 0xf0, 0x3c, 0xf, 0x3,

    /* U+0049 "I" */
    0xff, 0xff, 0xff,

    /* U+004A "J" */
    0x7e, 0xc, 0x18, 0x30, 0x60, 0xc1, 0x83, 0x6,
    0xf, 0x33, 0xc0,

    /* U+004B "K" */
    0xc0, 0xb0, 0x6c, 0x33, 0x18, 0xcc, 0x36, 0xf,
    0xc3, 0x98, 0xc3, 0x30, 0xcc, 0x1b, 0x3,

    /* U+004C "L" */
    0xc0, 0xc0, 0xc0, 0xc0, 0xc0, 0xc0, 0xc0, 0xc0,
    0xc0, 0xc0, 0xc0, 0xff,

    /* U+004D "M" */
    0xc0, 0x3c, 0x3, 0xe0, 0x7e, 0x7, 0xf0, 0xfd,
    0xb, 0xd9, 0xbc, 0x93, 0xc7, 0x3c, 0x63, 0xc0,
    0x3c, 0x3,

    /* U+004E "N" */
    0xc0, 0xf8, 0x3f, 0xf, 0xc3, 0xf8, 0xf7, 0x3c,
    0xef, 0x1f, 0xc3, 0xf0, 0xfc, 0x1f, 0x3,

    /* U+004F "O" */
    0xf, 0x83, 0xc, 0x60, 0x6c, 0x7, 0xc0, 0x3c,
    0x3, 0xc0, 0x3c, 0x3, 0x40, 0x76, 0x6, 0x30,
    0xc0, 0xf8,

    /* U+0050 "P" */
    0xfe, 0x61, 0xb0, 0x78, 0x3c, 0x1e, 0xf, 0xd,
    0xfc, 0xc0, 0x60, 0x30, 0x18, 0x0,

    /* U+0051 "Q" */
    0xf, 0x81, 0x86, 0x18, 0x19, 0x80, 0xec, 0x3,
    0x60, 0x1b, 0x0, 0xd8, 0x6, 0x60, 0x73, 0x3,
    0xc, 0x30, 0x3f, 0x0, 0x31, 0x0, 0x78,

    /* U+0052 "R" */
    0xfe, 0x61, 0xb0, 0x78, 0x3c, 0x1e, 0xf, 0xd,
    0xfc, 0xc6, 0x61, 0x30, 0xd8, 0x30,

    /* U+0053 "S" */
    0x3f, 0x30, 0xb0, 0x18, 0xe, 0x3, 0xf0, 0xfc,
    0x7, 0x1, 0x80, 0xf0, 0xcf, 0xc0,

    /* U+0054 "T" */
    0xff, 0x86, 0x3, 0x1, 0x80, 0xc0, 0x60, 0x30,
    0x18, 0xc, 0x6, 0x3, 0x1, 0x80,

    /* U+0055 "U" */
    0xc0, 0xf0, 0x3c, 0xf, 0x3, 0xc0, 0xf0, 0x3c,
    0xf, 0x3, 0xc0, 0xf0, 0x26, 0x18, 0x78,

    /* U+0056 "V" */
    0xc0, 0x28, 0xd, 0x81, 0x30, 0x63, 0xc, 0x61,
    0x4, 0x60, 0xc8, 0x1b, 0x1, 0xe0, 0x38, 0x3,
    0x0,

    /* U+0057 "W" */
    0x81, 0x81, 0x60, 0xc0, 0xb0, 0x70, 0xc8, 0x78,
    0x46, 0x24, 0x23, 0x13, 0x30, 0x99, 0x90, 0x48,
    0x48, 0x34, 0x2c, 0x1e, 0x1e, 0x6, 0x6, 0x3,
    0x3, 0x0,

    /* U+0058 "X" */
    0x60, 0xc4, 0x10, 0xc6, 0xd, 0x80, 0xa0, 0x1c,
    0x3, 0x80, 0xd8, 0x1b, 0x6, 0x31, 0x83, 0x30,
    0x60,

    /* U+0059 "Y" */
    0xc0, 0x70, 0x36, 0x18, 0x84, 0x33, 0x7, 0x81,
    0xe0, 0x30, 0xc, 0x3, 0x0, 0xc0, 0x30,

    /* U+005A "Z" */
    0xff, 0x80, 0xc0, 0xc0, 0xc0, 0x40, 0x60, 0x60,
    0x20, 0x30, 0x30, 0x30, 0x1f, 0xf0,

    /* U+005F "_" */
    0xff
};


/*---------------------
 *  GLYPH DESCRIPTION
 *--------------------*/

static const lv_font_fmt_txt_glyph_dsc_t glyph_dsc[] = {
    {.bitmap_index = 0, .adv_w = 0, .box_w = 0, .box_h = 0, .ofs_x = 0, .ofs_y = 0} /* id = 0 reserved */,
    {.bitmap_index = 0, .adv_w = 69, .box_w = 1, .box_h = 1, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 1, .adv_w = 216, .box_w = 12, .box_h = 12, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 19, .adv_w = 86, .box_w = 4, .box_h = 15, .ofs_x = 2, .ofs_y = -3},
    {.bitmap_index = 27, .adv_w = 87, .box_w = 4, .box_h = 15, .ofs_x = 0, .ofs_y = -3},
    {.bitmap_index = 35, .adv_w = 98, .box_w = 4, .box_h = 1, .ofs_x = 1, .ofs_y = 4},
    {.bitmap_index = 36, .adv_w = 58, .box_w = 2, .box_h = 2, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 37, .adv_w = 171, .box_w = 9, .box_h = 12, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 51, .adv_w = 95, .box_w = 5, .box_h = 12, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 59, .adv_w = 147, .box_w = 8, .box_h = 12, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 71, .adv_w = 146, .box_w = 9, .box_h = 12, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 85, .adv_w = 171, .box_w = 10, .box_h = 12, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 100, .adv_w = 147, .box_w = 9, .box_h = 12, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 114, .adv_w = 158, .box_w = 9, .box_h = 12, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 128, .adv_w = 153, .box_w = 9, .box_h = 12, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 142, .adv_w = 165, .box_w = 9, .box_h = 12, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 156, .adv_w = 158, .box_w = 9, .box_h = 12, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 170, .adv_w = 58, .box_w = 2, .box_h = 9, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 173, .adv_w = 147, .box_w = 9, .box_h = 12, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 187, .adv_w = 187, .box_w = 12, .box_h = 12, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 205, .adv_w = 194, .box_w = 10, .box_h = 12, .ofs_x = 2, .ofs_y = 0},
    {.bitmap_index = 220, .adv_w = 185, .box_w = 11, .box_h = 12, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 237, .adv_w = 211, .box_w = 11, .box_h = 12, .ofs_x = 2, .ofs_y = 0},
    {.bitmap_index = 254, .adv_w = 172, .box_w = 9, .box_h = 12, .ofs_x = 2, .ofs_y = 0},
    {.bitmap_index = 268, .adv_w = 163, .box_w = 8, .box_h = 12, .ofs_x = 2, .ofs_y = 0},
    {.bitmap_index = 280, .adv_w = 198, .box_w = 10, .box_h = 12, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 295, .adv_w = 208, .box_w = 10, .box_h = 12, .ofs_x = 2, .ofs_y = 0},
    {.bitmap_index = 310, .adv_w = 79, .box_w = 2, .box_h = 12, .ofs_x = 2, .ofs_y = 0},
    {.bitmap_index = 313, .adv_w = 131, .box_w = 7, .box_h = 12, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 324, .adv_w = 184, .box_w = 10, .box_h = 12, .ofs_x = 2, .ofs_y = 0},
    {.bitmap_index = 339, .adv_w = 152, .box_w = 8, .box_h = 12, .ofs_x = 2, .ofs_y = 0},
    {.bitmap_index = 351, .adv_w = 244, .box_w = 12, .box_h = 12, .ofs_x = 2, .ofs_y = 0},
    {.bitmap_index = 369, .adv_w = 208, .box_w = 10, .box_h = 12, .ofs_x = 2, .ofs_y = 0},
    {.bitmap_index = 384, .adv_w = 215, .box_w = 12, .box_h = 12, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 402, .adv_w = 185, .box_w = 9, .box_h = 12, .ofs_x = 2, .ofs_y = 0},
    {.bitmap_index = 416, .adv_w = 215, .box_w = 13, .box_h = 14, .ofs_x = 1, .ofs_y = -2},
    {.bitmap_index = 439, .adv_w = 186, .box_w = 9, .box_h = 12, .ofs_x = 2, .ofs_y = 0},
    {.bitmap_index = 453, .adv_w = 159, .box_w = 9, .box_h = 12, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 467, .adv_w = 150, .box_w = 9, .box_h = 12, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 481, .adv_w = 202, .box_w = 10, .box_h = 12, .ofs_x = 2, .ofs_y = 0},
    {.bitmap_index = 496, .adv_w = 182, .box_w = 11, .box_h = 12, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 513, .adv_w = 288, .box_w = 17, .box_h = 12, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 539, .adv_w = 172, .box_w = 11, .box_h = 12, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 556, .adv_w = 166, .box_w = 10, .box_h = 12, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 571, .adv_w = 168, .box_w = 9, .box_h = 12, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 585, .adv_w = 128, .box_w = 8, .box_h = 1, .ofs_x = 0, .ofs_y = -1}
};

/*---------------------
 *  CHARACTER MAPPING
 *--------------------*/

static const uint8_t glyph_id_ofs_list_0[] = {
    0, 0, 0, 0, 0, 1, 0, 0,
    2, 3, 0, 0, 0, 4, 5, 0,
    6, 7, 8, 9, 10, 11, 12, 13,
    14, 15, 16, 0, 0, 0, 0, 17,
    0, 18, 19, 20, 21, 22, 23, 24,
    25, 26, 27, 28, 29, 30, 31, 32,
    33, 34, 35, 36, 37, 38, 39, 40,
    41, 42, 43, 0, 0, 0, 0, 44
};

/*Collect the unicode lists and glyph_id offsets*/
static const lv_font_fmt_txt_cmap_t cmaps[] =
{
    {
        .range_start = 32, .range_length = 64, .glyph_id_start = 1,
        .unicode_list = NULL, .glyph_id_ofs_list = glyph_id_ofs_list_0, .list_length = 64, .type = LV_FONT_FMT_TXT_CMAP_FORMAT0_FULL
    }
};



/*--------------------
 *  ALL CUSTOM DATA
 *--------------------*/

#if LV_VERSION_CHECK(8, 0, 0)
/*Store all the custom data of the font*/
static  lv_font_fmt_txt_glyph_cache_t cache;
static const lv_font_fmt_txt_dsc_t font_dsc = {
#else
static lv_font_fmt_txt_dsc_t font_dsc = {
#endif
    .glyph_bitmap = glyph_bitmap,
    .glyph_dsc = glyph_dsc,
    .cmaps = cmaps,
    .kern_dsc = NULL,
    .kern_scale = 0,
    .cmap_num = 1,
    .bpp = 1,
    .kern_classes = 0,
    .bitmap_format = 0,
#if LV_VERSION_CHECK(8, 0, 0)
    .cache = &cache
#endif
};


/*-----------------
 *  PUBLIC FONT
 *----------------*/

/*Initialize a public general font descriptor*/
#if LV_VERSION_CHECK(8, 0, 0)
const lv_font_t font_montserrat_16_compressed = {
#else
lv_font_t font_montserrat_16_compressed = {
#endif
    .get_glyph_dsc = lv_font_get_glyph_dsc_fmt_txt,    /*Function pointer to get glyph's data*/
    .get_glyph_bitmap = lv_font_get_bitmap_fmt_txt,    /*Function pointer to get glyph's bitmap*/
    .line_height = 15,          /*The maximum line height required by the font*/
    .base_line = 3,             /*Baseline measured from the bottom of the line*/
#if !(LVGL_VERSION_MAJOR == 6 && LVGL_VERSION_MINOR == 0)
    .subpx = LV_FONT_SUBPX_NONE,
#endif
#if LV_VERSION_CHECK(7, 4, 0) || LVGL_VERSION_MAJOR >= 8
    .underline_position = -2,
    .underline_thickness = 1,
#endif
    .dsc = &font_dsc           /*The custom font data. Will be accessed by `get_glyph_bitmap/dsc` */
};



#endif /*#if FONT_MONTSERRAT_16_COMPRESSED*/

