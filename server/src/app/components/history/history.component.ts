import { Component } from '@angular/core';
import { NgForm } from '@angular/forms';
@Component({
  selector: 'app-history',
  templateUrl: './history.component.html',
  styleUrls: ['./history.component.css']
})
export class HistoryComponent {

  constructor() { }

  verGrafico(forma:NgForm){
    console.log(forma.controls.parametro.value);
    console.log(forma);
  }

}
