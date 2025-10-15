#include <string>
#include <iostream>
#include <chrono>
#include <vector>

struct Edge
{
    enum class Color { Yellow, Green, Blue, Purple };
    enum class Part { Top, Bottom }; // Top = head, Bottom = legs

    Color color;
    Part part;

    bool operator== (const Edge& other) const noexcept
    {
        return (this->color == other.color && this->part == other.part);
    }

    bool operator!=(const Edge& other) const noexcept
    {
        return !(*this == other);
    }

    std::string colorToString() const noexcept
    {
        switch (color)
        {
        case Color::Yellow: return "Yellow";
        case Color::Green: return "Green";
        case Color::Blue: return "Blue";
        case Color::Purple: return "Purple";
        }
        return "?";
    }

    std::string partToString() const noexcept
    {
        return (part == Part::Top) ? "Top" : "Bottom";
    }

    // Match: same color, opposite part (Top vs Bottom)
    bool matchEdges(const Edge& other) const noexcept
    {
        return (color == other.color) && (part != other.part);
    }
}; // struct Edge

struct Tile
{
    enum class EdgeDirection
    {
        Top = 0,
        Right = 1,
        Bottom = 2,
        Left = 3,
        Max = 4 // Number of edge directions
    };

    static const int NoOfEdges = static_cast<const int>(EdgeDirection::Max);

    int id;
    Edge edges[NoOfEdges];

    void rotate90cw() noexcept
    {
        Edge tmp = edges[static_cast<const int>(EdgeDirection::Left)];
        edges[static_cast<const int>(EdgeDirection::Left)] = edges[static_cast<const int>(EdgeDirection::Bottom)];
        edges[static_cast<const int>(EdgeDirection::Bottom)] = edges[static_cast<const int>(EdgeDirection::Right)];
        edges[static_cast<const int>(EdgeDirection::Right)] = edges[static_cast<const int>(EdgeDirection::Top)];
        edges[static_cast<const int>(EdgeDirection::Top)] = tmp;
    }

    //Compare with other tile. To do rotations during compare set rotateDuringCompare to true
    bool similarTile(const Tile& other, bool rotateDuringCompare) const noexcept
    {
        int noOfRotations;
        
        if (rotateDuringCompare == true)
        {
            noOfRotations = NoOfEdges; // Do rotate of other's edge to test all possible rotations
        }
        else
        {
            noOfRotations = 1; // Do not rotate during compare
        }

        // Rotate loop
        for (int rotateOtherEdge = 0; rotateOtherEdge < noOfRotations; rotateOtherEdge++)
        {
            bool match = true;
            for (int edge = 0; edge < NoOfEdges; ++edge)
            {
                if (this->edges[edge] != other.edges[(edge + rotateOtherEdge) % NoOfEdges])
                {
                    match = false;
                    break; // Break edge for-loop
                }                    
            }
            if (match == true)
            {
                return true;
            }
        }
        return false;
    }
}; // struct Tile

static const int N = 3;
static const int NumberOfTiles = N * N; // Tiles 3x3
using TileGrid = Tile[N][N]; // Type for having the tiles placed

struct TileGridStruct // To be used in vector (since vector cannot handle 2 dimensional arrays as element)
{
    TileGrid tiles;

    TileGridStruct(const TileGrid& copyFrom) noexcept
    {
        copyFromTileGrid(copyFrom);
    }

    void copyFromTileGrid(const TileGrid& copyFrom) noexcept
    {
        for (int r = 0; r < N; ++r)
        {
            for (int c = 0; c < N; ++c)
            {
                tiles[r][c] = copyFrom[r][c];
            }
        }
    }

    bool operator== (const TileGridStruct& other) const noexcept
    {
        for (int r = 0; r < N; ++r)
        {
            for (int c = 0; c < N; ++c)
            {
                // Do not rotate when comparing tiles. They have to match without rotating
                if (tiles[r][c].similarTile(other.tiles[r][c], false) == false)
                {
                    return false;
                }
            }
        }
        return true;
    }

private:
    std::string asciiColorCode(Edge::Color c) const noexcept
    {
        switch (c)
        {
            //case Color::Yellow: return "\033[33m"; // Yellow text
            //case Color::Green:  return "\033[32m"; // Green text
            //case Color::Blue:   return "\033[34m"; // Blue text
            //case Color::Purple: return "\033[35m"; // Purple text

            //case Color::Yellow: return "\033[43m"; // Yellow background
            //case Color::Green:  return "\033[42m"; // Green background
            //case Color::Blue:   return "\033[44m"; // Blue background
            //case Color::Purple: return "\033[45m"; // Purple background

        case Edge::Color::Yellow: return "\033[30;43m"; // Black text, yellow background
        case Edge::Color::Green:  return "\033[30;42m"; // Black text, green background
        case Edge::Color::Blue:   return "\033[37;44m"; // White text, blue background
        case Edge::Color::Purple: return "\033[37;45m"; // White text, purple background
        }
        return "\033[0m";
    }

