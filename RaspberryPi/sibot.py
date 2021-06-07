import configparser
import pprint
import json

from twitchio.ext import commands
import lightbulb


class SIBot(commands.Bot):
    def __init__(self, light: lightbulb.LightBulb):
        self.light = light

        self.config = configparser.ConfigParser()
        self.config.read("config.ini")

        super().__init__(irc_token=self.config["SIBot"]["TMI_TOKEN"],
                         client_id=self.config["SIBot"]["CLIENT_ID"],
                         client_secret=self.config["SIBot"]["CLIENT_SECRET"],
                         nick=self.config["SIBot"]["BOT_NICK"],
                         prefix=self.config["SIBot"]["BOT_PREFIX"],
                         initial_channels=[self.config["SIBot"]["CHANNEL"]])

    async def event_ready(self):
        """Setup bot when it is ready"""
        print("Bot started")

        # Get the User ID of the channel and register for events
        userID = await self.get_users(self.config["SIBot"]["CHANNEL"])
        await self.pubsub_subscribe(self.config["SIBot"]["OAUTH_TOKEN"], "channel-points-channel-v1." + userID[0].id)

    async def event_raw_pubsub(self, data):
        """Handle pubsub events"""
        print(data)
        if data["type"] == "MESSAGE":
            try:
                if json.loads(data["data"]["message"])["type"] == "reward-redeemed":
                    await self.handle_redemption(data)
            except:
                pass

    async def event_pubsub(self, data):
        pass

    async def handle_redemption(self, data):
        """Handles channel point redemption"""
        rewardTitle = json.loads(data["data"]["message"])["data"]["redemption"]["reward"]["title"]
        if rewardTitle == "Red Light":
            await self.light.set_red()
        elif rewardTitle == "Magenta Light":
            await self.light.set_magenta()

    @commands.command(name="rage")
    async def rage_command(self, ctx):
        """
        Command that will activate when a user types
        !rage and changes the lightbulb, also lets them know it activated
        """
        await ctx.send("Rage activated!")
        await self.light.set_rage_flash()
