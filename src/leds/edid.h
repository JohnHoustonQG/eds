/* vim: set et fde fdm=syntax ts=4 sts=4 sw=4 : */
/*
 * Copyright © 2010-2011 Saleem Abdulrasool <compnerd@compnerd.org>.
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 * 1. Redistributions of source code must retain the above copyright notice,
 *    this list of conditions and the following disclaimer.
 *
 * 2. Redistributions in binary form must reproduce the above copyright notice,
 *    this list of conditions and the following disclaimer in the documentation
 *    and/or other materials provided with the distribution.
 *
 * 3. The name of the author may not be used to endorse or promote products
 *    derived from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE AUTHOR "AS IS" AND ANY EXPRESS OR IMPLIED
 * WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF
 * MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO
 * EVENT SHALL THE AUTHOR BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
 * SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
 * PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS;
 * OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
 * WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR
 * OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF
 * ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#ifndef leds_edid_h
#define leds_edid_h

#include <stdint.h>
#include <stdbool.h>

#define EDID_I2C_DDC_DATA_ADDRESS                       (0x50)

#define EDID_BLOCK_SIZE                                 (0x80)
#define EDID_MAX_EXTENSIONS                             (0xfe)


static const uint8_t EDID_HEADER[] = { 0x00, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0x00 };


enum edid_extension_type {
    EDID_EXTENSION_TIMING           = 0x01, // Timing Extension
    EDID_EXTENSION_CEA              = 0x02, // Additional Timing Block Data (CEA EDID Timing Extension)
    EDID_EXTENSION_VTB              = 0x10, // Video Timing Block Extension (VTB-EXT)
    EDID_EXTENSION_EDID_2_0         = 0x20, // EDID 2.0 Extension
    EDID_EXTENSION_DI               = 0x40, // Display Information Extension (DI-EXT)
    EDID_EXTENSION_LS               = 0x50, // Localised String Extension (LS-EXT)
    EDID_EXTENSION_MI               = 0x60, // Microdisplay Interface Extension (MI-EXT)
    EDID_EXTENSION_DTCDB_1          = 0xa7, // Display Transfer Characteristics Data Block (DTCDB)
    EDID_EXTENSION_DTCDB_2          = 0xaf,
    EDID_EXTENSION_DTCDB_3          = 0xbf,
    EDID_EXTENSION_BLOCK_MAP        = 0xf0, // Block Map
    EDID_EXTENSION_DDDB             = 0xff, // Display Device Data Block (DDDB)
};

enum edid_display_type {
    EDID_DISPLAY_TYPE_MONOCHROME,
    EDID_DISPLAY_TYPE_RGB,
    EDID_DISPLAY_TYPE_NON_RGB,
    EDID_DISPLAY_TYPE_UNDEFINED,
};

enum edid_aspect_ratio {
    EDID_ASPECT_RATIO_16_10,
    EDID_ASPECT_RATIO_4_3,
    EDID_ASPECT_RATIO_5_4,
    EDID_ASPECT_RATIO_16_9,
};

enum edid_monitor_descriptor_type {
    EDID_MONITOR_DESCRIPTOR_STANDARD_TIMING_IDENTIFIERS = 0xfa,
    EDID_MONITOR_DESCRIPTOR_COLOR_POINT                 = 0xfb,
    EDID_MONITOR_DESCRIPTOR_MONITOR_NAME                = 0xfc,
    EDID_MONITOR_DESCRIPTOR_MONITOR_RANGE_LIMITS        = 0xfd,
    EDID_MONITOR_DESCRIPTOR_MONITOR_SERIAL_NUMBER       = 0xff,
};


struct __attribute__ (( packed )) edid_detailed_timing_descriptor {
    uint16_t pixel_clock;                               /* = value * 10000 */

    uint8_t  horizontal_active_lo;
    uint8_t  horizontal_blanking_lo;

    unsigned horizontal_blanking_hi         : 4;
    unsigned horizontal_active_hi           : 4;

    uint8_t  vertical_active_lo;
    uint8_t  vertical_blanking_lo;

    unsigned vertical_blanking_hi           : 4;
    unsigned vertical_active_hi             : 4;

    uint8_t  horizontal_sync_offset_lo;
    uint8_t  horizontal_sync_pulse_width_lo;

    unsigned vertical_sync_pulse_width_lo   : 4;
    unsigned vertical_sync_offset_lo        : 4;

    unsigned vertical_sync_pulse_width_hi   : 2;
    unsigned vertical_sync_offset_hi        : 2;
    unsigned horizontal_sync_pulse_width_hi : 2;
    unsigned horizontal_sync_offset_hi      : 2;

    uint8_t  horizontal_image_size_lo;
    uint8_t  vertical_image_size_lo;

    unsigned vertical_image_size_hi         : 4;
    unsigned horizontal_image_size_hi       : 4;

    uint8_t  horizontal_border;
    uint8_t  vertical_border;

    unsigned flags                          : 8;
};

