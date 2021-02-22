[![CI](https://github.com/tdegeus/XDMFWrite_HighFive/workflows/CI/badge.svg)](https://github.com/tdegeus/XDMFWrite_HighFive/actions)
[![Doxygen -> gh-pages](https://github.com/tdegeus/XDMFWrite_HighFive/workflows/gh-pages/badge.svg)](https://tdegeus.github.io/XDMFWrite_HighFive)
[![Conda Version](https://img.shields.io/conda/vn/conda-forge/XDMFWrite_HighFive.svg)](https://anaconda.org/conda-forge/XDMFWrite_HighFive)

# XDMFWrite_HighFive

<!-- MarkdownTOC -->

- [Fields](#fields)
    - [Geometry \(nodal-coordinates or vertices\)](#geometry-nodal-coordinates-or-vertices)
    - [Topology \(connectivity\)](#topology-connectivity)
    - [Attribute](#attribute)
- [Short-hand](#short-hand)
    - [Unstructured](#unstructured)
    - [Structured](#structured)
- [Grids](#grids)
    - [Grid](#grid)
    - [TimeSeries](#timeseries)
- [Output](#output)
    - [write](#write)
- [Miscellaneous](#miscellaneous)
    - [Join sequence of strings](#join-sequence-of-strings)
    - [Concatenate vectors](#concatenate-vectors)
    - [Change the indentation](#change-the-indentation)

<!-- /MarkdownTOC -->

XDMF meets HighFive: Write XDMF files to interpret HDF5 files.

## Fields

### Geometry (nodal-coordinates or vertices)

Interpret a DataSet as a Geometry (a.k.a. nodal-coordinates or vertices).

### Topology (connectivity)

Interpret a DataSet as a Topology (a.k.a. connectivity).

### Attribute 

Interpret a DataSet as an Attribute. 

## Short-hand

### Unstructured

Interpret a DataSets as Unstructured. 
The call

```cpp
Structured(file, "/path/to/geometry", "/path/to/topology", type);
```

Is simply short for 

```cpp
concatenate({
    Geometry(file, "/path/to/geometry"), // nodal-coordinates / vertices
    Topology(file, "/path/to/topology", type) // connectivity
});
```

### Structured

Interpret a DataSets as Structured (individual points). 
Use is made of a mock Topology `arange(N)`, with `N` the number of nodes (vertices).
The call

```cpp
Structured(file, "/path/to/geometry", "/path/to/topology");
```

Is simply short for 

```cpp
concatenate({
    Geometry(file, "/path/to/geometry"), // nodal-coordinates / vertices
    Topology(file, "/path/to/topology", ElementType::Polyvertex) // mock connectivity
});
```

## Grids

### Grid

Combine fields (e.g. Geometry, Topology, and Attribute) to a single grid.

### TimeSeries

Combine a series of fields (e.g. Geometry, Topology, and Attribute) to a time-series.
The syntax is as follows:

```cpp
TimeSeries series;
series.push_back(...);
series.push_back(...);
```

To then write a file simply pass `series` to `write`. 
To alternatively get the sequence of strings of TimeSeries use `series.get()`.

## Output

### write

Write a complete XDMF-file, e.g. from Grid or TimeSeries.

## Miscellaneous 

### Join sequence of strings

Join a sequence of strings:

```cpp
std::vector<std::string> lines = {"foo", "bar"};
std::string single = join(lines);
std::cout << single << std::endl;
```

### Concatenate vectors

Concatenate an arbitrary number of vectors:

```cpp
concatenate({...});
```

### Change the indentation

The indentation can be changed by specifying 

```cpp
#define XDMFWRITE_HIGHFIVE_INDENT 4
```

before

```cpp
#include <XDMFWrite_HighFive.hpp>
```
