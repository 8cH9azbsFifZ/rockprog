    /* Standard-Libraries */
#include <stdio.h>
#include <stdbool.h>
#include <inttypes.h>

    /* Bibliotheken */


#include "global.h"
#include "softrock.h"

/* ABPF Info */
uint16_t abpf[256];
bool have_abpf;
uint32_t num_abpf;
bool abpf_enabled;



/* ABPF lesen/schreiben */
bool softrock_get_set_abpf (usb_dev_handle *sdr, uint32_t index, uint32_t value)
{
    int error;


    error = usb_control_msg(
        sdr,
        USB_ENDPOINT_IN | USB_TYPE_VENDOR | USB_RECIP_DEVICE,
        0x17, /* Get/Set ABPF */
        value, /* wValue */
        index, /* wIndex */
        (char *)&abpf,
        512, /* wLength */
        300 /* timeout */
        );

    have_abpf = false;
    if (error < 0)
    {
        return false;
    }
    else if (error == 0)
    {
        printf ("Kann ABPF nicht lesen (Länge = 0)\n");
        return false;
    }
    else if (error % 2)
    {
        printf ("Kann ABPF nicht lesen (Länge ungerade)\n");
    }
    have_abpf = true;

    /* Anzahl Frequenzgrenzen und aktueller ein/aus Status */
    num_abpf = (error / 2) - 1;
    abpf_enabled = (abpf[num_abpf] != 0);

    return true;
}


bool softrock_have_abpf (void)
{
    return have_abpf;
}

void softrock_show_abpf (void)
{
    int i;

    printf ("ABPF %s, %d Filtergrenzen\n",
            abpf_enabled ? "ein" : "aus",
            num_abpf);
    for (i = 0; i < num_abpf; i++)
    {
        printf ("%2d: %f MHz\n", i, (double)(abpf[i] / (4.0 * 32.0)));
    }
}


/* VCO lesen */
bool softrock_read_vco (usb_dev_handle *sdr, double *freq)
{
    int error;
    uint32_t freq1121;


    error = usb_control_msg(
        sdr,
        USB_ENDPOINT_IN | USB_TYPE_VENDOR | USB_RECIP_DEVICE,
        0x3A, /* Get current frequency */
        0, /* wValue */
        0, /* wIndex */
        (char *)&freq1121,
        4, /* wLength */
        100 /* timeout */
        );

    if (error < 0)
    {
        return false;
    }

    *freq = freq1121 / (4.0 * 2097152.0);

    return true;
}



/* VCO schreiben */
bool softrock_write_vco (usb_dev_handle *sdr, double freq)
{
    int error;
    uint32_t freq1121;


    freq1121 = _11_21(4.0 * freq);

    error = usb_control_msg(
        sdr,
        USB_ENDPOINT_OUT | USB_TYPE_VENDOR | USB_RECIP_DEVICE,
        0x32, /* Set current frequency */
        0, /* wValue */
        0, /* wIndex */
        (char *)&freq1121,
        4, /* wLength */
        100 /* timeout */
        );

    if (error < 0)
    {
        return false;
    }

    return true;
}


/* Quarzfrequenz lesen */
bool softrock_read_xtal (usb_dev_handle *sdr, double *freq)
{
    int error;
    uint32_t freq824;


    error = usb_control_msg(
        sdr,
        USB_ENDPOINT_IN | USB_TYPE_VENDOR | USB_RECIP_DEVICE,
        0x3D, /* Get current XTAL frequency */
        0, /* wValue */
        0, /* wIndex */
        (char *)&freq824,
        4, /* wLength */
        100 /* timeout */
        );

    if (error < 0)
    {
        return false;
    }

    *freq = freq824 / (256.0 * 256.0 * 256.0);

    return true;
}