static inline uint16_t
edid_timing_pixel_clock(const struct edid_detailed_timing_descriptor * const dtb)
{
    return dtb->pixel_clock * 10000;
}

static inline uint16_t
edid_timing_horizontal_blanking(const struct edid_detailed_timing_descriptor * const dtb)
{
    return (dtb->horizontal_blanking_hi << 8) | dtb->horizontal_blanking_lo;
}

static inline uint16_t
edid_timing_horizontal_active(const struct edid_detailed_timing_descriptor * const dtb)
{
    return (dtb->horizontal_active_hi << 8) | dtb->horizontal_active_lo;
}

static inline uint16_t
edid_timing_vertical_blanking(const struct edid_detailed_timing_descriptor * const dtb)
{
    return (dtb->vertical_blanking_hi << 8) | dtb->vertical_blanking_lo;
}

static inline uint16_t
edid_timing_vertical_active(const struct edid_detailed_timing_descriptor * const dtb)
{
    return (dtb->vertical_active_hi << 8) | dtb->vertical_active_lo;
}

static inline uint8_t
edid_timing_vertical_sync_offset(const struct edid_detailed_timing_descriptor * const dtb)
{
    return (dtb->vertical_sync_offset_hi << 4) | dtb->vertical_sync_offset_lo;
}

static inline uint8_t
edid_timing_vertical_sync_pulse_width(const struct edid_detailed_timing_descriptor * const dtb)
{
    return (dtb->vertical_sync_pulse_width_hi << 4) | dtb->vertical_sync_pulse_width_lo;
}

static inline uint8_t
edid_timing_horizontal_sync_offset(const struct edid_detailed_timing_descriptor * const dtb)
{
    return (dtb->horizontal_sync_offset_hi << 4) | dtb->horizontal_sync_offset_lo;
}

static inline uint8_t
edid_timing_horizontal_sync_pulse_width(const struct edid_detailed_timing_descriptor * const dtb)
{
    return (dtb->horizontal_sync_pulse_width_hi << 4) | dtb->horizontal_sync_pulse_width_lo;
}

static inline uint16_t
edid_timing_horizontal_image_size(const struct edid_detailed_timing_descriptor * const dtb)
{
    return (dtb->horizontal_image_size_hi << 8) | dtb->horizontal_image_size_lo;
}

static inline uint16_t
edid_timing_vertical_image_size(const struct edid_detailed_timing_descriptor * const dtb)
{
    return (dtb->vertical_image_size_hi << 8) | dtb->vertical_image_size_lo;
}


struct __attribute__ (( packed )) edid_monitor_descriptor {
    uint16_t flag0;
    uint8_t  flag1;
    uint8_t  tag;
    uint8_t  flag2;
    uint8_t  data[13];
};


struct __attribute__ (( packed )) edid_standard_timing_descriptor {
    uint8_t  horizontal_active_pixels;         /* = (value + 31) * 8 */

    unsigned refresh_rate       : 6;           /* = value + 60 */
    unsigned image_aspect_ratio : 2;
};


struct __attribute__ (( packed )) edid {
    /* header information */
    uint8_t header[8];

    /* vendor/product identification */
    struct __attribute__ (( packed )) {
        unsigned id2  : 5;
        unsigned id1  : 5;
        unsigned id0  : 5;
        unsigned zero : 1;
    } manufacturer;

    uint8_t product[2];
    uint8_t serial_number[4];
    uint8_t manufacture_week;
    uint8_t manufacture_year;

    /* EDID version */
    uint8_t version;
    uint8_t revision;

    /* basic display parameters and features */
    struct __attribute__ (( packed )) {
        unsigned dfp_1x_vsync_serration : 1;   /* VESA DFP 1.x */
        unsigned green_video_sync       : 1;
        unsigned composite_sync         : 1;
        unsigned separate_sync          : 1;
        unsigned blank_to_black_setup   : 1;
        unsigned signal_level_standard  : 2;
        unsigned digital                : 1;
    } video_input_definition;

