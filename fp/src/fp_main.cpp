#include <functional>
#include <iostream>
#include <list>
#include <string>

using std::function;

template <typename T>
struct list {
 private:
  std::list<T> m_data;

  template <typename T>
  friend list<T> empty();

  template <typename T>
  friend list<T> addToFront(T t, list<T>);

  template <typename T>
  friend bool isEmpty(list<T>);

  template <typename T>
  friend T front(list<T>);

  template <typename T>
  friend list<T> rest(list<T> l);
};

template <typename T>
list<T> empty() {
  return list<T>();
}
template <typename T>
list<T> addToFront(T t, list<T> l) {
  l.m_data.push_front(t);
  return std::move(l);
}

template <typename T>
bool isEmpty(list<T> l) {
  return l.m_data.empty();
}

template <typename T>
T front(list<T> l) {
  return l.m_data.front();
}

template <typename T>
list<T> rest(list<T> l) {
  l.m_data.pop_front();
  return std::move(l);
}

template <typename F, typename T>
list<typename std::result_of<F(T)>::type> map(F f, list<T> list) {
  typedef typename std::result_of<F(T)>::type U;
  if (isEmpty(list))
    return empty<U>();
  else
    return addToFront(f(front(list)), map(f, rest(list)));
}

template <typename T, typename U>
U fold(function<U(T, U)> f, U u, list<T> list) {
  if (isEmpty(list))
    return u;
  else
    return f(front(list), fold(f, u, rest(list)));
}

template <typename F, typename T, typename U>
U fold(F f, U u, list<T> list) {
  if (isEmpty(list))
    return u;
  else
    return f(front(list), fold(f, u, rest(list)));
}

int sum(list<int> intList) {
  return fold<int, int>([](int i, int j) { return i + j; }, 0, intList);
}

template <typename T>
T sum(list<T> summableList) {
  return fold([](T i, T j) { return i + j; }, T(0), intList);
}

template <typename T>
list<T> identity(list<T> value) {
  return fold(addToFront<T>, empty<T>(), value);
}

template <typename T>
list<T> append(list<T> value, list<T> anotherValue) {
  return fold(addToFront<T>, anotherValue, value);
}

template <typename T>
list<T> reverse(list<T> value) {
  return fold([](T t,
                 list<T> l) { return append(l, addToFront(t, empty<T>())); },
              empty<T>(),
              value);
}

template <typename T >
std::ostream & operator<<( std::ostream& os, list<T> list )
{
  fold([&os](T t, int)->int {
         os << t << ' ';
         return 0;
       },
       0,
       reverse(list));
  return os;
}

template <typename F, typename T>
list<typename std::result_of<F(T)>::type> map2(F f, list<T> l) {
  typedef typename std::result_of<F(T)>::type U;
  return fold([f](T t, list<U> l) { return addToFront(f(t), l); },
              empty<U>(),
              l);
}

struct A{};
struct B{};

//struct AOrB {
//  bool hasA;
//  union {
//    A a;
//    B b;
//  } contents;
//};

//struct AOrB {
//  bool hasA;
//  A a;
//  B b;
//};

//struct AOrB {
//  bool hasA();
//  // Return the embedded 'A' object. The
//  // behavior is undefined unless 'hasA()'.
//  A getA();
//  // Return the embedded 'B' object. The
//  // behavior is undefined unless 'hasA()'.
//  B getB();
//  // post: 'hasA()'.
//  void setA();
//  // post: '!hasA()'.
//  void setB();
//private:
//  //...
//};

struct AOrB {
protected:
  virtual void dummy(){}
};

struct AOrBWithA : AOrB {
  A a;
};
struct AOrBWithB : AOrB {
  B b;
};
struct Unit{};

//typedef std::function< std::pair<Foo,int> (Unit) > Foo;

//struct Foo {
//  std::function< std::pair<Foo,int> (Unit) >
//    function;
//};

//struct Foo
//  : std::function< std::pair<int,Foo> (Unit) >
//{
//  template< typename F >
//  Foo( F && f )
//    : std::function< std::pair<int,Foo> (Unit) >(std::forward<F>(f))
//  {}
//};

//struct Foo
//  : std::function< std::pair<int,Foo> () >
//{
//  template< typename F >
//  Foo( F && f )
//    : std::function< std::pair<int,Foo> () >( std::forward<F>( f ) )
//  {}
//};

//Foo foo = [](){ return std::make_pair(1, foo); };

struct IntStream
  : std::function< std::pair<int,IntStream> () >
{
  template< typename F >
  IntStream( F && f )
    : std::function< std::pair<int,IntStream> () >( std::forward<F>( f ) )
  {}
};

std::pair<int,IntStream> naturalsFrom( int i ) {
  return std::make_pair(
    i,
    std::bind( naturalsFrom, i+1 ) );
}

IntStream naturals = [](){ return naturalsFrom( 0 ); };

int main()
{
  list<int> l = addToFront(6, addToFront(5, addToFront(3, empty<int>())));
  std::cout << l << std::endl;
  std::cout << append(l, l) << std::endl;
  std::cout << sum(l) << std::endl;
  std::cout << sum(append(l, l)) << std::endl;

  std::cout << map([](int i) { return "+" + std::to_string(i); }, l)
            << std::endl;

  std::cout << map2([](int i) { return "+" + std::to_string(i); }, l)
            << std::endl;

//  std::cout << foo().first << std::endl;
//  std::cout << foo().second().first << std::endl;
  std::cout << naturals().first << std::endl;
  std::cout << naturals().second().first << std::endl;
  std::cout << naturals().second().second().first << std::endl;
}