/* Quarzfrequenz schreiben */
bool softrock_write_xtal (usb_dev_handle *sdr, double freq)
{
    int error;
    uint32_t freq824;


    freq824 = _8_24(freq);

    error = usb_control_msg(
        sdr,
        USB_ENDPOINT_OUT | USB_TYPE_VENDOR | USB_RECIP_DEVICE,
        0x33, /* Set XTAL frequency */
        0, /* wValue */
        0, /* wIndex */
        (char *)&freq824,
        4, /* wLength */
        100 /* timeout */
        );

    if (error < 0)
    {
        return false;
    }

    return true;
}



/* Start-Frequenz 3. Oberwelle lesen */
bool softrock_read_3rd (usb_dev_handle *sdr, double *freq)
{
    int error;
    uint32_t freq1121;


    error = usb_control_msg(
        sdr,
        USB_ENDPOINT_IN | USB_TYPE_VENDOR | USB_RECIP_DEVICE,
        0xAB, /* FiFi-SDR Extra-Befehle (lesen) */
        0, /* wValue */
        3, /* wIndex = 3 --> Frequenz 3. Oberwelle */
        (char *)&freq1121,
        4, /* wLength */
        100 /* timeout */
        );

    if (error < 0)
    {
        return false;
    }

    *freq = freq1121 / (4.0 * 2097152.0);

    return true;
}



/* Start-Frequenz 3. Oberwelle schreiben */
bool softrock_write_3rd (usb_dev_handle *sdr, double freq)
{
    int error;
    uint32_t freq1121;


    freq1121 = _11_21(4.0 * freq);

    error = usb_control_msg(
        sdr,
        USB_ENDPOINT_OUT | USB_TYPE_VENDOR | USB_RECIP_DEVICE,
        0xAC, /* FiFi-SDR Extra-Befehle (schreiben) */
        0, /* wValue */
        3, /* wIndex = 3 --> Frequenz 3. Oberwelle */
        (char *)&freq1121,
        4, /* wLength */
        100 /* timeout */
        );

    if (error < 0)
    {
        return false;
    }

    return true;
}



/* Start-Frequenz 5. Oberwelle lesen */
bool softrock_read_5th (usb_dev_handle *sdr, double *freq)
{
    int error;
    uint32_t freq1121;


    error = usb_control_msg(
        sdr,
        USB_ENDPOINT_IN | USB_TYPE_VENDOR | USB_RECIP_DEVICE,
        0xAB, /* FiFi-SDR Extra-Befehle (lesen) */
        0, /* wValue */
        5, /* wIndex = 5 --> Frequenz 5. Oberwelle */
        (char *)&freq1121,
        4, /* wLength */
        100 /* timeout */
        );

    if (error < 0)
    {
        return false;
    }

    *freq = freq1121 / (4.0 * 2097152.0);

    return true;
}



/* Start-Frequenz 5. Oberwelle schreiben */
bool softrock_write_5th (usb_dev_handle *sdr, double freq)
{
    int error;
    uint32_t freq1121;


    freq1121 = _11_21(4.0 * freq);

    error = usb_control_msg(
        sdr,
        USB_ENDPOINT_OUT | USB_TYPE_VENDOR | USB_RECIP_DEVICE,
        0xAC, /* FiFi-SDR Extra-Befehle (schreiben) */
        0, /* wValue */
        5, /* wIndex = 5 --> Frequenz 5. Oberwelle */
        (char *)&freq1121,
        4, /* wLength */
        100 /* timeout */
        );

    if (error < 0)
    {
        return false;
    }

    return true;
}



/* Modus Preselektor lesen */
bool softrock_read_presel_mode (usb_dev_handle *sdr, uint32_t *mode)
{
    int error;
    uint32_t m;


    error = usb_control_msg(
        sdr,
        USB_ENDPOINT_IN | USB_TYPE_VENDOR | USB_RECIP_DEVICE,
        0xAB, /* FiFi-SDR Extra-Befehle (lesen) */
        0, /* wValue */
        6, /* wIndex = 6 --> Preselektor-Modus */
        (char *)&m,
        4, /* wLength */
        100 /* timeout */
        );

    if (error < 0)
    {
        return false;
    }

    *mode = m;

    return true;
}



