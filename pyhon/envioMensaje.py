 
username = "idiProteinlab" # enter your username
password = "Proteinlab2017!" # enter your password 

# -------------------------------------------------

import http.client
import base64


# create HTTP basic authentication string, this consists of 
# "username:password" base64 encoded 
#-----metodo obsoleto desde python 3
#auth = base64.encodestring("%s:%s"%(username,password))[:-1]
#



strAux ="%s:%s"%(username,password)
auth =(base64.b64encode(bytes(strAux,'utf-8')))
auth =auth.decode('utf-8')

#Autorizacion chequeo
print (auth)
mensaje = base64.b64encode(bytes("Estado OFF",'utf-8'))
# message to send to server
message = """<sci_request version="1.0">
<send_message cache = "false">
     <targets>
         <device id="00000000-00000000-00409DFF-FF6496C2"/>
     </targets>
    <rci_request version="1.1">
       <do_command target="xbgw">
           <send_serial addr="0013A200415B67F6">'%s'</send_serial>
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
if(statuscode == 200):
    print("Mensaje enviado con Exito")
    print (statusmessage)
    print(response_body)
