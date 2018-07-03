import { Component } from '@angular/core';
import { FirebaseService } from '../../service/firebase.service';

@Component({
  selector: 'app-history',
  templateUrl: './history.component.html',
  styleUrls: ['./history.component.css']
})
export class HistoryComponent {

  paramTemp:string = "Temperatura";
  paramHum:string = "Humedad";
  paramPres:string = "Presión";
  paramCO2:string = "Dioxido de carbono";

  gaugeUMTemp:string="°C";
  gaugeUMHum:string="% HR";
  gaugeUMPres:string="hPa";
  gaugeUMCO2:string="PPM";

  gaugeMinTemp:number=-20;
  gaugeMinHum:number=0;
  gaugeMinPres:number=0;
  gaugeMinCO2:number=0;

  gaugeMaxTemp:number=100;
  gaugeMaxHum:number=100;
  gaugeMaxPres:number=110000;
  gaugeMaxCO2:number=2000;

  constructor(public _firebaseService:FirebaseService) {

  }


}
