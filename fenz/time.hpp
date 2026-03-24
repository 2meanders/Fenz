#ifndef FENZ_TIME_HPP
#define FENZ_TIME_HPP

/// A function returning a long long representing the number of milliseconds since an arbitrary start time.
/// @note This must be defined by the user of the library for `Moment`s to work properly. If not defined, a linker error will be produced.
long long fenzTimeSource();

namespace fenz
{

    /// @brief Represents a time duration.
    /// Provides utility functions for conversion and arithmetic operations.
    struct Duration
    {
        /// The length of the duration in milliseconds.
        long long value;

        /// @brief Returns the number of milliseconds this Duration represents.
        /// @return The number of milliseconds.
        inline constexpr long long millis() const
        {
            return value;
        }

        /// @brief Returns the number of seconds this Duration represents.
        /// @return The number of seconds as a double.
        inline double seconds() const
        {
            return static_cast<double>(value) / 1000.0;
        }

    private:
        /// @brief Constructs a Duration from milliseconds.
        /// @param ms Number of milliseconds.
        constexpr Duration(long long ms) : value(ms) {}

    public:
        /// @brief Creates a Duration from a number of milliseconds.
        /// @param ms Number of milliseconds.
        /// @return Duration representing the given milliseconds.
        inline static constexpr Duration fromMillis(long long ms)
        {
            return {ms};
        }
        /// @brief Creates a Duration from a number of seconds.
        /// @param sec Number of seconds.
        /// @return Duration representing the given seconds.
        inline static Duration fromSeconds(double sec)
        {
            return {static_cast<long long>(sec * 1000)};
        }

    public:
        /// @brief Adds another Duration to this Duration.
        /// @param duration The Duration to add.
        /// @return Reference to this Duration.
        inline constexpr Duration &operator+=(const Duration &duration)
        {
            value += duration.value;
            return *this;
        }

        /// @brief Subtracts another Duration from this Duration.
        /// @param duration The Duration to subtract.
        /// @return Reference to this Duration.
        inline constexpr Duration &operator-=(const Duration &duration)
        {
            value -= duration.value;
            return *this;
        }

        /// @brief Multiplies this Duration by a coefficient.
        /// @param coefficient The multiplier.
        /// @return Reference to this Duration.
        inline constexpr Duration &operator*=(long long coefficient)
        {
            value *= coefficient;
            return *this;
        }
    };

    /// @brief Represents a point in time.
    /// Provides utility functions for time arithmetic and comparison.
    struct Moment
    {
        /// Time in milliseconds relative to the start time defined in the fenzTimeSource implementation.
        long long value;

    private:
        /// @brief Constructs a Moment from milliseconds.
        /// @param ms Number of milliseconds.
        constexpr Moment(long long ms) : value(ms) {}

    public:
        /// @brief Creates a Moment representing the current time.
        /// @return Moment representing the current time.
        static Moment now()
        {
            return {fenzTimeSource()};
        }

    private:
        /// @brief Creates a Moment from a number of milliseconds relative to the start time defined in the fenzTimeSource implementation.
        /// @param millis Number of milliseconds.
        /// @return Moment representing the given milliseconds.
        static constexpr Moment fromMillis(long long millis)
        {
            return {millis};
        }

        friend constexpr Moment operator+(const Moment &m, const Duration &d);
        friend constexpr Moment operator-(const Moment &m, const Duration &d);

    public:
        /// @brief Adds a Duration to this Moment.
        /// @param duration The Duration to add.
        /// @return Reference to this Moment.
        inline constexpr Moment &operator+=(const Duration &duration)
        {
            value += duration.value;
            return *this;
        }

        /// @brief Subtracts a Duration from this Moment.
        /// @param duration The Duration to subtract.
        /// @return Reference to this Moment.
        inline constexpr Moment &operator-=(const Duration &duration)
        {
            value -= duration.value;
            return *this;
        }
    };

    /// @brief Returns the Duration between two Moments.
    /// @param lhs The later Moment.
    /// @param rhs The earlier Moment.
    /// @return Duration between the two Moments.
    /// @note This may produce negative durations.
    inline constexpr Duration operator-(const Moment &lhs, const Moment &rhs)
    {
        return Duration::fromMillis(lhs.value - rhs.value);
    }

    /// @brief Adds a Duration to a Moment, returning a new Moment.
    /// @param m The original Moment.
    /// @param d The Duration to add.
    /// @return New Moment after addition.
    inline constexpr Moment operator+(const Moment &m, const Duration &d)
    {
        return Moment::fromMillis(m.value + d.value);
    }

