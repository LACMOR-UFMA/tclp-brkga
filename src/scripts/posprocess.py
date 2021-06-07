from os import listdir
import os
from os.path import isfile, join
import pandas as pd
import seaborn as sns
import matplotlib.pyplot as plt


def create_facet_grid_gplot(df: pd.DataFrame, xaxis: str, yaxis: str, col: str, xlabel: str, ylabel: str):
    g = sns.FacetGrid(df.sort_values(by=xaxis), col=col, col_wrap=5)
    g.map(sns.pointplot, xaxis, yaxis, capsize=0.1)
    g.set_axis_labels(xlabel, ylabel)

    return g


def read_data_line(file: str, line: str):
    if line.strip() != "Instance;FO;Time":  # skipping first line from result file
        line_result = line.strip().split(";")
        return {
            "instance": line_result[0][10:-4],
            "objective_function": float(line_result[1]),
            "time": float(line_result[2]),
            "max_threads": int(file.strip().split("_")[1].split(".")[0]),
        }
    else:
        return {}


def read_results(path: str):
    files_results = [f for f in listdir(path) if isfile(join(path, f))]

    data_rows = []
    for file_result in files_results:
        with open(join(path, file_result)) as file:
            for line in file:
                data = read_data_line(file_result, line)
                if data:
                    data_rows.append(data)

    return data_rows

def export_to_csv(df: pd.DataFrame, max_threads: int, file: str):
    return df[df['max_threads'] == max_threads].groupby('instance').mean().round(2).to_csv("./results_posprocess/" + file)

def main():
    df = pd.DataFrame(read_results("./results"))

    threads = list(df['max_threads'].unique())

    # export csv files by thread for all states with mean time and fitness
    [export_to_csv(df, t, f"results_{t}.csv") for t in threads]

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
