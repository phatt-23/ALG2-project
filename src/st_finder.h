#ifndef __SPANNING_TREES_FINDER_H
#define __SPANNING_TREES_FINDER_H

#include "edge.h"
#include "matrix.h"
#include "partition.h"
#include "disjoint_set.h"
#include <istream>
#include <vector>

/// @brief A utility class for performing various graph-related operations.
/// 
/// The Helper class provides static methods to read adjacency matrices,
/// create edges from those matrices, solve for various partitions, and
/// perform testing of trees for duplicity and cycle validation.
class SpanningTreesFinder
{
public:
    /// @brief Writes all found trees to a specified output stream.
    /// 
    /// This method outputs the details of each tree found in the provided 
    /// graph along with the corresponding partition information.
    /// @param output The output stream to write to.
    /// @param graph The graph containing the edges and vertices.
    /// @param ks A vector of partitions representing the trees.
    static void writeAllTrees(
        std::ofstream& output, 
        const Graph& graph, 
        const std::vector<Partition>& ks
    );

    /// @brief Writes only the k-th tree to a specified output stream.
    /// 
    /// This method outputs the details of the k-th tree based on its cost.
    /// @param output The output stream to write to.
    /// @param graph The graph containing the edges and vertices.
    /// @param ks A vector of partitions representing the trees.
    static void writeOnlyKth(
        std::ofstream& output, 
        const Graph& graph, 
        const std::vector<Partition>& ks
    );

    /// @brief Reads an adjacency matrix from an input stream.
    /// 
    /// This method reads the adjacency matrix and constructs a Matrix object.
    /// @param inputStream The input stream to read the adjacency matrix from.
    /// @return A Matrix object representing the adjacency matrix.
    [[nodiscard]]
    static Matrix<int> readAdjacencyMatrix(std::ifstream& inputStream);

    /// @brief Creates edges from an adjacency matrix.
    /// 
    /// This method generates edges based on the non-zero entries of the 
    /// adjacency matrix.
    /// @param adjMat The adjacency matrix from which to create edges.
    /// @return A vector of edges created from the adjacency matrix.
    [[nodiscard]]
    static std::vector<Edge> createEdges(const Matrix<int>& adjMat);

    /// @brief Creates a partition of the graph based on specified choices.
    /// 
    /// This method attempts to construct a spanning tree (partition) using the 
    /// choices provided and returns the resulting Partition object.
    /// @param choices A vector of choices that dictate which edges to include.
    /// @param g The graph from which to create the partition.
    /// @param ds The disjoint set used for cycle checking.
    /// @return A pointer to a Partition object, or nullptr if construction fails.
    [[nodiscard]]
    static Partition* createPartition(
        const std::vector<int>& choices, 
        const Graph& g, 
        DisjointSet<int>& ds
    );

    /// @brief Solves for all possible partitions of the graph.
    /// 
    /// This method finds all spanning trees of the given graph and 
    /// returns them sorted by their cost.
    /// @param g The graph for which to find spanning trees.
    /// @return A vector of partitions representing the spanning trees.
    [[nodiscard]]
    static std::vector<Partition> solve(const Graph& g);

    /// @brief Prints the details of the trees in the console.
    /// 
    /// This method outputs the partitions representing the trees in a 
    /// formatted manner based on the specified printing mode.
    /// @param ks A vector of partitions representing the trees.
    /// @param graph The graph for which the trees are defined.
    /// @param mode The mode of printing: different modes display different outputs.
    static void PrintTrees(
        const std::vector<Partition>& ks, 
        const Graph& graph, 
        int mode
    );

    /// @brief Tests for duplicate trees within the partitions.
    /// 
    /// This method checks all partitions for duplicity and reports any 
    /// duplicates found in the console.
    /// @param kts A vector of partitions to be checked for duplicates.
    static void testDuplicates(const std::vector<Partition>& kts);

    /// @brief Tests if all partitions are valid trees (i.e., contain no cycles).
    /// 
    /// This method checks each partition to ensure it is free of cycles and 
    /// thus qualifies as a tree.
    /// @param ks A vector of partitions representing the trees to be checked.
    /// @param g The graph associated with the partitions.
    static void testCycles(
        const std::vector<Partition>& ks, 
        const Graph& g
    );

    /// @brief Writes the content to an HTML file based on the graph and partitions.
    /// 
    /// This method writes a JavaScript representation of the graph and its 
    /// partitions to an HTML file using the specified format structure.
    /// @param outputPath The path to the output file.
    /// @param headPath The path to the HTML head content.
    /// @param tailPath The path to the HTML tail content.
    /// @param mode The mode determining which trees to write.
    /// @param g The graph to be represented in the HTML.
    /// @param ks A vector of partitions to write to the HTML.
    static void writeToHtml(
        const char* outputPath, 
        const char* headPath, 
        const char* tailPath, 
        int mode,
        const Graph& g,
        const std::vector<Partition>& ks
    );
};

#endif // __SPANNING_TREES_FINDER_H
