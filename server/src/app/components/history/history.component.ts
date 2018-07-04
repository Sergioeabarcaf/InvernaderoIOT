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
  gaugeMaxPres:number=1100;
  gaugeMaxCO2:number=2000;

  thresholdConfigTemp = {
    '-20': {color: 'blue'},
    '15': {color: 'green'},
    '30': {color: 'red'}
  };
  thresholdConfigHum = {
    '0': {color: 'blue'},
    '50': {color: 'green'},
    '80': {color: 'red'}
  };
  thresholdConfigPress = {
    '0': {color: 'blue'},
    '900': {color: 'green'},
    '1000': {color: 'red'}
  };
  thresholdConfigCO2 = {
    '0': {color: 'blue'},
    '700': {color: 'green'},
    '1500': {color: 'red'}
  };

  constructor(public _firebaseService:FirebaseService) {

  }


}
