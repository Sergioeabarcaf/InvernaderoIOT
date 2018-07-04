import { Injectable } from '@angular/core';
import { HttpClient } from '@angular/common/http';
import 'rxjs/Rx';

@Injectable()
export class FirebaseService {

  url:string = "https://libelium-91af3.firebaseio.com/";

  dataLast:any = {};

  temp24:any = [{}]
  hum24:any = [{}]
  pres24:any = [{}]
  co224:any = [{}]
  labelTHP24:any = []
  labelCO224:any = []

  constructor(private http:HttpClient) {
    this.getDataLast("last.json");
    this.getDataCO224("dataDevices/00:13:A2:00:41:5B:67:EB.json");
    this.getDataTHP24("dataDevices/00:13:A2:00:41:04:37:F1.json");
  }

  getDataLast(link:string){
    let url = `${ this.url }${ link }`
    this.http.get( url ).subscribe( res => {
      this.dataLast = res;
    });
  }

  getDataCO224(link:string){
    let values:any = []
    let url = `${ this.url }${ link }?orderBy="$key"&limitToLast=48`
    this.http.get( url ).subscribe( res => {
      for(let x of Object.keys(res)){
        values.push(parseFloat(res[x].values.CO2))
        this.labelCO224.push(this.timestampToHours(res[x].timestamp))
      }
      this.co224[0].data = values;
    })
  }

  getDataTHP24(link:string){
    let valuesTemp:any = []
    let valuesHum:any = []
    let valuesPres:any = []
    let url = `${ this.url }${ link }?orderBy="$key"&limitToLast=48`
    this.http.get( url ).subscribe( res => {
      for(let x of Object.keys(res)){
        valuesTemp.push(parseFloat(res[x].values.TC));
        valuesHum.push(parseFloat(res[x].values.HUM));
        valuesPres.push(parseFloat(res[x].values.PRES));
        this.labelTHP24.push(this.timestampToHours(res[x].timestamp));
      }
      this.temp24[0].data = valuesTemp;
      this.hum24[0].data = valuesHum;
      this.pres24[0].data = valuesPres;
    })
  }

  timestampToHours(timestamp:string){
    let arrayTimestamp = timestamp.split("T");
    let arrayHours = arrayTimestamp[1].split(":");
    let strHour = `${ arrayHours[0] }:${ arrayHours[1] }`;
    return strHour;
  }

}
