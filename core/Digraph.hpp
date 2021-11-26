// Digraph.hpp
//
// ICS 46 Winter 2019
// Project #4: Rock and Roll Stops the Traffic
//
// This header file declares a class template called Digraph, which is
// intended to implement a generic directed graph. The implementation
// uses the adjacency lists technique, so each vertex stores a linked
// list of its outgoing edges.
//
// Along with the Digraph class template is a class DigraphException
// and a couple of utility structs that aren't generally useful outside
// of this header file.
//
// In general, directed graphs are all the same, except in the sense
// that they store different kinds of information about each vertex and
// about each edge; these two types are the type parameters to the
// Digraph class template.

#ifndef DIGRAPH_HPP
#define DIGRAPH_HPP

#include <exception>
#include <functional>
#include <list>
#include <map>
#include <utility>
#include <vector>
#include <iostream>
#include <algorithm>
#include <queue>
#include <limits>



// DigraphExceptions are thrown from some of the member functions in the
// Digraph class template, so that exception is declared here, so it
// will be available to any code that includes this header file.

class DigraphException : public std::runtime_error
{
public:
    DigraphException(const std::string& reason);
};


inline DigraphException::DigraphException(const std::string& reason)
    : std::runtime_error{reason}
{
}



// A DigraphEdge lists a "from vertex" (the number of the vertex from which
// the edge points), a "to vertex" (the number of the vertex to which the
// edge points), and an EdgeInfo object.  Because different kinds of Digraphs
// store different kinds of edge information, DigraphEdge is a struct template.

template <typename EdgeInfo>
struct DigraphEdge
{
    int fromVertex;
    int toVertex;
    EdgeInfo einfo;
};



// A DigraphVertex includes two things: a VertexInfo object and a list of
// its outgoing edges.  Because different kinds of Digraphs store different
// kinds of vertex and edge information, DigraphVertex is a struct template.

template <typename VertexInfo, typename EdgeInfo>
struct DigraphVertex
{
    VertexInfo vinfo;
    std::list<DigraphEdge<EdgeInfo>> edges;
};



// Digraph is a class template that represents a directed graph implemented
// using adjacency lists.  It takes two type parameters:
//
// * VertexInfo, which specifies the kind of object stored for each vertex
// * EdgeInfo, which specifies the kind of object stored for each edge
//
// You'll need to implement the member functions declared here; each has a
// comment detailing how it is intended to work.
//
// Each vertex in a Digraph is identified uniquely by a "vertex number".
// Vertex numbers are not necessarily sequential and they are not necessarily
// zero- or one-based.

template <typename VertexInfo, typename EdgeInfo>
class Digraph
{
public:
    // The default constructor initializes a new, empty Digraph so that
    // contains no vertices and no edges.
    Digraph();

    // The copy constructor initializes a new Digraph to be a deep copy
    // of another one (i.e., any change to the copy will not affect the
    // original).
    Digraph(const Digraph& d);

    // The move constructor initializes a new Digraph from an expiring one.
    Digraph(Digraph&& d) noexcept;

    // The destructor deallocates any memory associated with the Digraph.
    ~Digraph() noexcept;

    // The assignment operator assigns the contents of the given Digraph
    // into "this" Digraph, with "this" Digraph becoming a separate, deep
    // copy of the contents of the given one (i.e., any change made to
    // "this" Digraph afterward will not affect the other).
    Digraph& operator=(const Digraph& d);

    // The move assignment operator assigns the contents of an expiring
    // Digraph into "this" Digraph.
    Digraph& operator=(Digraph&& d) noexcept;

    // vertices() returns a std::vector containing the vertex numbers of
    // every vertex in this Digraph.
    std::vector<int> vertices() const;

    // edges() returns a std::vector of std::pairs, in which each pair
    // contains the "from" and "to" vertex numbers of an edge in this
    // Digraph.  All edges are included in the std::vector.
    std::vector<std::pair<int, int>> edges() const;

    // This overload of edges() returns a std::vector of std::pairs, in
    // which each pair contains the "from" and "to" vertex numbers of an
    // edge in this Digraph.  Only edges outgoing from the given vertex
    // number are included in the std::vector.  If the given vertex does
    // not exist, a DigraphException is thrown instead.
    std::vector<std::pair<int, int>> edges(int vertex) const;

    // vertexInfo() returns the VertexInfo object belonging to the vertex
    // with the given vertex number.  If that vertex does not exist, a
    // DigraphException is thrown instead.
    VertexInfo vertexInfo(int vertex) const;