/* Modus Preselektor schreiben */
bool softrock_write_presel_mode (usb_dev_handle *sdr, uint32_t mode)
{
    int error;


    error = usb_control_msg(
        sdr,
        USB_ENDPOINT_OUT | USB_TYPE_VENDOR | USB_RECIP_DEVICE,
        0xAC, /* FiFi-SDR Extra-Befehle (schreiben) */
        0, /* wValue */
        6, /* wIndex = 6 --> Preselektor-Modus */
        (char *)&mode,
        4, /* wLength */
        100 /* timeout */
        );

    if (error < 0)
    {
        return false;
    }

    return true;
}



/* Funktions-Eintrag Preselektor lesen */
bool softrock_read_presel_entry (usb_dev_handle *sdr,
                                 int index, double *freq1, double *freq2, uint32_t *pattern)
{
    int error;
    uint8_t response[9];


    error = usb_control_msg(
        sdr,
        USB_ENDPOINT_IN | USB_TYPE_VENDOR | USB_RECIP_DEVICE,
        0xAB, /* FiFi-SDR Extra-Befehle (lesen) */
        index, /* wValue */
        7, /* wIndex = 7 --> Preselektor-Eintrag */
        (char *)&response,
        9, /* wLength */
        100 /* timeout */
        );

    if (error < 0)
    {
        return false;
    }

    *freq1 = *((uint32_t *)&response[0]) / (4.0 * 32.0 * 256.0 * 256.0);
    *freq2 = *((uint32_t *)&response[4]) / (4.0 * 32.0 * 256.0 * 256.0);
    *pattern = response[8];

    return true;
}



/* Funktions-Eintrag Preselektor schreiben */
bool softrock_write_presel_entry (usb_dev_handle *sdr,
                                  int index, double freq1, double freq2, uint32_t pattern)
{
    int error;
    uint8_t message[9];


    *((uint32_t *)&message[0]) = _11_21(4.0 * freq1);
    *((uint32_t *)&message[4]) = _11_21(4.0 * freq2);
    message[8] = pattern;

    error = usb_control_msg(
        sdr,
        USB_ENDPOINT_OUT | USB_TYPE_VENDOR | USB_RECIP_DEVICE,
        0xAC, /* FiFi-SDR Extra-Befehle (schreiben) */
        index, /* wValue */
        7, /* wIndex = 7 --> Preselektor-Eintrag */
        (char *)&message,
        9, /* wLength */
        100 /* timeout */
        );

    if (error < 0)
    {
        return false;
    }

    return true;
}


/* I2C-Adresse lesen */
bool softrock_read_i2c (usb_dev_handle *sdr, uint8_t *address)
{
    int error;
    uint8_t addr;


    error = usb_control_msg(
        sdr,
        USB_ENDPOINT_IN | USB_TYPE_VENDOR | USB_RECIP_DEVICE,
        0x41, /* Get/Set I2C address */
        0, /* wValue = 0 --> Nur lesen! */
        0, /* wIndex */
        (char *)&addr,
        1, /* wLength */
        100 /* timeout */
        );

    if (error < 0)
    {
        return false;
    }

    *address = addr;

    return true;
}



/* Si570-Register lesen */
bool softrock_read_registers (usb_dev_handle *sdr, uint8_t value[6])
{
    int error;


    error = usb_control_msg(
        sdr,
        USB_ENDPOINT_IN | USB_TYPE_VENDOR | USB_RECIP_DEVICE,
        0xAB, /* FiFi-SDR Extra-Befehle (lesen) */
        0, /* wValue */
        10, /* wIndex = 10 --> Virtuelle Register */
        (char *)&value[0],
        6, /* wLength */
        100 /* timeout */
        );

    if (error < 0)
    {
        return false;
    }

    return true;
}


