import datetime
from colormap import rgb2hex
import urllib.request
import http
COUNT_LED = 48
esp_ip = "http://192.168.1.121/"
def transfer(my_url):   #use to send and receive data
    try:
        n = urllib.request.urlopen(esp_ip + my_url).read()
        n = n.decode("utf-8")
        return n

    except http.client.HTTPException as e:
        return e
start = datetime.datetime.now()
transfer("clear_")

val = 48
for b in range(0,COUNT_LED // val):
    HEX_RGB = (rgb2hex(255, 255, 255)[1:] + "_") * val
    transfer(HEX_RGB)
print(datetime.datetime.now() - start)
