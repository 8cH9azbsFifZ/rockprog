ECHO BATCH-Programmierung mittels ROCKPROG0

REM Preselektor-Betriebsart: 4 Ausgänge
rockprog0 -w --presel --mode=1

REM Umschaltgrenzen der Tiefpässe auf FiFi-Preselektor
rockprog0 -w --presel --index=0 --freq-from=0     --freq-to=0.123 --pattern=1
rockprog0 -w --presel --index=1 --freq-from=0.123 --freq-to=0.307 --pattern=7
rockprog0 -w --presel --index=2 --freq-from=0.307 --freq-to=0.768 --pattern=0
rockprog0 -w --presel --index=3 --freq-from=0.768 --freq-to=1.92  --pattern=2
rockprog0 -w --presel --index=4 --freq-from=1.92  --freq-to=4.8   --pattern=6
rockprog0 -w --presel --index=5 --freq-from=4.8   --freq-to=12    --pattern=5
rockprog0 -w --presel --index=6 --freq-from=12    --freq-to=30    --pattern=3
rockprog0 -w --presel --index=7 --freq-from=30    --freq-to=75    --pattern=4
rockprog0 -w --presel --index=8 --freq-from=75    --freq-to=150   --pattern=12

REM Übrige Tabellen-Einträge löschen
rockprog0 -w --presel --index=9  --freq-from=0 --freq-to=0 --pattern=0
rockprog0 -w --presel --index=10 --freq-from=0 --freq-to=0 --pattern=0
rockprog0 -w --presel --index=11 --freq-from=0 --freq-to=0 --pattern=0
rockprog0 -w --presel --index=12 --freq-from=0 --freq-to=0 --pattern=0
rockprog0 -w --presel --index=13 --freq-from=0 --freq-to=0 --pattern=0
rockprog0 -w --presel --index=14 --freq-from=0 --freq-to=0 --pattern=0

REM Standardwert für Preselektor-Ausgänge
REM PRESELEKTOR-TABELLE WIRD NUR GESCHRIEBEN, WENN AM SCHLUSS INDEX 15 BENUTZT WIRD!
rockprog0 -w --presel --index=15 --freq-from=0.0 --freq-to=500 --pattern=0

REM Schaltgrenzen für 3. und 5. Oberwelle des LO
rockprog0 -w --3rd --freq=35.0
rockprog0 -w --5th --freq=85.0

REM Korrektur des rechnerischen Offset von LO und RX-Frequenz fuer DSP-Firmware
rockprog0 -w --offset --subtract=0.012

ECHO BATCH-Programmierung beendet
