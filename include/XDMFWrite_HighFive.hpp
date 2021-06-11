/**
XDMF meets HighFive: Write XDMF files to interpret HDF5 files.
References:

-   http://xdmf.org/index.php/XDMF_Model_and_Format

\file XDMFWrite_HighFive.hpp
\copyright Copyright 2020. Tom de Geus. All rights reserved.
\license This project is released under the GNU Public License (GPLv3).
*/

#ifndef XDMFWRITE_HIGHFIVE_H
#define XDMFWRITE_HIGHFIVE_H

#include <fstream>
#include <highfive/H5Easy.hpp>

/**
\cond
*/
#define Q(x) #x
#define QUOTE(x) Q(x)

#define XDMFWRITE_HIGHFIVE_ASSERT_IMPL(expr, file, line) \
    if (!(expr)) { \
        throw std::runtime_error( \
            std::string(file) + ':' + std::to_string(line) + \
            ": assertion failed (" #expr ") \n\t"); \
    }

#define XDMFWRITE_HIGHFIVE_THROW_IMPL(message, file, line) \
    std::runtime_error( \
        std::string(file) + ':' + std::to_string(line) + \
        ": " message " \n\t"); \
/**
\endcond
*/

/**
All assertions are implementation as::

    XDMFWRITE_HIGHFIVE_ASSERT(...)

They can be enabled by::

    #define XDMFWRITE_HIGHFIVE_ENABLE_ASSERT

(before including XDMFWrite_HighFive).
The advantage is that:

-   File and line-number are displayed if the assertion fails.
-   XDMFWrite_HighFive's assertions can be enabled/disabled independently
    from those of other libraries.

\throw std::runtime_error
*/
#ifdef XDMFWRITE_HIGHFIVE_ENABLE_ASSERT
    #define XDMFWRITE_HIGHFIVE_ASSERT(expr) XDMFWRITE_HIGHFIVE_ASSERT_IMPL(expr, __FILE__, __LINE__)
#else
    #define XDMFWRITE_HIGHFIVE_ASSERT(expr)
#endif

/**
Assertion that cannot be switched off. Implement assertion by::

    XDMFWRITE_HIGHFIVE_CHECK(...)

\throw std::runtime_error
*/
#define XDMFWRITE_HIGHFIVE_CHECK(expr) XDMFWRITE_HIGHFIVE_ASSERT_IMPL(expr, __FILE__, __LINE__)

/**
Throw error such that the source-file and line-number are added::

    throw XDMFWRITE_HIGHFIVE_THROW(...)
*/
#define XDMFWRITE_HIGHFIVE_THROW(message) XDMFWRITE_HIGHFIVE_THROW_IMPL(message, __FILE__, __LINE__)

/**
Current version.

Either:

-   Configure using CMake at install time. Internally uses::

        python -c "from setuptools_scm import get_version; print(get_version())"

-   Define externally using::

        -DXDMFWRITE_HIGHFIVE_VERSION="`python -c "from setuptools_scm import get_version; print(get_version())"`"

    From the root of this project. This is what ``setup.py`` does.

Note that both ``CMakeLists.txt`` and ``setup.py`` will construct the version string using
``setuptools_scm`` **unless** an environment ``PKG_VERSION`` is defined.
If ``PKG_VERSION`` is defined the version string will be read from that variable.
*/
#ifndef XDMFWRITE_HIGHFIVE_VERSION
#define XDMFWRITE_HIGHFIVE_VERSION "@PROJECT_VERSION@"
#endif

/**
Define the indentation level of the output.
To change the default to e.g. 2::

    #define XDMFWRITE_HIGHFIVE_INDENT 2

**before** including XDMFWrite_HighFive.
*/
#ifndef XDMFWRITE_HIGHFIVE_INDENT
    #define XDMFWRITE_HIGHFIVE_INDENT 4
#endif

#ifndef XDMFWRITE_HIGHFIVE_USE_GOOSEFEM
    #ifdef GOOSEFEM_VERSION
        #define XDMFWRITE_HIGHFIVE_USE_GOOSEFEM
        #include <GooseFEM/Mesh.h>
    #endif
#endif

namespace XDMFWrite_HighFive {

/**
Return version string, e.g.::

    "0.8.0"

\return std::string
*/
inline std::string version();

/**
Returns a string in which the string elements of a sequence have been joined by a separator.

\param lines Sequence of strings.
\param sep Separator with which to join the lines.
\return String.
*/
inline std::string join(const std::vector<std::string>& lines, const std::string& sep="\n");

/**
Concatenate lists.

\param args Lists, specified as ``{...}``.
\return Concatenated list.
*/
template <class T>
inline std::vector<T> concatenate(std::initializer_list<std::vector<T>> args);

/**
Specify the ElementType() for a certain Topology().
*/
enum class ElementType {
    Polyvertex, ///< Polyvertex
    Triangle, ///< Triangle
    Quadrilateral, ///< Quadrilateral
    Hexahedron ///< Hexahedron
};

/**
Specify the AttributeCenter() of a field.
See: https://www.xdmf.org/index.php/XDMF_Model_and_Format#Attribute
*/
enum class AttributeCenter {
    Cell, ///< Cell.
    Node ///< Node.
};

/**
Interpret a DataSet as a Geometry().
Other common terms for Geometry():
-   Nodal coordinates.
-   Vertices.

\param file An open and readable HighFive file.
\param dataset Path to the DataSet.
\return Sequence of strings to be used in an XDMF-file.
*/
inline std::vector<std::string> Geometry(
    const HighFive::File& file,
    const std::string& dataset);

/**
Interpret a DataSet as a Topology().
Other common terms for Topology():
-   Connectivity.

\param file An open and readable HighFive file.
\param dataset Path to the DataSet.
\param type Element-type (see ElementType).
\return Sequence of strings to be used in an XDMF-file.
*/
template <class T>
inline std::vector<std::string> Topology(
    const HighFive::File& file,
    const std::string& dataset,
    const T& type);

/**
Interpret a DataSet as an Attribute().
See: https://www.xdmf.org/index.php/XDMF_Model_and_Format#Attribute

By default the path of the DataSet is used as name in the XDMF-file.
An overload is available to specify a different name.

\param file An open and readable HighFive file.
\param dataset Path to the DataSet.
\param center How to center the Attribute (see AttributeCenter()).
\return Sequence of strings to be used in an XDMF-file.
*/
template <class T>
inline std::vector<std::string> Attribute(
    const HighFive::File& file,
    const std::string& dataset,
    const T& center);

/**
Interpret a DataSet as an Attribute().
See: https://www.xdmf.org/index.php/XDMF_Model_and_Format#Attribute

\param file An open and readable HighFive file.
\param dataset Path to the DataSet.
\param center How to center the Attribute (see AttributeCenter()).
\param name Name to use in the XDMF-file.
\return Sequence of strings to be used in an XDMF-file.
*/
template <class T>
inline std::vector<std::string> Attribute(
    const HighFive::File& file,
    const std::string& dataset,
    const T& center,
    const std::string &name);

/**
Combine fields (Geometry(), Topology(), Attribute()) to a single grid.

\param name
    Name of the grid.

\param args
    The fields (themselves a sequence of strings) to write.
    An arbitrary number of string sequences can be combined using ``{...}``.

\return
    Sequence of strings to be used in an XDMF-file.
*/
inline std::vector<std::string> Grid(
    const std::string& name,
    std::initializer_list<std::vector<std::string>> args);

/**
Combine fields (Geometry(), Topology(), Attribute()) to a single grid.
An overload is available to specify the name.

\param args
    The fields (themselves a sequence of strings) to write.
    An arbitrary number if string sequences can be combined using ``{...}``.

\return Sequence of strings to be used in an XDMF-file.
*/
inline std::vector<std::string> Grid(
    std::initializer_list<std::vector<std::string>> args);

/**
Combine a series of fields (e.g. Geometry(), Topology(), and Attribute) to a time-series.
*/
class TimeSeries
{
public:

    /**
    Constructor.
    An overload is available to specify the name of the TimeSeries.
    */
    TimeSeries() = default;

    /**
    Constructor, allowing a custom name of the TimeSeries.

    \param name Name of the TimeSeries.
    */
    TimeSeries(const std::string& name);

    /**
    Add a time-step given by a combination of fields (e.g. Geometry(), Topology(), and Attribute()).

    \param name Name of the increment.
    \param time Time value of the increment.
    \param args The fields (themselves a sequence of strings) to write.
    * An arbitrary number if string sequences can be combined using {...}.
    */
    template <class T>
    inline void push_back(
        const std::string& name,
        const T& time,
        std::initializer_list<std::vector<std::string>> args);

    /**
    Add a time-step given by a combination of fields (e.g. Geometry(), Topology(), and Attribute()).
    An overload is available to specify the time and name of the increment.

    \param time
        Time value of the increment.

    \param args
        The fields (themselves a sequence of strings) to write.
        An arbitrary number if string sequences can be combined using ``{...}``.
    */
    template <class T>
    inline void push_back(
        const T& time,
        std::initializer_list<std::vector<std::string>> args);

    /**
    Add a time-step given by a combination of fields (e.g. Geometry(), Topology(), and Attribute()).
    An overload is available to specify the time and name of the increment.

    \param args
        The fields (themselves a sequence of strings) to write.
        An arbitrary number if string sequences can be combined using ``{...}``.

    \return
        Sequence of strings to be used in an XDMF-file.
    */
    inline void push_back(
        std::initializer_list<std::vector<std::string>> args);


    /**
    Get sequence of strings to be used in an XDMF-file.

    \return Sequence of strings to be used in an XDMF-file.
    */
    inline std::vector<std::string> get() const;

private:
    std::vector<std::string> m_lines;
    std::string m_name = "TimeSeries";
    size_t m_n = 0;
};

/**
Interpret a DataSets as a Structured (individual points). This is simply short for the
concatenation of:

-   ``Geometry(file, "/coor")`` and
-   ``Topology(file, "/conn", ElementType::Polyvertex)``.

\param file An open and readable HighFive file.
\param dataset_geometry Path to the Geometry() DataSet.
\param dataset_topology Path to a mock Topology() arange(N), with N the number of nodes (vertices).
\return Sequence of strings to be used in an XDMF-file.
*/
inline std::vector<std::string> Structured(
    const HighFive::File& file,
    const std::string& dataset_geometry,
    const std::string& dataset_topology);

/**
Interpret a DataSets as a Unstructured
(Geometry() and Topology() / nodal-coordinates and connectivity). This is simply short for the
concatenation of
-   ``Geometry(file, "/coor")`` and
-   ``Topology(file, "/conn", type)``.

\param file An open and readable HighFive file.
\param dataset_geometry Path to the Geometry() DataSet.
\param dataset_topology Path to the Topology() DataSet.
\param type Element-type (see ElementType()).
\return Sequence of strings to be used in an XDMF-file.
*/
template <class T>
inline std::vector<std::string> Unstructured(
    const HighFive::File& file,
    const std::string& dataset_geometry,
    const std::string& dataset_topology,
    const T& type);

/**
Get a complete XDMF-file, e.g. from Grid() or TimeSeries().

\param arg The data (any of the XDMFWrite_HighFive-classes or a sequence of strings) to write.
\return XDMF-file as string.
*/
template <class T>
inline std::string write(const T& arg);

/**
Write a complete XDMF-file, e.g. from Grid() or TimeSeries().

\param filename The filename to write to (file is overwritten).
\param arg The data (any of the XDMFWrite_HighFive-classes or a sequence of strings) to write.
\return XDMF-file as string.
*/
template <class T>
inline std::string write(const std::string& filename, const T& arg);

// --- Implementation ---

namespace detail {

    inline std::string unquote(const std::string& arg)
    {
        std::string ret = arg;
        ret.erase(std::remove(ret.begin(), ret.end(), '\"'), ret.end());
        return ret;
    }

}

inline std::string version()
{
    return detail::unquote(std::string(QUOTE(XDMFWRITE_HIGHFIVE_VERSION)));
}

namespace detail {

    // SFINAE helper: check if a template argument is a string
    template <class T>
    struct is_string : std::false_type
    {
    };

    template <>
    struct is_string<std::string> : std::true_type
    {
    };

    // SFINAE helper: check if a template argument is a list of strings
    template <class T>
    struct is_string_list : std::false_type
    {
    };

    template <>
    struct is_string_list<std::vector<std::string>> : std::true_type
    {
    };

    // SFINAE helper: check if a template argument is an ElementType enum-class.
    template <class T>
    struct is_ElementType : std::false_type
    {
    };

    template <>
    struct is_ElementType<ElementType> : std::true_type
    {
    };

    // SFINAE helper: check if a template argument is an AttributeCenter enum-class.
    template <class T>
    struct is_AttributeCenter : std::false_type
    {
    };

    template <>
    struct is_AttributeCenter<AttributeCenter> : std::true_type
    {
    };

    // SFINAE helper: check if a template argument is an GooseFEM-ElementType enum-class.
    #ifdef XDMFWRITE_HIGHFIVE_USE_GOOSEFEM
    template <class T>
    struct is_GooseFEM_ElementType : std::false_type
    {
    };

    template <>
    struct is_GooseFEM_ElementType<GooseFEM::Mesh::ElementType> : std::true_type
    {
    };
    #endif

    // Convert object to string
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
            if (arg == ElementType::Polyvertex) {
                return "Polyvertex";
            }
            else if (arg == ElementType::Triangle) {
                return "Triangle";
            }
            else if (arg == ElementType::Quadrilateral) {
                return "Quadrilateral";
            }
            else if (arg == ElementType::Hexahedron) {
                return "Hexahedron";
            }
            throw XDMFWRITE_HIGHFIVE_THROW("Unknown ElementType");
        }

        static ElementType type(const T& arg)
        {
            return arg;
        }
    };

    #ifdef XDMFWRITE_HIGHFIVE_USE_GOOSEFEM
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
            throw XDMFWRITE_HIGHFIVE_THROW("Unknown ElementType");
        }

        static ElementType type(const T& arg)
        {
            if (arg == GooseFEM::Mesh::ElementType::Tri3) {
                return ElementType::Triangle;
            }
            else if (arg == GooseFEM::Mesh::ElementType::Quad4) {
                return ElementType::Quadrilateral;
            }
            else if (arg == GooseFEM::Mesh::ElementType::Hex8) {
                return ElementType::Hexahedron;
            }
            throw XDMFWRITE_HIGHFIVE_THROW("Unknown ElementType");
        }
    };
    #endif

    template <class T>
    struct to<T, typename std::enable_if_t<is_AttributeCenter<T>::value>>
    {
        static std::string str(const T& arg)
        {
            if (arg == AttributeCenter::Cell) {
                return "Cell";
            }
            else if (arg == AttributeCenter::Node) {
                return "Node";
            }
            throw XDMFWRITE_HIGHFIVE_THROW("Unknown AttributeType");
        }
    };

    template <class T>
    inline std::string join_as_string(const std::vector<T>& arg, std::string sep)
    {
        std::vector<std::string> ret;
        for (auto& i : arg) {
            ret.push_back(to<T>::str(i));
        }
        return join(ret, sep);
    }

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

    template <class T, class E>
    inline bool check_shape(const T& shape, E type)
    {
        auto t = detail::to<E>::type(type);

        if (shape.size() == 1 && t == ElementType::Polyvertex) {
            return true;
        }
        if (shape.size() != 2) {
            return false;
        }
        if (shape[1] == 3 && t == ElementType::Triangle) {
            return true;
        }
        if (shape[1] == 4 && t == ElementType::Quadrilateral) {
            return true;
        }
        if (shape[1] == 8 && t == ElementType::Hexahedron) {
            return true;
        }
        return false;
    }

    inline std::string indent()
    {
        std::string ret = "";
        if (XDMFWRITE_HIGHFIVE_INDENT == 0) {
            return ret;
        }
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

inline std::string join(const std::vector<std::string>& lines, const std::string& sep)
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

template <class T>
inline std::vector<T> concatenate(std::initializer_list<std::vector<T>> args)
{
    size_t n = 0;
    for (auto& arg : args) {
        n += arg.size();
    }
    std::vector<T> ret;
    ret.reserve(n);
    for (auto& arg : args) {
        ret.insert(ret.end(), arg.begin(), arg.end());
    }
    return ret;
}

inline std::vector<std::string> Geometry(
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
        throw XDMFWRITE_HIGHFIVE_THROW("Illegal number of dimensions.");
    }

    ret.push_back(
        detail::indent() + "<DataItem Dimensions=\"" + std::to_string(shape[0]) + " " +
        std::to_string(shape[1]) + "\" Format=\"HDF\">" + fname + ":" + dataset +
        "</DataItem>");

    ret.push_back("</Geometry>");

    return ret;
}

