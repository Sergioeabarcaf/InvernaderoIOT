import base64
import time
import http.client
import firebase_admin
from firebase_admin import credentials
from firebase_admin import db

cred = credentials.Certificate('invernadero-221913.json')
default_app = firebase_admin.initialize_app(cred, {
    'databaseURL': 'https://invernadero-221913.firebaseio.com/'})

# now print out the entire event object
Actuador = "00:13:A2:00:41:54:B4:E8"
urlDevice = 'Actuadores/' +Actuador 
ref = db.reference(urlDevice)
auxmensaje = input("mensaje:")

dataSend={}
mensaje = base64.b64encode(bytes(auxmensaje,'utf-8'))
mensaje = str(mensaje.decode('utf-8'))
strAux = mensaje
strAux = base64.b64decode(bytes(strAux,'utf-8'))
strAux = strAux.decode('utf-8')
dataSend["Actualizacion"] = strAux
timestamp = time.strftime("%Y-%m-%dT%H:%M:%S",time.localtime())
dataSend["timestamp"] = timestamp                    
    
#Lectura de datos Firebase
#-------------------------------------------------
username = "idiProteinlab" # enter your username
password = "Proteinlab2017!" # enter your password 

# -------------------------------------------------
strAux ="%s:%s"%(username,password)
auth =(base64.b64encode(bytes(strAux,'utf-8')))
auth = auth.decode('utf-8')

# message to send to server
message = """<sci_request version="1.0">
    <send_message>
        <targets>
            <device id="00000000-00000000-00409DFF-FF6496C2"/>
       </targets>
      <rci_request version="1.1">
         <do_command target="xbgw">
             <send_serial addr="0013A2004154B4E8">'%s'</send_serial>
         </do_command>
     </rci_request>
  </send_message>
</sci_request>"""%(mensaje)
webservice = http.client.HTTPSConnection("remotemanager.digi.com")

# to what URL to send the request with a given HTTP method
webservice.putrequest("POST", "/ws/sci")

# add the authorization string into the HTTP header
webservice.putheader("Authorization", "Basic %s"%auth)

webservice.putheader("Content-type", "text/xml")
webservice.putheader("Content-length", "%d" % len(message))
webservice.putheader("Accept", "text/xml");
webservice.endheaders()
webservice.send(message.encode())

# get the response
response = webservice.getresponse()
statuscode = response.status
statusmessage = response.reason
response_body = response.read()

# print the output to standard out
print (statuscode, statusmessage)
print (response_body)
response_body = str(response_body).split(">")
if (statuscode == 200 and "failure" not in response_body[9]):
    url = 'System/Python'
    db.reference(url).set(True)
    url = 'System/State'
    state = db.reference(url).get()
    while(state!=False):
        print("Esperando")
        time.sleep(15)
        state = db.reference("System/State").get()
    time.sleep(5)   
    estado = db.reference("Actuadores/State").get()
    dataSend["Estado"] = estado
else:
    estado = 0
    dataSend["Estado"] = estado
    
ref.push(dataSend)
time.sleep(5)
db.reference('Actuadores/State').set("?")
db.reference("System/Python").set(False)
