import { Component, Input } from '@angular/core';

@Component({
  selector: 'app-last24',
  templateUrl: './last24.component.html',
  styleUrls: ['./last24.component.css']
})
export class Last24Component {
  @Input() parametro:string;

  //Gauge
  gaugeType = "arch";
  gaugeForegroundColor = "rgba(0, 150, 136, 1)"; // definir colores linea
  gaugeBackgroundColor = "rgba(0, 0, 0, 0.1)"; // definir colores fondo
  @Input() gaugeValue: number = 28.3; //Input value
  @Input() gaugeMin: number = 0; //Input value min
  @Input() gaugeMax: number = 100; //Input value Max
  @Input() gaugeAppendText: string = "km/hr"; // Input Unit Mes


  //lineChart
  @Input() lineChartData:Array<any>;
  @Input() lineChartLabels:Array<any>;

 public lineChartColors:Array<any> = [
   {
     backgroundColor: 'rgba(79,136,161,0.8)',
     borderColor: 'rgb(200,123,37)',
     pointBackgroundColor: 'rgba(148,159,177,1)',
     pointBorderColor: '#fff',
     pointHoverBackgroundColor: '#fff',
     pointHoverBorderColor: 'rgba(148,159,177,0.8)'
   }
 ];
 public lineChartType:string = 'line';
  // events
  public chartClicked(e:any):void {
    console.log(e);
  }

  public chartHovered(e:any):void {
    console.log(e);
  }

}
