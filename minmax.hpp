#pragma once


class MinMax
{
public:
	int min_, max_;

	MinMax() = delete; //Default Constructor is deleted for Initilization safety
	
	MinMax(int minx, int maxx): min_(minx), max_(maxx)
	{}

	//Only left bounds are included
	bool within_bounds(const int pos) const __attribute__((always_inline))
	{
		return ((pos >= min_) && (pos < max_));
	}

	void set_bounds(int minx, int maxx) __attribute__((always_inline))
	{
		this->min_ = minx;
		this->max_ = maxx;
	}

};
