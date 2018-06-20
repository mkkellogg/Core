#pragma once

namespace Core {

    template <typename T, typename U = typename T::value_type>
    class ValueIterator {
    public:
        ValueIterator(const T& src): src(src) {

        }

        U operator*() {
            return *src;
        }

        bool operator != (const ValueIterator& other) {
            return this->src != other.src;
        }

        ValueIterator operator ++() {
            ++src;
            return *this;
        }

        T& getSrc() {
            return this->src;
        }

    private:
        T src;
    };

}