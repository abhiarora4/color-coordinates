#ifndef XYZDATA_H
#define XYZDATA_H

#include <stdint.h>
#include <stdbool.h>
#include <ctype.h>

#include <vector>
#include <string>


class XYZData {
public:
	struct Data3 {
		double x;
		double y;
		double z;
	};

public:
	Data3 operator[](double x) const;
	Data3 at(double x) const;

	bool has_x(double x) const;

	double y_at_x(double x) const;
	double z_at_x(double x) const;

	void insert(double x, double y, double z);

	std::size_t length(void) const 				{ return data_.size(); }
	bool empty(void) const 						{ return (length() == 0); }

public:
	void sort(void);
	void clear(void)							{ data_.clear(); }

public:
	XYZData& scaleX(double f);
	XYZData& scaleY(double f);
	XYZData& scaleZ(double f);

	XYZData& addToX(double off);
	XYZData& addToY(double off);
	XYZData& addToZ(double off);

	double firstX(void) const 					{ return data_.front().x; }
	double lastX(void) const 					{ return data_.back().x; }

	std::vector<double> toXVector(void) const;
	std::vector<double> toYVector(void) const;
	std::vector<double> toZVector(void) const;

	void toXYZVector(std::vector<double>& x, std::vector<double>& y, std::vector<double>& z) const;
	void toXYVector(std::vector<double>& x, std::vector<double>& y) const;
	void toXZVector(std::vector<double>& x, std::vector<double>& z) const;
	bool fromVector(const std::vector<double>& x, const std::vector<double>& y, const std::vector<double>& z);

	std::string toString(void) const;

public:
	std::vector<Data3>::iterator begin(void) 			{ return data_.begin(); }
	std::vector<Data3>::iterator end(void) 				{ return data_.end(); }

	std::vector<Data3>::const_iterator cbegin(void) 	{ return data_.cbegin(); }
	std::vector<Data3>::const_iterator cend(void) 		{ return data_.cend(); }

protected:
	int ndx_of_x(double x) const;


public:
	XYZData& operator=(XYZData other);

	friend void swap(XYZData& first, XYZData& second);

	friend bool operator==(const XYZData& lhs, const XYZData& rhs);
	friend bool operator!=(const XYZData& lhs, const XYZData& rhs);

	friend bool operator==(const Data3& lhs, const Data3& rhs);
	friend bool operator!=(const Data3& lhs, const Data3& rhs);

	bool isXEqual(XYZData& other);

public:
	XYZData(void);
	XYZData(const XYZData& other);
	XYZData(XYZData&& other);

	~XYZData(void);

private:
	std::vector<Data3> data_;
};


#endif /* XYZDATA_H */