/* Virtuelle Si570-Register lesen */
bool softrock_read_virtual_registers (usb_dev_handle *sdr, uint8_t value[6])
{
    int error;


    error = usb_control_msg(
        sdr,
        USB_ENDPOINT_IN | USB_TYPE_VENDOR | USB_RECIP_DEVICE,
        0x3F, /* Read Si570 registers */
        0, /* wValue = 0 --> Nur lesen! */
        0, /* wIndex */
        (char *)&value[0],
        6, /* wLength */
        100 /* timeout */
        );

    if (error < 0)
    {
        return false;
    }

    return true;
}


/* Virtuelle Si570-Register schreiben */
bool softrock_write_virtual_registers (usb_dev_handle *sdr, uint8_t value[6])
{
    int error;


    error = usb_control_msg(
        sdr,
        USB_ENDPOINT_OUT | USB_TYPE_VENDOR | USB_RECIP_DEVICE,
        0x30, /* Set frequency by registers */
        0, /* wValue */
        0, /* wIndex */
        (char *)&value[0],
        6, /* wLength */
        100 /* timeout */
        );

    if (error < 0)
    {
        return false;
    }

    return true;
}



/* Faktor für vituellen VCO lesen */
bool softrock_read_virtual_vco_factor (usb_dev_handle *sdr, uint32_t *factor)
{
    int error;


    error = usb_control_msg(
        sdr,
        USB_ENDPOINT_IN | USB_TYPE_VENDOR | USB_RECIP_DEVICE,
        0xAB, /* FiFi-SDR Extra-Befehle (lesen) */
        0, /* wValue */
        11, /* wIndex = 11 --> Faktor für virtuellen VCO */
        (char *)factor,
        4, /* wLength */
        100 /* timeout */
        );

    if (error < 0)
    {
        return false;
    }

    return true;
}



/* Faktor für vituellen VCO schreiben */
bool softrock_write_virtual_vco_factor (usb_dev_handle *sdr, long factor)
{
    int error;


    error = usb_control_msg(
        sdr,
        USB_ENDPOINT_OUT | USB_TYPE_VENDOR | USB_RECIP_DEVICE,
        0xAC, /* FiFi-SDR Extra-Befehle (schreiben) */
        0, /* wValue */
        11, /* wIndex = 11 --> Faktor für virtuellen VCO */
        (char *)&factor,
        4, /* wLength */
        100 /* timeout */
        );

    if (error < 0)
    {
        return false;
    }

    return true;
}


/* Startup-Frequenz schreiben */
bool softrock_write_startup (usb_dev_handle *sdr, double freq)
{
    int error;
    uint32_t freq1121;


    freq1121 = _11_21(4.0 * freq);

    error = usb_control_msg(
        sdr,
        USB_ENDPOINT_OUT | USB_TYPE_VENDOR | USB_RECIP_DEVICE,
        0x34, /* Set startup frequency */
        0, /* wValue */
        0, /* wIndex */
        (char *)&freq1121,
        4, /* wLength */
        100 /* timeout */
        );

    if (error < 0)
    {
        return false;
    }

    return true;
}


/* Startup-Frequenz lesen */
bool softrock_read_startup (usb_dev_handle *sdr, double *freq)
{
    int error;
    uint32_t freq1121;


    error = usb_control_msg(
        sdr,
        USB_ENDPOINT_IN | USB_TYPE_VENDOR | USB_RECIP_DEVICE,
        0x3C, /* Get startup frequency */
        0, /* wValue */
        0, /* wIndex */
        (char *)&freq1121,
        4, /* wLength */
        100 /* timeout */
        );

    if (error < 0)
    {
        return false;
    }

    *freq = freq1121 / (4.0 * 2097152.0);

    return true;
}



/* "Smooth-Tune" lesen */
bool softrock_read_smoothtune (usb_dev_handle *sdr, uint16_t *smoothtune)
{
    int error;


    error = usb_control_msg(
        sdr,
        USB_ENDPOINT_IN | USB_TYPE_VENDOR | USB_RECIP_DEVICE,
        0x3B, /* Get smooth tune */
        0, /* wValue */
        0, /* wIndex */
        (char *)smoothtune,
        2, /* wLength */
        100 /* timeout */
        );

    if (error < 0)
    {
        return false;
    }

    return true;
}



