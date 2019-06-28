#ifndef XYData_H
#define XYData_H

#include <iostream>
#include <istream>
#include <ostream>

#include <vector>
#include <string>
#include <iterator>

class XYData;

// Post inc, Pre inc / Dec
// +, -, two iterator subtraction
// <, >, <=, >=, +=, -=, [], *, ->
class Iterator: public std::iterator<std::random_access_iterator_tag, double> {
public:
	friend void operator++(Iterator& i)
	{
		i.iter_++;
	}
	friend void operator++(Iterator& i, int)
	{	
		i.iter_++;
	}

	friend void operator--(Iterator& i)
	{
		i.iter_--;
	}
	friend void operator--(Iterator& i, int)
	{	
		i.iter_--;
	}

public:
	friend Iterator operator+(Iterator& i, int offset)
	{	
		return Iterator(i.iter_ + offset);
	}
	friend Iterator operator+(int offset, Iterator& i)
	{	
		return (i + offset);
	}

	friend Iterator operator-(const Iterator& i, int offset)
	{	
		return Iterator(i.iter_ - offset);
	}
	friend Iterator operator-(int offset, const Iterator& i)
	{	
		return (i - offset);
	}

	friend difference_type operator-(const Iterator& first, const Iterator& second)
	{	
		return (first.iter_ - second.iter_);
	}

	Iterator& operator+=(const int& offset)
	{
		iter_ += offset;
		return *this;
	}
	Iterator& operator-=(const int& offset)
	{
		iter_ -= offset;
		return *this;
	}


public:
	friend bool operator==(const Iterator& first, const Iterator& second)
	{
		return (first.iter_ == second.iter_);
	}
	friend bool operator!=(const Iterator& first, const Iterator& second)
	{
		return !(first == second);
	}

	friend bool operator<(const Iterator& first, const Iterator& second)
	{
		return first.iter_ < second.iter_;
	}
	friend bool operator>(const Iterator& first, const Iterator& second)
	{
		return first.iter_ > second.iter_;
	}

	friend bool operator<=(const Iterator& first, const Iterator& second)
	{
		return first.iter_ <= second.iter_;
	}
	friend bool operator>=(const Iterator& first, const Iterator& second)
	{
		return first.iter_ >= second.iter_;
	}

public:
	friend reference operator*(Iterator& i);
	reference operator->(void);
	
	reference operator[](int offset);

public:
	Iterator(std::vector<std::pair<double,double>>::iterator in): iter_(in)
	{
		
	}
	~Iterator(void) { }

private:
	std::vector<std::pair<double,double>>::iterator iter_;
};

class CIterator: public std::iterator<std::random_access_iterator_tag, const double> {

public:
	friend void operator++(CIterator& i)
	{
		i.iter_++;
	}
	friend void operator++(CIterator& i, int)
	{	
		i.iter_++;
	}

	friend void operator--(CIterator& i)
	{
		i.iter_--;
	}
	friend void operator--(CIterator& i, int)
	{	
		i.iter_--;
	}

public:
	friend CIterator operator+(CIterator& i, int offset)
	{	
		return CIterator(i.iter_ + offset);
	}
	friend CIterator operator+(int offset, CIterator& i)
	{	
		return (i + offset);
	}

	friend CIterator operator-(const CIterator& i, int offset)
	{	
		return CIterator(i.iter_ - offset);
	}
	friend CIterator operator-(int offset, const CIterator& i)
	{	
		return (i - offset);
	}

	friend difference_type operator-(const CIterator& first, const CIterator& second)
	{	
		return (first.iter_ - second.iter_);
	}

	CIterator& operator+=(const int& offset)
	{
		iter_ += offset;
		return *this;
	}
	CIterator& operator-=(const int& offset)
	{
		iter_ -= offset;
		return *this;
	}


public:
	friend bool operator==(const CIterator& first, const CIterator& second)
	{
		return (first.iter_ == second.iter_);
	}
	friend bool operator!=(const CIterator& first, const CIterator& second)
	{
		return !(first == second);
	}

	friend bool operator<(const CIterator& first, const CIterator& second)
	{
		return first.iter_ < second.iter_;
	}
	friend bool operator>(const CIterator& first, const CIterator& second)
	{
		return first.iter_ > second.iter_;
	}

	friend bool operator<=(const CIterator& first, const CIterator& second)
	{
		return first.iter_ <= second.iter_;
	}
	friend bool operator>=(const CIterator& first, const CIterator& second)
	{
		return first.iter_ >= second.iter_;
	}

public:
	friend reference operator*(CIterator& i);
	reference operator->(void);
	
	reference operator[](int offset);

public:
	CIterator(std::vector<std::pair<double,double>>::const_iterator in): iter_(in)
	{
		
	}
	~CIterator(void) { }

private:
	std::vector<std::pair<double,double>>::const_iterator iter_;
};



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

	void clear(void);

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
	bool empty(void) const { return (length() == 0); }

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
	Iterator begin(void) { return Iterator(data_.begin()); }
	Iterator end(void) { return Iterator(data_.end()); }

	CIterator cbegin(void) { return CIterator(data_.cbegin()); }
	CIterator cend(void) { return CIterator(data_.cend()); }

public:
	std::pair<double,double>& at_ndx(unsigned ndx) { return data_.at(ndx); } 

public:
	friend class Iterator;

public:
	XYData(void);
	XYData(const std::vector<double>& x, const std::vector<double>& y);
	XYData(const std::string& filename);
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