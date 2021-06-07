from sibot import SIBot
from lightbulb import LightBulb
import asyncio


if __name__ == "__main__":
    bot = SIBot(LightBulb())
    bot.run()
