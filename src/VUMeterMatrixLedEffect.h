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

	uint16_t rightChannelVU = 0;
	uint16_t leftChannelVU = 0;

	uint16_t maxLevel;

public:
	VUMeterMatrixLedEffect(ILedMatrix* matrixConverter, uint16_t Hz, uint16_t noiseLevel = 256);
	~VUMeterMatrixLedEffect();
	
	void reset() override;

	void paint(uint16_t rightVU, uint16_t leftVU);

	operator const char* () const { return name; }

protected:

	virtual void drawLeftChannel();
	virtual void drawRightChannel();

	virtual void drawVUsignal(uint8_t x, uint8_t columnLevel);
	
	virtual uint8_t getLeftColumnLevel();
	virtual uint8_t getRightColumnLevel();

	virtual void autoGain();

private:

	void paint() override;

private:

	VUMeterMatrixLedEffect(const VUMeterMatrixLedEffect&) = delete;
	VUMeterMatrixLedEffect& operator=(const VUMeterMatrixLedEffect&) = delete;

};

#endif
