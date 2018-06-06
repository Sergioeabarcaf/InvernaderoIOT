import { Injectable } from '@angular/core';
import { HttpClient } from '@angular/common/http';
import 'rxjs/Rx';

@Injectable()
export class FirebaseService {

  dataFiresbase:any;
  valuesArray:any[] = [];
  paramValue:any[] = [];

  data:any[] = [];
  dispositivos:string[] = [];

  urlEM:string = "https://libelium-91af3.firebaseio.com/";

  constructor(private http:HttpClient) {
    // obtener dispositivos
    let urlDevices = `${ this.urlEM }dispositivos.json`;
    this.http.get( urlDevices ).subscribe( res => {
      for(let dispositivo of Object.keys(res)){
        this.dispositivos.push(dispositivo);
      }
    });

    let urlData = `${ this.urlEM }estacionMetereologica.json?orderBy"timestamp"`;
    this.http.get( urlData ).subscribe( res => {
      this.dataFiresbase = res;
      // Recorrer el JSON que en cada llave contiene un conjunto de datos
      let x:number = 0
      for( let objeto of Object.keys(this.dataFiresbase) ){
         this.data[x] = this.dataFiresbase[objeto];
         let i:number = 0;
         for (let param of Object.keys(this.dataFiresbase[objeto].values)){
           // extraer el parametro y valor para agregarlos en valuesArray, una vez agregados, limpiar el valuesArray
           this.paramValue[0] = param;
           this.paramValue[1] = parseFloat(this.dataFiresbase[objeto].values[param]);
           this.valuesArray[i] = this.paramValue;
           this.paramValue = [];
           i++;
         }
         this.data[x].values = this.valuesArray;
         this.valuesArray = [];
         x++;
       }
    })
  }

  getParamsDispositivo(dispositivo:string){
    for( let dat of this.data){
      if( dispositivo == dat.device){
        return dat.values
      }
    }
  }

}