    std::string resetAsciiColorCode() const noexcept
    {
        return "\033[0m";
    }

public:
    void printAsciiTiles() const noexcept
    {
        for (int row = 0; row < N; ++row)
        {
            // Each tile is 5 lines tall
            for (int sub = 0; sub < 5; ++sub)
            {
                for (int col = 0; col < N; ++col)
                {
                    const Tile& t = tiles[row][col];

                    // Find edges
                    const Edge& topEdge = t.edges[static_cast<int>(Tile::EdgeDirection::Top)];
                    const Edge& rightEdge = t.edges[static_cast<int>(Tile::EdgeDirection::Right)];
                    const Edge& bottomEdge = t.edges[static_cast<int>(Tile::EdgeDirection::Bottom)];
                    const Edge& leftEdge = t.edges[static_cast<int>(Tile::EdgeDirection::Left)];

                    // Determine symbols
                    char topSym = (topEdge.part == Edge::Part::Top) ? 'O' : '|';
                    char rightSym = (rightEdge.part == Edge::Part::Bottom) ? '-' : 'O';
                    char bottomSym = (bottomEdge.part == Edge::Part::Bottom) ? '|' : 'O';
                    char leftSym = (leftEdge.part == Edge::Part::Bottom) ? '-' : 'O';

                    // Colors
                    std::string topColor = asciiColorCode(topEdge.color);
                    std::string rightColor = asciiColorCode(rightEdge.color);
                    std::string bottomColor = asciiColorCode(bottomEdge.color);
                    std::string leftColor = asciiColorCode(leftEdge.color);
                    std::string reset = resetAsciiColorCode();

                    // Draw sub-line of this tile (7 chars wide)
                    if (sub == 0) // Top line of tile (Index 0 of 0-4)
                    {
                        std::cout << " +--" << topColor << topSym << reset << "--+";
                    }
                    else if (sub == 2) // Middle line of tile with symbol (Index 2 of 0-4)
                    {
                        std::cout << " " << leftColor << leftSym << reset
                            //<< "     " // Do not print tile id in middle
                            << "  " << t.id << "  " // Print tile id in middle
                            << rightColor << rightSym << reset;
                    }
                    else if (sub == 4) // Bottom line of tile (Index 4 of 0-4)
                    {
                        std::cout << " +--" << bottomColor << bottomSym << reset << "--+";
                    }
                    else // The lines that does not include symbols
                    {
                        std::cout << " |     |";
                    }

                    std::cout << "  "; // Spacing between tiles
                }
                std::cout << "\n";
            }
            std::cout << "\n"; // Spacing between tile rows
        }
    }

    void printDescriptionOfTiles() const noexcept
    {
        std::cout << "Each tile: Pos(row,col): id [top,right,bottom,left] : color/part:\n\n";
        for (int row = 0; row < N; ++row)
        {
            for (int col = 0; col < N; ++col)
            {
                const Tile& tile = tiles[row][col];
                std::cout << "Pos(" << row << "," << col << "): id=" << tile.id << " [ ";
                for (int edge = 0; edge < tile.NoOfEdges; ++edge)
                {
                    std::cout << tile.edges[edge].colorToString() << "/" << tile.edges[edge].partToString();
                    if (edge < tile.NoOfEdges - 1) // Don't set comma after last edge
                    {
                        std::cout << ", ";
                    }
                }
                std::cout << " ]\n";
            }
            std::cout << "\n";
        }
    }
}; // struct TileGridStruct

class Solutions : public std::vector<TileGridStruct>
{
public:
    // In case 2 tiles are similar then 2 solutions seem similar with only difference having 2 similar tiles swapped.
    // This function will remove all these duplicates having similar "looking" solutions
    void removeDuplicatesInSolutions() noexcept
    {
        for (int i = 0; i < this->size() - 1; ++i) // -1 to always have one to compare with
        {
            for (int u = i + 1; u < this->size();) // Loop the ones to compare with
            {
                if ((*this)[i] == (*this)[u])
                {
                    this->erase(this->begin() + u); // Don't increment u if we erase an element
                }
                else
                {
                    ++u;
                }
            }
        }
    }
}; // class Solutions

