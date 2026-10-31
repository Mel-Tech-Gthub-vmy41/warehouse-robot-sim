import subprocess
import json
import sys

def run_simulation(config_path="warehouse.json"):
    # Load the config
    with open(config_path) as f:
        config = json.load(f)

    print(f"Starting simulation with {len(config['robots'])} robots...")
    for r in config['robots']:
        print(f"  Robot {r['id']}: {r['start']} → {r['goal']}")
    print()

    # Call the compiled C binary, passing JSON via stdin
    result = subprocess.run(
        ["./warehouse"],
        input=json.dumps(config),
        capture_output=True,
        text=True
    )

    if result.returncode != 0:
        print("Simulation error:")
        print(result.stderr)
        sys.exit(1)

    # Print the raw simulation output
    print(result.stdout)

    # Parse summary line
    for line in result.stdout.split("\n"):
        if "Simulation complete" in line:
            print("✓", line.strip())

if __name__ == "__main__":
    config = sys.argv[1] if len(sys.argv) > 1 else "warehouse.json"
    run_simulation(config)