/* "Smooth-Tune" schreiben */
bool softrock_write_smoothtune (usb_dev_handle *sdr, uint16_t smoothtune)
{
    int error;


    error = usb_control_msg(
        sdr,
        USB_ENDPOINT_OUT | USB_TYPE_VENDOR | USB_RECIP_DEVICE,
        0x35, /* Set startup frequency */
        0, /* wValue */
        0, /* wIndex */
        (char *)&smoothtune,
        2, /* wLength */
        100 /* timeout */
        );

    if (error < 0)
    {
        return false;
    }

    return true;
}


/* Factory Default Si570-Register lesen */
bool softrock_read_factory_default_registers (usb_dev_handle *sdr, uint8_t value[6])
{
    int error;


    error = usb_control_msg(
        sdr,
        USB_ENDPOINT_IN | USB_TYPE_VENDOR | USB_RECIP_DEVICE,
        0xAB, /* FiFi-SDR Extra-Befehle (lesen) */
        0, /* wValue */
        12, /* wIndex = 12 --> Factory Defaults */
        (char *)&value[0],
        6, /* wLength */
        100 /* timeout */
        );

    if (error < 0)
    {
        return false;
    }

    return true;
}



/* Versions-Nummer auslesen */
bool softrock_read_version_number (usb_dev_handle *sdr, uint32_t *svn)
{
    int error;
	uint32_t nummer;


    error = usb_control_msg(
        sdr,
        USB_ENDPOINT_IN | USB_TYPE_VENDOR | USB_RECIP_DEVICE,
        0xAB, /* FiFi-SDR Extra-Befehle (lesen) */
        0, /* wValue */
        0, /* wIndex = 0 --> Versions-Nummer */
        (char *)&nummer,
        4, /* wLength */
        100 /* timeout */
        );

    if (error < 0)
    {
        return false;
    }

	*svn = nummer;

    return true;
}



/* Versions-String auslesen */
bool softrock_read_version_string (usb_dev_handle *sdr, char *version, uint32_t length)
{
    int error;


    error = usb_control_msg(
        sdr,
        USB_ENDPOINT_IN | USB_TYPE_VENDOR | USB_RECIP_DEVICE,
        0xAB, /* FiFi-SDR Extra-Befehle (lesen) */
        0, /* wValue */
        1, /* wIndex = 1 --> Versions-String */
        (char *)version,
        length, /* wLength */
        100 /* timeout */
        );

    if (error < 0)
    {
        return false;
    }

    return true;
}


/* Debug-Info auslesen */
bool softrock_read_debuginfo (usb_dev_handle *sdr, uint8_t *debuginfo, uint32_t length)
{
    int error;


    error = usb_control_msg(
        sdr,
        USB_ENDPOINT_IN | USB_TYPE_VENDOR | USB_RECIP_DEVICE,
        0xAB, /* FiFi-SDR Extra-Befehle (lesen) */
        0, /* wValue */
        0xFFFF, /* wIndex = FFFF --> Debug-Info */
        (char *)debuginfo,
        length, /* wLength */
        100 /* timeout */
        );

    if (error < 0)
    {
        return false;
    }

    return true;
}



/* Audio Volume schreiben */
bool softrock_write_volume (usb_dev_handle *sdr, int16_t volume)
{
    int error;


    error = usb_control_msg(
        sdr,
        USB_ENDPOINT_OUT | USB_TYPE_VENDOR | USB_RECIP_DEVICE,
        0xAC, /* FiFi-SDR Extra-Befehle (schreiben) */
        0, /* wValue */
        13, /* wIndex = 13 --> Volume schreiben */
        (char *)&volume,
        2, /* wLength */
        100 /* timeout */
        );

    if (error < 0)
    {
        return false;
    }

    return true;
}



