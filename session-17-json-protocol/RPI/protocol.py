import json
import time
import serial


class UARTProtocol:
    def __init__(self, port="/dev/ttyS0", baudrate=115200, timeout=0.5):
        self.ser = serial.Serial(port, baudrate, timeout=timeout)
        self.seq = 0

    def _next_seq(self):
        self.seq = (self.seq + 1) % 65536
        return self.seq

    def send_cmd(self, joints_deg, speed=25):
        if len(joints_deg) != 18:
            raise ValueError("joints_deg must contain exactly 18 values")

        seq = self._next_seq()

        frame = {
            "type": "CMD",
            "seq": seq,
            "ts": int(time.monotonic() * 1000),
            "data": {
                "joints": joints_deg,
                "speed": speed
            }
        }

        line = json.dumps(frame) + "\n"
        self.ser.write(line.encode("utf-8"))
        return seq

    def read_frame(self):
        raw = self.ser.readline().decode("utf-8").strip()

        if not raw:
            return None

        try:
            return json.loads(raw)
        except json.JSONDecodeError:
            return {
                "type": "ERR",
                "data": {
                    "code": "BAD_JSON",
                    "msg": raw
                }
            }

    def send_cmd_with_retry(self, joints_deg, speed=25, retries=3):
        for attempt in range(1, retries + 1):
            seq = self.send_cmd(joints_deg, speed)
            print(f"Sent CMD seq={seq}, attempt={attempt}")

            reply = self.read_frame()

            if reply is None:
                print("No reply - timeout")
                continue

            if reply.get("seq") != seq:
                print("Seq mismatch")
                continue

            if reply.get("type") == "ACK":
                print(f"Confirmed ACK seq={seq}")
                return True

            if reply.get("type") == "ERR":
                print("ESP32 error:", reply.get("data", {}).get("msg"))
                return False

        print("FAILED")
        return False

    def close(self):
        self.ser.close()


if __name__ == "__main__":
    protocol = UARTProtocol(port="/dev/ttyS0", baudrate=115200)

    try:
        while True:

            # 0 -> 180
            for angle in range(0, 181, 10):
                angles = [angle] * 18

                print(f"\nSending angle: {angle}")
                protocol.send_cmd_with_retry(
                    joints_deg=angles,
                    speed=25,
                    retries=3
                )

                time.sleep(1)

            # 180 -> 0
            for angle in range(170, -1, -10):
                angles = [angle] * 18

                print(f"\nSending angle: {angle}")
                protocol.send_cmd_with_retry(
                    joints_deg=angles,
                    speed=25,
                    retries=3
                )

                time.sleep(1)

    except KeyboardInterrupt:
        print("\nProgram stopped.")

    finally:
        protocol.close()
