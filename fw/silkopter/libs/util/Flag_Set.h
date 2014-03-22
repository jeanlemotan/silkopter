//#include <iostream>
#include <numeric>
#include "util/FString.h"

namespace util
{


template <typename enumT, typename storeT>
class Flag_Set
{
    public:

        typedef enumT                   enum_type;
        typedef storeT					store_type;

        // Default constructor (all 0s)
        Flag_Set() : flags_(store_type(0)) {}
		Flag_Set(enumT v0) : flags_(store_type(v0)) {}
		Flag_Set(enumT v0, enumT v1) : flags_(store_type(v0) | store_type(v1)) {}
		Flag_Set(enumT v0, enumT v1, enumT v2) : flags_(store_type(v0) | store_type(v1) | store_type(v2)) {}
		Flag_Set(enumT v0, enumT v1, enumT v2, enumT v3) : flags_(store_type(v0) | store_type(v1) | store_type(v2) | store_type(v3)) {}
		Flag_Set(enumT v0, enumT v1, enumT v2, enumT v3, enumT v4) : flags_(store_type(v0) | store_type(v1) | store_type(v2) | store_type(v3) | store_type(v4)) {}
		Flag_Set(enumT v0, enumT v1, enumT v2, enumT v3, enumT v4, enumT v5) : flags_(store_type(v0) | store_type(v1) | store_type(v2) | store_type(v3) | store_type(v4) | store_type(v5)) {}
		Flag_Set(enumT v0, enumT v1, enumT v2, enumT v3, enumT v4, enumT v5, enumT v6) : flags_(store_type(v0) | store_type(v1) | store_type(v2) | store_type(v3) | store_type(v4) | store_type(v5) | store_type(v6)) {}

        // Value constructor
        explicit Flag_Set(store_type value) : flags_(value)
        {

        }

        // Explicit conversion operator
        operator store_type() const
        {
            return flags_;
        }

        bool operator [] (enum_type flag) const
        {
            return test(flag);
        }

		template<size_t SIZE>
        void to_string(FString<SIZE>& dst) const
        {
			dst.resize(size());
            for (size_t x = 0; x < size(); ++x)
            {
                dst[size() - x - 1] = (flags_ & (1 << x) ? '1' : '0');
            }
        }

        Flag_Set& set()
        {
            flags_ = ~store_type(0);
            return *this;
        }

        Flag_Set& set(enum_type flag, bool val = true)
        {
            flags_ = (val ? (flags_|static_cast<store_type>(flag)) : (flags_&~static_cast<store_type>(flag)));
            return *this;
        }

        Flag_Set& reset()
        {
            flags_ = store_type(0);
            return *this;
        }

        Flag_Set& reset(enum_type flag)
        {
            flags_ &= ~static_cast<store_type>(flag);
            return *this;
        }

        Flag_Set& flip()
        {
            flags_ = ~flags_;
            return *this;
        }

        Flag_Set& flip(enum_type flag)
        {
            flags_ ^= static_cast<store_type>(flag);
            return *this;
        }

        size_t count() const
        {
            // http://www-graphics.stanford.edu/~seander/bithacks.html#CountBitsSetKernighan

            store_type bits = flags_;
            size_t total = 0;
            for (; bits != 0; ++total)
            {
                bits &= bits - 1; // clear the least significant bit set
            }
            return total;
        }

        constexpr size_t size() const   // constexpr not supported in vs2010 yet
        {
            return sizeof(enum_type)*8;
        }

        bool test(enum_type flag) const
        {
            return (flags_ & static_cast<store_type>(flag)) == flag;
        }

        bool any() const
        {
            return flags_ > 0;
        }

        bool none() const
        {
            return flags_ == 0;
        }

		store_type value() const
		{
			return flags_;
		}

    private:

        store_type flags_;

};

template<typename enumT, typename storeT>
Flag_Set<enumT, storeT> operator & (const Flag_Set<enumT, storeT>& lhs, const Flag_Set<enumT, storeT>& rhs)
{
    return Flag_Set<enumT, storeT>(Flag_Set<enumT, storeT>::store_type(lhs) & Flag_Set<enumT, storeT>::store_type(rhs));
}

template<typename enumT, typename storeT>
Flag_Set<enumT, storeT> operator | (const Flag_Set<enumT, storeT>& lhs, const Flag_Set<enumT, storeT>& rhs)
{
    return Flag_Set<enumT, storeT>(Flag_Set<enumT, storeT>::store_type(lhs) | Flag_Set<enumT, storeT>::store_type(rhs));
}

template<typename enumT, typename storeT>
Flag_Set<enumT, storeT> operator ^ (const Flag_Set<enumT, storeT>& lhs, const Flag_Set<enumT, storeT>& rhs)
{
    return Flag_Set<enumT, storeT>(Flag_Set<enumT, storeT>::store_type(lhs) ^ Flag_Set<enumT, storeT>::store_type(rhs));
}

// template <class charT, class traits, typename enumT>
// std::basic_ostream<charT, traits> & operator << (std::basic_ostream<charT, traits>& os, const Flag_Set<enumT>& flagSet)
// {
//     return os << flagSet.to_string();
// }


} //util

