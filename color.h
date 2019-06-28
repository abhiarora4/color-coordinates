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

	void set_x(const XYData& x) { x_ = x; }
	void set_y(const XYData& y) { y_ = y; }
	void set_z(const XYData& z) { z_ = z; }

	void set_x(XYData& x) { x_ = x; }
	void set_y(XYData& y) { y_ = y; }
	void set_z(XYData& z) { z_ = z; }

	bool fromFile(const std::string& xfilename, const std::string& yfilename, const std::string& zfilename);
	bool is_valid(void) const { return !(x_.empty() or y_.empty() or z_.empty()); }

public:
	friend void swap(ColorObserver& first, ColorObserver& second);

public:
	ColorObserver& operator=(ColorObserver other);

public:
	ColorObserver(void);
	ColorObserver(const XYData& x, const XYData& y, const XYData& z);
	ColorObserver(const std::string& xfilename, const std::string& yfilename, const std::string& zfilename);
	ColorObserver(const std::string& dirname);
	ColorObserver(const ColorObserver& other);
	ColorObserver(ColorObserver&& other);
	~ColorObserver(void);

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

	bool insert(std::string name, const ColorObserver& observer);
	bool insert(std::string name, const XYData& src);

public:

	TriValues to_XYZ(TriValues values);
	TriValues to_xyz(TriValues values); 

	TriValues sd_to_XYZ(XYData sd, std::string illuminant, std::string observer);
	TriValues sd_to_lab(XYData sd, std::string illuminant, std::string observer);

	TriValues ref_XYZ(std::string illuminant, std::string observer);
	friend std::ostream& operator<<(std::ostream& stream, Color& c);

public:
	Color(void);
	Color(const std::string& path);

private:
	static constexpr double WSTART = 360;
	static constexpr double WSTOP = 830;
	static constexpr double WJUMP = 5;

private:
	std::map<std::string, ColorObserver> observer_;
	std::map<std::string, XYData> illuminants_;
};



#endif /* COLOR_H */