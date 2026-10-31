# Warehouse Robot Path Simulator

A grid-based autonomous navigation system built in **C** and **Python**, 
modeled after fulfillment center robotics systems.

Two robots navigate a 10×10 warehouse floor simultaneously, finding 
shortest paths with BFS and avoiding collisions in real time.

![Demo](demo.gif)

## Features

- **BFS Pathfinding** — each robot computes its shortest path through the warehouse grid
- **Multi-robot collision avoidance** — robots reserve cells each step; conflicts are resolved by priority
- **Config-driven** — warehouse layout and robot tasks defined in JSON, no recompilation needed
- **Animated visualizer** — Python replays the simulation frame-by-frame in the terminal

## Project Structure



## How to Run

**Compile the C simulation:**
```bash
gcc -o warehouse warehouse.c pathfind.c robots.c
```

**Run the animated visualizer:**
```bash
python3 visualizer.py warehouse.json
```

**Run with custom speed (seconds per step):**
```bash
python3 visualizer.py warehouse.json 0.2
```

**Use a different warehouse config:**
```bash
python3 visualizer.py warehouse2.json 0.3
```

## Tech Stack

- **C** — core simulation engine, pathfinding, collision logic
- **Python** — config loader, subprocess controller, terminal visualizer
- **AWS** — (next: deploy simulation results to S3, log via DynamoDB)

## Inspiration

Inspired by Amazon Robotics fulfillment center autonomous navigation systems.

