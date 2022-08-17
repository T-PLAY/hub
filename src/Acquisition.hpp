#pragma once

#include <iostream>

#include "export.h"

class SRC_API Acquisition {
public:
	Acquisition(long long backendTimestamp, long long backendTimeOfArrival, const unsigned char* const data, size_t size);
	~Acquisition();

	Acquisition(const Acquisition& acq) = delete;
	Acquisition(Acquisition&& acq) noexcept;

	Acquisition& operator=(const Acquisition& acq) = delete;
	Acquisition& operator=(Acquisition&& acq) = delete;

public:
	Acquisition clone() const;

public:
	const long long mBackendTimestamp; // microseconds
	const long long mBackendTimeOfArrival; // microseconds
	const unsigned char* const mData;
	const size_t mSize;

private:
	bool mIsMoved = false;

public:
	SRC_API friend std::ostream& operator<<(std::ostream& os, const Acquisition& acq);
};

