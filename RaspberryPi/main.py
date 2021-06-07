from sibot import SIBot
from lightbulb import LightBulb
import asyncio

from aiohttp import web

lightbulb = LightBulb()

routes = web.RouteTableDef()


@routes.get("/rage")
async def handle_rage(request):

    query = request.query.get("start", None)
    if query is None:
        return
    elif query in ["true", "True"]:
        await lightbulb.set_rolling_rage_flash(True)
    elif query in ["false", "False"]:
        await lightbulb.set_rolling_rage_flash(False)


@routes.get("/enabled")
async def handle_enabled(request):
    query = request.query.get("enabled", None)
    if query is None:
        return
    elif query in ["true", "True"]:
        lightbulb.enabled = True
    elif query in ["false", "False"]:
        lightbulb.enabled = False

app = web.Application()
app.add_routes(routes)

async def start_aux_webserver():
    runner = web.AppRunner(app)
    await runner.setup()
    site = web.TCPSite(runner, "localhost", 10543)
    await site.start()

if __name__ == "__main__":
    bot = SIBot(lightbulb)
    asyncio.ensure_future(start_aux_webserver())
    bot.run()
