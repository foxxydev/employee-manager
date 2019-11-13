#ifndef CONST_H
#define CONST_H

#include <type_traits>

/**
 * @brief      Return a value as const.
 *
 * This is mainly used to ensure that we don't perform a deep copy of Qt
 * containers in range-based for-loops
 *
 * @param[in]  value  The value to convert.
 *
 * @tparam     T      The value-type.
 *
 * @return     returns a constant reference to the value
 */
template<typename T>
constexpr typename std::add_const<T>::type &asConst(const T &value) noexcept
{
    return value;
}

/**
 * @brief      Return a value as const.
 *
 * This is deleted (and unusable, i.e. returns void) for r-value references.
 *
 */
template<typename T>
void asConst(const T&&) = delete;


#endif // CONST_H
