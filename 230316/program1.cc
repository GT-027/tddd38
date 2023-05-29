#include <string>
#include <cassert>

// Implement Shared_Pointer here

template<typename T>
class Shared_Pointer
{
public:
    Shared_Pointer() = default;

    ~Shared_Pointer()
    {
        if (counter)
        {
            *counter = *counter - 1;
            if (*counter == 0)
            {
                delete data;
                delete counter;
            }
        }
            
    }

    Shared_Pointer(Shared_Pointer const& other) 
        : data{other.data}, counter{other.counter}
    {
        if (counter != nullptr)
            *counter = *counter + 1;
    }

    Shared_Pointer(Shared_Pointer && other)
        : data{other.data}, counter{other.counter}
    {
        other.data = nullptr;
        other.counter = nullptr; // kallar detta på destructorn?
    }

    Shared_Pointer& operator=(Shared_Pointer const& other) 
    {
        Shared_Pointer copy{other};
        std::swap(data, copy.data);
        std::swap(counter, copy.counter);
        return *this;
    }

    Shared_Pointer& operator=(Shared_Pointer && other)
    {
        std::swap(data, other.data);
        std::swap(counter, other.counter);
        return *this;
    }

    T& operator*()
    {
        return *data;
    }
    T const& operator*() const
    {
        return *data;
    }
    T* operator->()
    {
        return data;
    }
    T const* operator->() const
    {
        return data;
    }

    std::size_t count() const //reference?
    {
        if (counter)
            return *counter;
        return 0;
    }

private:
    Shared_Pointer(T* data, std::size_t* counter)
        : data{data}, counter{counter}
    {}

    template<typename U, typename ...Args>
    friend Shared_Pointer<U> make_shared(Args&&...args);

    T* data {nullptr};
    std::size_t* counter {nullptr};
};

template<typename T, typename ...Args>
Shared_Pointer<T> make_shared(Args&&...args)
{
    return {new T(std::forward<Args>(args)...), new std::size_t{1}};
}

int main()
{
    // Check that the behaviour of default initialized shared pointers is correct
    Shared_Pointer<int> ptr0;

    ptr0.count();
    assert( ptr0.count() == 0 );

    // General functionality test
    {
        auto ptr1 = make_shared<std::string>(3, ' ');

        // Check that operator* works
        assert( *ptr1 == "   " );

        // Check that we have one reference to this object
        assert( ptr1.count() == 1 );

        // Check that operator-> works
        assert( ptr1->size() == 3 );

        // Temporarily add one more reference
        {
            // Testing copy constructor
            auto ptr2 = ptr1;
            assert( ptr2.count() == 2 );

            // Modify the underlying data
            *ptr2 = std::string(4, '+');
        }

        // The underlying data should have changed
        assert( *ptr1 == "++++" );
        assert( ptr1.count() == 1 );
    }

    
    // Test assignments
    {
        Shared_Pointer<int> ptr3 { make_shared<int>(5) };
        assert( *ptr3 == 5 );
        assert( ptr3.count() == 1 );

        // Test copy assignment
        Shared_Pointer<int> ptr4;
        ptr4 = ptr3;

        assert( ptr3.count() == ptr4.count() );
        assert( ptr3.count() == 2 );
        assert( *ptr3 == *ptr4 );

        Shared_Pointer<int> ptr5 { make_shared<int>(6) };

        // Test move assignment
        ptr5 = std::move(ptr4);

        assert( ptr5.count() == ptr3.count() );
        assert( ptr5.count() == 2 );
    }

    // Test that const works properly
    Shared_Pointer<std::string> const ptr6 { make_shared<std::string>("abc") };
    assert( *ptr6 == "abc" );
    assert( ptr6->size() == 3 );
    assert( ptr6.count() == 1 );
}
