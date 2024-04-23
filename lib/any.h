#pragma once
#include <type_traits>
#include <utility>

class Any {
public: // structors
    Any() : man(nullptr), value_ptr() {}

    template<typename ValueType>
    Any(const ValueType& value): Any() {
        create(*this, value);
    }

    Any(const Any& other): Any() {
        if (other.man) {
            other.man(Copy, *this, &other);
        }
    }

    Any(Any&& other) noexcept : Any() {
        if (other.man) {
            other.man(Move, *this, &other);
        }
    }

    template<typename ValueType>
    Any(ValueType&& value
      , typename std::enable_if<!std::is_same<Any&, ValueType>::value >::type* = 0 // disable if value has type `basic_any&`
      , typename std::enable_if<!std::is_const<ValueType>::value >::type* = 0) // disable if value has type `const ValueType&&`
      : Any() {
        create(*this, static_cast<ValueType&&>(value)); // todo: maby change to forward
    }

    ~Any() {
        if (man) {
            man(Destroy, *this, nullptr);
        }
    }
public: // modifiers
    Any& swap(Any& rhs) {
        if (this == &rhs) {
            return *this;
        }

        if (man && rhs.man) {
            Any tmp;
            rhs.man(Move, tmp, &rhs);
            man(Move, rhs, this);
            tmp.man(Move, *this, &tmp);
        } else if (man) {
            man(Move, rhs, this);
        } else if (rhs.man) {
            rhs.man(Move, *this, &rhs);
        }

        return *this;
    }

    Any& operator=(const Any& rhs) {
        Any(rhs).swap(*this);
        return *this;
    }

    Any& operator=(Any&& rhs) noexcept {
        rhs.swap(*this);
        Any().swap(rhs);
        return *this;
    }

    template <class ValueType>
    Any& operator=(ValueType&& rhs) {
        Any(std::forward<ValueType>(rhs)).swap(*this);
        return *this;
    }
public: // queries
    bool empty() const {
        return !man;
    }
private:
    template<typename ValueType>
    friend ValueType * any_cast(Any*) noexcept;
private:
    enum operation {
        Destroy,
        Move,
        Copy
    };

    template<typename ValueType>
    static void* value_manager(operation op, Any& left, const Any* right) {
        switch (op) {
            case Destroy:
                delete static_cast<ValueType*>(left.value_ptr);
                break;
            case Move:
                left.value_ptr = right->value_ptr;
                left.man = right->man;
                const_cast<Any*>(right)->value_ptr = nullptr;
                const_cast<Any*>(right)->man = nullptr;
                break;
            case Copy:
                left.value_ptr = new ValueType(*static_cast<const ValueType*>(right->value_ptr));
                left.man = right->man;
                break;
        }
        return nullptr;
    }

    template <typename ValueType>
    static void create(Any& any, ValueType&& value) {
        typedef typename std::decay<const ValueType>::type DecayedType;
        any.man = &value_manager<DecayedType>;
        any.value_ptr = new DecayedType(std::forward<ValueType>(value));
    }

private:
    typedef void*(*manager)(operation op, Any& left, const Any* right);
    void* value_ptr;
    manager man;
};

template<typename ValueType>
ValueType* any_cast(Any* any) noexcept {
    return static_cast<ValueType*>(any->value_ptr);
}

template<typename ValueType>
const ValueType* any_cast(const Any* any) noexcept {
    return any_cast<ValueType>(const_cast<Any*>(any));
}