    /// @brief Subtracts a Duration from a Moment, returning a new Moment.
    /// @param m The original Moment.
    /// @param d The Duration to subtract.
    /// @return New Moment after subtraction.
    inline constexpr Moment operator-(const Moment &m, const Duration &d)
    {
        return Moment::fromMillis(m.value - d.value);
    }

    /// @brief Adds two Durations, returning a new Duration.
    /// @param lhs First Duration.
    /// @param rhs Second Duration.
    /// @return Sum of the two Durations.
    inline constexpr Duration operator+(const Duration &lhs, const Duration &rhs)
    {
        return Duration::fromMillis(lhs.value + rhs.value);
    }

    /// @brief Subtracts one Duration from another, returning a new Duration.
    /// @param lhs First Duration.
    /// @param rhs Second Duration.
    /// @return Difference of the two Durations.
    inline constexpr Duration operator-(const Duration &lhs, const Duration &rhs)
    {
        return Duration::fromMillis(lhs.value - rhs.value);
    }

    /// @brief Multiplies a Duration by a coefficient, returning a new Duration.
    /// @param lhs The Duration.
    /// @param coefficient The multiplier.
    /// @return Product of the Duration and the coefficient.
    inline constexpr Duration operator*(const Duration &lhs, const long long &coefficient)
    {
        return Duration::fromMillis(lhs.value * coefficient);
    }

    /// @brief Checks if one Moment is less than another.
    /// @param lhs First Moment.
    /// @param rhs Second Moment.
    /// @return True if lhs < rhs.
    inline constexpr bool operator<(const Moment &lhs, const Moment &rhs)
    {
        return lhs.value < rhs.value;
    }
    /// @brief Checks if one Moment is greater than another.
    /// @param lhs First Moment.
    /// @param rhs Second Moment.
    /// @return True if lhs > rhs.
    inline constexpr bool operator>(const Moment &lhs, const Moment &rhs)
    {
        return lhs.value > rhs.value;
    }
    /// @brief Checks if one Moment is less than or equal to another.
    /// @param lhs First Moment.
    /// @param rhs Second Moment.
    /// @return True if lhs <= rhs.
    inline constexpr bool operator<=(const Moment &lhs, const Moment &rhs)
    {
        return lhs.value <= rhs.value;
    }
    /// @brief Checks if one Moment is greater than or equal to another.
    /// @param lhs First Moment.
    /// @param rhs Second Moment.
    /// @return True if lhs >= rhs.
    inline constexpr bool operator>=(const Moment &lhs, const Moment &rhs)
    {
        return lhs.value >= rhs.value;
    }
    /// @brief Checks if two Moments are equal.
    /// @param lhs First Moment.
    /// @param rhs Second Moment.
    /// @return True if lhs == rhs.
    inline constexpr bool operator==(const Moment &lhs, const Moment &rhs)
    {
        return lhs.value == rhs.value;
    }

    /// @brief Checks if one Duration is less than another.
    /// @param lhs First Duration.
    /// @param rhs Second Duration.
    /// @return True if lhs < rhs.
    inline constexpr bool operator<(const Duration &lhs, const Duration &rhs)
    {
        return lhs.value < rhs.value;
    }
    /// @brief Checks if one Duration is greater than another.
    /// @param lhs First Duration.
    /// @param rhs Second Duration.
    /// @return True if lhs > rhs.
    inline constexpr bool operator>(const Duration &lhs, const Duration &rhs)
    {
        return lhs.value > rhs.value;
    }
    /// @brief Checks if one Duration is less than or equal to another.
    /// @param lhs First Duration.
    /// @param rhs Second Duration.
    /// @return True if lhs <= rhs.
    inline constexpr bool operator<=(const Duration &lhs, const Duration &rhs)
    {
        return lhs.value <= rhs.value;
    }
    /// @brief Checks if one Duration is greater than or equal to another.
    /// @param lhs First Duration.
    /// @param rhs Second Duration.
    /// @return True if lhs >= rhs.
    inline constexpr bool operator>=(const Duration &lhs, const Duration &rhs)
    {
        return lhs.value >= rhs.value;
    }
    /// @brief Checks if two Durations are equal.
    /// @param lhs First Duration.
    /// @param rhs Second Duration.
    /// @return True if lhs == rhs.
    inline constexpr bool operator==(const Duration &lhs, const Duration &rhs)
    {
        return lhs.value == rhs.value;
    }
}

#endif // FENZ_TIME_HPP
