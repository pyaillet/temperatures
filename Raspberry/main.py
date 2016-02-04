from flask import Flask
from influxdb import InfluxDBClient
import ConfigParser

app = Flask(__name__)

config = ConfigParser.RawConfigParser()
config.read('config.cfg')

@app.route("/Temperature/room/<room>/temp/<float:temperature>")
def updateTemp(room, temperature):
    json_body = [
        {
            "measurement": "temps",
            "tags": {
                "room": room,
            },
            "fields": {
                 "value": temperature
            }
        }
    ]
    host = config.get('influxdb', 'host')
    port = config.get('influxdb', 'port')
    username = config.get('influxdb', 'user')
    password = config.get('influxdb', 'pass')
    database = config.get('influxdb', 'database')
    client = InfluxDBClient(host, port, username, password, database)
    client.write_points(json_body)
    return "Ok"

if __name__ == "__main__":
    app.run()
