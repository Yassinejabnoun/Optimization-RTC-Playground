/*
 * CRtc.h
 *
 *  Created on: 15.01.2021
 *      Author: mnl
 */

#include "CSevenSegmentDisplay.h"

#ifndef CRTC_H_
#define CRTC_H_

/**
 * Provides easy access to the Real Time Clock.
 */
class CRtc {
public:
	CRtc(BusOut *leds, CSevenSegmentDisplay *SSD);

private:
	BusOut *m_leds;
	CSevenSegmentDisplay *m_display;

	/**
	 * liefert die  Seconds zurück.
	 */
	uint16_t getSeconds();

	/**
	 * liefert die Minuten zurück.
	 */
	uint32_t getMinutes();

	/**
	 * liefert die Hours zurück.
	 */
	uint32_t getHours();

	/**
	 * setzt die Minuten .
	 */
	void setMinutes(int minutes);

	/**
	 * setzt die Hours .
	 */
	void setHours(int hours);

	/**
	 * Enable power management and allow access to backup domain.
	 * Called once in constructor.
	 */
	void enableRtcAccess();

	/**
	 * Select LSE as clock source. Called once in constructor.
	 */
	void selectClockSource();

	/**
	 * Enable modification of RTC registers, stop counting.
	 */
	void enterInitializationMode();

	/**
	 * Disable modification of RTC registers, resume counting.
	 */
	void leaveInitializationMode();

public:
	/**
	 * Returns the time as number of seconds since midnight.
	 */
	uint32_t secondsSinceMidnight();

	/**
	 * Sets the time.
	 *
	 * @param secondsSinceMidnight the time specified as seconds since 00:00
	 */
	void updateTime(uint32_t secondsSinceMidnight);

	/**
	 * zeigt die zeit der Uhr auf dem Dispaly
	 */
	void showTime();

	/**
	 * aktualiesiert die zeit je nach dem gedruckten Taster
	 *
	 * @param Keys der aktuell gedruckten Taster
	 */
	void updateKeys(uint8_t keys);

	/**
	 * zeigt die Sekunden der aktuellen Uhrzeit
	 *  auf den LEDs als BCD-Zahl an.
	 */
	void BCD_Mode();

	/**
	 * zeigt die Sekunden der aktuellen Uhrzeit
	 *  auf den LEDs als Balken an.
	 */
	void Balken_Mode();
};

#endif /* CRTC_H_ */