template <class T>
inline std::vector<std::string> Topology(
    const HighFive::File& file,
    const std::string& dataset,
    const T& type)
{
    std::vector<std::string> ret;
    auto shape = H5Easy::getShape(file, dataset);
    auto fname = file.getName();

    XDMFWRITE_HIGHFIVE_ASSERT(detail::check_shape(shape, type));

    ret.push_back(
        "<Topology NumberOfElements=\"" + std::to_string(shape[0]) + "\" TopologyType=\"" +
        detail::to<T>::str(type) + "\">");

    ret.push_back(
        detail::indent() + "<DataItem Dimensions=\"" + detail::join_as_string(shape, " ") +
        "\" Format=\"HDF\">" + fname + ":" + dataset +
        "</DataItem>");

    ret.push_back("</Topology>");

    return ret;
}

inline std::vector<std::string> Structured(
    const HighFive::File& file,
    const std::string& dataset_geometry,
    const std::string& dataset_topology)
{
    auto shape_geometry = H5Easy::getShape(file, dataset_geometry);
    auto shape_topology = H5Easy::getShape(file, dataset_topology);

    XDMFWRITE_HIGHFIVE_ASSERT(shape_geometry[0] == shape_topology[0]);

    return concatenate({
        Geometry(file, dataset_geometry),
        Topology(file, dataset_topology, ElementType::Polyvertex)});
}

