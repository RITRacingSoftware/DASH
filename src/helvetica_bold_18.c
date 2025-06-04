/*******************************************************************************
 * Size: 18 px
 * Bpp: 1
 * Opts: --bpp 1 --size 18 --no-compress --font Helvetica-Bold.ttf --symbols 0123456789VCNM/LB:FR??:PSIpsi --format lvgl -o helvetica_bold_18.c
 ******************************************************************************/

#ifdef LV_LVGL_H_INCLUDE_SIMPLE
#include "lvgl.h"
#else
#include "lvgl/lvgl.h"
#endif

#ifndef HELVETICA_BOLD_18
#define HELVETICA_BOLD_18 1
#endif

#if HELVETICA_BOLD_18

/*-----------------
 *    BITMAPS
 *----------------*/

/*Store the image of the glyphs*/
static LV_ATTRIBUTE_LARGE_CONST const uint8_t glyph_bitmap[] = {
    /* U+002F "/" */
    0x6, 0x1c, 0x30, 0x61, 0x83, 0x6, 0x18, 0x30,
    0xc1, 0x83, 0xc, 0x0,

    /* U+0030 "0" */
    0x3e, 0x3f, 0x9d, 0xdc, 0x7e, 0x3f, 0x1f, 0x8f,
    0xc7, 0xe3, 0xf1, 0xdd, 0xcf, 0xe3, 0xe0,

    /* U+0031 "1" */
    0xc, 0x7f, 0xff, 0x1c, 0x71, 0xc7, 0x1c, 0x71,
    0xc7, 0x1c,

    /* U+0032 "2" */
    0x3e, 0x3f, 0xb8, 0xfc, 0x7e, 0x38, 0x3c, 0x3c,
    0x3e, 0x3c, 0x3c, 0x3c, 0x1f, 0xff, 0xf8,

    /* U+0033 "3" */
    0x3c, 0x3f, 0x31, 0xd8, 0xe0, 0x70, 0xf0, 0x7c,
    0xf, 0x3, 0xe1, 0xf9, 0xef, 0xe3, 0xe0,

    /* U+0034 "4" */
    0xf, 0x7, 0x87, 0xc2, 0xe3, 0x73, 0x39, 0x9d,
    0x8e, 0xff, 0xff, 0xc1, 0xc0, 0xe0, 0x70,

    /* U+0035 "5" */
    0x7f, 0x3f, 0x98, 0xc, 0xd, 0xe7, 0xfb, 0x9e,
    0x7, 0x3, 0xe1, 0xf9, 0xcf, 0xe3, 0xe0,

    /* U+0036 "6" */
    0x1e, 0x3f, 0x9c, 0xfc, 0xe, 0xe7, 0xfb, 0xdf,
    0xc7, 0xe3, 0xf1, 0xdd, 0xef, 0xe3, 0xe0,

    /* U+0037 "7" */
    0xff, 0xff, 0xc0, 0xc0, 0xe0, 0x60, 0x70, 0x30,
    0x38, 0x1c, 0x1c, 0xe, 0x7, 0x3, 0x80,

    /* U+0038 "8" */
    0x3e, 0x3f, 0x98, 0xcc, 0x66, 0x31, 0xf1, 0xfd,
    0xc7, 0xe3, 0xf1, 0xf8, 0xef, 0xe3, 0xe0,

    /* U+0039 "9" */
    0x3e, 0x3f, 0xb8, 0xdc, 0x7e, 0x3f, 0x1d, 0xfe,
    0x77, 0x3, 0xf1, 0xfd, 0xcf, 0xe3, 0xc0,

    /* U+003A ":" */
    0xfc, 0x0, 0x0, 0xfc,

    /* U+003F "?" */
    0x3e, 0x3f, 0xbc, 0xfc, 0x70, 0x38, 0x3c, 0x3c,
    0x3c, 0x1c, 0xe, 0x0, 0x3, 0x81, 0xc0,

    /* U+0042 "B" */
    0xff, 0x1f, 0xfb, 0x87, 0x70, 0xee, 0x1d, 0xff,
    0x3f, 0xf7, 0x7, 0xe0, 0xfc, 0x1f, 0x87, 0xff,
    0xef, 0xf8,

    /* U+0043 "C" */
    0x1f, 0x83, 0xfc, 0x78, 0xe7, 0x6, 0xe0, 0xe,
    0x0, 0xe0, 0xe, 0x0, 0xe0, 0xf, 0x6, 0x78,
    0xe3, 0xfc, 0x1f, 0x0,

    /* U+0046 "F" */
    0xff, 0xff, 0xf8, 0x1c, 0xe, 0x7, 0xff, 0xff,
    0xc0, 0xe0, 0x70, 0x38, 0x1c, 0xe, 0x0,

    /* U+0049 "I" */
    0xff, 0xff, 0xff, 0xff, 0xfe,

    /* U+004C "L" */
    0xe0, 0x70, 0x38, 0x1c, 0xe, 0x7, 0x3, 0x81,
    0xc0, 0xe0, 0x70, 0x38, 0x1f, 0xff, 0xf8,

    /* U+004D "M" */
    0xf0, 0x7f, 0xc7, 0xfe, 0x3f, 0xf1, 0xff, 0x8f,
    0xf6, 0x5f, 0xb6, 0xfd, 0xb7, 0xed, 0xbf, 0x29,
    0xf9, 0xcf, 0xce, 0x7e, 0x73, 0x80,

    /* U+004E "N" */
    0xf0, 0xfe, 0x1f, 0xe3, 0xfc, 0x7f, 0xcf, 0xd9,
    0xfb, 0xbf, 0x37, 0xe7, 0xfc, 0x7f, 0x87, 0xf0,
    0xfe, 0xe,

    /* U+0050 "P" */
    0xff, 0x3f, 0xee, 0x1f, 0x87, 0xe1, 0xf8, 0x7f,
    0xfb, 0xfc, 0xe0, 0x38, 0xe, 0x3, 0x80, 0xe0,
    0x0,

    /* U+0052 "R" */
    0xff, 0x9f, 0xfb, 0x87, 0xf0, 0x7e, 0xf, 0xc1,
    0xff, 0xf7, 0xfc, 0xe1, 0xdc, 0x3b, 0x87, 0x70,
    0xee, 0x1e,

    /* U+0053 "S" */
    0x3f, 0xf, 0xf3, 0x8f, 0x70, 0xef, 0x1, 0xfc,
    0x1f, 0xf0, 0x7f, 0x0, 0xfc, 0x1f, 0xc7, 0xbf,
    0xe1, 0xf0,

    /* U+0056 "V" */
    0x60, 0x77, 0x6, 0x70, 0xe3, 0xe, 0x38, 0xc3,
    0x9c, 0x19, 0xc1, 0x98, 0x1d, 0x80, 0xf8, 0xf,
    0x0, 0xf0, 0x7, 0x0,

    /* U+0069 "i" */
    0xfc, 0x7f, 0xff, 0xff, 0xfe,

    /* U+0070 "p" */
    0xee, 0x7f, 0xbd, 0xfc, 0x7e, 0x3f, 0x1f, 0x8f,
    0xef, 0xff, 0x77, 0x38, 0x1c, 0xe, 0x7, 0x0,

    /* U+0073 "s" */
    0x1f, 0xf, 0xe7, 0x1d, 0xe0, 0x7f, 0x7, 0xf0,
    0x1d, 0xc7, 0x7f, 0x8f, 0xc0
};


