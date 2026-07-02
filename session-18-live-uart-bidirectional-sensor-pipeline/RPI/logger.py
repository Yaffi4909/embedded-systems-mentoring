import csv
import os


class SensorLogger:
    def __init__(self, filename="sensor_log.csv"):
        self.filename = filename

        if not os.path.exists(self.filename):
            with open(self.filename, "w", newline="") as f:
                writer = csv.writer(f)
                writer.writerow(["seq", "ts", "temp", "hum", "dist", "bat"])

    def log(self, sensor):
        if not sensor:
            return

        with open(self.filename, "a", newline="") as f:
            writer = csv.writer(f)
            writer.writerow([
                sensor.get("seq", ""),
                sensor.get("ts", ""),
                sensor.get("temp", ""),
                sensor.get("hum", ""),
                sensor.get("dist", ""),
                sensor.get("bat", "")
            ])