    uint8_t maximum_horizontal_image_size;     /* cm */
    uint8_t maximum_vertical_image_size;       /* cm */

    uint8_t display_transfer_characteristics;  /* gamma = (value + 100) / 100 */

    struct __attribute__ (( packed )) {
        unsigned default_gtf                    : 1; /* generalised timing formula */
        unsigned preferred_timing_mode          : 1;
        unsigned standard_default_color_space   : 1;
        unsigned display_type                   : 2;
        unsigned active_off                     : 1;
        unsigned suspend                        : 1;
        unsigned standby                        : 1;
    } feature_support;

    /* color characteristics block */
    unsigned green_y_low    : 2;
    unsigned green_x_low    : 2;
    unsigned red_y_low      : 2;
    unsigned red_x_low      : 2;

    unsigned white_y_low    : 2;
    unsigned white_x_low    : 2;
    unsigned blue_y_low     : 2;
    unsigned blue_x_low     : 2;

    uint8_t red_x;
    uint8_t red_y;
    uint8_t green_x;
    uint8_t green_y;
    uint8_t blue_x;
    uint8_t blue_y;
    uint8_t white_x;
    uint8_t white_y;

    /* established timings */
    struct __attribute__ (( packed )) {
        unsigned timing_800x600_60   : 1;
        unsigned timing_800x600_56   : 1;
        unsigned timing_640x480_75   : 1;
        unsigned timing_640x480_72   : 1;
        unsigned timing_640x480_67   : 1;
        unsigned timing_640x480_60   : 1;
        unsigned timing_720x400_88   : 1;
        unsigned timing_720x400_70   : 1;

        unsigned timing_1280x1024_75 : 1;
        unsigned timing_1024x768_75  : 1;
        unsigned timing_1024x768_70  : 1;
        unsigned timing_1024x768_60  : 1;
        unsigned timing_1024x768_87  : 1;
        unsigned timing_832x624_75   : 1;
        unsigned timing_800x600_75   : 1;
        unsigned timing_800x600_72   : 1;
    } established_timings;

    struct __attribute__ (( packed )) {
        unsigned reserved            : 7;
        unsigned timing_1152x870_75  : 1;
    } manufacturer_timings;

    /* standard timing id */
    struct edid_standard_timing_descriptor standard_timing_id[8];

    /* detailed timing */
    union {
        struct edid_monitor_descriptor         monitor;
        struct edid_detailed_timing_descriptor timing;
    } detailed_timings[4];

    uint8_t extensions;
    uint8_t checksum;
};

static inline uint16_t
edid_gamma(const struct edid * const edid)
{
    return (edid->display_transfer_characteristics + 100) / 100;
}


struct __attribute__ (( packed )) edid_color_characteristics_data {
    struct {
        uint16_t x;
        uint16_t y;
    } red, green, blue, white;
};

static inline struct edid_color_characteristics_data
edid_color_characteristics(const struct edid * const edid)
{
    const struct edid_color_characteristics_data characteristics = {
        .red = {
            .x = (edid->red_x << 8) | edid->red_x_low,
            .y = (edid->red_y << 8) | edid->red_y_low,
        },
        .green = {
            .x = (edid->green_x << 8) | edid->green_x_low,
            .y = (edid->green_y << 8) | edid->green_y_low,
        },
        .blue = {
            .x = (edid->blue_x << 8) | edid->blue_x_low,
            .y = (edid->blue_y << 8) | edid->blue_y_low,
        },
        .white = {
            .x = (edid->white_x << 8) | edid->white_x_low,
            .y = (edid->white_y << 8) | edid->white_y_low,
        },
    };

    return characteristics;
}


struct __attribute__ (( packed )) edid_block_map {
    uint8_t tag;
    uint8_t extension_tag[126];
    uint8_t checksum;
};


struct __attribute__ (( packed )) edid_extension {
    uint8_t tag;
    uint8_t revision;
    uint8_t extension_data[125];
    uint8_t checksum;
};


static inline bool
edid_verify_checksum(const uint8_t * const block)
{
    uint8_t checksum = 0;
    int i;

    for (i = 0; i < EDID_BLOCK_SIZE; i++)
        checksum += block[i];

    return (checksum == 0);
}

#endif
