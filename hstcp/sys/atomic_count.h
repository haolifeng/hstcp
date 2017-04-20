/*
 * atomic_count.h
 *
 *  Created on: 2017年4月14日
 *      Author: hlf
 */

#ifndef ATOMIC_COUNT_H_
#define ATOMIC_COUNT_H_
#include <ext/atomicity.h>
using __gnu_cxx::__atomic_add;
using __gnu_cxx::__exchange_and_add;

class atomic_count
{
public:
	atomic_count() : value_(0) {}

	explicit atomic_count( long v ) : value_( static_cast< int >( v ) ) {}

	long operator++()
	{
		return __exchange_and_add( &value_, +1 ) + 1;
	}

	long operator--()
	{
		return __exchange_and_add( &value_, -1 ) - 1;
	}

	long add(long n)
	{
		return __exchange_and_add( &value_, n ) + n;
	}

	operator long() const
	{
		return __exchange_and_add( &value_, 0 );
	}

private:

	atomic_count(atomic_count const &);
	atomic_count & operator=(atomic_count const &);

	mutable _Atomic_word value_;
};




#endif /* ATOMIC_COUNT_H_ */
