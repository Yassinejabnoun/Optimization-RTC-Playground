/*
 * CRtc.cpp
 *
 *  Created on: 15.01.2021
 *      Author: mnl
 */

#include <mbed.h>
#include "CRtc.h"

void CRtc::enableRtcAccess() {
	// Enable power management and allow access to backup domain
	SET_BIT(RCC->APB1ENR, RCC_APB1ENR_PWREN);
	READ_BIT(RCC->APB1ENR, RCC_APB1ENR_PWREN); // Required delay
	SET_BIT(PWR->CR, PWR_CR_DBP);
	while (!READ_BIT(PWR->CR, PWR_CR_DBP)) {
	}
}

void CRtc::selectClockSource() {
	// Check if clock source must be changed (we want LSE)
	if (READ_BIT(RCC->BDCR, RCC_BDCR_RTCSEL)
			!= (1 << RCC_BDCR_RTCSEL_Pos)|| !READ_BIT(RCC->BDCR, RCC_BDCR_LSEON)) {
		// Reset backup domain to select clock source, restore other values
		// (except those that we want to modify)
		uint32_t oldBdcr = RCC->BDCR;
		SET_BIT(RCC->BDCR, RCC_BDCR_BDRST);
		CLEAR_BIT(RCC->BDCR, RCC_BDCR_BDRST);
		RCC->BDCR = oldBdcr & ~(RCC_BDCR_RTCSEL | RCC_BDCR_RTCEN);
		// If LSE isn't on, turn it on.
		if (!READ_BIT(RCC->BDCR, RCC_BDCR_LSEON)) {
			SET_BIT(RCC->BDCR, RCC_BDCR_LSEON);
		}
		while (!READ_BIT(RCC->BDCR, RCC_BDCR_LSERDY)) {
		}
		// Finally select LSE as clock source
		SET_BIT(RCC->BDCR, 1 << RCC_BDCR_RTCSEL_Pos);
	}

	// Make sure that the RTC is enabled
	if (!READ_BIT(RCC->BDCR, RCC_BDCR_RTCEN)) {
		SET_BIT(RCC->BDCR, RCC_BDCR_RTCEN);
	}
}

void CRtc::enterInitializationMode() {
	// Allow access to backup domain
	SET_BIT(PWR->CR, PWR_CR_DBP);
	while (!READ_BIT(PWR->CR, PWR_CR_DBP)) {
	}

	// Disable write protection for RTC registers
	RTC->WPR = 0xCAU;
	RTC->WPR = 0x53U;
	SET_BIT(RTC->ISR, RTC_ISR_INIT);
	while (!READ_BIT(RTC->ISR, RTC_ISR_INITF)) {
	}
}

void CRtc::leaveInitializationMode() {
	// Reset Init flag.
	CLEAR_BIT(RTC->ISR, RTC_ISR_INIT);

	// Enable write protection for RTC registers
	// and terminate backup domain access
	RTC->WPR = 0xFFU;
	CLEAR_BIT(PWR->CR, PWR_CR_DBP);
}

CRtc::CRtc(BusOut *leds, CSevenSegmentDisplay *display) {
	m_leds=leds;
	m_display=display;
	enableRtcAccess();
	selectClockSource();
}

uint32_t CRtc::secondsSinceMidnight() {
	// Make sure that the shadow registers are synchronized and get TR
	while (!READ_BIT(RTC->ISR, RTC_ISR_RSF)) {
	}
	uint32_t tr = RTC->TR;

	// Registers have been read.
	CLEAR_BIT(RTC->ISR, RTC_ISR_RSF);

	return (((tr >> 20) & 0x3) * 10 + ((tr >> 16) & 0xf)) * 3600
			+ (((tr >> 12) & 0x7) * 10 + ((tr >> 8) & 0xf)) * 60
			+ (((tr >> 4) & 0x7) * 10 + (tr & 0xf));
}

void CRtc::updateTime(uint32_t secondsSinceMidnight) {
	// Convert to BCD representation
	uint32_t bcdTime = 0;
	int secs = secondsSinceMidnight % 60;
	bcdTime |= secs % 10;
	bcdTime |= (secs / 10) << 4;
	int mins = secondsSinceMidnight / 60 % 60;
	bcdTime |= (mins % 10) << 8;
	bcdTime |= (mins / 10) << 12;
	int hours = secondsSinceMidnight / 3600 % 24;
	bcdTime |= (hours % 10) << 16;
	bcdTime |= (hours / 10) << 20;

	// Update RTC register
	enterInitializationMode();
	RTC->TR = bcdTime & RTC_TR_RESERVED_MASK;
	leaveInitializationMode();
}

void CRtc::BCD_Mode() {
	*m_leds = getSeconds() % 10;
	*m_leds = (getSeconds() / 10) << 4;

}

void CRtc::Balken_Mode() {
	int nLeds = (float) getSeconds() / 60 * 8;
	*m_leds = 0xff00 >> nLeds;
}

void CRtc::setMinutes(int minutes) {
	updateTime(secondsSinceMidnight()+(minutes*60));
}

void CRtc::setHours(int hours) {
	updateTime(secondsSinceMidnight()+(hours*3600));
}

void CRtc::updateKeys(uint8_t keys) {
	if (keys & 0x1) {
		if ((getMinutes() % 10) != 9) {
			// erste Stelle wird um eine Minute erhöht
			setMinutes(1);
		} else {
			// erste Stelle wird auf null gesetzt
			setMinutes(-9);
		}

	} else if (keys & 0x2) {
		if ((getMinutes() / 10) != 5) {
			// erste Stelle wird um eine Minute erhöht
			setMinutes(10);
		} else {
			// erste Stelle wird auf null gesetzt
			setMinutes(-50);
		}

	} else if (keys & 0x4) {
		if ((getHours() % 10) != 9) {
			// erste Stelle wird um eine Stunde erhöht
			setHours(1);
		} else {
			// erste Stelle wird auf null gesetzt
			setHours(-9);
		}

	} else if (keys & 0x8) {
		if ((getHours() % 10) < 4) {
			// zulässige werte sind 0 1 2
			if ((getHours() / 10) != 2) {
				setHours(10);
			}
			else
			{
				setHours(-20);
			}
		} else {
			// zulässige werte sind 0 1 2
			if ((getHours() / 10) != 1) {
				setHours(10);
			}
			else
			{
				setHours(-10);
			}
		}
	}
}

uint16_t CRtc::getSeconds() {
	return secondsSinceMidnight() % 60;
}

uint32_t CRtc::getMinutes() {
	return (secondsSinceMidnight() / 60) % 60;
}

uint32_t CRtc::getHours() {
	return secondsSinceMidnight() / (60 * 60);
}

void CRtc::showTime() {
	(*m_display)[0] = (getMinutes() % 10);
	(*m_display)[1] = (getMinutes() / 10);
	(*m_display)[2] = (getHours() % 10);
	(*m_display)[3] = (getHours() / 10);
}
