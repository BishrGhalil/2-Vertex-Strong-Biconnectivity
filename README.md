# Ensuring 2-Vertex-Strong Biconnectivity in C++

## Concept
A directed graph \( G = (V, E) \) is defined as 2-vertex-strongly-biconnected if it contains at least three vertices, and for every vertex \( w \) in \( G \), the graph \( G - \{w\} \) remains strongly biconnected.

## Objective
To verify the 2-vertex-strong biconnectivity of a graph, follow these steps:

### Step 1: Implement the Cheriyan-Mehlhorn-Gabow Algorithm
Utilize the Cheriyan-Mehlhorn-Gabow algorithm to identify strongly connected components (SCCs) in a directed graph. This algorithm runs in linear time, \( O(n + m) \), where \( n \) is the number of vertices and \( m \) is the number of edges.

### Step 2: Implement Jens Schmidt's Algorithm
Apply Jens Schmidt's algorithm to determine if an undirected graph is 2-connected, meaning it lacks any articulation points. This algorithm also operates in linear time.

### Step 3: Verify 2-Vertex-Strong Biconnectivity
Check if the directed graph is 2-vertex-strongly-biconnected by verifying the properties defined earlier. This final check runs in \( O(n(n + m)) \) time.

## Group Members

|              الإسم | الرقم |
| -----------------: | ----: |
|   بشر محمود الغليل |  5520 |
|     محسن منير أحمد |  5514 |
| ابراهيم يوسف حموده |  5513 |
|   ياسمين مهند عجيب |  6554 |
|     سدرة زهير غانم |  6547 |
|     مرام هيثم عباس |  5780 |
|        شذى علي علي |  4540 |
|      نور يوسف يونس |  6572 |
|  نور سعيد قره فاقي |  6582 |
|   نغم طلال اسماعيل |  5660 |