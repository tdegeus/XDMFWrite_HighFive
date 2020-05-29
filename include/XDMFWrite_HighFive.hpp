/*

(c - GPLv3) T.W.J. de Geus (Tom) | tom@geus.me | www.geus.me | github.com/tdegeus/XDMFWrite_HighFive

*/

#ifndef XDMFWRITE_HIGHFIVE_H
#define XDMFWRITE_HIGHFIVE_H

// See: http://xdmf.org/index.php/XDMF_Model_and_Format

#include <fstream>
#include <highfive/H5Easy.hpp>

#ifdef XDMFWRITE_HIGHFIVE_ENABLE_ASSERT
    #define XDMFWRITE_HIGHFIVE_ASSERT(expr) XDMFWRITE_HIGHFIVE_ASSERT_IMPL(expr, __FILE__, __LINE__)
    #define XDMFWRITE_HIGHFIVE_ASSERT_IMPL(expr, file, line) \
        if (!(expr)) { \
            throw std::runtime_error( \
                std::string(file) + ':' + std::to_string(line) + \
                ": assertion failed (" #expr ") \n\t"); \
        }
#else
    #define XDMFWRITE_HIGHFIVE_ASSERT(expr)
#endif

#define XDMFWRITE_HIGHFIVE_VERSION_MAJOR 0
#define XDMFWRITE_HIGHFIVE_VERSION_MINOR 0
#define XDMFWRITE_HIGHFIVE_VERSION_PATCH 1

#define XDMFWRITE_HIGHFIVE_VERSION_AT_LEAST(x, y, z) \
    (XDMFWRITE_HIGHFIVE_VERSION_MAJOR > x || (XDMFWRITE_HIGHFIVE_VERSION_MAJOR >= x && \
    (XDMFWRITE_HIGHFIVE_VERSION_MINOR > y || (XDMFWRITE_HIGHFIVE_VERSION_MINOR >= y && \
                                       XDMFWRITE_HIGHFIVE_VERSION_PATCH >= z))))

#define XDMFWRITE_HIGHFIVE_VERSION(x, y, z) \
    (XDMFWRITE_HIGHFIVE_VERSION_MAJOR == x && \
     XDMFWRITE_HIGHFIVE_VERSION_MINOR == y && \
     XDMFWRITE_HIGHFIVE_VERSION_PATCH == z)

#ifndef XDMFWRITE_HIGHFIVE_INDENT
#define XDMFWRITE_HIGHFIVE_INDENT 4
#endif

#ifndef XDMFWRITE_USE_GOOSEFEM
    #ifdef GOOSEFEM_VERSION_MAJOR
        #define XDMFWRITE_USE_GOOSEFEM
        #include <GooseFEM/Mesh.h>
    #endif
#endif

namespace XDMFWrite_HighFive {

// --- Overview ---

enum class ElementType {
    Triangle,
    Quadrilateral,
    Hexahedron };

enum class AttributeType {
    Cell,
    Node };

inline auto Geometry(
    const HighFive::File& file,
    const std::string& dataset);

template <class T>
inline auto Topology(
    const HighFive::File& file,
    const std::string& dataset,
    const T& type);

inline auto Attribute(
    const HighFive::File& file,
    const std::string& dataset,
    AttributeType type);

template <class T>
inline auto Attribute(
    const HighFive::File& file,
    const std::string& dataset,
    const T& type,
    const std::string &name);

inline auto Grid(
    const std::string& name,
    std::initializer_list<std::vector<std::string>> args);

inline auto Grid(
    std::initializer_list<std::vector<std::string>> args);

class TimeSeries
{
public:
    TimeSeries() = default;
    TimeSeries(const std::string& name);

    template <class T>
    inline void push_back(
        const std::string& name,
        const T& time,
        std::initializer_list<std::vector<std::string>> args);

    inline void push_back(
        std::initializer_list<std::vector<std::string>> args);

    inline auto get() const;

private:
    std::vector<std::string> m_lines;
    std::string m_name = "TimeSeries";
    size_t m_n = 0;
};

template <class T>
inline std::string write(const T& arg);

template <class T>
inline std::string write(const std::string& fname, const T& arg);

// --- Implementation ---

namespace detail {

    template <class T>
    struct is_string : std::false_type
    {
    };

    template <>
    struct is_string<std::string> : std::true_type
    {
    };

    template <class T>
    struct is_string_list : std::false_type
    {
    };

    template <>
    struct is_string_list<std::vector<std::string>> : std::true_type
    {
    };

    template <class T>
    struct is_ElementType : std::false_type
    {
    };

    template <>
    struct is_ElementType<ElementType> : std::true_type
    {
    };

    template <class T>
    struct is_AttributeType : std::false_type
    {
    };

    template <>
    struct is_AttributeType<AttributeType> : std::true_type
    {
    };

    #ifdef XDMFWRITE_USE_GOOSEFEM
    template <class T>
    struct is_GooseFEM_ElementType : std::false_type
    {
    };

    template <>
    struct is_GooseFEM_ElementType<GooseFEM::Mesh::ElementType> : std::true_type
    {
    };
    #endif


    template <class T, typename = void>
    struct to
    {
        static std::string str(const T& arg)
        {
            return std::to_string(arg);
        }
    };

    template <class T>
    struct to<T, typename std::enable_if_t<is_string<T>::value>>
    {
        static std::string str(const T& arg)
        {
            return arg;
        }
    };

    template <class T>
    struct to<T, typename std::enable_if_t<is_ElementType<T>::value>>
    {
        static std::string str(const T& arg)
        {
            if (arg == ElementType::Triangle) {
                return "Triangle";
            }
            else if (arg == ElementType::Quadrilateral) {
                return "Quadrilateral";
            }
            else if (arg == ElementType::Hexahedron) {
                return "Hexahedron";
            }
            throw std::runtime_error("Unknown ElementType");
        }
    };

    #ifdef XDMFWRITE_USE_GOOSEFEM
    template <class T>
    struct to<T, typename std::enable_if_t<is_GooseFEM_ElementType<T>::value>>
    {
        static std::string str(const T& arg)
        {
            if (arg == GooseFEM::Mesh::ElementType::Tri3) {
                return "Triangle";
            }
            else if (arg == GooseFEM::Mesh::ElementType::Quad4) {
                return "Quadrilateral";
            }
            else if (arg == GooseFEM::Mesh::ElementType::Hex8) {
                return "Hexahedron";
            }
            throw std::runtime_error("Unknown ElementType");
        }
    };
    #endif

    template <class T>
    struct to<T, typename std::enable_if_t<is_AttributeType<T>::value>>
    {
        static std::string str(const T& arg)
        {
            if (arg == AttributeType::Cell) {
                return "Cell";
            }
            else if (arg == AttributeType::Node) {
                return "Node";
            }
            throw std::runtime_error("Unknown AttributeType");
        }
    };

    template <class T, typename = void>
    struct convert
    {
        static auto get(const T& arg)
        {
            return arg.get();
        }
    };

    template <class T>
    struct convert<T, typename std::enable_if_t<is_string_list<T>::value>>
    {
        static auto get(const T& arg)
        {
            return arg;
        }
    };

    inline size_t number_of_columns(ElementType type)
    {
        if (type == ElementType::Triangle) {
            return 3;
        }
        else if (type == ElementType::Quadrilateral) {
            return 4;
        }
        else if (type == ElementType::Hexahedron) {
            return 8;
        }
        throw std::runtime_error("Unknown ElementType");
    }

    inline std::string indent()
    {
        std::string ret = "";
        for (size_t i = 0; i < XDMFWRITE_HIGHFIVE_INDENT; ++i) {
            ret += " ";
        }
        return ret;
    }

    inline std::string indent(size_t n)
    {
        std::string ret;
        std::string i = indent();
        for (size_t j = 0; j < n; ++j) {
            ret += i;
        }
        return ret;
    }

    inline void indent(std::vector<std::string>& lines)
    {
        auto i = indent();
        for (auto& line : lines) {
            line = i + line;
        }
    }

    inline void indent(std::vector<std::string>& lines, size_t start, size_t stop)
    {
        auto i = indent();
        for (size_t j = start; j < stop; ++j) {
            lines[j] = i + lines[j];
        }
    }

    inline void indent(size_t n, std::vector<std::string>& lines)
    {
        auto i = indent(n);
        for (auto& line : lines) {
            line = i + line;
        }
    }

    inline void indent(size_t n, std::vector<std::string>& lines, size_t start, size_t stop)
    {
        auto i = indent(n);
        for (size_t j = start; j < stop; ++j) {
            lines[j] = i + lines[j];
        }
    }

} // namespace detail

inline std::string join(const std::vector<std::string>& lines, const std::string& sep="\n")
{
    if (lines.size() == 1) {
        return lines[0];
    }

    std::string ret = "";

    for (auto line : lines) {
        if (ret.size() == 0) {
            ret += line;
            continue;
        }

        if (line[0] == sep[0]) {
            ret += line;
        }
        else if (ret[ret.size() - 1] == sep[0]) {
            ret += line;
        }
        else {
            ret += sep + line;
        }
    }

    return ret;
}

inline auto Geometry(
    const HighFive::File& file,
    const std::string& dataset)
{
    std::vector<std::string> ret;
    auto shape = H5Easy::getShape(file, dataset);
    auto fname = file.getName();

    XDMFWRITE_HIGHFIVE_ASSERT(shape.size() == 2);

    if (shape[1] == 1) {
        ret.push_back("<Geometry GeometryType=\"X\">");
    }
    else if (shape[1] == 2) {
        ret.push_back("<Geometry GeometryType=\"XY\">");
    }
    else if (shape[1] == 3) {
        ret.push_back("<Geometry GeometryType=\"XYZ\">");
    }
    else {
        throw std::runtime_error("Illegal number of dimensions.");
    }

    ret.push_back(
        detail::indent() + "<DataItem Dimensions=\"" + std::to_string(shape[0]) + " " +
        std::to_string(shape[1]) + "\" Format=\"HDF\">" + fname + ":" + dataset +
        "</DataItem>");

    ret.push_back("</Geometry>)");

    return ret;
}

template <class T>
inline auto Topology(
    const HighFive::File& file,
    const std::string& dataset,
    const T& type)
{
    std::vector<std::string> ret;
    auto shape = H5Easy::getShape(file, dataset);
    auto fname = file.getName();

    XDMFWRITE_HIGHFIVE_ASSERT(shape.size() == 2);
    XDMFWRITE_HIGHFIVE_ASSERT(shape[1] == detail::number_of_columns(type));

    ret.push_back(
        "<Topology NumberOfElements=\"" + std::to_string(shape[0]) + "\" TopologyType=\"" +
        detail::to<T>::str(type) + "\">");

    ret.push_back(
        detail::indent() + "<DataItem Dimensions=\"" + std::to_string(shape[0]) + " " +
        std::to_string(shape[1]) + "\" Format=\"HDF\">" + fname + ":" + dataset +
        "</DataItem>");

    ret.push_back("</Topology>");

    return ret;
}

template <class T>
inline auto Attribute(
    const HighFive::File& file,
    const std::string& dataset,
    const T& type,
    const std::string &name)
{
    std::vector<std::string> ret;
    auto shape = H5Easy::getShape(file, dataset);
    auto fname = file.getName();

    XDMFWRITE_HIGHFIVE_ASSERT(shape.size() > 0);
    XDMFWRITE_HIGHFIVE_ASSERT(shape.size() < 3);

    if (shape.size() == 1) {
        ret.push_back(
            "<Attribute AttributeType=\"Scalar\" Center=\"" + detail::to<T>::str(type) +
            "\" Name=\"" + name + "\">");

        ret.push_back(
            detail::indent() + "<DataItem Dimensions=\"" + std::to_string(shape[0]) +
            "\" Format=\"HDF\">" + fname + ":" + dataset + "</DataItem>");
    }
    else if (shape.size() == 2) {
        ret.push_back(
            "<Attribute AttributeType=\"Vector\" Center=\"" + detail::to<T>::str(type) +
            "\" Name=\"" + name + "\">");

        ret.push_back(
            detail::indent() + "<DataItem Dimensions=\"" + std::to_string(shape[0]) + " " +
            std::to_string(shape[1]) + "\" Format=\"HDF\">" + fname + ":" + dataset +
            "</DataItem>");
    }

    ret.push_back("</Attribute>)");

    return ret;
}

inline auto Attribute(
    const HighFive::File& file,
    const std::string& dataset,
    AttributeType type)
{
    return Attribute(file, dataset, type, dataset);
}

inline auto Grid(
    const std::string& name,
    std::initializer_list<std::vector<std::string>> args)
{
    std::vector<std::string> ret;
    ret.push_back(
        "<Grid CollectionType=\"Temporal\" GridType=\"Collection\" Name=\"" + name + "\">");
    ret.push_back("<Grid Name=\"" + name + "\">");
    for (auto& arg : args) {
        ret.insert(ret.end(), arg.begin(), arg.end());
    }
    ret.push_back("</Grid>");
    ret.push_back("</Grid>");
    detail::indent(3, ret, 2, ret.size() - 2);
    return ret;
}

inline auto Grid(std::initializer_list<std::vector<std::string>> args)
{
    return Grid("Grid", args);
}

inline TimeSeries::TimeSeries(const std::string& name) : m_name(name)
{
}

template <class T>
inline void TimeSeries::push_back(
    const std::string& name,
    const T& time,
    std::initializer_list<std::vector<std::string>> args)
{
    m_lines.push_back("<Grid Name=\"" + name + "\">");
    m_lines.push_back(detail::indent() + "<Time Value=\"" + detail::to<T>::str(time) + "\"/>");
    size_t start = m_lines.size();
    for (auto& arg : args) {
        m_lines.insert(m_lines.end(), arg.begin(), arg.end());
    }
    size_t stop = m_lines.size();
    detail::indent(m_lines, start, stop);
    m_lines.push_back("</Grid>");
    m_n++;
}

inline void TimeSeries::push_back(
    std::initializer_list<std::vector<std::string>> args)
{
    return this->push_back("Increment " + std::to_string(m_n), m_n, args);
}

inline auto TimeSeries::get() const
{
    std::vector<std::string> ret;
    ret.push_back(
        "<Grid CollectionType=\"Temporal\" GridType=\"Collection\" Name=\"" + m_name + "\">");
    ret.insert(ret.end(), m_lines.begin(), m_lines.end());
    ret.push_back("</Grid>");
    detail::indent(ret, 1, ret.size() - 1);
    return ret;
}

template <class T>
inline std::string write(const T& arg)
{
    std::vector<std::string> ret;
    std::vector<std::string> lines = detail::convert<T>::get(arg);
    ret.push_back("<Xdmf Version=\"3.0\">");
    ret.push_back(detail::indent() + "<Domain>");
    ret.insert(ret.end(), lines.begin(), lines.end());
    ret.push_back(detail::indent() + "</Domain>");
    ret.push_back("</Xdmf>");
    detail::indent(2, ret, 2, ret.size() - 2);
    return join(ret);
}

template <class T>
inline std::string write(const std::string& fname, const T& arg)
{
    auto ret = write(arg);
    std::ofstream myfile;
    myfile.open(fname);
    myfile << ret << std::endl;
    myfile.close();
    return ret;
}

} // namespace XDMFWrite_HighFive

#endif