/* Debug-Info auslesen */
bool softrock_read_demodulator_mode (usb_dev_handle *sdr, uint8_t *mode)
{
    int error;


    error = usb_control_msg(
        sdr,
        USB_ENDPOINT_IN | USB_TYPE_VENDOR | USB_RECIP_DEVICE,
        0xAB, /* FiFi-SDR Extra-Befehle (lesen) */
        0, /* wValue */
        15, /* wIndex = 15 --> Mode */
        (char *)mode,
        1, /* wLength */
        100 /* timeout */
        );

    if (error < 0)
    {
        return false;
    }

    return true;
}



/* Betriebsart des Demodulators setzen */
bool softrock_write_demodulator_mode (usb_dev_handle *sdr, uint8_t mode)
{
    int error;


    error = usb_control_msg(
        sdr,
        USB_ENDPOINT_OUT | USB_TYPE_VENDOR | USB_RECIP_DEVICE,
        0xAC, /* FiFi-SDR Extra-Befehle (schreiben) */
        0, /* wValue */
        15, /* wIndex = 15 --> Mode */
        (char *)&mode,
        1, /* wLength */
        100 /* timeout */
        );

    if (error < 0)
    {
        return false;
    }

    return true;
}



/* Subtract/Multiply für Frequenzeinstellung lesen */
bool softrock_read_subtract_multiply (usb_dev_handle *sdr, int32_t *subtract1121, uint32_t *multiply1121)
{
    int error;
	uint32_t buffer[2];


    error = usb_control_msg(
        sdr,
        USB_ENDPOINT_IN | USB_TYPE_VENDOR | USB_RECIP_DEVICE,
        0x39, /* Get subtract/multiply */
        0, /* wValue */
        0, /* wIndex = Band (hier nicht unterstützt!) */
        (char *)buffer,
        8, /* wLength */
        100 /* timeout */
        );

    if (error < 0)
    {
        return false;
    }

	*subtract1121 = buffer[0];
	*multiply1121 = buffer[1];

    return true;
}



/* "Smooth-Tune" schreiben */
bool softrock_write_subtract_multiply (usb_dev_handle *sdr, int32_t subtract1121, uint32_t multiply1121)
{
    int error;
	uint32_t buffer[2];


	buffer[0] = (uint32_t)subtract1121;
	buffer[1] = multiply1121;

    error = usb_control_msg(
        sdr,
        USB_ENDPOINT_OUT | USB_TYPE_VENDOR | USB_RECIP_DEVICE,
        0x31, /* Write subtract/multiply */
        0, /* wValue */
        0, /* wIndex */
        (char *)buffer,
        8, /* wLength */
        100 /* timeout */
        );

    if (error < 0)
    {
        return false;
    }

    return true;
}



/* Demodulator-Bandbreite lesen */
bool softrock_read_bandwidth (usb_dev_handle *sdr, uint32_t *bandwidth)
{
    int error;


    error = usb_control_msg(
        sdr,
        USB_ENDPOINT_IN | USB_TYPE_VENDOR | USB_RECIP_DEVICE,
        0xAB, /* FiFi-SDR Extra-Befehle (lesen) */
        0, /* wValue */
        16, /* wIndex = 16 --> Bandbreite */
        (char *)bandwidth,
        4, /* wLength */
        100 /* timeout */
        );

    if (error < 0)
    {
        return false;
    }

    return true;
}



/* Demodulator-Bandbreite schreiben */
bool softrock_write_bandwidth (usb_dev_handle *sdr, uint32_t bandwidth)
{
    int error;


    error = usb_control_msg(
        sdr,
        USB_ENDPOINT_OUT | USB_TYPE_VENDOR | USB_RECIP_DEVICE,
        0xAC, /* FiFi-SDR Extra-Befehle (schreiben) */
        0, /* wValue */
        16, /* wIndex = 16 --> Bandbreite */
        (char *)&bandwidth,
        4, /* wLength */
        100 /* timeout */
        );

    if (error < 0)
    {
        return false;
    }

    return true;
}

