import argparse
import csv
from collections import defaultdict
from pathlib import Path

import matplotlib.pyplot as plt


def read_results(csv_path):
    grouped_results = defaultdict(list)

    with csv_path.open(newline="", encoding="utf-8") as csv_file:
        reader = csv.DictReader(csv_file)
        required_columns = {
            "filter",
            "threads",
            "sequential_ms",
            "parallel_ms",
            "speedup",
            "equivalent",
        }

        if reader.fieldnames is None or not required_columns.issubset(reader.fieldnames):
            raise ValueError(
                "CSV must contain filter, threads, sequential_ms, parallel_ms, "
                "speedup and equivalent columns."
            )

        for row in reader:
            grouped_results[row["filter"]].append(
                {
                    "threads": int(row["threads"]),
                    "sequential_ms": float(row["sequential_ms"]),
                    "parallel_ms": float(row["parallel_ms"]),
                    "speedup": float(row["speedup"]),
                    "equivalent": row["equivalent"].strip().lower() == "true",
                }
            )

    if not grouped_results:
        raise ValueError("The benchmark CSV contains no result rows.")

    for results in grouped_results.values():
        results.sort(key=lambda result: result["threads"])

    return grouped_results


def plot_results(grouped_results, output_path):
    figure, (time_axis, speedup_axis) = plt.subplots(1, 2, figsize=(12, 5))

    for filter_name, results in sorted(grouped_results.items()):
        threads = [result["threads"] for result in results]
        parallel_times = [result["parallel_ms"] for result in results]
        speedups = [result["speedup"] for result in results]

        time_axis.plot(threads, parallel_times, marker="o", label=filter_name.title())
        speedup_axis.plot(threads, speedups, marker="o", label=filter_name.title())

        if not all(result["equivalent"] for result in results):
            print(f"Warning: {filter_name} has non-equivalent parallel output.")

    time_axis.set_title("Parallel execution time")
    time_axis.set_xlabel("OpenMP threads")
    time_axis.set_ylabel("Time (ms)")
    time_axis.grid(True, alpha=0.3)
    time_axis.legend()

    speedup_axis.set_title("Speedup")
    speedup_axis.set_xlabel("OpenMP threads")
    speedup_axis.set_ylabel("Sequential time / parallel time")
    speedup_axis.axhline(1.0, color="gray", linestyle="--", linewidth=1)
    speedup_axis.grid(True, alpha=0.3)
    speedup_axis.legend()

    figure.tight_layout()
    figure.savefig(output_path, dpi=160, bbox_inches="tight")
    plt.close(figure)


def parse_arguments():
    parser = argparse.ArgumentParser(
        description="Plot execution time and speedup from benchmark CSV results."
    )
    parser.add_argument("csv_file", type=Path, help="Benchmark CSV file")
    parser.add_argument(
        "--output",
        type=Path,
        default=Path("benchmark_results.png"),
        help="Output chart path (default: benchmark_results.png)",
    )
    return parser.parse_args()


def main():
    arguments = parse_arguments()
    grouped_results = read_results(arguments.csv_file)
    arguments.output.parent.mkdir(parents=True, exist_ok=True)
    plot_results(grouped_results, arguments.output)
    print(f"Chart saved to {arguments.output}")


if __name__ == "__main__":
    main()
