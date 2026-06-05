"""TransceiverFunction: sensory channel rates → brain Poisson generators.

Reads:
  - ``sensory`` (preprocessed JsonDataPack written by camera_to_sensory.py)
    — six per-channel rates from the HSV detector.
  - ``eat_kick_dc`` (NEST DC source registered by brain.py and updated
    each tick by drive_update.py) — its amplitude is the drive-state
    indicator: > 0 ↔ SATED, == 0 ↔ HUNGRY. We re-use the existing
    registered datapack rather than introducing a new gazebo-side
    state datapack (which would need its own registration plumbing).

Writes:
  - ``cam_{green,blue,grey}_{left,right}_poisson`` rates (NEST).

Drive gating
------------
When HUNGRY, the blue channel is zeroed before the rate is forwarded
to brain.py — the husky ignores rest zones while still looking for
food. When SATED, the green channel is zeroed — the husky no longer
pursues food, so the blue channel + the asymmetric POP_SATED search
bias steer it toward a rest zone. Without this gating both channels
drive the motors all the time and the husky bee-lines to whichever
coloured target it sees first, eating constantly and never visibly
entering the blue-seeking phase.

The grey-obstacle channel is gated by the alarm rule from
``helpers.apply_obstacle_alarm`` rather than by drive state — the
reflex needs to override goal-seeking in either drive state.

Tracked in EBR2-32.
"""

from nrp_core import (
    PreprocessedDataPack,
    EngineDataPack,
    TransceiverFunction,
    DataPackIdentifier,
)
from nrp_core.data.nrp_json import JsonDataPack

from helpers import apply_obstacle_alarm


def _is_sated(eat_kick_datapack):
    """Read the eat_kick_dc amplitude and translate to a SATED bool.

    The DC source's status dict has 'amplitude' at index 0 in the
    JsonDataPack data list (NEST node status convention). On the very
    first tick the datapack may be empty before drive_update has
    written anything — default to HUNGRY in that case (False).
    """
    if eat_kick_datapack is None or eat_kick_datapack.isEmpty():
        return False
    try:
        amp = float(eat_kick_datapack.data[0]['amplitude'])
    except (KeyError, IndexError, TypeError):
        return False
    return amp > 0.0


@PreprocessedDataPack(keyword='sensory',
                      id=DataPackIdentifier('sensory', 'gazebo'))
@EngineDataPack(keyword='eat_kick',
                id=DataPackIdentifier('eat_kick_dc', 'nest'))
@TransceiverFunction("nest")
def obstacle_to_avoidance(sensory, eat_kick):
    rates_in = {
        "green_left":  sensory.data["green_left"],
        "green_right": sensory.data["green_right"],
        "blue_left":   sensory.data["blue_left"],
        "blue_right":  sensory.data["blue_right"],
        "grey_left":   sensory.data["grey_left"],
        "grey_right":  sensory.data["grey_right"],
    }
    rates = apply_obstacle_alarm(rates_in)

    sated = _is_sated(eat_kick)
    if sated:
        rates["green_left"] = 0.0
        rates["green_right"] = 0.0
    else:
        rates["blue_left"] = 0.0
        rates["blue_right"] = 0.0

    out = []
    for name in ("green_left", "green_right",
                 "blue_left", "blue_right",
                 "grey_left", "grey_right"):
        d = JsonDataPack(f"cam_{name}_poisson", "nest")
        d.data["rate"] = rates[name]
        out.append(d)
    return out
