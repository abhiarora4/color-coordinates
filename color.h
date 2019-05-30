#ifndef COLOR_H
#define COLOR_H

#include "XYData.h"

#include <iostream>
#include <string>
#include <map>

#include <filesystem>

class ColorObserver {

public:
	XYData& get_x(void) { return x_; }
	XYData& get_y(void) { return y_; }
	XYData& get_z(void) { return z_; }

	void set_x(XYData& x) { x_ = x; }
	void set_y(XYData& y) { y_ = y; }
	void set_z(XYData& z) { z_ = z; }

public:
	ColorObserver& operator=(ColorObserver other)
	{
		swap(x_, other.x_);
		swap(y_, other.y_);
		swap(z_, other.z_);

		return *this;
	}

public:
	ColorObserver(void) { }
	ColorObserver(const std::string& xfilename, const std::string& yfilename, 
		const std::string& zfilename)
	{
		std::filesystem::path xPath(xfilename);
		std::filesystem::path yPath(yfilename);
		std::filesystem::path zPath(zfilename);

		// if (!(xPath.exists() and yPath.exists() and zPath.exists())) {
		// 	std::cout << "can't" << std::endl;
		// 	return;
		// }

		x_.fromFile(xPath);
		y_.fromFile(yPath);
		z_.fromFile(zPath);
	}

	ColorObserver(const std::string& dirname)
	{
		ColorObserver(dirname + "/x_bar.csv", dirname + "/y_bar.csv", dirname + "/z_bar.csv");
	}

	ColorObserver(const ColorObserver& other)
	{
		x_ = other.x_;
		y_ = other.y_;
		z_ = other.z_;
	}

	ColorObserver(const XYData& x, const XYData& y, const XYData& z): x_(x), y_(y), z_(z)
	{

	}

	ColorObserver(ColorObserver&& other)
	{
		swap(x_, other.x_);
		swap(y_, other.y_);
		swap(z_, other.z_);
	}

	~ColorObserver(void)
	{

	}

private:
	XYData x_;
	XYData y_;
	XYData z_;
};

class Color {

public:
	struct TriValues {
		double x;
		double y;
		double z;
	};

public:
	ColorObserver& get_observer(std::string name);
	bool has_observer(std::string name);
	XYData& get_illuminant(std::string name);
	bool has_illuminant(std::string name);

	TriValues to_XYZ(TriValues values);
	TriValues to_xyz(TriValues values); 

	TriValues sd_to_XYZ(XYData sd, std::string illuminant, std::string observer);
	TriValues sd_to_lab(XYData sd, std::string illuminant, std::string observer);

	TriValues ref_XYZ(std::string illuminant, std::string observer);


public:
	Color(void);
	Color(const std::string& path);

private:
	std::map<std::string, ColorObserver> observer_;
	std::map<std::string, XYData> illuminants_;
};



#endif /* COLOR_H */