/*
* VUWaveMatrixLedEffect.h
*
*/

#ifndef __VUWAVEMATRIXLEDEFFECT_H__
#define __VUWAVEMATRIXLEDEFFECT_H__

#if defined(ARDUINO) && ARDUINO >= 100
#include "Arduino.h"
#else
#include "WProgram.h"
#endif

#include "VUMeterMatrixLedEffect.h"

class VUWaveMatrixLedEffect :  public VUMeterMatrixLedEffect
{
public:

	static const char* const name;

public:
	VUWaveMatrixLedEffect(ILedMatrix* matrixConverter, uint16_t Hz, uint16_t noiseLevel = 256);
	~VUWaveMatrixLedEffect();

	operator const char* () const { return name; }

protected:

	virtual void drawVUsignal(uint8_t x, uint8_t columnLevel) override;

private:

	VUWaveMatrixLedEffect(const VUWaveMatrixLedEffect&) = delete;
	VUWaveMatrixLedEffect& operator=(const VUWaveMatrixLedEffect&) = delete;

};

#endif