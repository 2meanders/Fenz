#ifndef FENZ_ARRAY_HPP
#define FENZ_ARRAY_HPP

namespace fenz
{   
    template <typename T, int N>
    class Array;

    /// @brief A non-owning view of a portion of an array.
    /// @tparam T Type of the elements in the array.
    /// @tparam N Size of view portion of the array.
    template <typename T, int N>
    class Iterable
    {
        static_assert(N > 0, "Array size must be greater than zero");

        template <typename, int>
        friend class Iterable;

        friend class Array<T, N>;

    private:
        // A pointer to the first element of the array.
        T *data_;

    private:
        // To allow view creation
        constexpr Iterable(T* data);
    public:
        /// @brief Constructs a Iterable from a pointer to data.
        /// @param arr Pointer to the data.
        template <int M>
        constexpr Iterable(T (&arr)[M]);

        /// @brief Returns a reference to the element at the specified index.
        /// @tparam i Index of the element to access.
        /// @return Reference to the element at the specified index.
        template <int i>
        constexpr T &at();

        /// @brief Returns a const reference to the element at the specified index.
        /// @tparam i Index of the element to access.
        /// @return Const reference to the element at the specified index.
        template <int i>
        constexpr const T &at() const;

        /// @brief Performs an operation on each element of the array.
        /// @param func A callable that is the operation to perform on each element. The parameters to the function are `(const T&, int)` — the element and its index.
        template <typename Func>
        constexpr void enumerate(Func func);

        /// @brief Performs a const operation on each element of the array.
        /// @param func A callable that is the const operation to perform on each element. The parameters to the function are `(const T&, int)` — the element and its index.
        template <typename Func>
        constexpr const void enumerate(Func func) const;

        /// @brief Runs `func` on elements in `other` and `this` in parallel.
        /// @details For each index `i` from `0` to `N-1`, the function calls `func`.
        /// @tparam U The element type of the other iterable.
        /// @param other The other iterable to iterate over in parallel with this one.
        /// @param func A callable taking `(T&, U&)` —
        ///      the element from this iterable and the element from `other`.
        template <typename U, typename Func>
        constexpr void zip(Iterable<U, N> &other, Func func);

        /// @brief Runs `func` on elements in `other` and `this` in parallel.
        /// @details For each index `i` from `0` to `N-1`, the function calls `func`.
        /// @tparam U The element type of the other iterable.
        /// @param other The other iterable to iterate over in parallel with this one.
        /// @param func A callable taking `(const T&, const U&)` —
        ///      the element from this iterable and the element from `other`.
        template <typename U, typename Func>
        constexpr void zip(const Iterable<U, N> &other, Func func) const;

        // Range-based for support
        constexpr T *begin() { return data_; }
        constexpr T *end() { return data_ + N; }
        constexpr const T *begin() const { return data_; }
        constexpr const T *end() const { return data_ + N; }

        /// @brief Returns a view of this Iterable's data.
        /// @details This function returns an Iterable that starts at the specified index of this Iterable's data and has the specified size.
        /// @tparam Start Start index.
        /// @tparam End End index.
        /// @return An Iterable with the specified start index and end index.
        /// @note This function does not create a copy of the data.
        template <int Start, int End>
        constexpr Iterable<T, End - Start> view();

        /// @brief Returns a view of this Iterable's data.
        /// @details This function returns an Iterable that starts at the specified index of this Iterable's data and has the specified size.
        /// @tparam Start Start index.
        /// @tparam End End index.
        /// @return A ConstIterable with the specified start index and end index.
        /// @note This function does not create a copy of the data.
        template <int Start, int End>
        constexpr Iterable<const T, End - Start> view() const;
    };

    /// @brief A fixed-size array that owns its data.
    /// @tparam T Type of the elements in the array.
    /// @tparam N Size of the array.
    template <typename T, int N>
    class Array : public Iterable<T, N>
    {
    private:
        /// @brief The owned storage for the array elements.
        T ownedData_[N];

    public:
        /// @brief Constructs an Array with a default value for all elements.
        /// @param defaultValue The default value to initialize all elements of the array.
        Array(T defaultValue);

        Array(const Array &) = delete;
        Array &operator=(const Array &) = delete;
    };

    // =======================================
    //  Implementations only below this point
    // =======================================

    template <typename T, int N>
    constexpr Iterable<T, N>::Iterable(T* data) : data_(data) {}

    template <typename T, int N>
    template <int M>
    constexpr Iterable<T, N>::Iterable(T (&arr)[M]) : data_(arr)
    {
        static_assert(M >= N, "Source array too small");
    }

    template <typename T, int N>
    template <int i>
    constexpr T &Iterable<T, N>::at()
    {
        static_assert(i >= 0 && i < N, "Index out of bounds");
        return data_[i];
    }

    template <typename T, int N>
    template <int i>
    constexpr const T &Iterable<T, N>::at() const
    {
        static_assert(i >= 0 && i < N, "Index out of bounds");
        return data_[i];
    }

    template <typename T, int N>
    template <typename Func>
    constexpr void Iterable<T, N>::enumerate(Func func)
    {
        for (int i = 0; i < N; ++i)
        {
            func(data_[i], i);
        }
    }

    template <typename T, int N>
    template <typename Func>
    constexpr const void Iterable<T, N>::enumerate(Func func) const
    {
        for (int i = 0; i < N; ++i)
        {
            func(data_[i], i);
        }
    }

    template <typename T, int N>
    template <typename U, typename Func>
    constexpr void fenz::Iterable<T, N>::zip(Iterable<U, N> &other, Func func)
    {
        for (int i = 0; i < N; ++i)
        {
            func(data_[i], other.data_[i]);
        }
    }

    template <typename T, int N>
    template <typename U, typename Func>
    constexpr void fenz::Iterable<T, N>::zip(const Iterable<U, N> &other, Func func) const
    {
        for (int i = 0; i < N; ++i)
        {
            func(data_[i], other.data_[i]);
        }
    }

    template <typename T, int N>
    template <int Start, int End>
    constexpr Iterable<T, End - Start> Iterable<T, N>::view()
    {
        static_assert(Start >= 0 && End <= N, "Iterable out of bounds");
        static_assert(End > Start, "Size must be positive");
        return Iterable<T, End - Start>(data_ + Start);
    }

    template <typename T, int N>
    template <int Start, int End>
    constexpr Iterable<const T, End - Start> Iterable<T, N>::view() const
    {
        static_assert(Start >= 0 && End <= N, "Iterable out of bounds");
        static_assert(End > Start, "Size must be positive");

        return Iterable<const T, End - Start>(data_ + Start);
    }

    template <typename T, int N>
    inline Array<T, N>::Array(T defaultValue)
        : Iterable<T, N>(ownedData_)
    {
        for (int i = 0; i < N; ++i)
        {
            ownedData_[i] = defaultValue;
        }
    }
}

#endif // FENZ_ARRAY_HPP
