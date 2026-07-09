import math
import time
from adafruit_servokit import ServoKit
from ik_solver import leg_ik

class LegController:
    # Leg geometry in cm
    L1 = 2.5
    L2 = 10.0
    L3 = 12.0
    
    # Calibration offsets
    HIP_OFF = 90
    FEMUR_OFF = 90
    TIBIA_OFF = 90
    
    def __init__(self, kit, hip_ch, fem_ch, tib_ch):
        self.kit = kit
        self.ch = (hip_ch, fem_ch, tib_ch)
        
        self.foot = (self.L1 + self.L2, 0, -self.L3)
        
        for ch in self.ch:
            self.kit.servo[ch].set_pulse_width_range(550, 2670)

        self.go_home()
    
    def move_foot_to(self, x, y, z, steps=1):
        angles = leg_ik(x, y, z)
        
        if angles is None:
            print(f'Unreachable: ({x},{y},{z})')
            return False
        
        hip, fem, tib = angles
        
        sh = max(0, min(180, self.HIP_OFF + hip))
        sf = max(0, min(180, self.FEMUR_OFF + fem))
        st = max(30, min(150, self.TIBIA_OFF - tib))

        if steps == 1:
            self._send(sh, sf, st)
        else:
            self._smooth(sh, sf, st, steps)
            
        self.foot = (x, y, z)

        return True
    
    def _send(self, sh, sf, st):
        for ch, ang in zip(self.ch, (sh, sf, st)):
            self.kit.servo[ch].angle = ang
    
    def _smooth(self, sh, sf, st, steps):
        curr = []

        for c in self.ch:
            angle = self.kit.servo[c].angle
            if angle is None:
                angle = 90
            curr.append(angle)

        tgt = [sh, sf, st]

        for s in range(1, steps + 1):
            t = s / steps
            a = [
                int(curr[k] + (tgt[k] - curr[k]) * t)
                for k in range(3)
            ]

            self._send(*a)
            time.sleep(0.012)

    def go_home(self):
        self.move_foot_to(self.L1 + self.L2, 0, -self.L3)