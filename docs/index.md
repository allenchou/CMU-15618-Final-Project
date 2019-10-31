{% include mathjax.html %}

## Proposal

[PDF](./proposal.pdf)

## Summary

We will parallelize sequential algorithms for minimum spanning tree, including Kruskal’s algorithm and Borůvka’s algorithm, evaluate the speedup performance, and profile our implementations.

## Background

In a connected, edge-weighted undirected graph, a _minimum spanning tree_ (MST) is a subset of edges that connectsall nodes together with the smallest total weights. It has been well-studied for nearly a century and can be solved in polynomial time with different greedy algorithms. For example, [Prim’s algorithm](https://en.wikipedia.org/wiki/Prim%27s_algorithm) uses the _cut-property_ of MST, and constructs the MST by adding the smallest edge that connects the current nodes set and the remaining nodes set. However, Prim’s algorithm is hard to parallelize in nature because each step depends on the current sub-graph built on previous steps.

In this project, we are going to focus on [Kruskal’s algorithm](https://en.wikipedia.org/wiki/Kruskal%27s_algorithm) and [Borůvka’s algorithm](https://en.wikipedia.org/wiki/Kruskal%27s_algorithm). Kruskal’s algorithm utilizes the _cycles-property_ and build the MST by adding the smallest edge that does not create a cycle. The edge sorting takes $O(|E|\log|E|)$ operations and building the MST takes $O(|E|\alpha(|V|))$ operations, where $\alpha$ is the inverse [Ackermann function](https://en.wikipedia.org/wiki/Ackermann_function). In practice, $\alpha(|V|)$ grows extremely slow and hence the overall complexity is $O(|E|\log|E|)$. Borůvka’s algorithm starts from making each node as an individual component, and then keeps finding and contracting the smallest edge for each component that connects to any other component. The overall complexity for Borůvka’s algorithm is $O(|E|\log|V|)$. In this project, we will focus on these two algorithms. We will implement theserial algorithms first, and then parallelize it in shared memory space.

## Challenges

Kruskal's algorithm involves two steps: firstly, sort all edges by its weights, and then keep adding edges if no cycle is produced. Although there is a parallel sorting algorithm for the first part, the second part is inherently sequential and can not be properly parallelized. This is because whether the new edge produce cycle depends on all previous added edges.

Borůvka's algorithm has two main steps. The first is to find the adjacent edge with the smallest weight for each connected component, and the second is to contract the selected edges. The first part can be parallelized by components (vertices). However, race condition happens when edge contraction happens on the same vertex. How to improve efficiency while keeping the correctness is a challenging problem. 

## Resources

We will implement all algorithms from scratch. There are several [research papers](# References) about parallel MST. There is also a good summary in [Wikipedia](https://en.wikipedia.org/wiki/Parallel_algorithms_for_minimum_spanning_trees). As far as we know, there is no code available online. We are going to run our initial experiment on GHC machines and [CloudLab](https://www.cloudlab.us/) if available.

## Goals and Deliverables

### Plan to achieve

- Implement two sequential algorithms, Kruskal's algorithm and Borůvka's algorithms, as the baseline of our performance benchmark.
- Parallelize these two sequential algorithms in shared-memory model using OpenMP:
	- Krushal's algorithm with parallel sorting and Filter-Kruskal,
	- Borůvka's algorithm with edge contraction.
- Benchmark the speedup performance of our parallel implementations under different types and sizes of input graphs. We plan to consider two kinds of graphs: dense graphs and sparse graphs.
- Explore the speedup performance of different parallel components, such as the parallel sorting in Krushal's algorithm and the parallel finding adjacent edge with the smallest weight of each vertex in Borůvka's algorithm.

### Hope to achieve

- Explore the speedup performance on input graphs with power-law distribution.
- Implement a faster shared-memory MST algorithm proposed by Bader _et al._.
- Explore the possibility to implement parallel Krushal's and Borůvka's algorithms in message-passing model using MPI.

## Poster deliverables

We will deliver the speedup graphs of our parallel implementations under different numbers of processors and different types and sizes of input graphs. We will also deliver speedup graphs, or tables, of different parallel components. We think a reasonable speedup with respect to the number of processors will demonstrate that demonstrate we did a good job. If we cannot get a good speedup, we will try to profile our implementations to show that the parallel overhead is inevitable under our testing platform and provide potential specs of the testing platform to improve the speedup of our implementations.

## Platform Choice

We plan to use OpenMP as our parallel framework and choose GHC machines to test our implementation. After well-tested on GHC machines, we will try to run our benchmark experiments on CloudLab if available. We use the shared-memory model so we can not take advantage of the distributed cluster, but there are more powerful machines on CloudLab so it is still a good platform for us to do tests.

## Schedule

Week | Due   | Task                                                           | Assigned to      | Status    
-----|-------|----------------------------------------------------------------|------------------|---------
1    | 11.3  | Project Proposal                                               | Both 			 | Completed
1    | 11.3  | Implement graph data structure and graph data generator        | TBD              |           
2    | 11.10 | Implement sequential Kruskal's algorithm                       | TBD              |           
2    | 11.10 | Implement parallel Kruskal's algorithm with parallel sorting   | TBD              |           
3    | 11.17 | Implement parallel Kruskal's algorithm with Filter-Kruskal     | TBD              |           
3    | 11.17 | Benchmark and profile parallel Kruskals' algorithms            | TBD              |           
3    | 11.17 | Project checkpoint report                                      | TBD              |           
4    | 11.24 | Implement sequential Borůvka's algorithm                      | TBD              |           
5    | 12.1  | Implement sequential Borůvka's algorithm with edge contraction | TBD              |           
5    | 12.1  | Implement parallel Borůvka's algorithm                        | TBD              |           
6    | 12.8  | Benchmark and profile parallel Borůvka's algorithm            | TBD              |           
6    | 12.8  | Poster and final report                                        | TBD              |           

## References

1. S. Chung and A. Condon, “[Parallel implementation of bouvka’s minimum spanning tree algorithm](https://www.cs.ubc.ca/~condon/papers/chungcondon96.pdf),” in _Proceedingsof International Conference on Parallel Processing_, pp. 302–308, IEEE, 1996.
2. V. Osipov, P. Sanders, and J. Singler, “[The filter-kruskal minimum spanning tree algorithm](http://algo2.iti.kit.edu/documents/fkruskal.pdf),” in _Proceedings of theMeeting on Algorithm Engineering & Expermiments_, pp. 52–61, Society for Industrial and Applied Mathematics, 2009.
3. D. A. Bader and G. Cong, “[Fast shared-memory algorithms for computing the minimum spanning forest of sparsegraphs](https://www.cs.unm.edu/~treport/tr/03-12/MST-bader.pdf),” _Journal of Parallel and Distributed Computing_, vol. 66, no. 11, pp. 1366–1378, 2006.
