import { Component } from '@angular/core';
import { FirebaseService } from '../../service/firebase.service';

@Component({
  selector: 'app-home',
  templateUrl: './home.component.html',
  styleUrls: ['./home.component.css']
})
export class HomeComponent {

  dispositivo:string = "";
  parametros:any;
  parametro:string = "";

  constructor(private _firebaseService:FirebaseService) {

  }

  setDispositivo(dispositivo:string){
    this.dispositivo = dispositivo;
    this.parametros = this._firebaseService.getParamsDispositivo(this.dispositivo);
  }

  setParametro(param:string){
    this.parametro = param;
    console.log(this.parametro);
  }

}
