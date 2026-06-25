import time


class HexLeg:
    HOME = (90, 90, 90)
    STAND = (90, 60, 120)
    LIFT = (90, 30, 90)
    EXTEND = (135, 60, 120)
    RETRACT = (45, 60, 120)

    def __init__(self, kit, hip_ch, femur_ch, tibia_ch):
        self.kit = kit
        self.channels = (hip_ch, femur_ch, tibia_ch)

        self._set_range()
        self.move_to(self.HOME)

    def _set_range(self):
        for ch in self.channels:
            self.kit.servo[ch].set_pulse_width_range(550, 2670)

    def move_to(self, position, delay=0.3):
        hip, femur, tibia = position

        self.kit.servo[self.channels[0]].angle = hip
        self.kit.servo[self.channels[1]].angle = femur
        self.kit.servo[self.channels[2]].angle = tibia

        time.sleep(delay)

    def smooth_to(self, target, steps=25, delay=0.012):
        current = []

        for ch in self.channels:
            angle = self.kit.servo[ch].angle
            if angle is None:
                angle = 90
            current.append(angle)

        for step in range(1, steps + 1):
            t = step / steps

            new_position = (
                int(current[0] + (target[0] - current[0]) * t),
                int(current[1] + (target[1] - current[1]) * t),
                int(current[2] + (target[2] - current[2]) * t),
            )

            self.move_to(new_position, delay)