template <class T>
inline std::vector<std::string> Unstructured(
    const HighFive::File& file,
    const std::string& dataset_geometry,
    const std::string& dataset_topology,
    const T& type)
{
    return concatenate({
        Geometry(file, dataset_geometry),
        Topology(file, dataset_topology, type)});
}

template <class T>
inline std::vector<std::string> Attribute(
    const HighFive::File& file,
    const std::string& dataset,
    const T& center,
    const std::string &name)
{
    std::vector<std::string> ret;
    auto shape = H5Easy::getShape(file, dataset);
    auto fname = file.getName();

    XDMFWRITE_HIGHFIVE_ASSERT(shape.size() > 0);
    XDMFWRITE_HIGHFIVE_ASSERT(shape.size() < 3);

    std::string t;
    if (shape.size() == 1) {
        t = "Scalar";
    }
    else if (shape.size() == 2) {
        t = "Vector";
    }
    else {
        throw XDMFWRITE_HIGHFIVE_THROW("Type of data cannot be deduced");
    }

    ret.push_back(
        "<Attribute AttributeType=\"" + t + "\" Center=\"" + detail::to<T>::str(center) +
        "\" Name=\"" + name + "\">");

    ret.push_back(
        detail::indent() + "<DataItem Dimensions=\"" + detail::join_as_string(shape, " ") +
        "\" Format=\"HDF\">" + fname + ":" + dataset + "</DataItem>");

    ret.push_back("</Attribute>");

    return ret;
}

template <class T>
inline std::vector<std::string> Attribute(
    const HighFive::File& file,
    const std::string& dataset,
    const T& center)
{
    return Attribute(file, dataset, center, dataset);
}

inline std::vector<std::string> Grid(
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
    detail::indent(2, ret, 2, ret.size() - 2);
    return ret;
}

inline std::vector<std::string> Grid(std::initializer_list<std::vector<std::string>> args)
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

template <class T>
inline void TimeSeries::push_back(
    const T& time,
    std::initializer_list<std::vector<std::string>> args)
{
    return this->push_back("Increment " + std::to_string(m_n), time, args);
}

inline void TimeSeries::push_back(
    std::initializer_list<std::vector<std::string>> args)
{
    return this->push_back("Increment " + std::to_string(m_n), m_n, args);
}

inline std::vector<std::string> TimeSeries::get() const
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
