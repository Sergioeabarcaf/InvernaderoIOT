import { Component } from '@angular/core';
import { FirebaseService } from '../../service/firebase.service';

@Component({
  selector: 'app-home',
  templateUrl: './home.component.html',
  styleUrls: ['./home.component.css']
})
export class HomeComponent {

  data:any[] = [];
  valuesArray:any[] = [];
  paramValue:any[] = [];

  constructor(private _firebaseService:FirebaseService) {
    this._firebaseService.getData().subscribe( data => {
      // Recorrer el JSON que en cada llave contiene un conjunto de datos
      let x:number = 0;
      for( let objeto of Object.keys(data) ){
        this.data[x] = data[objeto];
        let i:number = 0;
        for (let param of Object.keys(data[objeto].values)){
          // extraer el parametro y valor para agregarlos en valuesArray, una vez agregados, limpiar el valuesArray
          this.paramValue[0] = param;
          this.paramValue[1] = parseFloat(data[objeto].values[param]);
          this.valuesArray[i] = this.paramValue;
          this.paramValue = [];
          i++;
        }
        this.data[x].values = this.valuesArray;
        this.valuesArray = [];
         x++;
      }
      console.log(this.data);
    });
  }

}
