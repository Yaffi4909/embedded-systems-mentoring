import time
import os

from protocol import UARTProtocol
from logger import SensorLogger


def build_pose(angle):
    return [angle] * 18


def print_dashboard(sensor):
    # os.system("clear")

    print("=== RoboGreeno Live Sensor Dashboard ===")

    if not sensor:
        print("No STATUS received yet")
        return

    print(f'seq : {sensor.get("seq", 0):>6}')
    print(f'temp: {sensor.get("temp", 0):>6.1f} C')
    print(f'hum : {sensor.get("hum", 0):>6.1f} %')
    print(f'dist: {sensor.get("dist", 0):>6.1f} cm')
    print(f'bat : {sensor.get("bat", 0):>6} %')
    print(f'ts  : {sensor.get("ts", 0):>6} ms')


def check_alert(protocol, sensor):
    if not sensor:
        return

    temp = sensor.get("temp", 0)
    dist = sensor.get("dist", 999)

    if temp > 30:
        protocol.send_alert("TEMP_HIGH", sensor)

    elif dist < 15:
        protocol.send_alert("DIST_LOW", sensor)
 
      
def main():
    protocol = UARTProtocol(
        port="/dev/ttyS0",
        baudrate=115200,
        timeout=0.1
    )

    logger = SensorLogger("sensor_log.csv")

    try:
        while True:
            for angle in range(0, 181, 10):
                pose = build_pose(angle)

                protocol.send_cmd_with_retry(
                    joints_deg=pose,
                    speed=25,
                    retries=3
                )

                sensor = protocol.get_sensor_snapshot()
                check_alert(protocol, sensor)
                logger.log(sensor)
                print_dashboard(sensor)

                time.sleep(1)

            for angle in range(170, -1, -10):
                pose = build_pose(angle)

                protocol.send_cmd_with_retry(
                    joints_deg=pose,
                    speed=25,
                    retries=3
                )

                sensor = protocol.get_sensor_snapshot()
                check_alert(protocol, sensor)
                logger.log(sensor)
                print_dashboard(sensor)

                time.sleep(1)

    except KeyboardInterrupt:
        print("\nProgram stopped.")

    finally:
        protocol.close()


if __name__ == "__main__":
    main()