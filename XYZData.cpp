#include "XYZData.h"

#include "algo.h"

#include <algorithm>
#include <sstream>




XYZData::XYZData(void)
{

}

XYZData::XYZData(const XYZData& other)
{
	data_ = other.data_;
}

XYZData::XYZData(XYZData&& other)
{
	swap(*this, other);
}

XYZData::~XYZData(void)
{

}

XYZData& XYZData::operator=(XYZData other)
{
	swap(*this, other);
	return *this;
}	

void swap(XYZData& first, XYZData& second)
{
	std::swap(first.data_, second.data_);
}

bool operator==(const XYZData::Data3& lhs, const XYZData::Data3& rhs)
{
	return (lhs.x == rhs.x) and (lhs.y == rhs.y) and (lhs.z == rhs.z);
}

bool operator!=(const XYZData::Data3& lhs, const XYZData::Data3& rhs)
{
	return !(lhs == rhs);
}

bool operator==(const XYZData& lhs, const XYZData& rhs)
{
	return lhs.data_ == rhs.data_;
}

bool operator!=(const XYZData& lhs, const XYZData& rhs)
{
	return lhs.data_ != rhs.data_;
}

bool XYZData::isXEqual(XYZData& other)
{
	if (length() != other.length())
		return false;
	for (std::size_t i = 0; i < length(); i++) {
		if (data_[i].x != other.data_[i].x)
			return false;
	}
	return true;
}

int XYZData::ndx_of_x(double x) const
{
	for (std::size_t i = 0; i < data_.size(); i++) {
		if (algo::GetFloatPrecision(x) == algo::GetFloatPrecision(data_[i].x))
			return i;
	}
	return -1;
}

XYZData::Data3 XYZData::operator[](double x) const
{
	try {
		return at(x);
	} catch (const std::out_of_range& exp) {
		std::cerr << exp.what() << std::endl;
	}

	return XYZData::Data3 {0, 0, 0};
}

XYZData::Data3 XYZData::at(double x) const
{
	int ndx_x = ndx_of_x(x);
	if (ndx_x < 0)
		throw std::out_of_range(std::string("Index not in range"));
	return data_[ndx_x];
}

bool XYZData::has_x(double x) const
{
	return ndx_of_x(x) > 0;
}

double XYZData::y_at_x(double x) const
{
	return operator[](x).y;
}

double XYZData::z_at_x(double x) const
{
	return operator[](x).z;
}

void XYZData::insert(double x, double y, double z)
{
	data_.push_back(Data3 { x, y, z});
}

XYZData& XYZData::addToX(double off)
{
	for (auto& d: data_) 
		d.x += off;
	return *this;
}

XYZData& XYZData::addToY(double off)
{
	for (auto& d: data_) 
		d.y += off;
	return *this;
}

XYZData& XYZData::addToZ(double off)
{
	for (auto& d: data_) 
		d.z += off;
	return *this;
}

XYZData& XYZData::scaleX(double f)
{
	for (auto& d: data_) 
		d.x *= f;
	return *this;
}

XYZData& XYZData::scaleY(double f)
{
	for (auto& d: data_) 
		d.y *= f;
	return *this;
}


XYZData& XYZData::scaleZ(double f)
{
	for (auto& d: data_) 
		d.z *= f;
	return *this;
}

std::vector<double> XYZData::toXVector(void) const
{
	std::vector<double> x;
	for (auto& d: data_)
		x.push_back(d.x);

	return x;
}

std::vector<double> XYZData::toYVector(void) const
{
	std::vector<double> y;
	for (auto& d: data_)
		y.push_back(d.y);

	return y;
}


std::vector<double> XYZData::toZVector(void) const
{
	std::vector<double> z;
	for (auto& d: data_)
		z.push_back(d.z);

	return z;
}

void XYZData::toXYZVector(std::vector<double>& x, std::vector<double>& y, std::vector<double>& z) const
{
	x.clear(); y.clear(); z.clear();

	for (auto& d: data_) {
		x.push_back(d.x);
		y.push_back(d.y);
		z.push_back(d.z);
	}
}

void XYZData::toXYVector(std::vector<double>& x, std::vector<double>& y) const
{
	x.clear(); y.clear();

	for (auto& d: data_) {
		x.push_back(d.x);
		y.push_back(d.y);
	}
}

void XYZData::toXZVector(std::vector<double>& x, std::vector<double>& z) const
{
	x.clear(); z.clear();

	for (auto& d: data_) {
		x.push_back(d.x);
		z.push_back(d.z);
	}
}

bool XYZData::fromVector(const std::vector<double>& x, const std::vector<double>& y, const std::vector<double>& z)
{
	if (x.size() != y.size())
		return false;
	if (y.size() != z.size())
		return false;

	int size = x.size();

	clear();

	for (int i = 0; i < size; i++)
		data_.push_back(Data3 { x[i], y[i], z[i] });

	return true;
}

std::string XYZData::toString(void) const
{
	std::stringstream stream;
	for (auto& d: data_)
		stream << algo::GetFloatPrecision(d.x) << "," << algo::GetFloatPrecision(d.y) << "," << algo::GetFloatPrecision(d.z) << std::endl;

	return stream.str();
}