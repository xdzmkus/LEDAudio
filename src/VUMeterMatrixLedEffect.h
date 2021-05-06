/* 
* VUMeterMatrixLedEffect.h
*
*/

#ifndef __VUMETERMATRIXLEDEFFECT_H__
#define __VUMETERMATRIXLEDEFFECT_H__

#if defined(ARDUINO) && ARDUINO >= 100
#include "Arduino.h"
#else
#include "WProgram.h"
#endif

#include <ILedEffect.h>
#include <ILedMatrix.h>

class VUMeterMatrixLedEffect : public ILedEffect
{
public:

	static const char* const name;

protected:

	ILedMatrix* matrix;
	const uint16_t NOISE_LEVEL;

private:

	uint16_t maxLevel;

public:
	VUMeterMatrixLedEffect(ILedMatrix* matrixConverter, uint16_t Hz, uint16_t noiseLevel = 255);
	~VUMeterMatrixLedEffect();
	
	void reset() override;

	void paint(uint16_t rightVU, uint16_t leftVU);

	void autoGain(uint16_t maxVU);

	operator const char* () const { return name; }

protected:

	virtual void drawLeftChannel(uint16_t currentVU);
	virtual void drawRightChannel(uint16_t currentVU);

	virtual void drawVUsignal(uint8_t x, uint8_t columnLevel);

	virtual uint8_t getColumnLevel(uint16_t currentVU);

private:

	void paint() override
	{
		// not used, call paint(rightVU, leftVU) instead
	};

private:

	VUMeterMatrixLedEffect(const VUMeterMatrixLedEffect&) = delete;
	VUMeterMatrixLedEffect& operator=(const VUMeterMatrixLedEffect&) = delete;

};

#endif
