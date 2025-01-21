#pragma once
#include "./RenderTypes.h"
#include <tuple>
#include <type_traits>

namespace FCT { 

template<VertexAttributeType Type, typename T>
struct VertexAttribute {
    T value;
    static constexpr VertexAttributeType type = Type;

    VertexAttribute() = default;
    explicit VertexAttribute(const T& val) : value(val) {}
};

template<typename... Attributes>
class Vertex {
    std::tuple<Attributes...> attributes;

public:
    Vertex(Attributes... attrs) : attributes(attrs...) {}

    template<VertexAttributeType Type, typename T>
    T* getAttribute() {
        return std::get_if<VertexAttribute<Type, T>>(&attributes);
    }

    template<VertexAttributeType Type, typename T>
    const T* getAttribute() const {
        return std::get_if<VertexAttribute<Type, T>>(&attributes);
    }

    static constexpr size_t getAttributeCount() {
        return sizeof...(Attributes);
    }

    template<size_t Index>
    static constexpr VertexAttributeType getAttributeType() {
        return std::tuple_element_t<Index, std::tuple<Attributes...>>::type;
    }

    template<size_t Index>
    static constexpr size_t getAttributeSize() {
        return sizeof(std::tuple_element_t<Index, std::tuple<Attributes...>>::value);
    }

    static constexpr size_t getStride() {
        return sizeof(Vertex);
    }

    template<size_t Index>
    static constexpr size_t getAttributeOffset() {
        if constexpr (Index == 0) {
            return 0;
        } else {
            return offsetof(Vertex, attributes) + 
                   offsetof(std::tuple<Attributes...>, std::get<Index>(std::declval<std::tuple<Attributes...>>()));
        }
    }
};

template<VertexAttributeType Type, typename T>
auto makeAttribute(const T& value) {
    return VertexAttribute<Type, T>(value);
}

} // namespace FCT