    // edgeInfo() returns the EdgeInfo object belonging to the edge
    // with the given "from" and "to" vertex numbers.  If either of those
    // vertices does not exist *or* if the edge does not exist, a
    // DigraphException is thrown instead.
    EdgeInfo edgeInfo(int fromVertex, int toVertex) const;

    // addVertex() adds a vertex to the Digraph with the given vertex
    // number and VertexInfo object.  If there is already a vertex in
    // the graph with the given vertex number, a DigraphException is
    // thrown instead.
    void addVertex(int vertex, const VertexInfo& vinfo);

    // addEdge() adds an edge to the Digraph pointing from the given
    // "from" vertex number to the given "to" vertex number, and
    // associates with the given EdgeInfo object with it.  If one
    // of the vertices does not exist *or* if the same edge is already
    // present in the graph, a DigraphException is thrown instead.
    void addEdge(int fromVertex, int toVertex, const EdgeInfo& einfo);

    // removeVertex() removes the vertex (and all of its incoming
    // and outgoing edges) with the given vertex number from the
    // Digraph.  If the vertex does not exist already, a DigraphException
    // is thrown instead.
    void removeVertex(int vertex);

    // removeEdge() removes the edge pointing from the given "from"
    // vertex number to the given "to" vertex number from the Digraph.
    // If either of these vertices does not exist *or* if the edge
    // is not already present in the graph, a DigraphException is
    // thrown instead.
    void removeEdge(int fromVertex, int toVertex);

    // vertexCount() returns the number of vertices in the graph.
    int vertexCount() const noexcept;

    // edgeCount() returns the total number of edges in the graph,
    // counting edges outgoing from all vertices.
    int edgeCount() const noexcept;

    // This overload of edgeCount() returns the number of edges in
    // the graph that are outgoing from the given vertex number.
    // If the given vertex does not exist, a DigraphException is
    // thrown instead.
    int edgeCount(int vertex) const;

    // isStronglyConnected() returns true if the Digraph is strongly
    // connected (i.e., every vertex is reachable from every other),
    // false otherwise.
    bool isStronglyConnected() const;

    // findShortestPaths() takes a start vertex number and a function
    // that takes an EdgeInfo object and determines an edge weight.
    // It uses Dijkstra's Shortest Path Algorithm to determine the
    // shortest paths from the start vertex to every other vertex
    // in the graph.  The result is returned as a std::map<int, int>
    // where the keys are vertex numbers and the value associated
    // with each key k is the precedessor of that vertex chosen by
    // the algorithm.  For any vertex without a predecessor (e.g.,
    // a vertex that was never reached, or the start vertex itself),
    // the value is simply a copy of the key.
    std::map<int, int> findShortestPaths(
        int startVertex,
        std::function<double(const EdgeInfo&)> edgeWeightFunc) const;


private:
    // Add whatever member variables you think you need here.  One
    // possibility is a std::map where the keys are vertex numbers
    // and the values are DigraphVertex<VertexInfo, EdgeInfo> objects.

    // You can also feel free to add any additional member functions
    // you'd like (public or private), so long as you don't remove or
    // change the signatures of the ones that already exist.

	// keep track of which keys belong to which vertex
	// key = vertex number // value = outgoing edges
    std::map<int, DigraphVertex<VertexInfo, EdgeInfo>> ImTheMap;

};



// You'll need to implement the member functions below.  There's enough
// code in place to make them compile, but they'll all need to do the
// correct thing instead.

template <typename VertexInfo, typename EdgeInfo>
Digraph<VertexInfo, EdgeInfo>::Digraph()
{
	// empty by default, so nothing
}


template <typename VertexInfo, typename EdgeInfo>
Digraph<VertexInfo, EdgeInfo>::Digraph(const Digraph& d)
{
	// clear current map
	this->ImTheMap.clear();
	// copies the map
	this->ImTheMap = d.ImTheMap;	
}


template <typename VertexInfo, typename EdgeInfo>
Digraph<VertexInfo, EdgeInfo>::Digraph(Digraph&& d) noexcept
{
	// clear current map
	this->ImTheMap.clear();
	// moves dying map in d into current map
	this->ImTheMap = std::move(d.ImTheMap);
}


template <typename VertexInfo, typename EdgeInfo>
Digraph<VertexInfo, EdgeInfo>::~Digraph() noexcept
{
	ImTheMap.clear();
}


template <typename VertexInfo, typename EdgeInfo>
Digraph<VertexInfo, EdgeInfo>& Digraph<VertexInfo, EdgeInfo>::operator=(const Digraph& d)
{
	// make sure existing map is cleared
	ImTheMap.clear();
	// d's ImTheMap is assigned this's map
	this->ImTheMap = d.ImTheMap;
    return *this;
}


