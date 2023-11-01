
#include <iostream>

#include <tuple>

template <class... Ts>
class Serial
{
  public:
    //    template <class... Ts>
    Serial( Ts&&... ts ) : m_ts( std::forward<Ts&>( ts )... ) {}

    //    void print() {
    //        for (const auto & t : m_ts) {
    //            std::cout << t << std::endl;
    //        }
    //    }

    void print() { print( m_ts ); }

    template <std::size_t I = 0, typename... Tp>
    inline typename std::enable_if<I == sizeof...( Tp ), void>::type print( std::tuple<Tp...>& t ) {
    }

    template <std::size_t I = 0, typename... Tp>
        inline typename std::enable_if <
        I<sizeof...( Tp ), void>::type print( std::tuple<Tp...>& t ) {
        std::cout << std::get<I>( t ) << std::endl;
        print<I + 1, Tp...>( t );
    }

    //  private:
    std::tuple<Ts...> m_ts;
};

template <class... Ts>
auto make_serial( Ts&&... ts ) {
    return Serial<Ts&&...>( ts... );
}

class BigObject {
    int a[100];
};

class User
{
  public:
    int a;
    double b;
    bool c;
    BigObject d;
    //    auto serial = make_serial(a, b, c);
    //    Serial<int&, double&, bool&> serial = make_serial(a, b, c);
    Serial<int&, double&, bool&, BigObject&> serial { a, b, c, d };
//    Serial<const int*, const double*, const bool*> serial { &a, &b, &c };
//    auto serial<> = make_serial { a, b, c };
    //    Serial<int> m_serial{a};
};

// template <class T>
// void print(const T & t) {
//     std::cout << t << std::endl;
// }

int main() {

    int a    = 5;
    double b = 2.0;
    bool c   = true;
    //    std::tuple<int&> tuple{a};
    //    std::tuple<int&> tuple{a};
//    auto tuple = std::make_tuple( a );

    User user{1, 2.0, true};

//    User user2 = std::move(user);
//    user.print();
//    user.serial.print();

    std::cout << sizeof(user) << std::endl;

//    auto serial = make_serial( a, b, c );

    //    Serial serial(5, true, 2.0);

//    serial.print();

    //    for (int i = 0; i < 50; ++i) {
    //        std::cout << i <<  "\033[" << std::to_string(i) << "mhello" << "\033[0m" << std::endl;
    //    }

    return 0;
}
