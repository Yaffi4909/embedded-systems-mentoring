import json
import time
import serial
import threading


class UARTProtocol:
    def __init__(self, port="/dev/ttyS0", baudrate=115200, timeout=0.1):
        self.ser = serial.Serial(port, baudrate, timeout=timeout)
        self.seq = 0

        self.lock = threading.Lock()
        self.sensor = {}
        self.acks = {}
        self.running = True

        self.rx_thread = threading.Thread(target=self._recv_loop, daemon=True)
        self.rx_thread.start()

    def _next_seq(self):
        self.seq = (self.seq + 1) % 65536
        return self.seq

    def _validate_joints(self, joints_deg):
        if len(joints_deg) != 18:
            raise ValueError("joints_deg must contain exactly 18 values")

        for angle in joints_deg:
            if angle < 0 or angle > 180:
                raise ValueError("Each angle must be between 0 and 180")

    def build_cmd_frame(self, seq, joints_deg, speed=25):
        self._validate_joints(joints_deg)

        return {
            "type": "CMD",
            "seq": seq,
            "ts": int(time.monotonic() * 1000),
            "data": {
                "joints": joints_deg,
                "speed": speed
            }
        }

    def send_frame(self, frame):
        line = json.dumps(frame) + "\n"
        self.ser.write(line.encode("utf-8"))
        self.ser.flush()

    def _recv_loop(self):
        while self.running:
            try:
                raw = self.ser.readline().decode("utf-8", errors="ignore").strip()

                if not raw:
                    continue

                frame = json.loads(raw)
                frame_type = frame.get("type")

                if frame_type == "STATUS":
                    with self.lock:
                        self.sensor = frame.get("data", {})
                        self.sensor["seq"] = frame.get("seq")
                        self.sensor["ts"] = frame.get("ts")

                elif frame_type in ("ACK", "ERR"):
                    seq = frame.get("seq")
                    with self.lock:
                        self.acks[seq] = frame

                else:
                    print("Unknown frame:", frame)

            except json.JSONDecodeError:
                print("BAD JSON:", raw)

            except Exception as e:
                print("RX error:", e)

    def wait_for_reply(self, seq, timeout=2):
        start = time.time()

        while time.time() - start < timeout:
            with self.lock:
                if seq in self.acks:
                    return self.acks.pop(seq)

            time.sleep(0.02)

        return None

    def send_cmd_with_retry(self, joints_deg, speed=25, retries=3):
        seq = self._next_seq()
        frame = self.build_cmd_frame(seq, joints_deg, speed)

        for attempt in range(1, retries + 1):
            print(f"Sent CMD seq={seq}, attempt={attempt}")
            self.send_frame(frame)

            reply = self.wait_for_reply(seq, timeout=2)

            if reply is None:
                print("No ACK - timeout")
                continue

            if reply.get("type") == "ACK":
                print(f"Confirmed ACK seq={seq}")
                return True

            if reply.get("type") == "ERR":
                print("ESP32 error:", reply.get("data", {}).get("msg"))
                return False

        print("FAILED")
        return False

    def get_sensor_snapshot(self):
        with self.lock:
            return dict(self.sensor)

    def close(self):
        self.running = False
        time.sleep(0.2)
        self.ser.close()

    def build_alert_frame(self, seq, reason, sensor):
        return {
            "type": "ALERT",
            "seq": seq,
            "ts": int(time.monotonic() * 1000),
            "data": {
                "reason": reason,
                "temp": sensor.get("temp"),
                "dist": sensor.get("dist")
            }
        }


    def send_alert(self, reason, sensor):
        seq = self._next_seq()
        frame = self.build_alert_frame(seq, reason, sensor)

        print(f"Sending ALERT seq={seq}, reason={reason}")
        self.send_frame(frame)

        reply = self.wait_for_reply(seq, timeout=2)

        if reply and reply.get("type") == "ACK":
            print("ALERT confirmed by ESP32")
            return True

        print("ALERT failed or no ACK")
        return False
