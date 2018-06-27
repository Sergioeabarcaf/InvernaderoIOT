import { Component, Input } from '@angular/core';

@Component({
  selector: 'app-last24',
  templateUrl: './last24.component.html',
  styleUrls: ['./last24.component.css']
})
export class Last24Component {
  @Input() parametro:string;

  //gauge
  @Input() gaugeValue:number;
  @Input() gaugeMin:number;
  @Input() gaugeMax:number;

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
