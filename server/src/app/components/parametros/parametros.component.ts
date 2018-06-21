import { Component, Input, Output, EventEmitter } from '@angular/core';

@Component({
  selector: 'app-parametros',
  templateUrl: './parametros.component.html',
  styleUrls: ['./parametros.component.css']
})
export class ParametrosComponent {

  @Input() params:any = {};

  @Output() parametroSeleccionado: EventEmitter<string>;

  constructor() {
    this.parametroSeleccionado = new EventEmitter();
  }

  seleccionParametro(param){
    this.parametroSeleccionado.emit(param);
  }


}
