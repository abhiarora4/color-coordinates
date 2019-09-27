#ifndef SPECTRUM_H_
#define SPECTRUM_H_

#include "XYData.h"


class Spectrum: public XYData {

public:
	enum x_units {
		RAW = 0,
		WAVE_NM,
		WAVE_UM,
		WAVE_MM,
		WAVE_CM,
		WAVE_M,
	};

	enum y_units {
		RAW = 0,
		Y_PERCENT,
		Y_RATIO,
	};
	// Iterator

public:
	Spectrum(void): XYData(), x_unit_(WAVE_NM), y_unit_(RAW) { }
	Spectrum()

private:
	x_units x_unit_;
	y_units y_unit_;
};


#endif