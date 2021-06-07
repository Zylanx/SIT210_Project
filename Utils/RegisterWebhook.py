import configparser
import sys
import requests


config = configparser.ConfigParser()
config.read("../RaspberryPi/config.ini")


def getID():
    headers = {
        "Authorization": "Bearer " + config["SIBot"]["OAUTH_TOKEN"],
        "Client-Id": config["SIBot"]["CLIENT_ID"]
    }

    response = requests.get("https://api.twitch.tv/helix/users?login=" + config["SIBot"]["CHANNEL"], headers=headers)
    return response.json()["data"][0]["id"]


payload = {
    "type": "channel." + sys.argv[1],
    "version": "1",
    "condition": {
        "broadcaster_user_id": getID()
    },
    "transport": {
        "method": "webhook",
        "callback": sys.argv[2],
        "secret": config["SIBot"]["CLIENT_SECRET"]
    }
}

headers = {"Content-Type":"application/json", "Client-ID": config["SIBot"]["CLIENT_ID"], "Authorization": "Bearer " + config["SIBot"]["OAUTH_APP_TOKEN"]}

response = requests.post("https://api.twitch.tv/helix/eventsub/subscriptions", json=payload, headers=headers)
print(response)
print(response.content)