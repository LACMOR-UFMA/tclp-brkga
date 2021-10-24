from os import listdir
from os.path import isfile, join
import pandas as pd
import seaborn as sns
import matplotlib.pyplot as plt

RESULTS_FOLDER = "results_08092021"
RESULTS_POSPROCESS_FOLDER = "results_posprocess_08092021"
SHOULD_CALCULATE_SPEEDUP = True


def create_facet_grid_gplot(
    df: pd.DataFrame, xaxis: str, yaxis: str, col: str, xlabel: str, ylabel: str
):
    g = sns.FacetGrid(df.sort_values(by=xaxis), col=col, col_wrap=5)
    g.map(sns.pointplot, xaxis, yaxis, capsize=0.1)
    g.set_axis_labels(xlabel, ylabel)

    return g


def read_data_line(file: str, line: str):
    nthreads = file.strip().split("_")

    if len(nthreads) == 1:
        nthreads = 1
    else:
        nthreads = int(nthreads[1].split(".")[0])

    if line.strip()[:9] == "instances":  # skipping first line from result file
        line_result = line.strip().split(";")
        return {
            "instance": line_result[0][10:-4],
            "objective_function": float(line_result[1]),
            "time": float(line_result[2]),
            "max_threads": nthreads,
        }
    else:
        return {}


def read_results(path: str):
    files_results = [f for f in listdir(path) if isfile(join(path, f)) and f.endswith(".out")]

    data_rows = []
    for file_result in files_results:
        with open(join(path, file_result)) as file:
            for line in file:
                data = read_data_line(file_result, line)
                if data:
                    data_rows.append(data)

    return data_rows


def export_to_csv(df: pd.DataFrame, max_threads: int, file: str):
    return (
        df[df["max_threads"] == max_threads]
        .groupby("instance")
        .mean()
        .round(2)
        .to_csv(f"./{RESULTS_POSPROCESS_FOLDER}/" + file)
    )


def get_time_instance_single_thread(df_baseline, row):
    return df_baseline[
        (df_baseline["instance"] == row[0]) & (df_baseline["max_threads"] == 1)
    ]["time"]


def get_speed_up(df_baseline, row):
    return get_time_instance_single_thread(df_baseline, row) / row[1]

def get_speedup_dataframe(df: pd.DataFrame):
    df_baseline = (
        df.sort_values("max_threads")
        .groupby(["instance", "max_threads"], as_index=False)
        .mean()
        .round(2)
    )
    zipped_baseline = zip(
        df_baseline["instance"], df_baseline["time"], df_baseline["max_threads"]
    )

    df_baseline["speedup"] = [
        get_speed_up(df_baseline, row).iloc[0] for row in zipped_baseline
    ]

    return df_baseline


def main():
    sns.set_theme()

    df = pd.DataFrame(read_results(f"./{RESULTS_FOLDER}"))

    if SHOULD_CALCULATE_SPEEDUP:
        # calculate speedup
        df_speedup = get_speedup_dataframe(df)

        # plot speedup comparison
        sns.relplot(
            data=df_speedup,
            kind="line",
            x="max_threads",
            y="speedup",
            hue="instance",
            style="instance",
            facet_kws=dict(sharex=False),
        )

    # export csv files by thread for all states with mean time and fitness
    [
        export_to_csv(df, t, f"{RESULTS_POSPROCESS_FOLDER}_{t}.csv")
        for t in list(df["max_threads"].unique())
    ]

    # plot Time x Max Threads
    create_facet_grid_gplot(
        df, "max_threads", "time", "instance", "Max Threads", "Running time"
    )

    # plot Objective Function x Max Threads
    create_facet_grid_gplot(
        df,
        "max_threads",
        "objective_function",
        "instance",
        "Max Threads",
        "Objective function",
    )

    plt.show()


if __name__ == "__main__":
    main()
