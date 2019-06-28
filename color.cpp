#include "color.h"

#include "algo.h"

#include <iostream>
#include <algorithm>
#include <string>

#include <stdio.h>
#include <math.h>


ColorObserver::ColorObserver(void)
{

}

ColorObserver::ColorObserver(const std::string& xfilename, const std::string& yfilename, 
	const std::string& zfilename)
{
	fromFile(xfilename, yfilename, zfilename);
}

ColorObserver::ColorObserver(const std::string& dirname)
{
	fromFile(dirname + "/x_bar.csv", dirname + "/y_bar.csv", dirname + "/z_bar.csv");
}

ColorObserver::ColorObserver(const ColorObserver& other)
{
	x_ = other.x_;
	y_ = other.y_;
	z_ = other.z_;
}

ColorObserver::ColorObserver(const XYData& x, const XYData& y, const XYData& z): x_(x), y_(y), z_(z)
{

}

ColorObserver::ColorObserver(ColorObserver&& other)
{	
	swap(*this, other);
}

ColorObserver::~ColorObserver(void)
{

}

ColorObserver& ColorObserver::operator=(ColorObserver other)
{
	swap(*this, other);
	return *this;
}

void swap(ColorObserver& first, ColorObserver& second)
{
	swap(first.x_, second.x_);
	swap(first.y_, second.y_);
	swap(first.z_, second.z_);
}

bool ColorObserver::fromFile(const std::string& xfilename, const std::string& yfilename, 
	const std::string& zfilename)
{
	std::filesystem::directory_entry xEntry(xfilename);
	std::filesystem::directory_entry yEntry(yfilename);
	std::filesystem::directory_entry zEntry(zfilename);

	if (!(xEntry.is_regular_file() and yEntry.is_regular_file() and zEntry.is_regular_file()))
		return false;

	XYData x(xEntry.path());
	XYData y(yEntry.path());
	XYData z(zEntry.path());
	if (x.empty() or y.empty() or z.empty())
		return false;

	x_ = std::move(x);
	y_ = std::move(y);
	z_ = std::move(z);
	return true;
}

Color::Color(void)
{

}

Color::Color(const std::string& path)
{
	auto getDirEntry = [](std::string path, std::string dir) {
		std::transform(dir.begin(), dir.end(), dir.begin(), ::tolower);
		for (auto& e: std::filesystem::recursive_directory_iterator(path)) {
			if (!e.is_directory())
				continue;
			if (!e.path().stem().compare(dir))
				return e;
		}
		return std::filesystem::directory_entry();
	};

	auto cmfsDirEntry = getDirEntry(path, "cmfs");
	auto illuminantsDirEntry = getDirEntry(path, "illuminants");

	for (auto& e: std::filesystem::directory_iterator(illuminantsDirEntry.path())) {
		if (e.is_directory())
			continue;
		if (e.path().extension().compare(".csv"))
			continue;

		insert(e.path().stem(), XYData(e.path()));
	}

	for (auto& e: std::filesystem::directory_iterator(cmfsDirEntry.path())) {
		if (!e.is_directory())
			continue;
		insert(e.path().stem(), ColorObserver(std::string(e.path())));
	}
}

ColorObserver& Color::get_observer(std::string name)
{
	std::transform(name.begin(), name.end(), name.begin(), ::tolower);
	return observer_[name];
}

bool Color::has_observer(std::string name)
{
	std::transform(name.begin(), name.end(), name.begin(), ::tolower);
	if (observer_.find(name) == observer_.end())
		return false;
	return true;
}

bool Color::insert(std::string name, const ColorObserver& observer)
{
	if (!observer.is_valid())
		return false;
	std::transform(name.begin(), name.end(), name.begin(), ::tolower);
	observer_.insert(std::make_pair(name, observer));
	return true;
}

bool Color::insert(std::string name, const XYData& src)
{
	if (!src.length())
		return false;
	std::transform(name.begin(), name.end(), name.begin(), ::tolower);
	illuminants_.insert(std::make_pair(name, src));
	return true;
}

XYData& Color::get_illuminant(std::string name)
{
	std::transform(name.begin(), name.end(), name.begin(), ::tolower);
	return illuminants_[name];
}

bool Color::has_illuminant(std::string name)
{
	std::transform(name.begin(), name.end(), name.begin(), ::tolower);
	if (illuminants_.find(name) == illuminants_.end())
		return false;
	return true;
}

std::ostream& operator<<(std::ostream& stream, Color& c)
{
	stream << "----Observer----" << std::endl;
	for (auto& p: c.observer_)
		stream << "Name: " << p.first << " Range: " << p.second.get_x().length() << std::endl;

	stream << "----Illuminant----" << std::endl;
	for (auto& p: c.illuminants_)
		stream << "Name: " << p.first << " Range: " << p.second.length() << std::endl;

	return stream;
}

Color::TriValues Color::sd_to_XYZ(XYData sd, std::string illuminant, std::string observer)
{
	TriValues value = {0,0,0};
	long double n = 0;
    const double diff = 1;

	if (!has_illuminant(illuminant))
    	return value;
    if (!has_observer(observer))
    	return value;

    sd = algo::interpolate(sd, WSTART, WSTOP, WJUMP);

    XYData src = algo::interpolate(get_illuminant(illuminant), WSTART, WSTOP, WJUMP);
    ColorObserver obs = algo::interpolate(get_observer(observer), WSTART, WSTOP, WJUMP);


    for (std::size_t i = 0; i < sd.length(); i++)
        n += obs.get_y().y_at_ndx(i) * src.y_at_ndx(i) * diff;

    for (std::size_t i = 0; i < sd.length(); i++) {
        const double c = sd.y_at_ndx(i) * src.y_at_ndx(i) * diff;

        value.x += c * obs.get_x().y_at_ndx(i);
        value.y += c * obs.get_y().y_at_ndx(i);
        value.z += c * obs.get_z().y_at_ndx(i);
    }

    value.x /= n;
    value.y /= n;
    value.z /= n;

    return value;
}


Color::TriValues Color::sd_to_lab(XYData sd, std::string illuminant, std::string observer)
{
	TriValues value = {0,0,0};
	return value;
}

Color::TriValues Color::ref_XYZ(std::string illuminant, std::string observer)
{
	TriValues value = {0,0,0};
	long double n = 0;
    const double diff = 1;

	if (!has_illuminant(illuminant))
    	return value;
    if (!has_observer(observer))
    	return value;

    XYData src = algo::interpolate(get_illuminant(illuminant), WSTART, WSTOP, WJUMP);
    std::cout << src;
    ColorObserver obs = algo::interpolate(get_observer(observer), WSTART, WSTOP, WJUMP);
    std::cout << obs.get_y();

    for (std::size_t i = 0; i < src.length(); i++)
        n += obs.get_y().y_at_ndx(i) * src.y_at_ndx(i) * diff;

    for (std::size_t i = 0; i < src.length(); i++) {
        const double c = 100 * src.y_at_ndx(i) * diff;

        value.x += c * obs.get_x().y_at_ndx(i);
        value.y += c * obs.get_y().y_at_ndx(i);
        value.z += c * obs.get_z().y_at_ndx(i);
    }

    value.x /= n;
    value.y /= n;
    value.z /= n;

    return value;
}


