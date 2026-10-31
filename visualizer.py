import subprocess
import json
import time
import os
import sys

def clear():
    os.system('clear')

def parse_simulation_output(output):
    """Parse the C program's output into a list of frames."""
    frames = []
    current_frame = None
    step = -1

    for line in output.split('\n'):
        line = line.rstrip()

        # New step header
        if line.startswith('--- Step'):
            if current_frame is not None:
                frames.append(current_frame)
            step = int(line.split()[2])
            current_frame = {'step': step, 'grid_lines': [], 'status': []}

        elif current_frame is not None:
            # Grid lines start with '#' or '.'or a digit
            stripped = line.strip()
            if stripped and stripped[0] in ('#', '.', '0', '1', '2', '3', '4', '5', '6', '7', '8', '9'):
                current_frame['grid_lines'].append(stripped)
            # Status lines (Robot X: ...)
            elif 'Robot' in line or 'Simulation' in line:
                current_frame['status'].append(line.strip())

    # Don't forget the last frame
    if current_frame is not None:
        frames.append(current_frame)

    return frames

def render_frame(frame, total_steps, robot_configs):
    clear()

    # Header
    print("=" * 30)
    print("  WAREHOUSE ROBOT SIMULATOR")
    print("=" * 30)
    print(f"  Step {frame['step']} / {total_steps - 1}")
    print()

    # Grid with color codes
    for row in frame['grid_lines']:
        colored = ""
        for ch in row:
            if ch == '#':
                colored += '\033[90m#\033[0m'    # dark gray walls
            elif ch == '.':
                colored += '\033[2m.\033[0m'      # dim dots
            elif ch == '1':
                colored += '\033[92m1\033[0m'     # green robot 1
            elif ch == '2':
                colored += '\033[94m2\033[0m'     # blue robot 2
            else:
                colored += ch
        print(" ", colored)

    # Status
    print()
    for s in frame['status']:
        if 'DONE' in s:
            print(f"  \033[92m✓ {s}\033[0m")     # green when done
        elif 'waiting' in s:
            print(f"  \033[93m⏸ {s}\033[0m")     # yellow when waiting
        else:
            print(f"  \033[97m→ {s}\033[0m")      # white when moving

    # Legend
    print()
    print("  \033[90m#\033[0m wall/shelf  "
          "\033[92m1\033[0m Robot 1  "
          "\033[94m2\033[0m Robot 2")
    print()

def run_visualizer(config_path="warehouse.json", delay=0.4):
    # Load config
    with open(config_path) as f:
        config = json.load(f)

    # Run the C simulation
    result = subprocess.run(
        ["./warehouse"],
        input=json.dumps(config),
        capture_output=True,
        text=True
    )

    if result.returncode != 0:
        print("Simulation error:", result.stderr)
        sys.exit(1)

    # Parse output into frames
    frames = parse_simulation_output(result.stdout)

    if not frames:
        print("No frames parsed — check simulation output.")
        sys.exit(1)

    total_steps = len(frames)

    # Print startup message
    clear()
    print("=" * 30)
    print("  WAREHOUSE ROBOT SIMULATOR")
    print("=" * 30)
    print(f"\n  Config: {config_path}")
    print(f"  Robots: {len(config['robots'])}")
    for r in config['robots']:
        print(f"    Robot {r['id']}: {r['start']} → {r['goal']}")
    print(f"\n  Starting in 2 seconds...")
    time.sleep(2)

    # Animate frames
    for frame in frames:
        render_frame(frame, total_steps, config['robots'])
        time.sleep(delay)

    # Final message
    print(f"  Simulation complete — {total_steps - 1} steps total.")
    print()

if __name__ == "__main__":
    config = sys.argv[1] if len(sys.argv) > 1 else "warehouse.json"
    speed = float(sys.argv[2]) if len(sys.argv) > 2 else 0.4
    run_visualizer(config, speed)
