import matplotlib.pyplot as plt
import pandas as pd
import seaborn as sns
import numpy as np

# Before clustering
plt.figure()
df = pd.read_csv("data/mall_data.csv")
df.columns = ["CustomerID", "Genre", "Age", "Annual income (k$)",
              "Spending Score (1-100)"]

ax = sns.scatterplot(x=df["Annual income (k$)"], 
                y=df["Spending Score (1-100)"],
                data=df)
plt.title("Scatterplot of spending (y) vs income (x)")
plt.savefig("data/income_score.pdf", format="pdf", bbox_inches="tight")

# After clustering
plt.figure()
df = pd.read_csv("data/kmeans_results.csv")
df.columns = ["income", "score", "cluster"]
sns.scatterplot(x=df["income"], y=df["score"], 
                hue=df.cluster, 
                palette=sns.color_palette("hls", n_colors=5))
plt.xlabel("Annual income (k$)")
plt.ylabel("Spending Score (1-100)")
plt.title("Scatterplot Clusters: spending (y) vs income (x)")

plt.savefig("data/kmeans_results.pdf", format="pdf", bbox_inches="tight")