#include <array>
#include <cassert>
#include <type_traits>

template <typename T, size_t N>
class CircularDeque
{
    // Compile-time check
    static_assert(N > 0, "Capacity must be greater than 0");

    // ARM9 Optimization: Bitwise AND is faster than Modulo if N is a power of 2
    static constexpr bool N_IS_POWER_OF_2 = (N & (N - 1)) == 0;
    static constexpr size_t MASK = N - 1;

    // Index calculation helper
    static constexpr size_t wrap(size_t idx)
    {
        if constexpr (N_IS_POWER_OF_2) return idx & MASK; // Fast path
        else return idx % N; // Slow path 
    }

public:
    CircularDeque() = default;

    explicit CircularDeque(const T& initialValue)
    {
        m_data.fill(initialValue);
    }

    // --- PUSH (Automatic Overwrite) ---

    void push_back(const T& value)
    {
        if (isFull()) m_head = wrap(m_head + 1);
        else m_count++;

        m_data[m_tail] = value;
        m_tail = wrap(m_tail + 1);
    }

    void push_front(const T& value)
    {
        if (isFull()) m_tail = wrap(m_tail - 1 + N);
        else m_count++;
        
        m_head = wrap(m_head - 1 + N);
        m_data[m_head] = value;
    }

    // --- POP ---

    void pop_front()
    {
        assert(!isEmpty());
        m_head = wrap(m_head + 1);
        m_count--;
    }

    void pop_back()
    {
        assert(!isEmpty());
        m_tail = wrap(m_tail - 1 + N);
        m_count--;
    }

    // --- ACCESS ---

    T& operator[](size_t index)
    {
        assert(index < m_count);
        return m_data[wrap(m_head + index)];
    }

    const T& operator[](size_t index) const
    {
        assert(index < m_count);
        return m_data[wrap(m_head + index)];
    }

    // --- GETTERS ---

    [[nodiscard]] constexpr size_t size() const { return m_count; }
    [[nodiscard]] constexpr size_t capacity() const { return N; }
    [[nodiscard]] constexpr bool isEmpty() const { return m_count == 0; }
    [[nodiscard]] constexpr bool isFull() const { return m_count == N; }

    void clear() { m_head = 0; m_tail = 0; m_count = 0; }

private:
    // Data stored inline/stack (no heap allocation)
    std::array<T, N> m_data; 
    
    size_t m_head = 0;
    size_t m_tail = 0;
    size_t m_count = 0;
};