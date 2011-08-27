
#ifndef __SOFTROCK_H
#define __SOFTROCK_H

#include <stdbool.h>

    /* Bibliotheken */
#include <usb.h>                /* libusb 0.1 */

#include "global.h"


bool softrock_get_current_frequency (usb_dev_handle *sdr, double *freq);
bool softrock_get_set_abpf (usb_dev_handle *sdr, uint32_t index, uint32_t value);
bool softrock_have_abpf (void);
void softrock_show_abpf (void);
bool softrock_read_vco (usb_dev_handle *sdr, double *freq);
bool softrock_write_vco (usb_dev_handle *sdr, double freq);
bool softrock_read_xtal (usb_dev_handle *sdr, double *freq);
bool softrock_write_xtal (usb_dev_handle *sdr, double freq);
bool softrock_read_3rd (usb_dev_handle *sdr, double *freq);
bool softrock_write_3rd (usb_dev_handle *sdr, double freq);
bool softrock_read_5th (usb_dev_handle *sdr, double *freq);
bool softrock_write_5th (usb_dev_handle *sdr, double freq);
bool softrock_read_presel_mode (usb_dev_handle *sdr, uint32_t *mode);
bool softrock_write_presel_mode (usb_dev_handle *sdr, uint32_t mode);
bool softrock_read_presel_entry (usb_dev_handle *sdr,
                                 int index, double *freq1, double *freq2, uint32_t *pattern);
bool softrock_write_presel_entry (usb_dev_handle *sdr,
                                 int index, double freq1, double freq2, uint32_t pattern);
bool softrock_read_i2c (usb_dev_handle *sdr, uint8_t *address);
bool softrock_read_registers (usb_dev_handle *sdr, uint8_t value[6]);
bool softrock_read_virtual_registers (usb_dev_handle *sdr, uint8_t value[6]);
bool softrock_write_virtual_registers (usb_dev_handle *sdr, uint8_t value[6]);
bool softrock_read_virtual_vco_factor (usb_dev_handle *sdr, uint32_t *factor);
bool softrock_write_virtual_vco_factor (usb_dev_handle *sdr, long factor);
bool softrock_read_startup (usb_dev_handle *sdr, double *freq);
bool softrock_write_startup (usb_dev_handle *sdr, double freq);
bool softrock_read_smoothtune (usb_dev_handle *sdr, uint16_t *smoothtune);
bool softrock_write_smoothtune (usb_dev_handle *sdr, uint16_t smoothtune);
bool softrock_read_factory_default_registers (usb_dev_handle *sdr, uint8_t value[6]);
bool softrock_read_version_number (usb_dev_handle *sdr, uint32_t *svn);
bool softrock_read_version_string (usb_dev_handle *sdr, char *version, uint32_t length);
bool softrock_read_debuginfo (usb_dev_handle *sdr, uint8_t *debuginfo, uint32_t length);
bool softrock_write_volume (usb_dev_handle *sdr, int16_t volume);
bool softrock_read_demodulator_mode (usb_dev_handle *sdr, uint8_t *mode);
bool softrock_write_demodulator_mode (usb_dev_handle *sdr, uint8_t mode);
bool softrock_read_subtract_multiply (usb_dev_handle *sdr, int32_t *subtract1121, uint32_t *multiply1121);
bool softrock_write_subtract_multiply (usb_dev_handle *sdr, int32_t subtract1121, uint32_t multiply1121);
bool softrock_read_bandwidth (usb_dev_handle *sdr, uint32_t *bandwidth);
bool softrock_write_bandwidth (usb_dev_handle *sdr, uint32_t bandwidth);

#endif
