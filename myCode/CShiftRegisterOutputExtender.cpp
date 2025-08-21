/*
 * CShiftRegisterOutputExtender.cpp
 *
 *  Created on: 17.10.2020
 *      Author: mnl
 */

#include "CShiftRegisterOutputExtender.h"

CShiftRegisterOutputExtender::CShiftRegisterOutputExtender(
		DigitalOut *serialData, DigitalOut *shiftRegisterClock,
		DigitalOut *bufferRegisterClock) {
	m_serialData = serialData ;
	m_shiftRegisterClock = shiftRegisterClock;
	m_bufferRegisterClock = bufferRegisterClock;
	m_preparedOutput = 0;
	m_currentOutput = 0;

	// ToDo (Wie Sie sehen, bewirkt "ToDo" die Anzeige eines Merkers
	// links neben der Zeilennummer. Löschen Sie daher diesen Kommentar,
	// wenn Sie die Implementierung fertiggestellt haben. Alle offenen
	// ToDo s in Ihrem Projekt können Sie sich auch unten im Reiter
	// "Tasks" anzeigen lassen, wenn Sie den Filter entsprechend
	// konfigurieren.
}

void CShiftRegisterOutputExtender::prepareOutput(uint8_t value) {
	// Schieben Sie den übergebenen Wert durch Ansteuern
	// von m_serialData und m_shiftRegisterClock in einer
	// Schleife (!) in das Schieberegister.
	// Speichern Sie den Wert außerdem in m_preparedOutput, damit
	// Sie ihn in updateOutput nach m_currentOutput übernehmen können.
	// Schieben Sie den Wert so in das Schieberegister, dass das
	// höchstwertige Bit (b7) später an QH ausgegeben wird und das
	// niederwertigste Bit (b0) an QA.
	// ToDo

	m_preparedOutput = value;

	for (int i = 7; i >= 0; i--) {
		*m_serialData = (value >> i) & 0x01;
		*m_shiftRegisterClock = 0;
		*m_shiftRegisterClock = 1;
		*m_shiftRegisterClock = 0;
	}
}


void CShiftRegisterOutputExtender::updateOutput(){
	// Übernehmen Sie den Wert im Schieberegister durch Ansteuern
	// von m_bufferRegisterClock an die Ausgänge.
	// Aktualisieren Sie m_currentOutput;
	// ToDo
	*m_bufferRegisterClock = 1;
	*m_bufferRegisterClock = 0;
	m_currentOutput = m_preparedOutput;
}

void CShiftRegisterOutputExtender::write(uint8_t value) {
	prepareOutput(value);
	updateOutput();
}

CShiftRegisterOutputExtender& CShiftRegisterOutputExtender::operator =(
		uint8_t value) {
	// ToDo
	write(value);
	return *this;
}

uint8_t CShiftRegisterOutputExtender::read() const {
	// ToDo
	return m_currentOutput;
}

CShiftRegisterOutputExtender::operator int() const {
	// ToDo
	return read();
}
