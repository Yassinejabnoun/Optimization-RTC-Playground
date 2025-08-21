/*
 * CExponentialFilter.h
 *
 *  Created on: 26.05.2022
 *      Author: jabno
 */

#ifndef CEXPONENTIALFILTER_H_
#define CEXPONENTIALFILTER_H_
#include "mbed.h"

/**
 * Ein Exponentialfilter wird erstellt. Der neue geglättete Wert ergibt sich aus
 * dem alten geglätteten Wert plus der Differenz aus neuem und altem Wert
 * gewichtet mit einem Faktor .
 */
class CExponentialFilter {
private:
	// geschwichtete Faktor
	float m_factor;
	// vorherige Messwert
	float m_last;
public:
	/**
	 *	Konstruktor : erzeugt einen neuen Filter mit dem angegebenen
	 *	Wert für den Faktor
	 *
	 *	@param factor : Faktor der Filter
	 */
	CExponentialFilter(float factor);

	/**
	 * überladenen Operator : die neue Werten werden in den Filter hineingegeben
	 *
	 * @param factor :neuer Wert der in der  Glättung verwendet wird.
	 */
	CExponentialFilter& operator<<(float value);

	/**
	 * gibt den geglätteten Wert aus
	 */
	operator float() const;
};

#endif /* CEXPONENTIALFILTER_H_ */
