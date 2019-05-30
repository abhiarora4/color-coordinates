#include "algo.h"

#include <iostream>

#include <vector>
#include <map>

#include <algorithm>
#include <math.h>




double algo::GetFloatPrecision(double value, double precision)
{
    return (floor((value * pow(10, precision) + 0.5)) / pow(10, precision)); 
}

XYData algo::interpolate(XYData& s, double xStart, double xStop, double jump, bool mode)
{
	// Handle size of one
	XYData result;
	if (s.length() <= 1)
		return result;

	double lowerX, upperX;
	double lowerY, upperY;

	long i, j;

	for (double current = xStart; current <= xStop; current += jump) {
		long length = s.length();
		// Find the Element just smaller than Current
		for (i = length - 1; (i >= 0) and (current < s.x_at_ndx(i)); i--);

		// Finding the Element just larger than Current
		for (j = 0; (j < length) and (current > s.x_at_ndx(j)); j++);

		// If current lies at left side of the xCoords. Current is the smallest
		if ((i < 0) and (j == 0)) {
			i = j;
			if (!mode) j++;
		}

		// If current lies at right side of the xCoords. Current is the largest
		if ((i == (length - 1)) and (j >= length)) {
			j = i;
			if (!mode) i--;
		}

		lowerX = s.x_at_ndx(i); lowerY = s.y_at_ndx(i);

		upperX = s.x_at_ndx(j); upperY = s.y_at_ndx(j);

		double slope = 0;
		if (lowerX != upperX)
			slope = (upperY - lowerY) / (upperX - lowerX);

		double intercept = upperY - (slope * upperX);
		double value = slope * current + intercept;

		result.insert(current, value);
	}
	return result;
}

XYData algo::trim(XYData& data, double xStart, double xStop)
{
	XYData result;
	for (std::size_t i = 0; i < data.length(); i++) {
		double x = GetFloatPrecision(data.x_at_ndx(i));
		if (x > xStop)
			break;
		if (x < xStart)
			continue;
		result.insert(x, data.y_at_ndx(i));
	}
	return result;
}

XYData algo::interpolate(XYData& s, double jump)
{
	if (s.length() <= 1)
		return XYData();
	return interpolate(s, s.x_at_ndx(0), s.x_at_ndx(s.length() - 1), jump);
}

std::pair<XYData, XYData> algo::intersect(XYData& first, XYData& second)
{
	XYData resFirst;
	XYData resSecond;

	std::size_t i = 0, j = 0;

	while ((i < first.length()) and (j < second.length())) {
		double firstWave = GetFloatPrecision(first.x_at_ndx(i));
		double secondWave = GetFloatPrecision(second.x_at_ndx(j));
		if (firstWave < secondWave) {
			i++;
		} else if (firstWave > secondWave) {
			j++;
		} else {
			resFirst.insert(firstWave, first.y_at_ndx(i));
			resSecond.insert(secondWave, second.y_at_ndx(j));
			i++; j++;
		}
	}

	return std::make_pair(resFirst, resSecond);
}

void algo::transmittance_to_XYZ(const XYData& data, const XYData& src, const XYData& obsX, const XYData& obsY, 
					const XYData& obsZ, double& x, double& y, double& z)
{
	// TODO: Make sure they intersect and have constant diff
    long double n = 0;
    const double diff = 1;

    for (std::size_t i = 0; i < data.length(); i++)
        n += obsY.y_at_ndx(i) * src.y_at_ndx(i) * diff;

    for (std::size_t i = 0; i < data.length(); i++) {
        const double c = data.y_at_ndx(i) * src.y_at_ndx(i) * diff;

        x += c * obsX.y_at_ndx(i);
        y += c * obsY.y_at_ndx(i);
        z += c * obsZ.y_at_ndx(i);
    }

    x /= n;
    y /= n;
    z /= n;
}

void algo::reference_XYZ(const XYData& src, const XYData& obsX, const XYData& obsY, 
					const XYData& obsZ, double& x, double& y, double& z)
{
	// TODO: Make sure they intersect and have constant diff
    long double n = 0;
    const double diff = 1;

    for (std::size_t i = 0; i < src.length(); i++)
        n += obsY.y_at_ndx(i) * src.y_at_ndx(i) * diff;

    for (std::size_t i = 0; i < src.length(); i++) {
        const double c = 100 * src.y_at_ndx(i) * diff;

        x += c * obsX.y_at_ndx(i);
        y += c * obsY.y_at_ndx(i);
        z += c * obsZ.y_at_ndx(i);
    }

    x /= n;
    y /= n;
    z /= n;
}

void algo::xyz_to_lab(double x, double y, double z, double& l, double& a, double& b)
{
	// TODO Calculate reference dynamically
    double Reference_X = 94.811;
    double Reference_Y = 100;
    double Reference_Z = 107.304;

    double var_X = x / Reference_X;
    double var_Y = y / Reference_Y;
    double var_Z = z / Reference_Z;

    if (var_X > 0.008856)
        var_X = cbrt(var_X);
    else
        var_X = (7.787 * var_X) + (16.0 / 116.0);
    if (var_Y > 0.008856)
        var_Y = cbrt(var_Y);
    else
        var_Y = (7.787 * var_Y) + (16.0 / 116.0);

    if (var_Z > 0.008856)
        var_Z = cbrt(var_Z);
    else
        var_Z = (7.787 * var_Z) + (16.0 / 116.0);

    l = (116 * var_Y) - 16;
    a = 500 * (var_X - var_Y);
    b = 200 * (var_Y - var_Z);

}
