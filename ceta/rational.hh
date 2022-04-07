/** \file
 * Rational numbers (based on boost::rational).
 */

#ifndef _rational_h_
#define _rational_h_

#include <numeric>

namespace ceta {

template<typename T>
class rational_t {
	T num, denom;

public:
	rational_t(T num = 0) : num(num), denom(1) {}

	rational_t(T num, T denom) : num(num), denom(denom) {
		normalize();
	}

	rational_t(const rational_t& other) = default;

	void normalize() {
		if (num == 0)
			denom = 1;
		else {
			auto gcd_nd = std::gcd(num, denom);
			num /= gcd_nd;
			denom /= gcd_nd;

			if (denom < 0) {
				num = -num;
				denom = -denom;
			}
		}
	}

	bool operator ==(const rational_t &other) const {
		return num == other.num && denom == other.denom;
	}

	bool operator !=(const rational_t &other) const {
		return !(*this == other);
	}

	const rational_t& operator +=(const rational_t &other) {
		auto gcd_d = std::gcd(denom, other.denom);
		denom /= gcd_d;
		num = num * (other.denom / gcd_d) + other.num * denom;
		gcd_d = std::gcd(num, gcd_d);
		num /= gcd_d;
		denom *= other.denom / gcd_d;
		return *this;
	}

	const rational_t& operator -=(const rational_t &other) {
		auto gcd_d = std::gcd(denom, other.denom);
		denom /= gcd_d;
		num = num * (other.denom / gcd_d) - other.num * denom;
		gcd_d = std::gcd(num, gcd_d);
		num /= gcd_d;
		denom *= other.denom / gcd_d;
		return *this;
	}

	const rational_t& operator *=(const rational_t &other) {
		auto gcd1 = std::gcd(num, other.denom);
		auto gcd2 = std::gcd(other.num, denom);
		num = (num / gcd1) * (other.num / gcd2);
		denom = (denom / gcd2) * (other.denom / gcd1);
		return *this;
	}

	const rational_t& operator /=(const rational_t &other) {
		if (num == 0)
			return *this;

		// Avoid overflow and preserve normalization
		auto gcd_n = std::gcd(num, other.num);
		auto gcd_d = std::gcd(denom, other.denom);
		num = (num / gcd_n) * (other.denom / gcd_d);
		denom = (denom / gcd_d) * (other.num / gcd_n);

		if (denom < 0) {
			num = -num;
			denom = -denom;
		}
		return *this;
	}

	rational_t operator -() const {
		return rational_t(-num, denom);
	}

	rational_t operator +(const rational_t &other) const {
		rational_t res = *this;
		return res += other;
	}

	rational_t operator -(const rational_t &other) const {
		rational_t res = *this;
		return res -= other;
	}

	rational_t operator *(const rational_t &other) const {
		rational_t res = *this;
		return res *= other;
	}

	rational_t operator /(const rational_t &other) const {
		rational_t res = *this;
		return res /= other;
	}

	bool operator <(const rational_t &other) const {
		return num * other.denom < other.num * denom;
	}

	bool operator >(const rational_t &other) const {
		return other < *this;
	}

	bool operator <=(const rational_t &other) const {
		return *this == other || *this < other;
	}

	T numerator() const {
		return num;
	}

	T denominator() const {
		return denom;
	}
};

template<typename T>
std::ostream& operator <<(std::ostream& out, const rational_t<T>& data) {
	return (out << data.numerator() << "/" << data.denominator());
}

}

#endif // _rational_h_
