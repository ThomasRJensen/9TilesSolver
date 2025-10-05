#include <string>
#include <iostream>
#include <chrono>

enum class Color { Yellow, Green, Blue, Purple };
enum class Part { Top, Bottom }; // Top = overkrop, Bottom = underkrop

struct Edge
{
    Color color;
    Part part;
};

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

    void rotate90cw()
    {
        Edge tmp = edges[static_cast<const int>(EdgeDirection::Left)];
        edges[static_cast<const int>(EdgeDirection::Left)] = edges[static_cast<const int>(EdgeDirection::Bottom)];
        edges[static_cast<const int>(EdgeDirection::Bottom)] = edges[static_cast<const int>(EdgeDirection::Right)];
        edges[static_cast<const int>(EdgeDirection::Right)] = edges[static_cast<const int>(EdgeDirection::Top)];
        edges[static_cast<const int>(EdgeDirection::Top)] = tmp;
    }
};

static const int N = 3;
static const int NumberOfTiles = N * N; // Tiles 3x3
Tile tiles[NumberOfTiles] =
{
    {0, {{Color::Purple, Part::Bottom}, {Color::Blue, Part::Top}, {Color::Yellow, Part::Top}, {Color::Green, Part::Bottom}}}, // Tile 0 (0,0 on picture)
    {1, {{Color::Purple, Part::Bottom}, {Color::Blue, Part::Top}, {Color::Yellow, Part::Top}, {Color::Blue, Part::Bottom}}}, // Tile 1 (1,0 on picture)
    {2, {{Color::Purple, Part::Bottom}, {Color::Green, Part::Top}, {Color::Yellow, Part::Top}, {Color::Blue, Part::Bottom}}}, // Tile 2 (2,0 on picture)
    {3, {{Color::Yellow, Part::Bottom}, {Color::Blue, Part::Top}, {Color::Green, Part::Top}, {Color::Purple, Part::Bottom}}}, // Tile 3 (0,1 on picture)
    {4, {{Color::Yellow, Part::Bottom}, {Color::Green, Part::Top}, {Color::Purple, Part::Top}, {Color::Blue, Part::Bottom}}}, // Tile 4 (1,1 on picture)
    {5, {{Color::Yellow, Part::Bottom}, {Color::Green, Part::Top}, {Color::Purple, Part::Top}, {Color::Green, Part::Bottom}}}, // Tile 5 (2,1 on picture)
    {6, {{Color::Green, Part::Bottom}, {Color::Blue, Part::Top}, {Color::Purple, Part::Top}, {Color::Yellow, Part::Bottom}}}, // Tile 6 (0,2 on picture)
    {7, {{Color::Purple, Part::Bottom}, {Color::Green, Part::Top}, {Color::Blue, Part::Top}, {Color::Yellow, Part::Bottom}}}, // Tile 7 (1,2 on picture)
    {8, {{Color::Purple, Part::Bottom}, {Color::Blue, Part::Top}, {Color::Yellow, Part::Top}, {Color::Green, Part::Bottom}}} // Tile 8 (2,2 on picture)
};

bool used[NumberOfTiles] = { false };
Tile placed[N][N];

bool matchEdges(const Edge& a, const Edge& b)
{
    // Match: same color, opposite part (Top vs Bottom)
    return (a.color == b.color) && (a.part != b.part);
}

std::string colorToString(Color c)
{
    switch (c)
    {
    case Color::Yellow: return "Yellow";
    case Color::Green: return "Green";
    case Color::Blue: return "Blue";
    case Color::Purple: return "Purple";
    }
    return "?";
}

std::string partToString(Part p)
{
    return (p == Part::Top) ? "Top" : "Bottom";
}

bool backtrack(int pos)
{
    if (pos == NumberOfTiles)
    {
        return true;
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
                if (!matchEdges(leftNeighborRightEdge, myLeft))
                {
                    ok = false;
                }
            }
            // Check top neighbor
            if (row > 0)
            {
                Edge topNeighborBottomEdge = placed[row - 1][col].edges[static_cast<const int>(Tile::EdgeDirection::Bottom)]; // Neighbor's bottom
                Edge myTop = tiles[i].edges[static_cast<const int>(Tile::EdgeDirection::Top)];
                if (!matchEdges(topNeighborBottomEdge, myTop))
                {
                    ok = false;
                }
            }

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

void PrintDescriptionOfTiles(const Tile (&tiles)[N][N])
{
    std::cout << "Solution found (each tile: Pos(row,col): id [top,right,bottom,left] : color/part):\n\n";
    for (int row = 0; row < N; ++row)
    {
        for (int col = 0; col < N; ++col)
        {
            const Tile& tile = tiles[row][col];
            std::cout << "Pos(" << row << "," << col << "): id=" << tile.id << " [ ";
            for (int edge = 0; edge < tile.NoOfEdges; ++edge)
            {
                std::cout << colorToString(tile.edges[edge].color) << "/" << partToString(tile.edges[edge].part);
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

std::string colorCode(Color c)
{
    switch (c)
    {
        case Color::Yellow: return "\033[33m";
        case Color::Green:  return "\033[32m";
        case Color::Blue:   return "\033[34m";
        case Color::Purple: return "\033[35m";
    }
    return "\033[0m";
}

std::string resetColor()
{
    return "\033[0m";
}

void PrintAsciiTiles(const Tile (&tiles)[N][N])
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
                char topSym = (topEdge.part == Part::Top) ? 'O' : '|';
                char rightSym = (rightEdge.part == Part::Bottom) ? '-' : 'O';
                char bottomSym = (bottomEdge.part == Part::Bottom) ? '|' : 'O';
                char leftSym = (leftEdge.part == Part::Bottom) ? '-' : 'O';

                // Colors
                std::string topColor = colorCode(topEdge.color);
                std::string rightColor = colorCode(rightEdge.color);
                std::string bottomColor = colorCode(bottomEdge.color);
                std::string leftColor = colorCode(leftEdge.color);
                std::string reset = resetColor();

                // Draw sub-line of this tile (7 chars wide)
                if (sub == 0) // Top line of tile (Index 0 of 0-4)
                {
                    std::cout << "+--" << topColor << topSym << reset << "--+";
                }
                else if (sub == 2) // Middle line of tile with symbol (Index 2 of 0-4)
                {
                    std::cout << leftColor << leftSym << reset
                        << "     "
                        << rightColor << rightSym << reset;
                }
                else if (sub == 4) // Bottom line of tile (Index 4 of 0-4)
                {
                    std::cout << "+--" << bottomColor << bottomSym << reset << "--+";
                }
                else // The lines that does not include symbols
                {
                    std::cout << "|     |";
                }

                std::cout << "  "; // Spacing between tiles
            }
            std::cout << "\n";
        }
        std::cout << "\n"; // Spacing between tile rows
    }
}

int main()
{
    auto start_time = std::chrono::high_resolution_clock::now();
    bool result = backtrack(0);
    auto end_time = std::chrono::high_resolution_clock::now();
    if (result)
    {
        PrintDescriptionOfTiles(placed);
        PrintAsciiTiles(placed);
    }
    else
    {
        std::cout << "No solution found with the given tiles.\n";
    }

    std::cout << "Elapsed time for algorithm: " << std::chrono::duration_cast<std::chrono::microseconds>(end_time - start_time).count() << " microseconds\n";

    return 0;
}
