/* 
* SpectrumMatrixLedEffect.h
*
*/

#ifndef __SPECTRUMMATRIXLEDEFFECT_H__
#define __SPECTRUMMATRIXLEDEFFECT_H__

#include <ILedEffect.h>
#include <ILedMatrix.h>

#if defined(ARDUINO) && ARDUINO >= 100
#include "Arduino.h"
#else
#include "WProgram.h"
#endif

class SpectrumMatrixLedEffect : public ILedEffect
{
public:

	static const char* const name;

protected:

	ILedMatrix* matrix;
	uint8_t* spectrum;
	const uint8_t width;

private:
	long gain = 100;
	float k = 0.05;
	float maxValue_f = 0.0;

public:
	SpectrumMatrixLedEffect(ILedMatrix* converter, uint16_t Hz, uint8_t spectrum[], uint8_t width);
	~SpectrumMatrixLedEffect();
	
	void reset() override;
	bool paint() override;

	operator const char* () const {	return name; }

private:
	SpectrumMatrixLedEffect(const SpectrumMatrixLedEffect&) = delete;
	SpectrumMatrixLedEffect& operator=(const SpectrumMatrixLedEffect&) = delete;

};

#endif