/*---------------------
 *  GLYPH DESCRIPTION
 *--------------------*/

static const lv_font_fmt_txt_glyph_dsc_t glyph_dsc[] = {
    {.bitmap_index = 0, .adv_w = 0, .box_w = 0, .box_h = 0, .ofs_x = 0, .ofs_y = 0} /* id = 0 reserved */,
    {.bitmap_index = 0, .adv_w = 80, .box_w = 7, .box_h = 13, .ofs_x = -1, .ofs_y = 0},
    {.bitmap_index = 12, .adv_w = 160, .box_w = 9, .box_h = 13, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 27, .adv_w = 160, .box_w = 6, .box_h = 13, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 37, .adv_w = 160, .box_w = 9, .box_h = 13, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 52, .adv_w = 160, .box_w = 9, .box_h = 13, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 67, .adv_w = 160, .box_w = 9, .box_h = 13, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 82, .adv_w = 160, .box_w = 9, .box_h = 13, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 97, .adv_w = 160, .box_w = 9, .box_h = 13, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 112, .adv_w = 160, .box_w = 9, .box_h = 13, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 127, .adv_w = 160, .box_w = 9, .box_h = 13, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 142, .adv_w = 160, .box_w = 9, .box_h = 13, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 157, .adv_w = 96, .box_w = 3, .box_h = 10, .ofs_x = 2, .ofs_y = 0},
    {.bitmap_index = 161, .adv_w = 176, .box_w = 9, .box_h = 13, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 176, .adv_w = 208, .box_w = 11, .box_h = 13, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 194, .adv_w = 208, .box_w = 12, .box_h = 13, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 214, .adv_w = 176, .box_w = 9, .box_h = 13, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 229, .adv_w = 80, .box_w = 3, .box_h = 13, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 234, .adv_w = 176, .box_w = 9, .box_h = 13, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 249, .adv_w = 240, .box_w = 13, .box_h = 13, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 271, .adv_w = 208, .box_w = 11, .box_h = 13, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 289, .adv_w = 192, .box_w = 10, .box_h = 13, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 306, .adv_w = 208, .box_w = 11, .box_h = 13, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 324, .adv_w = 192, .box_w = 11, .box_h = 13, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 342, .adv_w = 192, .box_w = 12, .box_h = 13, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 362, .adv_w = 80, .box_w = 3, .box_h = 13, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 367, .adv_w = 176, .box_w = 9, .box_h = 14, .ofs_x = 1, .ofs_y = -4},
    {.bitmap_index = 383, .adv_w = 160, .box_w = 10, .box_h = 10, .ofs_x = 0, .ofs_y = 0}
};

