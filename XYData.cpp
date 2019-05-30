#include "XYData.h"
#include "color.h"
#include "algo.h"

#include <iostream>

#include <fstream>

#include <stdio.h>

#include <filesystem>
#include <math.h>
#include <algorithm>
#include <sstream>

// TODO:
// 1. Sort
// 2. Redirection operator overload
// 3. toString
// 4. Algrebra operator overload

// 5. Functions to trim
// 6. Functions to interpolate
// 7. Search Functions
// 8. Intializer_list
// 9. Iterators
// 10. Remove Methods: Single and range
// 11. has_x
// 12. Better search algorithms? Binary Search!
// 13. upper bound and lower bound
// 14. Fill X from a range with same Y


// Default Constructor
XYData::XYData(void)
{

}

// Parameterized Constructor
XYData::XYData(const std::vector<double>& x, const std::vector<double>& y)
{
	// std::cout << "Parameterized Constructor" << std::endl;
	fromVector(x, y);
}

// Copy Constructor
XYData::XYData(const XYData& other)
{
	// std::cout << "Copy Constructor" << std::endl;
	data_ = other.data_;
}

// Move Constructor
XYData::XYData(XYData&& other)
{
	// std::cout << "Move Constructor" << std::endl;
	swap(*this, other);
}

XYData::XYData(const std::initializer_list<double>& list)
{
	std::size_t size = list.size() & (~(std::size_t(1) << 0));
	if (size < 2)
		return;

	for (std::size_t i = 0; i < size; i+=2)
		data_.push_back(std::make_pair(*(list.begin() + i), *(list.begin() + i + 1)));
}

XYData::XYData(const std::initializer_list<std::initializer_list<double>>& list)
{
	auto ptr = list.begin();
	auto end = list.end();
	for (; ptr != end; ptr++) {
		auto current = *ptr;
		if (current.size() != 2)
			continue;
		data_.push_back(std::make_pair(*current.begin(), *(current.begin() + 1)));
	}
}

// Destructor
XYData::~XYData(void)
{
	// std::cout << "Destructor" << std::endl;
}

void XYData::fromVector(const std::vector<double>& x, const std::vector<double>& y)
{
	if (x.size() == 0)
		return;
	if (x.size() != y.size())
		return;

	data_.clear();

	for (size_t i = 0; i < x.size(); i++)
		data_.push_back(std::make_pair(x[i], y[i]));

	sort();
}

bool XYData::fromFile(const std::string& filename)
{
	std::string line;
	std::ifstream stream;
	stream.open(filename, std::ios::in);

	if (!stream.is_open())
		return false;

	getline(stream, firstLine_);
	while (stream >> *this);
	return true;
}


bool XYData::toFile(const std::string& filename) const
{
	std::ofstream stream;
	stream.open(filename, std::ios::out);

	if (!stream.is_open())
		return false;

	stream << firstLine_ << std::endl;
	stream << *this;
	return true;
}

XYData& XYData::scaleX(double f)
{
	for (auto& p: data_)
		p.first *= f;

	return *this;
}

XYData& XYData::scaleY(double f)
{
	for (auto& p: data_)
		p.second *= f;

	return *this;
}

std::vector<double> XYData::toXVector(void) const
{
	std::vector<double> v;
	for (auto& p: data_)
		v.push_back(algo::GetFloatPrecision(p.first));

	return v;
}

std::vector<double> XYData::toYVector(void) const
{
	std::vector<double> v;
	for (auto& p: data_)
		v.push_back(p.second);

	return v;
}

void XYData::toVector(std::vector<double>& x, std::vector<double>& y) const
{
	x = toXVector();
	y = toYVector();	
}


std::string XYData::toString(void) const
{
	std::stringstream stream;
	for (auto& p: data_)
		stream << algo::GetFloatPrecision(p.first) << "," << p.second << std::endl;

	return stream.str();
}

void XYData::insert(double x, double y)
{
	data_.push_back(std::make_pair(x, y));
}

void XYData::sort(void)
{
	auto comparator = [](std::pair<double,double> v1, std::pair<double,double> v2) -> bool 
	{ return algo::GetFloatPrecision(v1.first) < algo::GetFloatPrecision(v2.first); };
	std::sort(data_.begin(), data_.end(), comparator);
}

