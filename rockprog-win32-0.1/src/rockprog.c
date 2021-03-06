/*
 * main.c
 */

#if __linux__
#define _XOPEN_SOURCE 500
#endif

    /* Standard-Libraries */
#include <stdbool.h>
#include <inttypes.h>
#include <unistd.h>
#include <string.h>
#include <math.h>

#include "global.h"

    /* Bibliotheken */
#include <usb.h>
#include <popt.h>                       /* Parsen von Kommandozeilen-Optionen */

#include "softrock.h"


struct usb_bus *busses;
usb_dev_handle *fifisdr;


/* Variablen fuer Argumente aus der Kommandozeile */
int cmdline_write = false;
long cmdline_vid = 0x16C0;
long cmdline_pid = 0x05DC;
int cmdline_abpf = false;
long cmdline_index = -1;
double cmdline_freq = -999.0;
int cmdline_vco = false;
int cmdline_xtal = false;
int cmdline_3rd = false;
int cmdline_5th = false;
int cmdline_presel = false;
int cmdline_i2c = false;
long cmdline_pattern = -1;
long cmdline_mode = -1;
int cmdline_regs = false;
int cmdline_vregs = false;
char *cmdline_regset = "";
double cmdline_freq_from = -999.0;
double cmdline_freq_to = -999.0;
int cmdline_virtual_vco_factor = false;
long cmdline_factor = -1;
int cmdline_startup = false;
int cmdline_version = false;
#if __linux__
const char rockprog_version[] = "263 05.01.2011";   // Datum als Versionsnummer
#else
const char rockprog_version[] = "263/270 05.01.2011";   // Datum als Versionsnummer
#endif
int cmdline_smoothtune = false;
long cmdline_ppm = -1;
int cmdline_autotune = false;
int cmdline_firmware = false;
int cmdline_debuginfo = false;
long cmdline_volume = -1;
int cmdline_demod = false;
char *cmdline_demodmode = "";
double cmdline_subtract = -999.0;
double cmdline_multiply = -999.0;
int cmdline_offset = false;
int cmdline_bandwidth = false;




/* FiFi-SDR suchen und öffnen */
bool such_fifi (void)
{
    struct usb_bus *bus;
    struct usb_device *device;
    bool found;


    /* Ist ein passendes Geraet dabei? */
    found = false;
    for (bus = busses; bus; bus = bus->next) {
        /* Geräteinfo holen */
        for (device = bus->devices; device; device = device->next) {
            /* Pruefen ob dies das passende Geraet ist */
            if ((cmdline_vid == device->descriptor.idVendor) &&
                (cmdline_pid == device->descriptor.idProduct))
            {
                found = true;
                break;
            }
        }
    }
        
    /* Kann es los gehen? */
    if (found)
    {
        fifisdr = usb_open (device);
        if (fifisdr == 0) {
            printf("usb_open() fehlgeschlagen\n");
            found = false;
        }
        else {
            if (usb_set_configuration(fifisdr, 1) < 0) {
                printf("usb_set_configuration(1) fehlgeschlagen\n");
                found = false;
            }
            else {
                if (usb_claim_interface(fifisdr, 0) < 0) {
                    printf("usb_claim_interface(0) fehlgeschlagen\n");
                    found = false;
                }
            }
        }
    }

    return found;
}




