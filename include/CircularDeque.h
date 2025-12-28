#include <array>
#include <cassert>
#include <type_traits>
#include "Debug.h"

template <typename T, size_t N>
class CircularDeque
{
    // Compile-time check
    static_assert(N > 0, "Capacity must be greater than 0");

    // ARM9 Optimization: Bitwise AND is faster than Modulo if N is a power of 2
    static constexpr bool N_IS_POWER_OF_2 = (N & (N - 1)) == 0;
    static constexpr size_t MASK = N - 1;

    // Index calculation helper
    static constexpr size_t wrap(size_t index)
    {
        if constexpr (N_IS_POWER_OF_2) return index & MASK; // Fast path
        else return index % N; // Slow path 
    }

public:
    CircularDeque() = default;

    explicit CircularDeque(T const& initialValue)
    {
        m_data.fill(initialValue);
    }

    // --- PUSH (Automatic Overwrite) ---

    void pushBack(T const& value)
    {
        pushBackNoRealloc();
        m_data[backIdx()] = value;
    }

    void pushBackNoRealloc()
    {
        if (isFull()) m_frontIdx = wrap(m_frontIdx - 1);
        else m_count++;
        //Debug::nocashLogF("Front %i, Back %i, Count %i", m_frontIdx, backIdx(), m_count);
    }

    void pushFront(T const& value)
    {
        pushFrontNoRealloc();
        m_data[m_frontIdx] = value;
    }

    void pushFrontNoRealloc()
    {
        if(!isEmpty()) m_frontIdx = wrap(m_frontIdx + 1);
        if (!isFull()) m_count++;
        //Debug::nocashLogF("Front %i, Back %i, Count %i", m_frontIdx, backIdx(), m_count);
    }

    // --- POP ---

    void popFront()
    {
        assert(!isEmpty());
        m_frontIdx = wrap(m_frontIdx - 1);
        m_count--;
    }

    void popBack()
    {
        assert(!isEmpty());
        m_count--;
    }

    // --- ACCESS ---

    T& operator[](size_t index)
    {
        assert(index < m_count);
        return m_data[wrap(m_frontIdx + index)];
    }

    T const& operator[](size_t index) const
    {
        assert(index < m_count);
        return m_data[wrap(m_frontIdx + index)];
    }

    T& at(size_t index)
    {
        assert(index < N);
        return m_data[index];
    }

    T const& at(size_t index) const
    {
        assert(index < N);
        return m_data[index];
    }

    T& front()
    {
        return m_data[m_frontIdx];
    }

    T const& front() const
    {
        return m_data[m_frontIdx];
    }

    T& back()
    {
        return m_data[backIdx()];
    }

    T const& back() const
    {
        return m_data[backIdx()];
    }

    // --- GETTERS ---

    [[nodiscard]] constexpr size_t size() const { return m_count; }
    [[nodiscard]] constexpr size_t capacity() const { return N; }
    [[nodiscard]] constexpr bool isEmpty() const { return m_count == 0; }
    [[nodiscard]] constexpr bool isFull() const { return m_count == N; }

    void clear() { m_frontIdx = 0; m_count = 0; }

private:
    // Data stored inline/stack (no heap allocation)
    std::array<T, N> m_data; 
    
    size_t m_frontIdx = 0;
    size_t m_count = 0;

    int backIdx() const { return (m_count > 1) * wrap(m_frontIdx - (m_count-1)); }
};