void swap(XYData& first, XYData& second)
{
	// Swap of each data member
	std::swap(first.data_, second.data_);
}

std::istream& operator>>(std::istream& stream, XYData& obj)
{
	double w, v;
	std::string line;
	getline(stream, line);

	if (sscanf(line.c_str(), " %lf , %lf\r\n", &w, &v) == 2)
		obj.insert(w, v);

	return stream;
}

std::ostream& operator<<(std::ostream& stream, const XYData& obj)
{
	stream << obj.toString();
	return stream;
}

XYData operator+(const XYData& l, const XYData& r)
{
	XYData result;
	if (!isXEqual(l, r))
		return result;

	for (size_t i = 0; i < l.length(); i++)
		result.insert(l.data_[i].first, l.data_[i].second + r.data_[i].second);
	
	return result;
}

XYData operator-(const XYData& l, const XYData& r)
{
	XYData result;
	if (!isXEqual(l, r))
		return result;

	for (size_t i = 0; i < l.length(); i++)
		result.insert(l.data_[i].first, l.data_[i].second - r.data_[i].second);
	
	return result;
}

XYData operator*(const XYData& l, const XYData& r)
{
	XYData result;
	if (!isXEqual(l, r))
		return result;

	for (size_t i = 0; i < l.length(); i++)
		result.insert(l.data_[i].first, l.data_[i].second * r.data_[i].second);
	
	return result;
}

XYData operator/(const XYData& l, const XYData& r)
{
	XYData result;
	if (!isXEqual(l, r))
		return result;

	for (size_t i = 0; i < l.length(); i++)
		result.insert(l.data_[i].first, l.data_[i].second / r.data_[i].second);
	
	return result;
}

XYData operator+(const XYData& l, const double& r)
{
	XYData result;
	for (size_t i = 0; i < l.length(); i++)
		result.insert(l.data_[i].first, l.data_[i].second + r);
	
	return result;
}

XYData operator-(const XYData& l, const double& r)
{
	XYData result;
	for (size_t i = 0; i < l.length(); i++)
		result.insert(l.data_[i].first, l.data_[i].second - r);
	
	return result;
}

XYData operator*(const XYData& l, const double& r)
{
	XYData result;
	for (size_t i = 0; i < l.length(); i++)
		result.insert(l.data_[i].first, l.data_[i].second * r);
	
	return result;
}

XYData operator/(const XYData& l, const double& r)
{
	XYData result;
	for (size_t i = 0; i < l.length(); i++)
		result.insert(l.data_[i].first, l.data_[i].second / r);
	
	return result;
}

XYData operator+(const double& l, const XYData& r)
{
	return operator+(r, l);
}

XYData operator-(const double& l, const XYData& r)
{
	return operator-(r, l);
}

XYData operator*(const double& l, const XYData& r)
{
	return operator*(r, l);
}

XYData operator/(const double& l, const XYData& r)
{
	return operator/(r, l);
}

bool operator==(const XYData& first, const XYData& second)
{
	return (first.data_ == second.data_);

}

bool operator!=(const XYData& first, const XYData& second)
{
	return (first.data_ != second.data_);
}

bool isXEqual(const XYData& first, const XYData& second)
{
	if (first.length() != second.length())
		return false;

	for (size_t i = 0; i < first.length(); i++) {
		if (algo::GetFloatPrecision(first.data_[i].first) != algo::GetFloatPrecision(second.data_[i].first))
			return false;
	}

	return true;
}

void XYData::erase(double x)
{
	long ndx = ndx_of_x(x);
	if (ndx < 0)
		return;
	data_.erase(data_.begin() + ndx);
}

void XYData::erase(double xStart, double xStop)
{
	for (auto ptr = data_.begin(); ptr != data_.end();) {
		double current = ptr->first;
		if ((current >= xStart) and (current <= xStop))
			ptr = data_.erase(ptr);
		else
			ptr++;
	}
}

bool XYData::has_x(double x) const
{
	return (ndx_of_x(x) >= 0);
}

