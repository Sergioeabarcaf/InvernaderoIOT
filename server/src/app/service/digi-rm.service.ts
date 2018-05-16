import { Injectable } from '@angular/core';
import { HttpClient, HttpHeaders } from '@angular/common/http';
import 'rxjs/Rx';

@Injectable()
export class DigiRmService {

  private header = new HttpHeaders();
  private user:string = "idiProteinlab";
  private pass:string = "Proteinlab2017!";

  private urlBase:string = "http://remotemanager.digi.com/ws/v1/streams/history/00000000-00000000-00409DFF-FF6496C2/xbee.serialIn/[00:13:A2:00:41:04:37:F1]!";

  constructor( private http:HttpClient ) { }

  // console.log(atob("cGFzc3dvcmQ=")); // password
  // console.log(btoa("password")); // cGFzc3dvcmQ=

  getData(){
    this.header.set('Content-type', 'application/json');
    this.header.set('Authorization', `Basic ${ btoa(this.user) }:${ btoa(this.pass) }`);

    console.log(this.header);


    return this.http.get( this.urlBase ).map( res => {
      console.log("respuesta?");
      console.log(res);
      return res;
    });
  }

}
