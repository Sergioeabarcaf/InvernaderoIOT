import { Injectable } from '@angular/core';
import { Http, Headers } from '@angular/http';
import 'rxjs/Rx';

@Injectable()
export class FirebaseService {

  url:string = "https://libelium-91af3.firebaseio.com/";

  constructor(private http:Http) { }

  getData(){
    let urlDisp = `${ this.url }estacionMetereologica.json?orderBy"timestamp"`;
    return this.http.get( urlDisp ).map( res => {
      return res.json();
    })
  }
}
