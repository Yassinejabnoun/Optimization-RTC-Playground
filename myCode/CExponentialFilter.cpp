/*
 * CExponentialFilter.cpp
 *
 *  Created on: 26.05.2022
 *      Author: jabno
 */

#include "CExponentialFilter.h"

CExponentialFilter::CExponentialFilter(float factor) {
	m_factor=factor;
	m_last = 0 ;
}

CExponentialFilter& CExponentialFilter::operator <<(float value) {
	m_last=m_last+m_factor*(value-m_last);
	return *this ;
}

CExponentialFilter::operator float() const {
	return m_last ;
}
