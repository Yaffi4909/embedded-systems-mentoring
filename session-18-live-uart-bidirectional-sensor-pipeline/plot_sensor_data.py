import csv
import matplotlib.pyplot as plt


CSV_FILE = "sensor_log.csv"
OUTPUT_IMAGE = "sensor_plot.png"


timestamps = []
temps = []
hums = []
dists = []


with open(CSV_FILE, "r") as f:
    reader = csv.DictReader(f)

    for row in reader:
        timestamps.append(float(row["ts"]) / 1000)
        temps.append(float(row["temp"]))
        hums.append(float(row["hum"]))
        dists.append(float(row["dist"]))


plt.figure()

plt.plot(timestamps, temps, label="Temperature")
plt.plot(timestamps, hums, label="Humidity")
plt.plot(timestamps, dists, label="Distance")

plt.xlabel("Time (seconds)")
plt.ylabel("Value")
plt.title("Sensor Data Over Time")
plt.legend()
plt.grid(True)

plt.savefig(OUTPUT_IMAGE)
plt.show()