template <typename VertexInfo, typename EdgeInfo>
Digraph<VertexInfo, EdgeInfo>& Digraph<VertexInfo, EdgeInfo>::operator=(Digraph&& d) noexcept
{
	// make sure existing map is cleared
	ImTheMap.clear();
	// d's ImTheMap is assigned this's map by std::move
	this->ImTheMap = std::move(d.ImTheMap);
    return *this;
}


template <typename VertexInfo, typename EdgeInfo>
std::vector<int> Digraph<VertexInfo, EdgeInfo>::vertices() const
{
	// vector of vertex numbers
	std::vector<int> ArthriticJoints;
	// iterate through map to get int number
	for (typename std::map<int, DigraphVertex<VertexInfo, EdgeInfo>>::const_iterator itr = ImTheMap.begin(); itr != ImTheMap.end(); ++itr)
	{
		// push int vertex number to vector
		ArthriticJoints.push_back(itr->first);
	}
    return ArthriticJoints;
}


template <typename VertexInfo, typename EdgeInfo>
std::vector<std::pair<int, int>> Digraph<VertexInfo, EdgeInfo>::edges() const
{
	// vector of pairs of ints
	std::vector<std::pair<int, int>> BoneHurtingJuice;
	// iterate through map
	for (typename std::map<int, DigraphVertex<VertexInfo, EdgeInfo>>::const_iterator itr = ImTheMap.begin(); itr != ImTheMap.end(); ++itr)
	{
		// iterate through std::list edges within the map
		for (typename std::list<DigraphEdge<EdgeInfo>>::const_iterator igor = itr->second.edges.begin(); igor != itr->second.edges.end(); ++igor)
		{
			// push back the to BoneHurtingJuice <fromVertex, toVertex> in the std::list edges
			BoneHurtingJuice.push_back(std::pair<int, int>(igor->fromVertex, igor->toVertex));
		}
	}
    return BoneHurtingJuice;
}


template <typename VertexInfo, typename EdgeInfo>
std::vector<std::pair<int, int>> Digraph<VertexInfo, EdgeInfo>::edges(int vertex) const
{
	// check if map is empty
	if (ImTheMap.size() == 0)
	{
		throw DigraphException("Empty Digraph");
	}
	// check if map even has the vertex number
	// if == then not found // else if != then not found
	else if (ImTheMap.find(vertex) == ImTheMap.end())
	{
		throw DigraphException("No edges exist that are outgoing from this vertex");
	}
	// else make the vector of pairs of int
	else
	{
		std::vector<std::pair<int, int>> BoneHurtingJuice;
		// iterate through map
		for (typename std::map<int, DigraphVertex<VertexInfo, EdgeInfo>>::const_iterator itr = ImTheMap.begin(); itr != ImTheMap.end(); ++itr)
		{
			// iterate through std::list edges within the map
			for (typename std::list<DigraphEdge<EdgeInfo>>::const_iterator igor = itr->second.edges.begin(); igor != itr->second.edges.end(); ++igor)
			{
				// if fromVertex is == vertex
				if (igor->fromVertex == vertex)
				{
					// push back the to BoneHurtingJuice <fromVertex, toVertex> in the std::list edges
					BoneHurtingJuice.push_back(std::pair<int, int>(igor->fromVertex, igor->toVertex));
				}	
			}
		}
		return BoneHurtingJuice;
	}
    
}


template <typename VertexInfo, typename EdgeInfo>
VertexInfo Digraph<VertexInfo, EdgeInfo>::vertexInfo(int vertex) const
{
	// Check if map is empty
	if (ImTheMap.size() == 0)
	{
		throw DigraphException("Empty Digraph");
	}
	// Check if vertex given even exists
	else if (ImTheMap.find(vertex) == ImTheMap.end())
	{
		throw DigraphException("No vertex with that number exists");
	}
	// return the value.vinfo at key
	else
	{
		return ImTheMap.at(vertex).vinfo;
	}
}


