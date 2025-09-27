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
    static const int NoOfEdges = 4;

    int id;
    Edge edges[NoOfEdges]; // 0=top,1=right,2=bottom,3=left

    void rotate90cw()
    {
        Edge tmp = edges[3];
        edges[3] = edges[2];
        edges[2] = edges[1];
        edges[1] = edges[0];
        edges[0] = tmp;
    }
};

static const int N = 3; // Tiles 3x3
Tile tiles[N * N] =
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

bool used[N * N] = { false };
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
    if (pos == N * N) return true;
    int row = pos / N;
    int col = pos % N;

    for (int i = 0; i < 9; ++i)
    {
        if (used[i]) continue;

        // Try all 4 rotations
        for (int rot = 0; rot < 4; ++rot)
        {
            bool ok = true;
            // Check left neighbor
            if (col > 0)
            {
                Edge leftNeighborRightEdge = placed[row][col - 1].edges[1]; // Neighbor's right
                Edge myLeft = tiles[i].edges[3];
                if (!matchEdges(leftNeighborRightEdge, myLeft))
                {
                    ok = false;
                }
            }
            // Check top neighbor
            if (row > 0)
            {
                Edge topNeighborBottomEdge = placed[row - 1][col].edges[2]; // Neighbor's bottom
                Edge myTop = tiles[i].edges[0];
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

int main()
{
    auto start_time = std::chrono::high_resolution_clock::now();

    if (backtrack(0))
    {
        std::cout << "Løsning fundet (hver brik: Pos(row,col): id [top,right,bottom,left] : color/part):\n\n";
        for (int row = 0; row < N; ++row)
        {
            for (int col = 0; col < N; ++col)
            {
                Tile& tile = placed[row][col];
                std::cout << "Pos(" << row << "," << col << "): id=" << tile.id << " [ ";
                for (int edge = 0; edge < tile.NoOfEdges; ++edge)
                {
                    std::cout << colorToString(tile.edges[edge].color) << "/" << partToString(tile.edges[edge].part);
                    if (edge < 3)
                    {
                        std::cout << ", ";
                    }
                }
                std::cout << " ]\n";
            }
            std::cout << "\n";
        }
    }
    else
    {
        std::cout << "Ingen løsning fundet med de angivne brikker.\n";
    }

    auto end_time = std::chrono::high_resolution_clock::now();
    std::cout << "Elapsed time: " << std::chrono::duration_cast<std::chrono::milliseconds>(end_time - start_time).count() << " ms\n";

    return 0;
}
