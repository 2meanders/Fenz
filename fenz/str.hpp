#ifndef FENZ_STR_HPP
#define FENZ_STR_HPP

#include "array.hpp"

namespace fenz {

    /// @brief A fixed-length string class stored on the stack.
    /// @tparam Length The length of the string
    template<int Length>
    class Str : public Array<char, Length>
    {
        template <int M>
        friend class Str;

        static_assert(Length > 0, "Str length must be greater than zero");
    public:

        /// @brief Constructs a Str from a null-terminated string literal.
        /// @param literal The string literal to initialize the Str with. Must be null-terminated and have a length of `Length`.
        template <unsigned int M>
        constexpr Str(const char (&literal)[M]);

        /// @brief Copy constructor for Str.
        constexpr Str(const Str<Length> & other);
        /// @brief Copy assignment operator for Str.
        constexpr Str<Length>& operator=(const Str<Length> & other);


        /// @brief Construct a Str from a signed integer
        explicit Str(long long value) : Array<char, Length>('0') {
            setToFormattedNumeric("%0*lld", value);
        }

        /// @brief Construct a Str from an unsigned integer
        explicit Str(unsigned long long value) : Array<char, Length>('0') {
            setToFormattedNumeric("%0*llu", value);
        }

        /// @brief Construct a Str from a floating-point number
        explicit Str(double value) : Array<char, Length>('0') {
            setToFormattedNumeric("%0*g", value);
        }


        /// @brief Concatenates this Str with another Str, returning a new Str with the combined contents.
        /// @tparam M The length of the other Str to concatenate.
        /// @param other The other Str to concatenate with this one.
        /// @return The concatenated Str.
        template <int M>
        constexpr Str<Length + M> operator+(const Str<M>& other) const;


        /// @brief Concatenates a charater to the end of this Str, returning a new Str with the combined contents.
        /// @param c The character to concatenate to the end of this Str.
        /// @return The concatenated Str.
        constexpr Str<Length + 1> operator+(char c) const;

    private:

        constexpr Str() : Array<char, Length>('-') {}
        
        /**
         * @brief Centralized formatting logic
         * @param fmt The printf format string.
         * @param val The value to format
         */
        template<typename T>
        void setToFormattedNumeric(const char* fmt, T val) {
            // +1 for the mandatory null terminator from snprintf
            char temp[Length + 1];

            std::snprintf(temp, Length + 1, fmt, Length, val);

            // Transfer from temp to storage
            this->template enumerate<>([&](char& c, int i) {
                c = temp[i];
            });
        }
    };

    
    // =======================================
    //  Implementations only below this point
    // =======================================

    template <int Length>
    template <unsigned int M>
    constexpr Str<Length>::Str(const char (&literal)[M]) : Array<char, Length>('\0') {
        static_assert(M - 1 == Length, "Length mismatch");

        this->template enumerate<>([&](char& c, int i) {
            c = literal[i];
        });
    }

    template <int Length>
    inline constexpr Str<Length>::Str(const Str& other) : Array<char, Length>('\0')
    {
        this->template zip<>(other, [](char& dest, const char& src) {
            dest = src;
        });
    }

    template <int Length>
    inline constexpr Str<Length>& Str<Length>::operator=(const Str &other)
    {
        this->template zip<>(other, [](char& dest, const char& src) {
            dest = src;
        });
        return *this;
    }

    template <int Length>
    inline constexpr Str<Length + 1> Str<Length>::operator+(char c) const
    {
        Str<Length + 1> result;
        result.template view<0, Length>().zip(*this, [](char& dest, const char& src) constexpr {
            dest = src;
        });
        result.template at<Length>() = c;
        return result;
    }

    template <int Length>
    template <int M>
    constexpr Str<Length + M> Str<Length>::operator+(const Str<M> &other) const
    {
        Str<Length + M> result;
        result.template view<0, Length>().zip(this->template view<0, Length>(), [](char& dest, const char& src) constexpr {
            dest = src;
        });

        result.template view<Length, Length + M>().zip(other.template view<0, M>(), [](char& dest, const char& src) constexpr {
            dest = src;
        });
        return result;
    }
}

#endif