template <typename VertexInfo, typename EdgeInfo>
EdgeInfo Digraph<VertexInfo, EdgeInfo>::edgeInfo(int fromVertex, int toVertex) const
{
	// check if map is empty
	if (ImTheMap.size() == 0)
	{
		throw DigraphException("Empty Digraph");
	}
	// check if fromVertex exists
	// if == then not found // else if != then not found
	if (ImTheMap.find(fromVertex) == ImTheMap.end() || ImTheMap.find(toVertex) == ImTheMap.end())
	{
		throw DigraphException("No such vertex with that number exists");
	}
	else
	{
		// iterate through map
		for (typename std::map<int, DigraphVertex<VertexInfo, EdgeInfo>>::const_iterator itr = ImTheMap.begin(); itr != ImTheMap.end(); ++itr)
		{
			// iterate through std::list edges within the map
			for (typename std::list<DigraphEdge<EdgeInfo>>::const_iterator igor = itr->second.edges.begin(); igor != itr->second.edges.end(); ++igor)
			{
				// if vertices match, return to edge info
				if (igor->fromVertex == fromVertex && igor->toVertex == toVertex)
				{
					return igor->einfo;
				}
			}
		}
		throw DigraphException("No such edge exists");
	}
}


template <typename VertexInfo, typename EdgeInfo>
void Digraph<VertexInfo, EdgeInfo>::addVertex(int vertex, const VertexInfo& vinfo)
{
	// if there's at least already one with same key
	// if != to end, then there exists a key already
	if (ImTheMap.find(vertex) != ImTheMap.end())
	{
		throw DigraphException("Vertex already exists");
	}
	else
	{
		// insert into the map the vertex key and a new Digraph vertex
		ImTheMap.insert(std::pair<int, DigraphVertex<VertexInfo, EdgeInfo>>(vertex, DigraphVertex<VertexInfo, EdgeInfo>{vinfo}));
	}
}


template <typename VertexInfo, typename EdgeInfo>
void Digraph<VertexInfo, EdgeInfo>::addEdge(int fromVertex, int toVertex, const EdgeInfo& einfo)
{
	// check if vertices exist
	// if == then not found // else if != then not found
	if (ImTheMap.find(fromVertex) == ImTheMap.end() || ImTheMap.find(toVertex) == ImTheMap.end())
	{
		throw DigraphException("One or Both vertex does not exist");
	}
	// check if edge already exists
	for (typename std::map<int, DigraphVertex<VertexInfo, EdgeInfo>>::const_iterator itr = ImTheMap.begin(); itr != ImTheMap.end(); ++itr)
	{
		// iterate through std::list edges within the map
		for (typename std::list<DigraphEdge<EdgeInfo>>::const_iterator igor = itr->second.edges.begin(); igor != itr->second.edges.end(); ++igor)
		{
			if (igor->fromVertex == fromVertex && igor->toVertex == toVertex)
			{
				throw DigraphException("Not a valid edge");
			}
		}
	}
	// push back the edge inside a vertex
	ImTheMap.at(fromVertex).edges.push_back(DigraphEdge<EdgeInfo>{fromVertex, toVertex, einfo});

}


template <typename VertexInfo, typename EdgeInfo>
void Digraph<VertexInfo, EdgeInfo>::removeVertex(int vertex)
{
	// check if vertex exists
	// if == then not found // else if != then not found
	if (ImTheMap.find(vertex) == ImTheMap.end())
	{
		throw DigraphException("Vertex does not exist");
	}
	else
	{
		// iterate through map
		for (typename std::map<int, DigraphVertex<VertexInfo, EdgeInfo>>::iterator itr = ImTheMap.begin(); itr != ImTheMap.end(); ++itr)
		{
			// iterate through std::list edges within the map
			// FoL is element in list
			for (typename std::list<DigraphEdge<EdgeInfo>>::iterator FoL = itr->second.edges.begin(); FoL != itr->second.edges.end(); ++FoL)
			{
				// if FoL == vertex given
				if (FoL->toVertex == vertex)
				{
					// delete from edge
					itr->second.edges.erase(FoL);
				}
			}
		}
	}
	// finally erase the vertex from map
	ImTheMap.erase(vertex);
}


template <typename VertexInfo, typename EdgeInfo>
void Digraph<VertexInfo, EdgeInfo>::removeEdge(int fromVertex, int toVertex)
{
	// check if vertices exist
	// if == then not found // else if != then not found
	if (ImTheMap.find(fromVertex) == ImTheMap.end() || ImTheMap.find(toVertex) == ImTheMap.end())
	{
		throw DigraphException("Either one vertex or both vertcies do not exist");
	}
	// checking if toVertex && fromVertex exists together
	else
	{
		// use std::find to find pair in vector of pairs
		std::vector<std::pair<int, int>> EdgeLords = edges();
		// if find becomes end, not found
		if (std::find(EdgeLords.begin(), EdgeLords.end(), std::pair<int, int>(fromVertex, toVertex)) == EdgeLords.end())
		{
			throw DigraphException("Edge does not exist");
		}
	}
	// iterate through map
	for (typename std::map<int, DigraphVertex<VertexInfo, EdgeInfo>>::iterator itr = ImTheMap.begin(); itr != ImTheMap.end(); ++itr)
	{
		// iterate through std::list edges within the map
		// FoL is element in list
		for (typename std::list<DigraphEdge<EdgeInfo>>::iterator FoL = itr->second.edges.begin(); FoL != itr->second.edges.end(); ++FoL)
			{
				// if equal to toVertex && fromVertex
				if (FoL->toVertex == toVertex && FoL->fromVertex == fromVertex)
				{
					// delete FoL
					itr->second.edges.erase(FoL);
				}
			}
	}
}