class Solver
{
private:
    Solutions solutions;

    Tile tiles[NumberOfTiles]{};
    bool used[NumberOfTiles]{};
    TileGrid placed;

    uint64_t numberOfTestedCombinations = 0;
    
    bool backtrack(int pos) noexcept
    {
        if (pos == NumberOfTiles)
        {
            //Add solution to vector
            solutions.emplace_back(placed);
            //return true; // If only one solution is needed
            return false; //To proceed finding the next solution
        }

        int row = pos / N;
        int col = pos % N;

        for (int i = 0; i < NumberOfTiles; ++i)
        {
            if (used[i]) continue;

            // Try all 4 rotations
            for (int rot = 0; rot < Tile::NoOfEdges; ++rot)
            {
                bool ok = true;
                // Check left neighbor
                if (col > 0)
                {
                    Edge leftNeighborRightEdge = placed[row][col - 1].edges[static_cast<const int>(Tile::EdgeDirection::Right)]; // Neighbor's right
                    Edge myLeft = tiles[i].edges[static_cast<const int>(Tile::EdgeDirection::Left)];
                    if (leftNeighborRightEdge.matchEdges(myLeft) == false)
                    {
                        ok = false;
                    }
                }
                // Check top neighbor
                if (row > 0)
                {
                    Edge topNeighborBottomEdge = placed[row - 1][col].edges[static_cast<const int>(Tile::EdgeDirection::Bottom)]; // Neighbor's bottom
                    Edge myTop = tiles[i].edges[static_cast<const int>(Tile::EdgeDirection::Top)];
                    if (topNeighborBottomEdge.matchEdges(myTop) == false)
                    {
                        ok = false;
                    }
                }

                ++numberOfTestedCombinations;

                if (ok)
                {
                    placed[row][col] = tiles[i]; // Copy current rotated orientation
                    used[i] = true;
                    if (backtrack(pos + 1))
                    {
                        return true;
                    }
                    used[i] = false;
                }
                // Rotate tile for next try
                tiles[i].rotate90cw();
            }
            // After 4 rotations tile is back to original orientation
        }
        return false;
    }

public:
    Solver(const Tile(&inputTiles)[NumberOfTiles])
    {
        for (size_t i = 0; i < NumberOfTiles; ++i)
        {
            tiles[i] = inputTiles[i];
        }
    }

    void solve()
    {
        solutions.clear();
        std::fill(std::begin(used), std::end(used), false); // Reset used array to false
        numberOfTestedCombinations = 0;
        backtrack(0);
    }

    Solutions getSolutions()
    {
        return solutions;
    }

    uint64_t getNumberOfTestedCombinations()
    {
        return numberOfTestedCombinations;
    }

    // Find and print duplicates of tiles (if 2 or more tiles are similar includes rotation to compare)
    void printDuplicatesOfTiles() noexcept
    {
        std::vector<std::vector<const Tile*>> duplicatesCollection;
        std::fill(std::begin(used), std::end(used), false); // Reset used array to false

        // Find all duplicates and put them in duplicatesCollection
        for (int i = 0; i < NumberOfTiles - 1; ++i) // -1 since we need at least one more to compare with
        {
            std::vector<const Tile*> duplicates{ &tiles[i] }; //Prepare the vector by adding current tile. This will be used in case there are duplicates
            for (int other = i + 1; other < NumberOfTiles; ++other)
            {
                // Rotate during comparison since there is no gurantee that they have same orientation
                if (used[other] == false && tiles[i].similarTile(tiles[other], true) == true)
                {
                    duplicates.emplace_back(&tiles[other]);
                    used[other] = true;
                }
            }
            if (duplicates.size() > 1)
            {
                duplicatesCollection.emplace_back(std::move(duplicates));
            }
        }

        if (duplicatesCollection.size() == 0)
        {
            std::cout << "No duplicates of tiles found\n";
        }
        else
        {
            // Print all duplicates
            std::cout << "Duplicates of tiles found:\n";
            for (const auto& dc : duplicatesCollection)
            {
                std::cout << "  Similiar tiles (id): ";
                bool firstId = true;
                for (const auto& d : dc)
                {
                    std::cout << (firstId == true ? "" : ",") << d->id;
                    firstId = false;
                }
                std::cout << std::endl;
            }
        }
    }
}; // class Solver

