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

#include <ILedEffect.h>
#include <ILedMatrix.h>
#include "IAudioConverter.h"

class SpectrumMatrixLedEffect : public ILedEffect
{
public:

	static const char* const name;

protected:

	ILedMatrix* matrix;
	IAudioConverter* audio;

public:
	SpectrumMatrixLedEffect(ILedMatrix* matrixConverter, uint16_t Hz, IAudioConverter* audioConverter);
	~SpectrumMatrixLedEffect();
	
	void reset() override;
	bool paint() override;

	operator const char* () const {	return name; }

private:

	SpectrumMatrixLedEffect(const SpectrumMatrixLedEffect&) = delete;
	SpectrumMatrixLedEffect& operator=(const SpectrumMatrixLedEffect&) = delete;

};

#endif
