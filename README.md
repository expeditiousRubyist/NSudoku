# N-Sudoku
Template library for solving sudoku boards of arbitrary size

# What is an n-sudoku?
An n-sudoku puzzle is an *n²* ✕ *n²* grid, which shall be labeled as *M* (or the "main" grid). This grid is further subdivided into *n²* subgrids, which shall be labeled as *Sᵢ* (for "Subgrid 1" to "Subgrid *n²*"), where *M* has *n* rows and *n* columns of subgrids.

To solve an n-sudoku puzzle, one is given *M* with certain elements initialized to some number in the range 1 .. *n²*, and all other elements initialized to 0. One must find a grid with those initialized elements in their respective positions, and satisfying the following properties:

* Each of the *n²* rows of *M* contain the numbers 1 .. *n²*
* Each of the *n²* columns of *M* contain the numbers 1 .. *n²*
* Each of the *n²* subgrids of *M* contain the numbers 1 .. *n²*

The traditional sudoku puzzle with which most people are accustomed to is considered a 3-sudoku under this definition. A 2-sudoku puzzle, might, for example, look like this:

|   |   |   |   |
|---|---|---|---|
| 1 | 0 | 2 | 0 |
| 0 | 4 | 0 | 1 |
| 0 | 1 | 0 | 2 |
| 4 | 2 | 1 | 0 |

Its solution, in turn, might look like this:

|   |   |   |   |
|---|---|---|---|
| 1 | 3 | 2 | 4 |
| 2 | 4 | 3 | 1 |
| 3 | 1 | 4 | 2 |
| 4 | 2 | 1 | 3 |

# Program Input
To use the library, one must use the function `sudoku::solve`. It takes as template arguments any integer type, and the value of *n* for the sudoku board. Its sole function argument is a pointer to a block of memory containing the sudoku board. It is assumed that for some n-sudoku puzzle, the board contains *n⁴* elements, such that the element in row *i* and column *j* of the board (assuming we label the 1st row or column as row 0 or column 0) is located at offset *i² + j* in memory. As an example, the offsets of a 2-sudoku (in hex) are as follows:

|   |   |   |   |
|---|---|---|---|
| 0 | 1 | 2 | 3 |
| 4 | 5 | 6 | 7 |
| 8 | 9 | A | B |
| C | D | E | F |

# Program output
Upon the function's termination, if the board was solvable, the function will return `true`, and the memory block containing the initial state *M* will contain a solved board. If the board was not solvable, the function will return `false`.