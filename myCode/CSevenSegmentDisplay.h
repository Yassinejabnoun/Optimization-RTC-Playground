/*
 * CSevenSegmentDisplay.h
 *
 *  Created on: 18.10.2020
 *      Author: mnl
 */

#ifndef CSEVENSEGMENTDISPLAY_H_
#define CSEVENSEGMENTDISPLAY_H_

#include "CShiftRegisterOutputExtender.h"
extern "C" void TIM1_UP_TIM10_IRQHandler();

/**
 * Modelliert eine 4-stellige 7-Segment-Anzeige. Um eine "aktive" und
 * flackerfreie Darstellung zu erhalten, muss sichergestellt werden,
 * dass die Methode "activateNextDigit" mindestens alle 5 ms aufgerufen wird.
 */
class CSevenSegmentDisplay {
private:
	static uint8_t patterns[];
	CShiftRegisterOutputExtender* m_ssdSegments;
	DigitalOut* m_digits[4];
	uint8_t m_activeDigit;
	uint8_t m_values[4];
	uint8_t m_decimalPoint[4] = { 0 };
	static CSevenSegmentDisplay *m_instance;

	/**
	 * Hilfsmethode für das Zeitmultiplexverfahren. Aktiviert die nächste
	 * Anzeigestelle.
	 */
	void activateNextDigit();

	/**
	 * Hilfsmethode für das Zeitmultiplexverfahren. Deaktiviert die nächste
	 * Anzeigestelle.
	 */
	void deactivateNextDigit();

	/**
	 * Hilfsmethode, soll den Timer 10 so konfigurieren, dass er alle 5 ms
	 * einen Update-Interrupt auslöst
	 */
	void initTimer();


public:
	/**
	 * Erzeugt ein neues Objekt, das eine 4-stellige 7-Segment-Anzeige
	 * ansteuert, deren Segmente (und Dezimalpunkt) an das über
	 * ssdSegments ansteuerbare Schieberegister angeschlossen sind und
	 * deren Anzeigestellen über die übergebenen digitalen Ausgänge
	 * aktiviert werden können.
	 *
	 * @param ssdSegments die Modellierung des Schieberegisters zur
	 *  Ansteuerung der Segemente
	 * @param digits die Ausgänge zum Aktivieren der Anzeigestellen
	 */
	CSevenSegmentDisplay(CShiftRegisterOutputExtender* ssdSegments,
			DigitalOut* digits[4]);


	/**
	 * Ermöglicht den Zugriff auf die an der ausgewählten Anzeigestelle
	 * darzustellende Ziffer. Da eine Referenz auf den gespeicherten Wert
	 * zurückgeliefert wird, kann der Operator sowohl als Ziel einer
	 * Zuweisung als auch zum Abfragen des aktuell gespeicherten Wertes
	 * verwendet werden.
	 *
	 * @param position Index der Anzeigestelle (0 für Stelle ganz rechts)
	 * @return Referenz auf den an der ausgewählten Stelle anzuzeigenden Wert
	 */
	uint8_t& operator[] (int position);

	/**
	 * Erzeugt ein Anzeige für die Dezimalpunkt auf der 7 Segment Display .
	 * @param digit Stelle der Dezimalpunkt auf dem Display
	 */
	void setDecimalPoint(int digit);

	/**
	 * Zeigt den angegebenen integer Wert auf dem Display an .
	 * @param value der anzuzeigenen Wert
	 */
	void setValue(int value);

	/**
	 * Überladener Operator , der die Methode setValue(int value) aufruft
	 * @param value der anzuzeigene Wert
	 */
	CSevenSegmentDisplay& operator=(int value);

	/**
	 * Zeigt den angegebenen float Werte mit passende Komma an
	 * @param value der anzuzeigende Wert
	 */
	void setValue(float value);

	/**
	 * Überladener Operator , der die Methode setValue(flaot value) aufruft
	 * @param value der anzuzeigene Wert
	 */
	CSevenSegmentDisplay& operator=(float value);

	/**
	 * Als friend deklarierter Interrupt-Handler, damit die Funktion auf die
	 * private Funktion zur Aktivierung der nächsten Stelle zugreifen kann.
	 */
	friend void TIM1_UP_TIM10_IRQHandler();

	/**
	 * Setzt die Helligkeit zwichen 5 und 100% .
	 * helligkeit kleiner 5 wird als 5betrachtet, helligkeit größer 100 als 100.
	 *
	 * @param percent Heilligkeit in procent.
	 */
	void setBrightness(int percent);

	/**
	 * Liefert die aktuelles Heilligkeit zurück .
	 *
	 * @return die Helligkeit
	 */
	int getBrightness();
};

#endif /* CSEVENSEGMENTDISPLAY_H_ */