template <typename VertexInfo, typename EdgeInfo>
int Digraph<VertexInfo, EdgeInfo>::vertexCount() const noexcept
{
	// number of vertex = size of map
    return ImTheMap.size();
}


template <typename VertexInfo, typename EdgeInfo>
int Digraph<VertexInfo, EdgeInfo>::edgeCount() const noexcept
{
    int EdgeLordCouncil = 0;
    for (typename std::map<int, DigraphVertex<VertexInfo, EdgeInfo>>::const_iterator itr = ImTheMap.begin(); itr != ImTheMap.end(); ++itr)
    {
    	// size of list of edges is how many edges that vertex has
    	EdgeLordCouncil += itr->second.edges.size();
    }
    return EdgeLordCouncil;
}


template <typename VertexInfo, typename EdgeInfo>
int Digraph<VertexInfo, EdgeInfo>::edgeCount(int vertex) const
{
	// check if vertex exists
	// if == then not found // else if != then not found
	if (ImTheMap.find(vertex) == ImTheMap.end())
	{
		throw DigraphException("Vertex does not exist");
	}
	// else return the size of edge list that vertex has
	else
	{
		return ImTheMap.at(vertex).edges.size();
	}
}



template <typename VertexInfo, typename EdgeInfo>
bool Digraph<VertexInfo, EdgeInfo>::isStronglyConnected() const
{
	for (typename std::map<int, DigraphVertex<VertexInfo, EdgeInfo>>::const_iterator itr = ImTheMap.begin(); itr != ImTheMap.end(); ++itr)
	{
		// strongly connected vertex should have the same number of edges as there are as many vertices
		if (edgeCount(itr->first) != vertexCount())
		{
			return false;
		}
	}
	return true;
}	

template <typename VertexInfo, typename EdgeInfo>
std::map<int, int> Digraph<VertexInfo, EdgeInfo>::findShortestPaths(
    int startVertex,
    std::function<double(const EdgeInfo&)> edgeWeightFunc) const
{
	// kv = unvisited boolean flags
	std::map<int, bool> lonelyBoi;
	// dv = distance mapping
	std::map<int, double> shortBoi;
	// pv = predecessor
	std::map<int, int> pathBoi;
	// queue with min as priority
	std::priority_queue<int, std::vector<int>, std::greater<int>> qtBoi;
	// for each vertex in the map
	for (typename std::map<int, DigraphVertex<VertexInfo, EdgeInfo>>::const_iterator iro = ImTheMap.begin(); iro != ImTheMap.end(); ++iro)
	{
		// queue each vertex
		qtBoi.push(iro->first);
		// set kv = false
		lonelyBoi[iro->first] = false;
		// set dv = "infinite"
		shortBoi[iro->first] = INT_MAX;
	}

	// setting first vertex in dv to 0
	shortBoi[startVertex] = 0;
	// setting first predecessor to itself
	pathBoi[startVertex] = startVertex;

	//while queue isnt empty
	while (!qtBoi.empty())
	{
		// v = take the most priority in the queue
		int curry_Boi = qtBoi.top();
		// set boolean flag to true
		lonelyBoi[curry_Boi] = true;
		// dequeue the top
		qtBoi.pop();
		// for each edge from vertex
		for (typename std::list<DigraphEdge<EdgeInfo>>::const_iterator igor = ImTheMap.at(curry_Boi).edges.begin();
		 igor != ImTheMap.at(curry_Boi).edges.end(); ++igor)
		{
			// set kw to true
			lonelyBoi[igor->toVertex] = true;	
			// calculate dv weight
			double dv = shortBoi[curry_Boi] + edgeWeightFunc(igor->einfo);
			// dw = neighbor vertex
			double dw = igor->toVertex;

			if (shortBoi[dw] > dv)
			{
				// dw = dv + C(v, w)
				shortBoi[dw] = dv;
				// pw = v
				pathBoi[dw] = curry_Boi;
			}

		}
	}

	return pathBoi;

}



#endif // DIGRAPH_HPP