const Tile tiles[NumberOfTiles] =
{
    {0, {{Edge::Color::Purple, Edge::Part::Bottom}, {Edge::Color::Blue, Edge::Part::Top}, {Edge::Color::Yellow, Edge::Part::Top}, {Edge::Color::Green, Edge::Part::Bottom}}}, // Tile 0 (0,0 on picture)
    {1, {{Edge::Color::Purple, Edge::Part::Bottom}, {Edge::Color::Blue, Edge::Part::Top}, {Edge::Color::Yellow, Edge::Part::Top}, {Edge::Color::Blue, Edge::Part::Bottom}}}, // Tile 1 (1,0 on picture)
    {2, {{Edge::Color::Purple, Edge::Part::Bottom}, {Edge::Color::Green, Edge::Part::Top}, {Edge::Color::Yellow, Edge::Part::Top}, {Edge::Color::Blue, Edge::Part::Bottom}}}, // Tile 2 (2,0 on picture)
    {3, {{Edge::Color::Yellow, Edge::Part::Bottom}, {Edge::Color::Blue, Edge::Part::Top}, {Edge::Color::Green, Edge::Part::Top}, {Edge::Color::Purple, Edge::Part::Bottom}}}, // Tile 3 (0,1 on picture)
    {4, {{Edge::Color::Yellow, Edge::Part::Bottom}, {Edge::Color::Green, Edge::Part::Top}, {Edge::Color::Purple, Edge::Part::Top}, {Edge::Color::Blue, Edge::Part::Bottom}}}, // Tile 4 (1,1 on picture)
    {5, {{Edge::Color::Yellow, Edge::Part::Bottom}, {Edge::Color::Green, Edge::Part::Top}, {Edge::Color::Purple, Edge::Part::Top}, {Edge::Color::Green, Edge::Part::Bottom}}}, // Tile 5 (2,1 on picture)
    {6, {{Edge::Color::Green,  Edge::Part::Bottom}, {Edge::Color::Blue, Edge::Part::Top}, {Edge::Color::Purple, Edge::Part::Top}, {Edge::Color::Yellow, Edge::Part::Bottom}}}, // Tile 6 (0,2 on picture)
    {7, {{Edge::Color::Purple, Edge::Part::Bottom}, {Edge::Color::Green, Edge::Part::Top}, {Edge::Color::Blue, Edge::Part::Top}, {Edge::Color::Yellow, Edge::Part::Bottom}}}, // Tile 7 (1,2 on picture)
    {8, {{Edge::Color::Purple, Edge::Part::Bottom}, {Edge::Color::Blue, Edge::Part::Top}, {Edge::Color::Yellow, Edge::Part::Top}, {Edge::Color::Green, Edge::Part::Bottom}}} // Tile 8 (2,2 on picture)
};

int main()
{
    Solver solver(tiles);

    auto start_time = std::chrono::high_resolution_clock::now();
    solver.solve();
    auto end_time = std::chrono::high_resolution_clock::now();
    
    Solutions solutions = solver.getSolutions();

    std::cout << "In case there are duplicates of tiles the solution will look the same just with the 2 similar tiles swapped\n";
    std::cout << "Lets remove these duplicates\n";
    std::cout << "Solutions before removal:" << solutions.size() << "\n";
    solutions.removeDuplicatesInSolutions(); // There are 2 similar tiles which then doubles the combinations. Lets remove the duplicate solutions
    std::cout << "Solutions after removal:" << solutions.size() << "\n\n\n";

    if (solutions.size() > 0)
    {
        for (const auto& solution : solutions)
        {
            solution.printDescriptionOfTiles();
            solution.printAsciiTiles();
        }
        std::cout << "Number of solutions: " << solutions.size() << std::endl;
    }
    else
    {
        std::cout << "No solution found with the given tiles.\n";
    }

    std::cout << "Number of tested combinations: " << solver.getNumberOfTestedCombinations() << std::endl;
    std::cout << "Elapsed time for algorithm: " << std::chrono::duration_cast<std::chrono::microseconds>(end_time - start_time).count() << " microseconds\n";

    std::cout << std::endl;
    solver.printDuplicatesOfTiles();

    return 0;
}
