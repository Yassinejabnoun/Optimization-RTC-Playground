#include "mbed.h"
#include "breadboard.h"
#include "CShiftRegisterOutputExtender.h"
#include "CSevenSegmentDisplay.h"
#include "CPolledTimer.h"
#include "CRtc.h"
#include "CExponentialFilter.h"
// Legen Sie die globalen Objekte, die die Peripheriekomponenten
// repräsentieren, hier an (vor den Funktionen).

// DigitalOut led1(LED1);

BusIn keys(BB_BTN_0, BB_BTN_1, BB_BTN_2, BB_BTN_3,
		BB_BTN_4, BB_BTN_5, BB_BTN_6, BB_BTN_7);

BusOut leds(BB_LED_0, BB_LED_1, BB_LED_2, BB_LED_3, BB_LED_4, BB_LED_5,
BB_LED_6, BB_LED_7);

AnalogIn poti(BB_POTI, 3.3);

DigitalOut ssdSerialData(SSD_SER);
DigitalOut ssdShiftRegisterClock(SSD_SCLK);
DigitalOut ssdBufferRegisterClock(SSD_RCLK);

DigitalOut ssdDigit0(SSD_DIGIT_0, OpenDrainNoPull);
DigitalOut ssdDigit1(SSD_DIGIT_1, OpenDrainNoPull);
DigitalOut ssdDigit2(SSD_DIGIT_2, OpenDrainNoPull);
DigitalOut ssdDigit3(SSD_DIGIT_3, OpenDrainNoPull);

void task1() {
	// lokales Objekt zur Steuerung der Segmenten
	CShiftRegisterOutputExtender ssdSegments(&ssdSerialData,
			&ssdShiftRegisterClock, &ssdBufferRegisterClock);

	// Erzeugung für SSD wird vorbereitet
	DigitalOut *digitalOuts[4] = { &ssdDigit3, &ssdDigit2, &ssdDigit1,
			&ssdDigit0 };
	CSevenSegmentDisplay ssd(&ssdSegments, digitalOuts);

	// Led 0 defienieren
	DigitalOut led0(BB_LED_0);

	// die letzten vier Stellen meiner Matriklelnummer
	//werden auf SSD dargestellt
	ssd = 9510;

	// Helligkeit wird eingestellt
	ssd.setBrightness(5);

	while (true) {
		// Ausführung von Interrupt Handlern verhindern und LED ausschalten.
		__disable_irq();
		led0 = 0;

		// Auf einen Interrupt warten. Wenn er auftritt wird die CPU wieder
		// aktiviert, der Interrupt Handler wird aber noch nicht ausgeführt.
		// Der Interrupt "steht aus" ("is pending").
		__WFI();

		// LED einschalten und die Ausführung von Interrupt Handlern wieder
		// zulassen. Der Handler wird damit sofort nach __enable_irq()
		// aufgerufen.
		led0 = 1;
		__enable_irq();
	}
}

void task2() {
	// lokales Objekt zur Steuerung der Segmenten
	CShiftRegisterOutputExtender ssdSegments(&ssdSerialData,
			&ssdShiftRegisterClock, &ssdBufferRegisterClock);

	// Erzeugung für SSD wird vorbereitet
	DigitalOut *digitalOuts[4] = { &ssdDigit3, &ssdDigit2, &ssdDigit1,
			&ssdDigit0 };
	CSevenSegmentDisplay ssd(&ssdSegments, digitalOuts);

	// Objet für die RealTimeClock
	CRtc rt_Clock(&leds, &ssd);

	// Timer für drücken der Tastern
	CPolledTimer timer(1000);

	while (true) {
		// zeit auf dem 7segment anzeigen
		rt_Clock.showTime();

		// sekunden auf Leds als BCD darstellen
		//rt_Clock.BCD_Mode();

		// Sekunden aud Leds als Balken darstellen
		rt_Clock.Balken_Mode();

		// wenn einen Taster gedrückt ist
		if (keys.read() != 0) {
			// jede Sekunde
			if (timer.timeReached()) {
				// wird die Zeit je nach dem gedrückten Taster aktualisiert
				rt_Clock.updateKeys(keys.read());
			}
		}
	}
}

/**
 * Haupt- oder Einstiegsfunktion des Programms. Wird beim Start nach der
 * Initialisierung der globalen Variablen (bei Objekten: Aufruf des
 * Konstruktors) ausgeführt.
 */
int main() {
	// Falls notwendig, ergänzen Sie hier Aufrufe, um Konfigurationen
	// an globalen Objekten vorzunehmen, die nicht durch die Angabe von
	// Argumenten beim Aufruf des Konstruktors vorgenommen werden können.
	keys.mode(PullDown);

	// Aufruf der Funktion, die die Lösung der gerade bearbeiteten Aufgabe
	// implementiert.
	task1();
	task2();

	/*
	 * Normalerweise endet der Aufruf einer der Funktionen task1(), task2()
	 * etc. nie, d.h. der nachfolgende Code wird nie ausgeführt. Für den
	 * Fall, dass Ihnen bei der Implementierung einer der Funktionen ein
	 * Fehler unterläuft, ist hier "sicherheitshalber" noch mal eine
	 * Endlossschleife.
	 */
    while (true) {
    }
}

