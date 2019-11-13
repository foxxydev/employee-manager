#ifndef SINGLETON_H
#define SINGLETON_H

#include <mutex>
#include <atomic>

/**
 * @brief       Front-end to get access to a unique object.
 *
 * This is mainly to avoid duplicating the singleton-code everywhere... However,
 * since we use this in numerous libraries, we need to be extra carefull when
 * we change this definition.
 *
 * @note        You are not allowed to inherit form a singleton.
 *
 * @tparam      T    The class that should be exposed as a singleton.
 */
template<class T>
class Singleton final : public T {

    Singleton(const Singleton &) = delete;
    Singleton(Singleton &&) = delete;
    Singleton &operator = (const Singleton &) = delete;
    Singleton &operator = (Singleton &&) = delete;

public:
    /**
     * @brief      Request an instance of an object.
     *
     * Each instance has a unique type-signature. Thus, the following statement
     * is true:
     * @code{.cpp}
     * Singleton::instance("test") == Singleton::instance("asdf")
     * @endcode
     *
     * @param[in]  args  The arguments of the singleton.
     *
     * @tparam     Args  Types of the arguments of the singleton, usually
     * deduced.
     *
     * @return     Returns a unique instance to a Singleton, identified by the
     * argument types.
     */
    template<class... Args>
    static Singleton *instance(Args... args)
    {
        static std::mutex mutex;
        static std::atomic<Singleton<T> *> instance;

        // Lazy initialization, thread-safe.
        if (instance.load(std::memory_order_acquire) == nullptr) {
            std::lock_guard<std::mutex> lock(mutex);
            if (instance.load(std::memory_order_acquire) == nullptr) {
                auto tmp = new Singleton(args...);
                instance.store(tmp, std::memory_order_release);
            }
        }

        return instance;
    }

private:
    template<class... Args>
    Singleton(Args... args) : T(args...) {}
};

#endif // SINGLETON_H