/*---------------------
 *  CHARACTER MAPPING
 *--------------------*/

static const uint16_t unicode_list_1[] = {
    0x0, 0x3, 0x4, 0x7, 0xa, 0xd, 0xe, 0xf,
    0x11, 0x13, 0x14, 0x17, 0x2a, 0x31, 0x34
};

/*Collect the unicode lists and glyph_id offsets*/
static const lv_font_fmt_txt_cmap_t cmaps[] =
{
    {
        .range_start = 47, .range_length = 12, .glyph_id_start = 1,
        .unicode_list = NULL, .glyph_id_ofs_list = NULL, .list_length = 0, .type = LV_FONT_FMT_TXT_CMAP_FORMAT0_TINY
    },
    {
        .range_start = 63, .range_length = 53, .glyph_id_start = 13,
        .unicode_list = unicode_list_1, .glyph_id_ofs_list = NULL, .list_length = 15, .type = LV_FONT_FMT_TXT_CMAP_SPARSE_TINY
    }
};

/*-----------------
 *    KERNING
 *----------------*/


/*Pair left and right glyphs for kerning*/
static const uint8_t kern_pair_glyph_ids[] =
{
    3, 3,
    18, 24,
    22, 24,
    24, 12,
    24, 25
};

/* Kerning between the respective left and right glyphs
 * 4.4 format which needs to scaled with `kern_scale`*/
static const int8_t kern_pair_values[] =
{
    -16, -21, -5, -16, -5
};

/*Collect the kern pair's data in one place*/
static const lv_font_fmt_txt_kern_pair_t kern_pairs =
{
    .glyph_ids = kern_pair_glyph_ids,
    .values = kern_pair_values,
    .pair_cnt = 5,
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
const lv_font_t helvetica_bold_18 = {
#else
lv_font_t helvetica_bold_18 = {
#endif
    .get_glyph_dsc = lv_font_get_glyph_dsc_fmt_txt,    /*Function pointer to get glyph's data*/
    .get_glyph_bitmap = lv_font_get_bitmap_fmt_txt,    /*Function pointer to get glyph's bitmap*/
    .line_height = 17,          /*The maximum line height required by the font*/
    .base_line = 4,             /*Baseline measured from the bottom of the line*/
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



#endif /*#if HELVETICA_BOLD_18*/

