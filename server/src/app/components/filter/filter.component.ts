import { Component } from '@angular/core';
import { NgForm } from '@angular/forms';

@Component({
  selector: 'app-filter',
  templateUrl: './filter.component.html',
  styleUrls: ['./filter.component.css']
})
export class FilterComponent {

  constructor() { }

  verGrafico(forma:NgForm){
    console.log(forma.controls.parametro.value);
    console.log(forma);
  }

}
