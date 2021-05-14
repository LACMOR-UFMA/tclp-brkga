from os import listdir
from os.path import isfile, join
import pandas as pd
import seaborn as sns
import matplotlib.pyplot as plt

path_results = "./results/"
files_results = [f for f in listdir(path_results) if isfile(join(path_results, f))]

data_rows = []
for f in files_results:
    with open(join(path_results, f)) as file:
        for line in file:
            if (
                line.strip() != "Instance;FO;Time"
            ):  # skipping first line from result file
                line_result = line.strip().split(";")
                data_rows.append(
                    {
                        "instance": line_result[0][10:-4],
                        "objective_function": float(line_result[1]),
                        "time": float(line_result[2]),
                        "max_threads": int(f.strip().split("_")[1].split(".")[0]),
                    }
                )

df = pd.DataFrame(data_rows)

g = sns.FacetGrid(df.sort_values(by="max_threads"), col="instance", col_wrap=3)
g.map(sns.pointplot, "max_threads", "time", capsize=.1)
g.set_axis_labels("Max Threads", "Running time")

plt.show()