double XYData::x_at_ndx(size_t ndx) const
{
	if (ndx >= data_.size())
		return 0;
	return algo::GetFloatPrecision(data_[ndx].first);
}

double XYData::y_at_ndx(size_t ndx) const
{
	if (ndx >= data_.size())
		return 0;
	return data_[ndx].second;
}

long XYData::ndx_of_x(double x) const
{
	for (std::size_t i = 0; i < data_.size(); i++) {
		if (algo::GetFloatPrecision(x) == algo::GetFloatPrecision(data_[i].first))
			return i;
	}
	return -1;
}

XYData& XYData::operator=(XYData other)
{
	// std::cout << "Copy and Swap Idiom" << std::endl;
	swap(*this, other);
	return *this;
}

double XYData::operator[](const double& x)
{
	return at(x);
}

double XYData::at(const double& x) const
{
	for (auto& p: data_) {
		if (algo::GetFloatPrecision(p.first) == algo::GetFloatPrecision(x))
			return p.second;
	}

	return 0.0;
}

double XYData::y_at_x(const double& x) const
{
	return at(x);
}

void scaleXOfFiles()
{
	for (auto& dirEntry: std::filesystem::recursive_directory_iterator("./csv/cmfs")) {
		if (!dirEntry.is_regular_file())
			continue;
		std::filesystem::path file = dirEntry.path();
		if (file.extension() != ".csv")
			continue;
		XYData data;
		data.fromFile(file);
		if (data.firstX() > 1.0) {
			std::cout << "Good File: " << file << std::endl;
			continue;
		}
		data.scaleX(1000000000);
		std::cout << "File: " << file.filename() << " Data: " << data.firstX() << std::endl;
		data.toFile(file);
	}
}

int main(int argc, char *argv[])
{
	// ColorObserver obs("./csv/cmfs/CIE 1931 2 Degree Standard Observer");
	Color c("./../csv");

	return 0;

	XYData d65;
	d65.fromFile("./csv/illuminants/D65.csv");

	d65 = algo::interpolate(d65, 360, 830, 1);

	XYData obs2_x;
	obs2_x.fromFile("./csv/cmfs/CIE 1931 2 Degree Standard Observer/x_bar.csv");
	// obs2_x = algo::interpolate(obs2_x, 360, 780, 5);

	XYData obs2_y;
	obs2_y.fromFile("./csv/cmfs/CIE 1931 2 Degree Standard Observer/y_bar.csv");
	// obs2_y = algo::interpolate(obs2_y, 360, 780, 5);

	XYData obs2_z;
	obs2_z.fromFile("csv/cmfs/CIE 1931 2 Degree Standard Observer/z_bar.csv");
	// obs2_z = algo::interpolate(obs2_z, 360, 780, 5);

	std::cout << "----------------D65----------------" << std::endl;
	std::cout << d65;

	std::cout << "----------------2 Observer----------------" << std::endl;
	std::cout << obs2_x;

	double x, y, z;
	algo::reference_XYZ(d65, obs2_x, obs2_y, obs2_z, x, y, z);
	std::cout << "X: " << x << " Y: " << y << " Z: " << z << std::endl;

	return 0;
	// std::cout << s << " - " << std::endl;
	XYData s2 = {1, 0.1, 2, 0.2, 3, 0.3, 4, 0.4, 5};
	XYData s3 = {{1, 0.1}, { 2, 0.2}, {3, 0.3}, {4, 0.4}};

	// XYData s4 = algo::trim(s, 301, 400);
	std::cout << algo::interpolate(d65, 290, 310, 1, false);
	std::cout << algo::interpolate(d65, 290, 310, 1, true);

	// std::cout << s2 << " - " << std::endl;
	// std::cout << s3 << " - " << std::endl;

	// auto pair = intersect(s, s2);

	// std::cout << pair.first;
	return 0;


	XYData s1(std::move(d65));

	std::cout << "-" << std::endl;
	XYData a = s1;
	std::cout << "-" << std::endl;
	// a = a;
	a = std::move(s1);
	std::cout << "-" << std::endl;

	// s.insert(400, 0.3);
	// s.insert(410, 0.1);
	// s.insert(405, 0.2);

	// s.sort();

	// s.toFile("./example.csv");

	return 0;
}