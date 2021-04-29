/* 
* SpectrumMatrixLedEffect.h
*
*/

#ifndef __SPECTRUMMATRIXLEDEFFECT_H__
#define __SPECTRUMMATRIXLEDEFFECT_H__

#if defined(ARDUINO) && ARDUINO >= 100
#include "Arduino.h"
#else
#include "WProgram.h"
#endif

#include <EffectTimer.h>
#include <ILedEffect.h>
#include <ILedMatrix.h>
#include "ISpectrumBandConverter.h"

class SpectrumMatrixLedEffect : public ILedEffect
{
public:

	static const char* const name;

protected:

	ILedMatrix* matrix;
	ISpectrumBandConverter* audio;

private:

	MillisTimer fallTimer;

	static const unsigned long MAXIMUM_HOLD_TIME = 600;

	struct BAND
	{
		uint16_t level = 0;
		unsigned long maximumTime = 0;
		uint8_t maximumPosition = 0;
	}
	*bands;

public:
	SpectrumMatrixLedEffect(ILedMatrix* matrixConverter, uint16_t Hz, ISpectrumBandConverter* audioConverter);
	~SpectrumMatrixLedEffect();
	
	void reset() override;
	void paint() override;

	operator const char* () const {	return name; }

private:

	SpectrumMatrixLedEffect(const SpectrumMatrixLedEffect&) = delete;
	SpectrumMatrixLedEffect& operator=(const SpectrumMatrixLedEffect&) = delete;

};

#endif