#include <stdio.h>
int main (int argc, char *argv[])
{
    bool ok;
    int i;


    /* Kommandozeilen-Parameter */
    struct poptOption optionsTable[] =
    {
        { "write", 'w', POPT_ARG_NONE, &cmdline_write, 0,
                "Werte schreiben" },
        { "vid", '\0', POPT_ARG_LONG, &cmdline_vid, 0,
                "USB-PID des FiFi-SDR (Vorgabe: 0x16C0)" },
        { "pid", '\0', POPT_ARG_LONG, &cmdline_pid, 0,
                "USB-VID des FiFi-SDR (Vorgabe: 0x05DC)" },
        { "abpf", '\0', POPT_ARG_NONE, &cmdline_abpf, 0,
                "Zugriff auf automatisches Bandpaßfilter" },
        { "index", '\0', POPT_ARG_LONG, &cmdline_index, 0,
                "Index fuer z.B. ABPF" },
        { "freq", '\0', POPT_ARG_DOUBLE, &cmdline_freq, 0,
                "Frequenzangabe [MHz]" },
        { "vco", '\0', POPT_ARG_NONE, &cmdline_vco, 0,
                "VCO-Frequenz" },
        { "xtal", '\0', POPT_ARG_NONE, &cmdline_xtal, 0,
                "Quarz-Frequenz" },
        { "3rd", '\0', POPT_ARG_NONE, &cmdline_3rd, 0,
                "Start fuer RX auf 3. Oberwelle" },
        { "5th", '\0', POPT_ARG_NONE, &cmdline_5th, 0,
                "Start fuer RX auf 5. Oberwelle" },
        { "presel", '\0', POPT_ARG_NONE, &cmdline_presel, 0,
                "Preselektor" },
        { "pattern", '\0', POPT_ARG_LONG, &cmdline_pattern, 0,
                "Pattern fuer z.B. Preselektor" },
        { "mode", '\0', POPT_ARG_LONG, &cmdline_mode, 0,
                "Modus fuer Preselektor" },
        { "freq-from", '\0', POPT_ARG_DOUBLE, &cmdline_freq_from, 0,
                "Startfrequenz [MHz]" },
        { "freq-to", '\0', POPT_ARG_DOUBLE, &cmdline_freq_to, 0,
                "Endfrequenz [MHz]" },
        { "i2c", '\0', POPT_ARG_NONE, &cmdline_i2c, 0,
                "I2C-Adresse lesen" },
        { "regs", '\0', POPT_ARG_NONE, &cmdline_regs, 0,
                "Si570-Register" },
        { "vregs", '\0', POPT_ARG_NONE, &cmdline_vregs, 0,
                "Virtuelle Si570-Register" },
        { "regset", '\0', POPT_ARG_STRING, &cmdline_regset, 0,
                "Register-Werte fuer Si570 (6 Hex-Werte mit Komma getrennt: 0xAB,0xCD,...)" },
        { "vfact", '\0', POPT_ARG_NONE, &cmdline_virtual_vco_factor, 0,
                "Faktor fuer virtuellen VCO" },
        { "factor", '\0', POPT_ARG_LONG, &cmdline_factor, 0,
                "Faktor (zusammen mit --vfact)" },
        { "startup", '\0', POPT_ARG_NONE, &cmdline_startup, 0,
                "Startup-Frequenz" },
        { "version", '\0', POPT_ARG_NONE, &cmdline_version, 0,
#if __linux__
                "rockprog Version abfragen (SVN Rev)" },
#else
                "rockprog Version abfragen (SVN Rev. Linux Basisversion / SVN Rev. Win32 Port)" },
#endif
        { "smooth", '\0', POPT_ARG_NONE, &cmdline_smoothtune, 0,
                "Smooth Tune" },
        { "ppm", '\0', POPT_ARG_LONG, &cmdline_ppm, 0,
                "ppm-Wert (für --smooth)" },
        { "autotune", '\0', POPT_ARG_NONE, &cmdline_autotune, 0,
                "Quarzfrequenz automatisch abgleichen" },
        { "firmware", '\0', POPT_ARG_NONE, &cmdline_firmware, 0,
                "Version der Firmware auslesen" },
        { "debuginfo", '\0', POPT_ARG_NONE, &cmdline_debuginfo, 0,
                "Einige Debug-Werte auslesen" },
        { "volume", '\0', POPT_ARG_LONG, &cmdline_volume, 0,
                "Volume (0 oder 1)" },
        { "demod", '\0', POPT_ARG_NONE, &cmdline_demod, 0,
                "Demodulator" },
        { "demodmode", '\0', POPT_ARG_STRING, &cmdline_demodmode, 0,
                "Demodulator-Modus (LSB, USB, AM)" },
        { "offset", '\0', POPT_ARG_NONE, &cmdline_offset, 0,
                "Offset (mit --subtract und --multiply)" },
        { "subtract", '\0', POPT_ARG_DOUBLE, &cmdline_subtract, 0,
                "fuer --offset" },
        { "multiply", '\0', POPT_ARG_DOUBLE, &cmdline_multiply, 0,
                "fuer --offset" },
        { "bandwidth", '\0', POPT_ARG_NONE, &cmdline_bandwidth, 0,
                "Bandbreite Demodulator-Filter (mit --freq)" },
        POPT_AUTOHELP
        { NULL, POPT_ARG_NONE, 0, NULL, 0 }
    };

    /* Kommandozeile parsen */
    poptContext optCon;
    optCon = poptGetContext (NULL, argc, (const char **)argv, optionsTable, 0);
    poptSetOtherOptionHelp( optCon, " [options]" );

    /* Kurz-Hilfe, wenn gar keine Argumente angegeben werden. */
    if (argc < 2)
    {
        poptPrintUsage( optCon, stderr, 0 );
        return 1;
    }

    /* Optionskürzel abfragen */
    char c;
    while ((c = poptGetNextOpt(optCon)) >= 0)
    {
	}

    /* Prüfen, ob ein Fehler bei der Bearbeitung der Optionen aufgetreten ist. */
    if( c < -1 )
    {
        printf ("\"%s\" %s\n", poptBadOption( optCon, POPT_BADOPTION_NOALIAS ), poptStrerror(c) );
        return 1;
    }

    poptFreeContext( optCon );

    if (cmdline_version)
    {
        printf( "%s",rockprog_version);
    }

    /* libusb initialisieren */
    usb_init();
    usb_find_busses();
    usb_find_devices();
    busses = usb_get_busses();

    /* FiFi-SDR suchen und öffnen */
    if (such_fifi())
    {
        ok = true;

        /* ABPF? */
        if (cmdline_abpf)
        {
            /* Beim Schreiben Werte prüfen */
            if (cmdline_write)
            {
                if ((cmdline_index < 0) || (cmdline_freq < 0.0))
                {
                    ok = false;
                }
            }
            else
            {
                /* Index und Frequenz haben feste Werte */
                cmdline_index = 255;
                cmdline_freq = 0.0;
            }

            if (ok)
            {
                if (softrock_get_set_abpf (fifisdr, cmdline_index, _11_5(4.0 * cmdline_freq)))
                {
                    /* Aktuelle Werte zeigen */
                    softrock_show_abpf();
                }
            }
        }

        /* VCO-Frequenz */
        if (cmdline_vco)
        {
            /* Beim Schreiben Werte prüfen */
            if (cmdline_write)
            {
                if (cmdline_freq < 0.0)
                {
                    printf ("Keine Frequenz angegeben\n");
                }
                else
                {
                    softrock_write_vco (fifisdr, cmdline_freq);
                }
            }
            else
            {
                double f;
                if (softrock_read_vco (fifisdr, &f))
                {
                    printf ("VCO = %f MHz\n", f);
                }
            }
        }

        /* Quarz-Frequenz */
        if (cmdline_xtal)
        {
            /* Beim Schreiben Werte prüfen */
            if (cmdline_write)
            {
                if (cmdline_freq < 0.0)
                {
                    printf ("Keine Frequenz angegeben\n");
                }
                else
                {
                    softrock_write_xtal (fifisdr, cmdline_freq);
                }
            }
            else
            {
                double f;
                if (softrock_read_xtal (fifisdr, &f))
                {
                    printf ("XTAL = %f MHz\n", f);
                }
            }
        }


        /* Start-Frequenz 3. Oberwelle */
        if (cmdline_3rd)
        {
            /* Beim Schreiben Werte prüfen */
            if (cmdline_write)
            {
                if (cmdline_freq < 0.0)
                {
                    printf ("Keine Frequenz angegeben\n");
                }
                else
                {
                    softrock_write_3rd (fifisdr, cmdline_freq);
                }
            }
            else
            {
                double f;
                if (softrock_read_3rd (fifisdr, &f))
                {
                    printf ("3. Oberwelle ab = %f MHz\n", f);
                }
            }
        }

        /* Start-Frequenz 5. Oberwelle */
        if (cmdline_5th)
        {
            /* Beim Schreiben Werte prüfen */
            if (cmdline_write)
            {
                if (cmdline_freq < 0.0)
                {
                    printf ("Keine Frequenz angegeben\n");
                }
                else
                {
                    softrock_write_5th (fifisdr, cmdline_freq);
                }
            }
            else
            {
                double f;
                if (softrock_read_5th (fifisdr, &f))
                {
                    printf ("5. Oberwelle ab = %f MHz\n", f);
                }
            }
        }

        /* Preselektor */
        if (cmdline_presel)
        {
            /* Beim Schreiben Werte prüfen */
            if (cmdline_write)
            {
                if (cmdline_mode != -1)
                {
                    if ((cmdline_mode >= 0) && (cmdline_mode <= 3))
                    {
                        softrock_write_presel_mode (fifisdr, cmdline_mode);
                    }
                    else
                    {
                        printf ("--mode nicht in [0,3]\n");
                    }
                }
                else
                {
                    if ((cmdline_index == -1) ||
                        (cmdline_freq_from < 0.0) ||
                        (cmdline_freq_to < 0.0) ||
                        (cmdline_pattern == -1))
                    {
                        printf ("--freq-from/--freq-to/--pattern prüfen\n");
                    }
                    else
                    {
                        softrock_write_presel_entry (fifisdr,
                                                     cmdline_index,
                                                     cmdline_freq_from,
                                                     cmdline_freq_to,
                                                     cmdline_pattern);
                    }
                }
            }
            else
            {
                uint32_t presel_mode;
                if (softrock_read_presel_mode (fifisdr, &presel_mode))
                {
                    printf ("Preselektor-Modus = %d%s\n",
                            presel_mode,
                            (presel_mode == 0) ? " (Es gelten die Umschaltpunkte für ABPF)" : "");
                }

                uint32_t pattern;
                double f1, f2;
                for (i = 0; i < 16; i++)
                {
                    if (softrock_read_presel_entry (fifisdr, i, &f1, &f2, &pattern))
                    {
                        printf ("%2d: %10f - %10f MHz, Ausgaenge = %c%c%c%cb (%d)\n",
                                      i, f1, f2,
                                      pattern & 8 ? '1' : '0',
                                      pattern & 4 ? '1' : '0',
                                      pattern & 2 ? '1' : '0',
                                      pattern & 1 ? '1' : '0',
                                      pattern & 0x0F
                                      );
                    }
                }
            }
        }

        /* I2C-Adresse */
        if (cmdline_i2c)
        {
            /* Schreiben nicht unterstützt */
            if (cmdline_write)
            {
                printf ("Schreiben der I2C-Adresse nicht möglich (wird automatisch bestimmt).\n");
            }
            else
            {
                uint8_t addr;
                if (softrock_read_i2c (fifisdr, &addr))
                {
                    printf ("I2C-Adresse = 0x%02X\n", (int)addr);
                }
            }
        }

        /* Si570-Register lesen/schreiben */
        if (cmdline_regs || cmdline_vregs)
        {
            uint8_t regs[6];

            /* Beim Schreiben Werte prüfen */
            if (cmdline_write)
            {
                int scanargs[6];

                /* Register-Werte korrekt angegeben? */
                if (6 != sscanf(cmdline_regset, "%x,%x,%x,%x,%x,%x",
                    &scanargs[0], &scanargs[1], &scanargs[2], &scanargs[3], &scanargs[4], &scanargs[5]))
                {
                    printf ("--regset braucht 6 Register (hex, getrennt durch Komma)\n");
                }
                else
                {
                    regs[0] = scanargs[0];
                    regs[1] = scanargs[1];
                    regs[2] = scanargs[2];
                    regs[3] = scanargs[3];
                    regs[4] = scanargs[4];
                    regs[5] = scanargs[5];

                    softrock_write_virtual_registers (fifisdr, regs);
                }
            }
            else
            {
                bool result = false;;

                if (cmdline_regs)
                {
                    result = softrock_read_registers (fifisdr, regs);
                }
                if (cmdline_vregs)
                {
                    result = softrock_read_virtual_registers (fifisdr, regs);
                }
                if (result)
                {
                    /* Felder HS_DIV, N1 und RFREQ rausziehen */
                    uint32_t hs_div;
                    uint32_t n1;
                    uint32_t rfreq_int;
                    uint32_t rfreq_frac;
                    double rfreq;

                    /* HS_DIV im Register */
                    hs_div = (regs[0] >> 5) & 7;
                    /* Effektiver Wert */
                    hs_div += 4;

                    /* N1 im Register */
                    n1 = ((regs[0] & 0x1F) << 2) | ((regs[1] >> 6) & 3);
                    /* Effektiver Wert */
                    n1 += 1;

                    rfreq_int = regs[1] & 0x3F;
                    rfreq_int = (rfreq_int << 4) | ((regs[2] >> 4) & 0x0F);
                    rfreq_frac = regs[2] & 0x0F;
                    rfreq_frac = (rfreq_frac << 8) | regs[3];
                    rfreq_frac = (rfreq_frac << 8) | regs[4];
                    rfreq_frac = (rfreq_frac << 8) | regs[5];
                    rfreq = (double)rfreq_int + (double)rfreq_frac / pow(2,28);

                    /* Mit Quarzfrequenz kann noch die echte RX-Frequenz bestimmt werden.
                     * Auch der virtuelle VCO-Faktor wird ggf. benötigt.
                     */
                    double rxfreq = 0.0;
                    double xtal;
                    uint32_t factor;
                    if (softrock_read_xtal (fifisdr, &xtal))
                    {
                        if (cmdline_vregs)
                        {
                            if (softrock_read_virtual_vco_factor (fifisdr, &factor))
                            {
                                rxfreq = ((rfreq * xtal) / (hs_div * n1)) / factor;
                            }
                        }
                        else
                        {
                            rxfreq = ((rfreq * xtal) / (hs_div * n1)) / 4.0;
                        }
                    }

                    printf ("Si570-Register 7-12: 0x%02X 0x%02X 0x%02X 0x%02X 0x%02X 0x%02X"
                            ", HS_DIV=%d, N1=%d, RFREQ=%0.9f (%f MHz)%s\n",
                             regs[0], regs[1], regs[2], regs[3], regs[4], regs[5],
                             hs_div, n1, rfreq, rxfreq,
                             cmdline_vregs ? " (virtuell)" : "");
                }
            }
        }

        /* Faktor für virtuelle Register */
        if (cmdline_virtual_vco_factor)
        {
            /* Beim Schreiben Werte prüfen */
            if (cmdline_write)
            {
                if (cmdline_factor < 0)
                {
                    printf ("Kein Faktor angegeben (--factor)\n");
                }
                else
                {
                    softrock_write_virtual_vco_factor (fifisdr, cmdline_factor);
                }
            }
            else
            {
                uint32_t factor;
                if (softrock_read_virtual_vco_factor (fifisdr, &factor))
                {
                    printf ("Faktor fuer virtuellen VCO = %" PRIu32 "\n", factor);
                }
            }
        }

        /* Startup-Frequenz */
        if (cmdline_startup)
        {
            /* Beim Schreiben Werte prüfen */
            if (cmdline_write)
            {
                if (cmdline_freq < 0.0)
                {
                    printf ("Keine Frequenz angegeben\n");
                }
                else
                {
                    softrock_write_startup (fifisdr, cmdline_freq);
                }
            }
            else
            {
                double f;
                if (softrock_read_startup (fifisdr, &f))
                {
                    printf ("Startup-Frequenz = %f MHz\n", f);
                }
            }
        }

        /* Smooth Tune */
        if (cmdline_smoothtune)
        {
            uint16_t ppm;

            /* Beim Schreiben Werte pruefen */
            if (cmdline_write)
            {
                if (cmdline_ppm < 0)
                {
                    printf ("Kein ppm-Wert angegeben\n");
                }
                else
                {
                    ppm = cmdline_ppm;
                    softrock_write_smoothtune (fifisdr, ppm);
                }
            }
            else
            {
                if (softrock_read_smoothtune (fifisdr, &ppm))
                {
                    printf ("Smooth Tune = %d ppm\n", (int)ppm);
                }
            }
        }

        /* Autotune für Quarzfrequenz */
        if (cmdline_autotune) {
            uint8_t regs[6];
            bool result = false;;

            /* Factory defaults lesen */
            result = softrock_read_factory_default_registers (fifisdr, regs);

            if (result) {
                /* Felder HS_DIV, N1 und RFREQ rausziehen */
                uint32_t hs_div;
                uint32_t n1;
                uint32_t rfreq_int;
                uint32_t rfreq_frac;
                double rfreq;

                /* HS_DIV im Register */
                hs_div = (regs[0] >> 5) & 7;
                /* Effektiver Wert */
                hs_div += 4;

                /* N1 im Register */
                n1 = ((regs[0] & 0x1F) << 2) | ((regs[1] >> 6) & 3);
                /* Effektiver Wert */
                n1 += 1;

                rfreq_int = regs[1] & 0x3F;
                rfreq_int = (rfreq_int << 4) | ((regs[2] >> 4) & 0x0F);
                rfreq_frac = regs[2] & 0x0F;
                rfreq_frac = (rfreq_frac << 8) | regs[3];
                rfreq_frac = (rfreq_frac << 8) | regs[4];
                rfreq_frac = (rfreq_frac << 8) | regs[5];
                rfreq = (double)rfreq_int + (double)rfreq_frac / pow(2,28);

                /* Mit Quarzfrequenz kann noch die echte RX-Frequenz bestimmt werden. */
                double rxfreq = 0.0;
                double xtal = 114.285;  /* Mit nomineller Quarzfrequenz rechnen! */
                rxfreq = ((rfreq * xtal) / (hs_div * n1)) / 4.0;

                /* Annahme: Startup-Frequenz ist im Raster 5 kHz.
                 * Abweichung von rxfreq zu diesem Raster bestimmen. Daraus Korrekturfaktor fuer XTAL ableiten.
                 * (Per Kommandozeile kann geratene Sollfrequenz ueberschrieben werden).
                 */
                double rasterfreq = (double)((uint32_t)((rxfreq + 0.0025) / 0.005)) * 0.005;
                if (cmdline_freq >= 0.0) {
                    rasterfreq = cmdline_freq;
                }
                double newxtal = xtal * (rasterfreq / rxfreq);

                if (cmdline_write) {
                    softrock_write_xtal (fifisdr, newxtal);
                }
                else {
                    printf ("Factory-Startup: %f MHz, Vorschlag: XTAL=%f\n",
                             rxfreq, newxtal);
                }
            }
        }

        /* Version-String auslesen */
        if (cmdline_firmware) {
            const uint32_t length = 255;
			uint32_t svn = 0;
            char version[length + 1];
            memset(version, 0, sizeof(version));

            if (softrock_read_version_number (fifisdr, &svn)) {
                printf ("Firmware-Stand (SVN) = %d\n", (int)svn);
            }

            if (softrock_read_version_string (fifisdr, version, length)) {
                printf ("Firmware-Bezeichnung = %s\n", version);
            }
        }
 
        /* Debug-Werte auslesen */
        if (cmdline_debuginfo) {
			union {
				uint32_t d[8];
				uint16_t w[16];
				uint8_t b[32];
			} debug;
            memset(&debug, 0, sizeof(debug));

            if (softrock_read_debuginfo (fifisdr, (uint8_t *)&debug, 32)) {
                printf ("Debug: 0x%08X:0x%08X:0x%08X:0x%08X:0x%08X:0x%08X:0x%08X:0x%08X\n",
                                debug.d[0], debug.d[1], debug.d[2], debug.d[3],
                                debug.d[4], debug.d[5], debug.d[6], debug.d[7]);
            }
        }
 
        /* Volume setzen */
        if (cmdline_volume != -1) {
			if (cmdline_volume != 0) {
				cmdline_volume = 0x0600;
			}

			if (cmdline_write) {
            	softrock_write_volume(fifisdr, cmdline_volume);
			}
			else {
				printf("Volume kann nur geschrieben werden\n!");
			}
        }
 
        /* Demodulator-Mode abfragen/setzen */
		if (cmdline_demod) {
			uint8_t demodMode = 255;

			if (!strcmp(cmdline_demodmode, "lsb") || !strcmp(cmdline_demodmode, "LSB")) {
				demodMode = 0;
			}
			if (!strcmp(cmdline_demodmode, "usb") || !strcmp(cmdline_demodmode, "USB")) {
				demodMode = 1;
			}
			if (!strcmp(cmdline_demodmode, "am") || !strcmp(cmdline_demodmode, "AM")) {
				demodMode = 2;
			}

			if (cmdline_write) {
				if (demodMode == 255) {
					printf("Kein Modus (\"LSB\", \"USB\", \"AM\") angegeben\n");
				}
	        	softrock_write_demodulator_mode(fifisdr, demodMode);
			}
			else {
	        	if (softrock_read_demodulator_mode(fifisdr, &demodMode)) {
					char *modeName = "UNBEKANNT";
					switch (demodMode) {
						case 0: modeName = "LSB"; break;
						case 1: modeName = "USB"; break;
						case 2: modeName = "AM"; break;
						case 3: modeName = "FM"; break;
					}
					printf("Demodulator: %s\n", modeName);
				}
			}
		}
 
        /* Offset abfragen/setzen */
		if (cmdline_offset) {
			int32_t subtract1121;
			uint32_t multiply1121;

			/* Zunächst aktuellen Stand lesen */
            if (softrock_read_subtract_multiply(fifisdr, &subtract1121, &multiply1121)) {
				if (cmdline_write) {
					if ((cmdline_subtract >= -1.0) && (cmdline_subtract <= +1.0)) {
						subtract1121 = (int32_t)(cmdline_subtract * (4.0 * 2097152.0));
					}
					if (cmdline_multiply >= 0.0) {
						multiply1121 = (uint32_t)(cmdline_multiply * 2097152.0);
					}
		            softrock_write_subtract_multiply(fifisdr, subtract1121, multiply1121);
				}
				else {
					double sub = subtract1121 / (4.0 * 2097152.0);
					double mul = multiply1121 / 2097152.0;

	                printf ("Subtract: %f MHz, Multiply: %f\n", sub, mul);
				}
            }
		}

        /* Demodulator-Bandbreite */
        if (cmdline_bandwidth)
        {
            uint32_t freq;

            /* Beim Schreiben Werte prüfen */
            if (cmdline_write)
            {
                if (cmdline_freq < 0.0)
                {
                    printf ("Keine Frequenz (--freq in Hz) angegeben\n");
                }
                else
                {
                    freq = (uint32_t)cmdline_freq;
                    if (softrock_write_bandwidth (fifisdr, freq)) {
						uint32_t freq2;
		                if (softrock_read_bandwidth (fifisdr, &freq2))
		                {
							if (freq != freq2) {
								printf("Warnung: Bandbreite: %d Hz\n", freq2);
							}
						}
					}
                }
            }
            else
            {
                if (softrock_read_bandwidth (fifisdr, &freq))
                {
                    printf ("Bandbreite: %d Hz\n", freq);
                }
            }
        }
 
 		usb_close (fifisdr);
    }
    else
    {
        printf ("Kein passendes FiFi-SDR gefunden\n" );
    }

    /* Nach dem Schreiben etwas warten, damit das FiFi-SDR Zeit hat das Flash zu beschreiben. */
    if (cmdline_write)
    {
        usleep (200000);    /* 200 ms */
    }

    return (0);
}
