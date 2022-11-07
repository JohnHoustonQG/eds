/* vim: set et fde fdm=syntax ft=c.doxygen ts=4 sts=4 sw=4 : */
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

#include "edid.h"

/*static inline*/ uint32_t
edid_detailed_timing_pixel_clock(const struct edid_detailed_timing_descriptor * const dtb)
{
    return dtb->pixel_clock * 10000;
}

/*static inline*/ uint16_t
edid_detailed_timing_horizontal_blanking(const struct edid_detailed_timing_descriptor * const dtb)
{
    return (dtb->horizontal_blanking_hi << 8) | dtb->horizontal_blanking_lo;
}

/*static inline*/ uint16_t
edid_detailed_timing_horizontal_active(const struct edid_detailed_timing_descriptor * const dtb)
{
    return (dtb->horizontal_active_hi << 8) | dtb->horizontal_active_lo;
}

/*static inline*/ uint16_t
edid_detailed_timing_vertical_blanking(const struct edid_detailed_timing_descriptor * const dtb)
{
    return (dtb->vertical_blanking_hi << 8) | dtb->vertical_blanking_lo;
}

/*static inline*/ uint16_t
edid_detailed_timing_vertical_active(const struct edid_detailed_timing_descriptor * const dtb)
{
    return (dtb->vertical_active_hi << 8) | dtb->vertical_active_lo;
}

/*static inline*/ uint8_t
edid_detailed_timing_vertical_sync_offset(const struct edid_detailed_timing_descriptor * const dtb)
{
    return (dtb->vertical_sync_offset_hi << 4) | dtb->vertical_sync_offset_lo;
}

/*static inline*/ uint8_t
edid_detailed_timing_vertical_sync_pulse_width(const struct edid_detailed_timing_descriptor * const dtb)
{
    return (dtb->vertical_sync_pulse_width_hi << 4) | dtb->vertical_sync_pulse_width_lo;
}

/*static inline*/ uint8_t
edid_detailed_timing_horizontal_sync_offset(const struct edid_detailed_timing_descriptor * const dtb)
{
    return (dtb->horizontal_sync_offset_hi << 4) | dtb->horizontal_sync_offset_lo;
}

/*static inline*/ uint8_t
edid_detailed_timing_horizontal_sync_pulse_width(const struct edid_detailed_timing_descriptor * const dtb)
{
    return (dtb->horizontal_sync_pulse_width_hi << 4) | dtb->horizontal_sync_pulse_width_lo;
}

/*static inline*/ uint16_t
edid_detailed_timing_horizontal_image_size(const struct edid_detailed_timing_descriptor * const dtb)
{
    return (dtb->horizontal_image_size_hi << 8) | dtb->horizontal_image_size_lo;
}

/*static inline*/ uint16_t
edid_detailed_timing_vertical_image_size(const struct edid_detailed_timing_descriptor * const dtb)
{
    return (dtb->vertical_image_size_hi << 8) | dtb->vertical_image_size_lo;
}

/*static inline*/ uint8_t
edid_detailed_timing_stereo_mode(const struct edid_detailed_timing_descriptor * const dtb)
{
    return (dtb->stereo_mode_hi << 2 | dtb->stereo_mode_lo);
}

/*const inline*/ uint32_t
edid_standard_timing_horizontal_active(const struct edid_standard_timing_descriptor * const desc)
{
    return ((desc->horizontal_active_pixels + 31) << 3);
}

/*const inline*/ uint32_t
edid_standard_timing_vertical_active(const struct edid_standard_timing_descriptor * const desc)
{
    const uint32_t hres = edid_standard_timing_horizontal_active(desc);

    switch (desc->image_aspect_ratio) {
    case EDID_ASPECT_RATIO_16_10:
        return ((hres * 10) >> 4);
    case EDID_ASPECT_RATIO_4_3:
        return ((hres * 3) >> 2);
    case EDID_ASPECT_RATIO_5_4:
        return ((hres << 2) / 5);
    case EDID_ASPECT_RATIO_16_9:
        return ((hres * 9) >> 4);
    }

    return hres;
}

/*const inline*/ uint32_t
edid_standard_timing_refresh_rate(const struct edid_standard_timing_descriptor * const desc)
{
    return (desc->refresh_rate + 60);
}

/*static inline*/ void
edid_manufacturer(const struct edid * const edid, char manufacturer[4])
{
    manufacturer[0] = '@' + ((edid->manufacturer & 0x007c) >> 2);
    manufacturer[1] = '@' + (((edid->manufacturer & 0x0003) >> 00) << 3)
                          | (((edid->manufacturer & 0xe000) >> 13) << 0);
    manufacturer[2] = '@' + ((edid->manufacturer & 0x1f00) >> 8);
    manufacturer[3] = '\0';
}

/*static inline*/ double
edid_gamma(const struct edid * const edid)
{
    return (edid->display_transfer_characteristics + 100) / 100.0;
}

/*static inline*/ bool
edid_detailed_timing_is_monitor_descriptor(const struct edid * const edid,
                                           const uint8_t timing)
{
    const struct edid_monitor_descriptor * const mon =
        &edid->detailed_timings[timing].monitor;

    assert(timing < ARRAY_SIZE(edid->detailed_timings));

    return mon->flag0 == 0x0000 && mon->flag1 == 0x00 && mon->flag2 == 0x00;
}

/*static inline*/ struct edid_color_characteristics_data
edid_color_characteristics(const struct edid * const edid)
{
    const struct edid_color_characteristics_data characteristics = {
        .red = {
            .x = (edid->red_x << 2) | edid->red_x_low,
            .y = (edid->red_y << 2) | edid->red_y_low,
        },
        .green = {
            .x = (edid->green_x << 2) | edid->green_x_low,
            .y = (edid->green_y << 2) | edid->green_y_low,
        },
        .blue = {
            .x = (edid->blue_x << 2) | edid->blue_x_low,
            .y = (edid->blue_y << 2) | edid->blue_y_low,
        },
        .white = {
            .x = (edid->white_x << 2) | edid->white_x_low,
            .y = (edid->white_y << 2) | edid->white_y_low,
        },
    };

    return characteristics;
}

/*static inline*/ bool
edid_verify_checksum(const uint8_t * const block)
{
    uint8_t checksum = 0;
    int i;

    for (i = 0; i < EDID_BLOCK_SIZE; i++)
        checksum += block[i];

    return (checksum == 0);
}

/*static inline*/ double
edid_decode_fixed_point(uint16_t value)
{
    double result = 0.0;

    assert((~value & 0xfc00) == 0xfc00);    /* edid fraction is 10 bits */

    for (uint8_t i = 0; value && (i < 10); i++, value >>= 1)
        result = result + ((value & 0x1) * (1.0 / (1 << (10 - i))));

    return result;
}

