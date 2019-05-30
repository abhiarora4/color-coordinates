#ifndef ALGO_H
#define ALGO_H

#include "XYData.h"
#include <iostream>


namespace algo {

double GetFloatPrecision(double value, double precision = 5);

XYData trim(XYData& data, double xStart, double xStop);

XYData interpolate(XYData& s, double jump = 1.0);

XYData interpolate(XYData& s, double xStart, double xStop, double jump = 1.0, bool mode = false);
std::pair<XYData, XYData> intersect(XYData& first, XYData& second);

void transmittance_to_XYZ(const XYData& data, const XYData& src, const XYData& obsX, const XYData& obsY, 
					const XYData& obsZ, double& x, double& y, double& z);

void reference_XYZ(const XYData& src, const XYData& obsX, const XYData& obsY, 
					const XYData& obsZ, double& x, double& y, double& z);

void xyz_to_lab(double x, double y, double z, double& l, double& a, double& b);


}

#endif