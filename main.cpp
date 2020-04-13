#include <tuple>
#include <cstring>
#include <string>
#include <iostream>

using std::tuple;
using std::get;

template<const int index, typename... Types>
constexpr void foreach_tuple(std::tuple<Types...> t, auto callable, auto... args)
{
    if constexpr (index < 0)
    {
        throw std::exception("Invalid index in iterate<index, Component...>");
    }
    else if constexpr (index < sizeof...(Types))
    {
        callable(std::get<index>(t), args...);
        foreach_tuple<index + 1, Types...>(t, callable, args...);
    }
    else
    {
        return; // Bottom of recursion
    }
}

template<typename... Types>
constexpr void foreach(std::tuple<Types...> t, auto callable, auto... args)
{
    foreach_tuple<0, Types...>(t, callable, args...);
}

template<typename SelfType>
concept TreeType = requires(SelfType tree) 
{
    { tree.Root }
    { tree.Leafs }
};

int id = 0;

template<const int index, const int... indexes>
auto & Location(TreeType & tree)
{
    if constexpr (sizeof...(indexes) == 0)
    {
        if constexpr (index == 0)
        {
            return tree.Root;
        }
        else return std::get<index - 1>(tree.Leafs);
    }
    else
    {
        return Location<indexes...>(std::get<index - 1>(tree.Leafs));
    }
}

template<typename RootType, typename... Ts>
struct Tree
{
    RootType Root = ++id;

    tuple<Ts...> Leafs;

    unsigned int LeafsCount = sizeof...(Ts);

    void DFS(auto lambda)
    {
        foreach(this->Leafs, [this](auto leaf, auto f) 
        {
            l(leaf, f);
        }, lambda);

        lambda(Root);
    }

private:

    void l(TreeType tr, auto lambda)
    {
        tr.DFS(lambda);
        lambda(tr.Root);
    }

    void l(auto d, auto lambda)
    {
        lambda(d);
    }
};

int main()
{
    auto print = [](auto i) 
    {
        std::cout << i << std::endl;
    };

    Tree<int, int, int, Tree<int, int, int>> t;
    
    auto & i = Location<3, 0>(t);
    print(i);

    
    return 0;
}