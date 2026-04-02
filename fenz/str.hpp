#ifndef FENZ_STR_HPP
#define FENZ_STR_HPP

#include "array.hpp"

namespace fenz {

    /// @brief A fixed-length string class stored on the stack.
    /// @tparam Length The length of the string
    template<int Length>
    class Str : public Array<char, Length>
    {
        static_assert(Length > 0, "Str length must be greater than zero");
    public:

        /// @brief Constructs a Str from a null-terminated string literal.
        /// @param literal The string literal to initialize the Str with. Must be null-terminated and have a length of `Length`.
        template <unsigned int M>
        constexpr Str(const char (&literal)[M]);

        /// @brief Constructs a Str filled with a default character.
        /// @param defaultChar The default character with which to fill the Str.
        constexpr Str(char defaultChar);

        constexpr Str(const Str<Length> & other);
        constexpr Str<Length>& operator=(const Str<Length> & other);


        /// @brief Concatenates this Str with another Str, returning a new Str with the combined contents.
        /// @tparam M The length of the other Str to concatenate.
        /// @param other The other Str to concatenate with this one.
        /// @return The concatenated Str.
        template <int M>
        constexpr Str<Length + M> operator+(const Str<M>& other) const;
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
    inline constexpr fenz::Str<Length>::Str(char defaultChar) : Array<char, Length>(defaultChar) {}

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
    template <int M>
    constexpr Str<Length + M> Str<Length>::operator+(const Str<M> &other) const
    {
        Str<Length + M> result('\0');
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