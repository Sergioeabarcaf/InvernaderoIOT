import { Component, Input } from '@angular/core';

@Component({
  selector: 'app-parametros',
  templateUrl: './parametros.component.html',
  styleUrls: ['./parametros.component.css']
})
export class ParametrosComponent {

  @Input() dispositivo:string;

  constructor() { }


}
