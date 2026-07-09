import time


HOME    = (12.5, 0, -8.0)
STAND   = (12.5, 0, -10.0)
LIFT    = (12.5, 0, -3.0)
EXTEND  = (18.0, 0, -10.0)
RETRACT = (7.0,  0, -10.0)

STEP_CYCLE = [
    STAND,
    LIFT,
    EXTEND,
    STAND,
    RETRACT,
    STAND
]

def straight_line(leg, start, end, steps=30, delay=0.012):
    x0, y0, z0 = start
    x1, y1, z1 = end

    for s in range(steps + 1):
        t = s / steps

        x = x0 + (x1 - x0) * t
        y = y0 + (y1 - y0) * t
        z = z0 + (z1 - z0) * t

        leg.move_foot_to(x, y, z)
        time.sleep(delay)

def waypoint_path(leg, waypoints, steps=20, delay=0.012):
    for i in range(len(waypoints) - 1):
        straight_line(
            leg,
            waypoints[i],
            waypoints[i + 1],
            steps,
            delay
        )

def one_step(leg, speed=0.012):
    for waypoint in STEP_CYCLE:
        leg.move_foot_to(*waypoint, steps=25)
        time.sleep(0.1)