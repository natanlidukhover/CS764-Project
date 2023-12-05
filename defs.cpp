#include <stdio.h>
#include <stdlib.h>

#include "defs.h"
#include "Table.h"
#include <fstream>
using namespace std;
// -----------------------------------------------------------------

Trace::Trace (bool const trace, char const * const function,
		char const * const file, int const line )
	: _output (trace), _function (function), _file (file), _line (line)
{
	_trace (">>>>>");
} // Trace::Trace

Trace::~Trace ()
{
	_trace ("<<<<<");
} // Trace::~Trace

void Trace::_trace (char const lead [])
{
	if (_output)
	{
		//printf ("%s %s (%s:%d)\n", lead, _function, _file, _line);
		outTrace << lead <<  " " << _function << " " << _file << " " << _line << endl;
	}
		
} // Trace::_trace

// -----------------------------------------------------------------

size_t Random (size_t const range)
{
	return (size_t) rand () % range;
} // Random

size_t Random (size_t const low_incl, size_t const high_incl)
{
	return low_incl + (size_t) rand () % (high_incl - low_incl + 1);
} // Random

size_t RoundDown (size_t const x, size_t const y)
{
	return x - (x % y);
} // RoundDown

size_t RoundUp (size_t const x, size_t const y)
{
	size_t const z = x % y;
	return (z == 0 ? x : x + y - z);
} // RoundUp

bool IsPowerOf2 (size_t const x)
{
	return x > 0 && (x & (x - 1)) == 0;
} // IsPowerOf2

size_t lsb (size_t const x)
{
	size_t const y = x & (x - 1);
	return x ^ y;
} // lsb

size_t msb (size_t const x)
{
	size_t y = x;
	for (size_t z;  z = y & (y - 1), z != 0;  y = z)
		; // nothing
	return y;
} // msb

int msbi (size_t const x)
{
	int i = 0;
	for (size_t z = 2;  z <= x;  ++ i, z <<= 1)
		; // nothing
	return i;
} // msbi

char const * YesNo (bool const b)
{
	return b ? "Yes" : "No";
} // YesNo

char const * OkBad (bool const b)
{
	return b ? "Ok" : "Bad";
} // OkBad

/**
 * Offset-value code calculation for a row given its previous row, the domain, the arity, and if the sort is ascending
 * For first row, input a row of zeros
 * 
 * @param prevRow is the pointer to the Row of records before the one for which the OVC is being calculated
 * @param currentRow is the pointer to the Row of records for which the OVC is being calculated
 * @param domain is the size_t representing the number of values that can be placed as a Record
 * @param artiy is the size_t representing the number of columns
*/
unsigned short calculateOffsetValueCode(Table t, size_t prevRow, size_t
		currentRow, size_t domain,  size_t arity, bool isAscending) {
	unsigned short offset = t._rowSize;
	unsigned short value = domain;
	for (unsigned short i = 0; i < t._rowSize; i++) {
		if (t[currentRow][i] != t[prevRow][i]) {
			offset = i;
			value = t[currentRow][i];
			break;
		}
	}

	if (isAscending) {
		offset = arity - offset;
	} else {
		value = domain - value;
	}

	// E.g. domain is 1 to 99 -> 100; offset is 3; value is 94 (domain 100 - actual value of 6);
	// zerosInDomain = floor(log10(abs(100))) = floor(2) = 2;
	// offsetValueCode = offset * 10^2 + value = 3 * 100 + 94 = 300 + 94 = 394
	unsigned short zerosInDomain = floor(log10(domain));
	unsigned short offsetValueCode = offset * pow(10, zerosInDomain);
	return offsetValueCode + value;
}
