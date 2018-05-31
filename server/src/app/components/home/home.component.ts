import { Component } from '@angular/core';
import { FirebaseService } from '../../service/firebase.service';

@Component({
  selector: 'app-home',
  templateUrl: './home.component.html',
  styleUrls: ['./home.component.css']
})
export class HomeComponent {

  data:any[] = [];
  objetoArray:any[] = [];
  dispositivos:string[] = ["WaspTHP", "WaspTHP_CO2"];

  constructor(private _firebaseService:FirebaseService) {
    this._firebaseService.getData().subscribe( data => {
      let x:number = 0;
      for( let objeto of Object.keys(data) ){
        this.objetoArray[0] = data[objeto].device;
        this.objetoArray[1] = data[objeto].timestamp;
        let i:number = 2;
        for (let param of Object.keys(data[objeto].values)){
          this.objetoArray[i] = `${param}:${data[objeto].values[param]}`
          i++;
        }
        this.data[x] = this.objetoArray;
        this.objetoArray = [];
        x++;
      }
      console.log(this.data);
    });
    //consulta por los dispositivos asociados a firebase y obtiene la data
    // for(let i=0; i < this.dispositivos.length; i++){
    //   this._firebaseService.getData( this.dispositivos[i] ).subscribe( data =>{
    //     console.log(data)
    //     // Se obtiene la data eliminando la key
    //     this.data[i] = Object.values(data)[0];
    //     // para cada parametro se obtiene su clave y valor
    //     for(let param of Object.keys(this.data[i])){
    //       console.log( `${ param } : ${ this.data[i][param] }` );
    //       this.dataSwap[i].
    //       this.dataSwap.push(`${ param } : ${ this.data[i][param] }`);
    //     }
    //   })
    // }
  }

}
