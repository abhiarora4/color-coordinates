#ifndef XYData_H
#define XYData_H

#include <istream>
#include <ostream>

#include <vector>
#include <string>

class XYData {

public:
	// Redirection operator

	friend std::istream& operator>>(std::istream& stream, XYData& obj);
	friend std::ostream& operator<<(std::ostream& stream, const XYData& obj);

public:

	friend void swap(XYData& first, XYData& second);

	friend XYData operator+(const XYData& l, const XYData& r);
	friend XYData operator-(const XYData& l, const XYData& r);
	friend XYData operator*(const XYData& l, const XYData& r);
	friend XYData operator/(const XYData& l, const XYData& r);

	friend XYData operator+(const XYData& l, const double& r);
	friend XYData operator-(const XYData& l, const double& r);
	friend XYData operator*(const XYData& l, const double& r);
	friend XYData operator/(const XYData& l, const double& r);

	friend XYData operator+(const double& l, const XYData& r);
	friend XYData operator-(const double& l, const XYData& r);
	friend XYData operator*(const double& l, const XYData& r);
	friend XYData operator/(const double& l, const XYData& r);

	friend bool operator==(const XYData& l, const XYData& r);
	friend bool operator!=(const XYData& l, const XYData& r);

public:
	friend bool isXEqual(const XYData& s1, const XYData& s2);

public:
	// Copy Assignment Operator
	XYData& operator=(XYData other);

	double operator[](const double& x);
	double at(const double& x) const;

public:
	void fromVector(const std::vector<double>& x, const std::vector<double>& y);
	bool fromFile(const std::string& filename);

	XYData& scaleX(double f);
	XYData& scaleY(double f);

	double firstX(void) const 	{ return data_.front().first; }
	double lastX(void) const 	{ return data_.back().first; }

	std::vector<double> toXVector(void) const;
	std::vector<double> toYVector(void) const;
	bool toFile(const std::string& filename) const;
	void toVector(std::vector<double>& x, std::vector<double>& y) const;

	std::string toString(void) const;

	void insert(double x, double y);

	std::size_t length(void) const { return data_.size(); }

	bool isXEqual(void) const;

	void erase(double x);
	void erase(double xStart, double xStop);

	bool has_x(double x) const;
	double x_at_ndx(size_t ndx) const;
	double y_at_ndx(size_t ndx) const;
	double y_at_x(const double& x) const;

	void sort(void);

protected:
	long ndx_of_x(double x) const;


public:
	XYData(void);
	XYData(const std::vector<double>& x, const std::vector<double>& y);
	XYData(const XYData& other);
	XYData(XYData&& other);

	XYData(const std::initializer_list<double>& list);
	XYData(const std::initializer_list<std::initializer_list<double>>& list);

	~XYData(void);

private:
	std::string firstLine_;
	std::vector<std::pair<double,double>> data_;
};

#endif