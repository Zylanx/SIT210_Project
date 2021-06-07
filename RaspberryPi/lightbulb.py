import asyncio

import lifxlan


class LightBulb:
    light: lifxlan.Light

    def __init__(self):
        self._enabled = True

        self.lan = lifxlan.LifxLAN(1)
        self.light = self.lan.get_lights()[0]

        # Set some colours
        self.normal = [65535, 0, 65535, 3000]
        self.red = lifxlan.RED
        self.magenta = [54395, 65535, 65535, 3000]

        # Set the colour to normal to begin with
        self.light.set_color(self.normal)

        self.currentAnimation = None

    async def set_red(self):
        """
        Set the light to red temporarily
        """

        if not self.enabled:
            return

        if self.currentAnimation is not None and not self.currentAnimation.cancelled() and self.currentAnimation.get_name() == "RollingRage":
            return

        if self.currentAnimation is not None and not self.currentAnimation.cancelled():
            self.currentAnimation.cancel()
            self.light.set_color(self.normal, 3000)

        self.currentAnimation = asyncio.create_task(self._set_red())

    async def _set_red(self):
        # Save the original colour
        originalColour = self.light.get_color()

        # Set the colour to red, wait a bit, then change it back
        self.light.set_color(self.red, 2000)
        await asyncio.sleep(10)
        self.light.set_color(originalColour, 2000)

    async def set_magenta(self):
        """
        Set the light to red temporarily
        """

        if not self.enabled:
            return

        if self.currentAnimation is not None and not self.currentAnimation.cancelled() and self.currentAnimation.get_name() == "RollingRage":
            return

        if self.currentAnimation is not None and not self.currentAnimation.cancelled():
            self.currentAnimation.cancel()
            self.light.set_color(self.normal, 3000)

        self.currentAnimation = asyncio.create_task(self._set_magenta())

    async def _set_magenta(self):
        # Save the original colour
        originalColour = self.light.get_color()

        # Set the colour to magenta, wait a bit, then change it back
        self.light.set_color(self.magenta, 2000)
        await asyncio.sleep(10)
        self.light.set_color(originalColour, 2000)

    async def set_temporary_rage_flash(self):
        """
        Make the light strobe red to signify 'rage'
        """

        if not self.enabled:
            return

        if self.currentAnimation is not None and not self.currentAnimation.cancelled() and self.currentAnimation.get_name() == "RollingRage":
            return

        if self.currentAnimation is not None and not self.currentAnimation.cancelled():
            self.currentAnimation.cancel()
            self.light.set_color(self.normal, 3000)

        self.currentAnimation = asyncio.create_task(self._set_temporary_rage_flash())

    async def _set_temporary_rage_flash(self):
        # Save the original colour
        originalColour = self.light.get_color()

        # Set the colour to a slightly dark red then flash
        # it between that and bright red
        self.light.set_color([65535, 65535, int(65535 * 0.5), 3000])
        await asyncio.sleep(0)
        self.light.set_waveform(1, self.red, 1000, 3, 0, 1)
        await asyncio.sleep(1 * 3)
        self.light.set_color(originalColour, 3000)

    async def set_rolling_rage_flash(self, start=True):
        if not self.enabled:
            return

        if start:
            if self.currentAnimation is not None and not self.currentAnimation.cancelled():
                self.currentAnimation.cancel()
                self.light.set_color(self.normal, 3000)

            self.currentAnimation = asyncio.create_task(self._set_rolling_rage_flash(), name="RollingRage")
        else:
            if self.currentAnimation is not None and not self.currentAnimation.cancelled():
                self.currentAnimation.cancel()
                self.light.set_color(self.normal, 3000)

    async def _set_rolling_rage_flash(self):
        # Save the original colour
        originalColour = self.light.get_color()

        # Set the colour to a slightly dark red then flash
        # it between that and bright red
        self.light.set_color([65535, 65535, int(65535 * 0.5), 3000])
        await asyncio.sleep(0)
        while True:
            self.light.set_waveform(1, self.red, 1000, 3, 0, 1)
            await asyncio.sleep(1 * 3)

    @property
    def enabled(self):
        return self._enabled

    @enabled.setter
    def enabled(self, value):
        if not value:
            if self.currentAnimation is not None and not self.currentAnimation.cancelled():
                self.currentAnimation.cancel()
            self.light.set_color(self.normal, 3000)
            self._enabled = False
        else:
            self._enabled = True

