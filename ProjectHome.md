# WiFi Mesh Simulator Pro #
Allows to simulate traffic and routing building between moving in 2D space stations. Each station has the following properties:
  * Initial location
  * Data traffic schedule
  * Velocity vector

During simulation, each station faces with constantly changing network topology, updating dynamic routing tables automatically, finding a route to destination.

Simulator allows to analyze:
  * Total traffic ratio
  * Effective traffic ratio
  * Collisions ratio
  * Maximum/Minimum/Average throughput

Simulator has very [extensive GUI](http://wifi-mesh.googlecode.com/svn/trunk/WiFiMesh/MeshGUI/Images/screenshot.png), allowing performing deeper research, such as:
  * Sniffing - all frame transactions (filtering by frame type may be applied)
  * Current station position and it's velocity vector
  * All data transaction shown on the world map
  * Each station may be investigated for packets queue and routing table