import { Component, Input, Output, EventEmitter } from '@angular/core';
import { FirebaseService } from '../../service/firebase.service';

@Component({
  selector: 'app-dispositivos',
  templateUrl: './dispositivos.component.html',
  styleUrls: ['./dispositivos.component.css']
})
export class DispositivosComponent {

  @Output() dispositivoSeleccionado: EventEmitter<string>;

  constructor(private _firebaseService:FirebaseService) {
    this.dispositivoSeleccionado = new EventEmitter();
  }

  enviarDispositivo(dispositivo:string){
    this.dispositivoSeleccionado.emit(dispositivo);
  }

}
