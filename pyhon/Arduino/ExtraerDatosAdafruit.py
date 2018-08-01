from Adafruit_IO import Client
import json


aio = Client('idiProteinlab','c905c763e9d444b28a3051b0a57d10b9')

def GetDataAll(feed_key):
        data = aio.data(feed_key)
        print(data)

def GetDataLast(feed_key):
        data = aio.receive(feed_key)
        return data

def obtenerData(Parametros,valores, time):
        obtenerParametros(Parametros)
        for x in range(len(Parametros)):
                data = GetDataLast(Parametros[x])
                valores.append(data.value)
                time.append(data.created_at)
        return Parametros,valores, time

def obtenerParametros(Parametros):
        aux = 0
        data = aio.feeds()
        for x in range(len(data)):
                Parametros.append(data[x].key)
        return Parametros
      




        
        

